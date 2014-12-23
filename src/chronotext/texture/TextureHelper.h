/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/texture/TextureData.h"

namespace chr
{
    class TextureHelper
    {
    public:
        static ci::gl::TextureRef loadTexture(const std::string &resourceName, bool useMipmap = false, TextureRequest::Flags flags = TextureRequest::FLAGS_NONE);
        static ci::gl::TextureRef loadTexture(InputSource::Ref inputSource, bool useMipmap = false, TextureRequest::Flags flags = TextureRequest::FLAGS_NONE);
        static ci::gl::TextureRef loadTexture(const TextureRequest &textureRequest);
        
        static TextureData fetchTextureData(const TextureRequest &textureRequest);
        static ci::gl::TextureRef uploadTextureData(const TextureData &textureData);
        
    protected:
        static void textureDeallocator(void *refcon);
        static bool isOverSized(const TextureRequest &textureRequest, const ci::Vec2i &size);
        
        static TextureData fetchTranslucentTextureData(const TextureRequest &textureRequest);
        static TextureData fetchPowerOfTwoTextureData(const TextureRequest &textureRequest);
    };
}


