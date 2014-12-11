/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/texture/TextureHelper.h"
#include "chronotext/texture/Texture.h"

#include <map>

namespace chronotext
{
    class TextureManager
    {
    public:
        Texture::Ref getTexture(const std::string &resourceName, bool useMipmap = false, TextureRequest::Flags flags = TextureRequest::FLAGS_NONE);
        Texture::Ref getTexture(InputSource::Ref inputSource, bool useMipmap = false, TextureRequest::Flags flags = TextureRequest::FLAGS_NONE);
        Texture::Ref getTexture(const TextureRequest &textureRequest, bool forceLoad = true);
        
        bool remove(const TextureRequest &textureRequest);
        void clear();
        
        void discard();
        void reload();
        
    protected:
        std::map<TextureRequest, Texture::Ref> textures;
    };
}

namespace chr = chronotext;
