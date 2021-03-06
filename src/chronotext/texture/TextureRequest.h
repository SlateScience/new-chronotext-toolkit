/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/InputSource.h"

#include "cinder/gl/Texture.h"

namespace chronotext
{
    struct TextureRequest
    {
        enum
        {
            FLAGS_NONE = 0x0,
            FLAGS_TRANSLUCENT = 0x1,
            FLAGS_POT = 0x2,
            FLAGS_POT_CROP = 0x4
        };
        
        InputSourceRef inputSource;
        bool useMipmap;
        int flags;
        GLenum wrapS;
        GLenum wrapT;
        ci::Vec2i maxSize;
        
        TextureRequest()
        {}
        
        TextureRequest(InputSourceRef inputSource, bool useMipmap = false, int flags = FLAGS_NONE)
        :
        inputSource(inputSource),
        useMipmap(useMipmap),
        flags(flags),
        wrapS(GL_CLAMP_TO_EDGE),
        wrapT(GL_CLAMP_TO_EDGE),
        maxSize(0, 0)
        {}
        
        ci::gl::Texture::Format getFormat() const
        {
            ci::gl::Texture::Format format;
            format.setWrap(wrapS, wrapT);
            
            if (useMipmap)
            {
                format.enableMipmapping(true);
                format.setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
            }
            
            return format;
        }
        
        TextureRequest& setWrap(GLenum s, GLenum t)
        {
            wrapS = s;
            wrapT = t;
            return *this;
        }
        
        TextureRequest& setMaxSize(const ci::Vec2i &size)
        {
            maxSize = size;
            return *this;
        }
        
        bool operator<(const TextureRequest &rhs) const
        {
            if (std::tie(useMipmap, flags, wrapS, wrapT) == std::tie(rhs.useMipmap, rhs.flags, rhs.wrapS, rhs.wrapT))
            {
                return (inputSource->getURI() < rhs.inputSource->getURI());
            }
            else
            {
                return std::tie(useMipmap, flags, wrapS, wrapT) < std::tie(rhs.useMipmap, rhs.flags, rhs.wrapS, rhs.wrapT);
            }
        }
    };
}

namespace chr = chronotext;
