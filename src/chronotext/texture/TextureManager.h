/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/texture/Texture.h"

#include <map>

namespace chr
{
    class TextureManager
    {
    public:
        Texture::Ref preloadTexture(const TextureRequest &textureRequest); // CAN THROW
        bool discardTexture(const TextureRequest &textureRequest);

        /*
         * THE RETURNED POINTER IS NOT INTENDED FOR STORAGE
         */
        Texture* getTexture(const TextureRequest &textureRequest);

        void discardTextures();
        void reloadTextures();
        
    protected:
        std::map<TextureRequest, Texture::Ref> textures;
    };
}


