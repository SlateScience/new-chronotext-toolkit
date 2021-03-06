/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/texture/TextureHelper.h"
#include "chronotext/texture/Texture.h"
#include "chronotext/texture/PVRHelper.h"
#include "chronotext/utils/Utils.h"
#include "chronotext/utils/MathUtils.h"

#include "cinder/ImageIo.h"
#include "cinder/ip/Fill.h"

using namespace std;
using namespace ci;

namespace chronotext
{
    gl::TextureRef TextureHelper::loadTexture(const string &resourceName, bool useMipmap, int flags)
    {
        return loadTexture(InputSource::getResource(resourceName), useMipmap, flags);
    }
    
    gl::TextureRef TextureHelper::loadTexture(InputSourceRef inputSource, bool useMipmap, int flags)
    {
        return loadTexture(TextureRequest(inputSource, useMipmap, flags));
    }
    
    gl::TextureRef TextureHelper::loadTexture(const TextureRequest &textureRequest)
    {
        TextureData textureData = fetchTextureData(textureRequest);
        
        if (textureData.undefined())
        {
            throw Texture::Exception("TEXTURE IS UNDEFINED");
        }
        else if ((textureRequest.maxSize.x > 0) && (textureRequest.maxSize.y > 0))
        {
            const Vec2i size = textureData.getSize();
            
            if ((size.x > textureRequest.maxSize.x) || (size.y > textureRequest.maxSize.y))
            {
                string msg = "TEXTURE IS OVER-SIZED (" + toString(size.x) + "x" + toString(size.y) + "): " + textureRequest.inputSource->getFilePathHint();
                LOGI << msg << endl;
                throw Texture::Exception(msg);
            }
        }

        return uploadTextureData(textureData);
    }
    
    TextureData TextureHelper::fetchTextureData(const TextureRequest &textureRequest)
    {
        if (boost::ends_with(textureRequest.inputSource->getFilePathHint(), ".pvr.gz"))
        {
            if (textureRequest.inputSource->isFile())
            {
                return TextureData(textureRequest, PVRHelper::decompressPVRGZ(textureRequest.inputSource->getFilePath())); // RVO-READY
            }
            else
            {
                throw Texture::Exception("PVR.GZ TEXTURES CAN ONLY BE LOADED FROM FILES");
            }
        }
        else if (boost::ends_with(textureRequest.inputSource->getFilePathHint(), ".pvr.ccz"))
        {
            return TextureData(textureRequest, PVRHelper::decompressPVRCCZ(textureRequest.inputSource->loadDataSource())); // RVO-READY
        }
        else if (boost::ends_with(textureRequest.inputSource->getFilePathHint(), ".pvr"))
        {
            return TextureData(textureRequest, textureRequest.inputSource->loadDataSource()->getBuffer()); // RVO-READY
        }
        else
        {
            if (textureRequest.flags & TextureRequest::FLAGS_TRANSLUCENT)
            {
                return TextureData(fetchTranslucentTextureData(textureRequest)); // RVO-READY
            }
            else if (textureRequest.flags & TextureRequest::FLAGS_POT_CROP)
            {
                return TextureData(fetchCroppablePowerOfTwoTextureData(textureRequest));
            }
            else if (textureRequest.flags & TextureRequest::FLAGS_POT)
            {
                return TextureData(fetchPowerOfTwoTextureData(textureRequest)); // RVO-READY
            }
            else
            {
                return TextureData(textureRequest, loadImage(textureRequest.inputSource->loadDataSource())); // RVO-READY
            }
        }
        
        return TextureData();
    }
    
    gl::TextureRef TextureHelper::uploadTextureData(const TextureData &textureData)
    {
        gl::TextureRef texture;
        
        if (!textureData.undefined())
        {
            gl::Texture::Format format = textureData.request.getFormat();
            
            /*
             * NECESSARY IN ORDER TO CLEANUP EVENTUAL ERRORS
             */
            while(glGetError() != GL_NO_ERROR)
            {}
            
            switch (textureData.type)
            {
                case TextureData::TYPE_SURFACE:
                    texture = gl::Texture::create(textureData.surface, format);
                    texture->setCleanTexCoords(textureData.maxU, textureData.maxV);
                    break;
                    
                case TextureData::TYPE_IMAGE_SOURCE:
                    texture = gl::Texture::create(textureData.imageSource, format);
                    break;
                    
                case TextureData::TYPE_PVR:
                    texture = PVRHelper::getPVRTexture(textureData.buffer, format.hasMipmapping(), format.getWrapS(), format.getWrapT());
                    break;
                    
                case TextureData::TYPE_DATA:
                    format.setInternalFormat(textureData.glInternalFormat);
                    texture = gl::Texture::create(textureData.data.get(), textureData.glFormat, textureData.width, textureData.height, format);
                    break;
            }
            
            if (glGetError() == GL_OUT_OF_MEMORY)
            {
                throw Texture::Exception("GL: OUT-OF-MEMORY");
            }
            else if (texture)
            {
                texture->setDeallocator(&TextureHelper::textureDeallocator, texture.get());
                
                LOGD <<
                "TEXTURE UPLOADED: " <<
                textureData.request.inputSource->getFilePathHint() << " | " <<
                texture->getId() << " | " <<
                texture->getWidth() << "x" << texture->getHeight() <<
                endl;
            }
        }
        
        return texture;
    }
    
    void TextureHelper::bindTexture(gl::Texture *texture)
    {
        glBindTexture(GL_TEXTURE_2D, texture->getId());
    }
    
    void TextureHelper::beginTexture(gl::Texture *texture)
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnable(GL_TEXTURE_2D);
        
        glBindTexture(GL_TEXTURE_2D, texture->getId());
    }
    
    void TextureHelper::endTexture()
    {
        glDisable(GL_TEXTURE_2D);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
    
    /*
     * XXX: INCLUDES WORKAROUND FOR ci::gl::Texture::getCleanWidth() AND CO. WHICH ARE NOT WORKING ON GL-ES
     */
    void TextureHelper::drawTextureFromCenter(gl::Texture *texture)
    {
        drawTexture(texture, texture->getWidth() * texture->getMaxU() * 0.5f, texture->getHeight() * texture->getMaxV() * 0.5f);
    }
    
    /*
     * XXX: INCLUDES WORKAROUND FOR ci::gl::Texture::getCleanWidth() AND CO. WHICH ARE NOT WORKING ON GL-ES
     */
    void TextureHelper::drawTexture(gl::Texture *texture, float rx, float ry)
    {
        float tx = texture->getMaxU();
        float ty = texture->getMaxV();
        
        float x1 = -rx;
        float y1 = -ry;
        
        float x2 = x1 + texture->getWidth() * tx;
        float y2 = y1 + texture->getHeight() * ty;
        
        const float vertices[] =
        {
            x1, y1,
            x2, y1,
            x2, y2,
            x1, y2
        };
        
        const float coords[] =
        {
            0, 0,
            tx, 0,
            tx, ty,
            0, ty
        };
        
        glTexCoordPointer(2, GL_FLOAT, 0, coords);
        glVertexPointer(2, GL_FLOAT, 0, vertices);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
    
    /*
     * XXX: ONLY WORKS FOR "TRUE" POWER-OF-TWO TEXTURES
     */
    void TextureHelper::drawTextureInRect(gl::Texture *texture, const Rectf &rect, float ox, float oy)
    {
        const float vertices[] =
        {
            rect.x1, rect.y1,
            rect.x2, rect.y1,
            rect.x2, rect.y2,
            rect.x1, rect.y2
        };
        
        float tx1 = (rect.x1 - ox) / texture->getWidth();
        float ty1 = (rect.y1 - oy) / texture->getHeight();
        float tx2 = (rect.x2 - ox) / texture->getWidth();
        float ty2 = (rect.y2 - oy) / texture->getHeight();
        
        const float coords[] =
        {
            tx1, ty1,
            tx2, ty1,
            tx2, ty2,
            tx1, ty2
        };
        
        glTexCoordPointer(2, GL_FLOAT, 0, coords);
        glVertexPointer(2, GL_FLOAT, 0, vertices);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
    
    void TextureHelper::textureDeallocator(void *refcon)
    {
        gl::Texture *texture = reinterpret_cast<gl::Texture*>(refcon);
        
        LOGD <<
        "TEXTURE UNLOADED: " <<
        texture->getId() <<
        endl;
    }
    
    /*
     * BASED ON https://github.com/cinder/Cinder/blob/v0.8.5/src/cinder/gl/Texture.cpp#L478-490
     */
    TextureData TextureHelper::fetchTranslucentTextureData(const TextureRequest &textureRequest)
    {
        Surface surface(loadImage(textureRequest.inputSource->loadDataSource()));
        
        Channel8u &channel = surface.getChannel(0);
        shared_ptr<uint8_t> data;
        
        if ((channel.getIncrement() != 1) || (channel.getRowBytes() != channel.getWidth() * sizeof(uint8_t)))
        {
            data = shared_ptr<uint8_t>(new uint8_t[channel.getWidth() * channel.getHeight()], checked_array_deleter<uint8_t>());
            uint8_t *dest = data.get();
            int8_t inc = channel.getIncrement();
            int32_t width = channel.getWidth();
            int32_t height = channel.getHeight();
            
            for (int y = 0; y < height; ++y)
            {
                const uint8_t *src = channel.getData(0, y);
                
                for (int x = 0; x < width; ++x)
                {
                    *dest++ = *src;
                    src += inc;
                }
            }
        }
        else
        {
            data = shared_ptr<uint8_t>(channel.getData(), checked_array_deleter<uint8_t>());
        }
        
        return TextureData(textureRequest, data, GL_ALPHA, GL_ALPHA, channel.getWidth(), channel.getHeight()); // RVO-READY
    }
    
    TextureData TextureHelper::fetchPowerOfTwoTextureData(const TextureRequest &textureRequest)
    {
        /*
         * NO EXTRA DATA-COPYING WILL OCCUR BECAUSE ci::Surface
         * IS EMULATING shared_ptr BEHAVIOR INTERNALLY
         */
        
        Surface src(loadImage(textureRequest.inputSource->loadDataSource()));
        
        int srcWidth = src.getWidth();
        int srcHeight = src.getHeight();
        
        int dstWidth = nextPowerOfTwo(srcWidth);
        int dstHeight = nextPowerOfTwo(srcHeight);
        
        if ((srcWidth != dstWidth) || (srcHeight != dstHeight))
        {
            Surface dst(dstWidth, dstHeight, src.hasAlpha(), src.getChannelOrder());
            
            /*
             * NO NEED TO CLEAR THE WHOLE SURFACE
             */
            ip::fill(&dst, ColorA::zero(), Area(srcWidth + 1, 0, dstWidth, srcHeight));
            ip::fill(&dst, ColorA::zero(), Area(0, srcHeight + 1, srcWidth, dstHeight));
            ip::fill(&dst, ColorA::zero(), Area(srcWidth + 1, srcHeight + 1, dstWidth, dstHeight));
            
            dst.copyFrom(src, Area(0, 0, srcWidth, srcHeight), Vec2i::zero());
            
            /*
             * DUPLICATING THE RIGHT AND BOTTOM EDGES:
             * NECESSARY TO AVOID BORDER ARTIFACTS WHEN THE
             * TEXTURE IS NOT DRAWN AT ITS ORIGINAL SCALE
             */
            dst.copyFrom(src, Area(srcWidth - 1, 0, srcWidth, srcHeight), Vec2i(1, 0));
            dst.copyFrom(src, Area(0, srcHeight - 1, srcWidth, srcHeight), Vec2i(0, 1));
            
            return TextureData(textureRequest, dst, srcWidth / float(dstWidth), srcHeight / float(dstHeight)); // RVO-READY
        }
        else
        {
            return TextureData(textureRequest, src); // RVO-READY
        }
    }

    static inline bool IsPowerOfTwo(unsigned int x)
    {
        return x == 0 || (x & (x - 1)) == 0;
    }
    
    TextureData TextureHelper::fetchCroppablePowerOfTwoTextureData(const TextureRequest &textureRequest)
    {
        Surface src(loadImage(textureRequest.inputSource->loadDataSource()));
        
        int srcWidth = src.getWidth();
        int srcHeight = src.getHeight();
        
        int dstWidth = nextPowerOfTwo(srcWidth);
        int dstHeight = nextPowerOfTwo(srcHeight);
        
        assert(IsPowerOfTwo(textureRequest.maxSize.x));
        if(textureRequest.maxSize.x > 0 && srcWidth > textureRequest.maxSize.x) {
            if(textureRequest.flags & TextureRequest::FLAGS_POT_CROP) {
                dstWidth = textureRequest.maxSize.x;
            }
            else {
                stringstream msg;
                msg << "TEXTURE IS OVER-SIZED (width=" << srcWidth << " > " << textureRequest.maxSize.x << "): " << textureRequest.inputSource->getFilePathHint();
                LOGI << msg.str() << endl;
                throw Texture::Exception(msg.str());
            }
        }
        
        assert(IsPowerOfTwo(textureRequest.maxSize.y));
        if(textureRequest.maxSize.y > 0 && srcHeight > textureRequest.maxSize.y) {
            if(textureRequest.flags & TextureRequest::FLAGS_POT_CROP) {
                dstHeight = textureRequest.maxSize.y;
            }
            else {
                stringstream msg;
                msg << "TEXTURE IS OVER-SIZED (height=" << srcHeight << " > " << textureRequest.maxSize.y << "): " << textureRequest.inputSource->getFilePathHint();
                LOGI << msg.str() << endl;
                throw Texture::Exception(msg.str());
            }
        }
        
        if ((srcWidth  != dstWidth) ||
            (srcHeight != dstHeight))
        {
            Surface dst(dstWidth, dstHeight, src.hasAlpha(), src.getChannelOrder());
            
            // 1. Fill new surface with black:
            if(dstWidth > srcWidth) {
                ip::fill(&dst, ColorA::zero(), Area(srcWidth + 1, 0, dstWidth, dstHeight));
            }
            if(dstHeight > srcHeight) {
                ip::fill(&dst, ColorA::zero(), Area(0, srcHeight + 1, dstWidth, dstHeight));
            }
            
            // 2. Crop? If needed, crop in a centered way
            Vec2f srcCorner(0,0);
            Vec2f copySize(srcWidth, srcHeight);
            if(dstWidth < srcWidth) {
                srcCorner.x = (srcWidth - dstWidth) * 0.5f;
                copySize.x = dstWidth;
            }
            if(dstHeight < srcHeight) {
                srcCorner.y = (srcHeight - dstHeight) * 0.5f;
                copySize.y = dstHeight;
            }
            
            // 3. The copy
            dst.copyFrom(src, Area(srcCorner, srcCorner + copySize), Vec2i::zero());
            
            // 4. DUPLICATING THE RIGHT EDGE:
            //    (NECESSARY TO AVOID BORDER ARTIFACTS WHEN THE
            //     TEXTURE IS NOT DRAWN AT ITS ORIGINAL SCALE)
            if(dstWidth > srcWidth) {
                dst.copyFrom(src, Area(srcWidth - 1, 0, srcWidth, srcHeight), Vec2i(1, 0));
            }
            if(dstHeight > srcHeight) {
                dst.copyFrom(src, Area(0, srcHeight - 1, srcWidth, srcHeight), Vec2i(0, 1));
            }
            
            return TextureData(textureRequest, dst, srcWidth / float(dstWidth), srcHeight / float(dstHeight));
        }
        else
        {
            return TextureData(textureRequest, src);
        }
    }
}
