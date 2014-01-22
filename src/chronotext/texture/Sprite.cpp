/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/texture/Sprite.h"
#include "chronotext/texture/TextureManager.h"
#include "chronotext/utils/Utils.h"

using namespace std;
using namespace ci;

namespace chronotext
{
    Sprite::Sprite(InputSourceRef inputSource, TextureManager& textureManager)
    :
    inputSource(inputSource),
    textureManager(textureManager),
    ox(0),
    oy(0),
    rotated(false),
    tx1(0),
    ty1(0)
    {
        loadTexture();
        
        tx2 = texture->getMaxU();
        ty2 = texture->getMaxV();
        
        w = ow = texture->getWidth() * texture->getMaxU();
        h = oh = texture->getHeight() * texture->getMaxV();
    }
    
    Sprite::Sprite(InputSourceRef inputSource, TextureManager& textureManager,
                   float w, float h, float ox, float oy, float ow, float oh, bool rotated, float tx1, float ty1, float tx2, float ty2)
    :
    inputSource(inputSource),
    textureManager(textureManager),
    w(w),
    h(h),
    ox(ox),
    oy(oy),
    ow(ow),
    oh(oh),
    rotated(rotated),
    tx1(tx1),
    ty1(ty1),
    tx2(tx2),
    ty2(ty2)
    {}
    
    Vec2f Sprite::getSize() const
    {
        return Vec2f(ow, oh);
    }
    
    float Sprite::getWidth() const
    {
        return ow;
    }
    
    float Sprite::getHeight() const
    {
        return oh;
    }
    
    void Sprite::beginTexture()
    {
        if(!texture) {
            LOGD << "Loading texture on the fly for: " << inputSource->getFilePathHint() << endl;
            loadTexture();
        }
        texture->begin();
    }
    
    void Sprite::endTexture()
    {
        assert(texture);
        texture->end();
    }
    
    void Sprite::drawFromCenter()
    {
        draw(ow * 0.5f, oh * 0.5f);
    }
    
    void Sprite::draw(float rx, float ry)
    {
        float vertices[4][2];
        float coords[4][2];
        
        float x1 = -rx + ox;
        float y1 = -ry + oy;
        
        float x2 = x1 + w;
        float y2 = y1 + h;
        
        if (rotated)
        {
            vertices[0][0] = y1; vertices[0][1] = x2;
            vertices[1][0] = y2; vertices[1][1] = x2;
            vertices[2][0] = y2; vertices[2][1] = x1;
            vertices[3][0] = y1; vertices[3][1] = x1;
            
            coords[0][0] = tx1; coords[0][1] = ty1;
            coords[1][0] = tx1; coords[1][1] = ty2;
            coords[2][0] = tx2; coords[2][1] = ty2;
            coords[3][0] = tx2; coords[3][1] = ty1;
        }
        else
        {
            vertices[0][0] = x1; vertices[0][1] = y1;
            vertices[1][0] = x2; vertices[1][1] = y1;
            vertices[2][0] = x2; vertices[2][1] = y2;
            vertices[3][0] = x1; vertices[3][1] = y2;
            
            coords[0][0] = tx1; coords[0][1] = ty1;
            coords[1][0] = tx2; coords[1][1] = ty1;
            coords[2][0] = tx2; coords[2][1] = ty2;
            coords[3][0] = tx1; coords[3][1] = ty2;
        }
        
        glTexCoordPointer(2, GL_FLOAT, 0, coords);
        glVertexPointer(2, GL_FLOAT, 0, vertices);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);    
    }

    bool Sprite::isLoaded() const
    {
        return !!texture;
    }
    
    void Sprite::purgeTexture()
    {
        if(!texture)
            return;
        textureManager.remove(texture);
        texture.reset();
    }
    
    void Sprite::loadTexture()
    {
        if(texture)
            return;
        
        TextureRequest request(inputSource, true, TextureRequest::FLAGS_POT);
        request.maxSize = Vec2i(1024, 1024);
        
        texture = textureManager.getTexture(request);
    }
    
    InputSourceRef Sprite::getInputSource() const
    {
        return inputSource;
    }
}
