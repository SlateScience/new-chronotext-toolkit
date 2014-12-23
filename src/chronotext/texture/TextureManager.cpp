/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/texture/TextureManager.h"

using namespace std;
using namespace ci;

namespace chr
{
    Texture::Ref TextureManager::preloadTexture(const TextureRequest &textureRequest)
    {
        if (textureRequest.inputSource)
        {
            auto element = textures.find(textureRequest);
            
            if (element == textures.end())
            {
                auto texture = make_shared<Texture>(textureRequest); // CAN THROW
                textures[textureRequest] = texture;
                return texture;
            }
            else
            {
                return element->second;
            }
        }
        
        throw EXCEPTION(TextureManager, "INVALID INPUT-SOURCE");
    }
    
    bool TextureManager::discardTexture(const TextureRequest &textureRequest)
    {
        if (textureRequest.inputSource)
        {
            auto element = textures.find(textureRequest);
            
            if (element != textures.end())
            {
                element->second->discard();
                return true;
            }
        }
        
        return false;
    }
    
    Texture* TextureManager::getTexture(const TextureRequest &textureRequest)
    {
        if (textureRequest.inputSource)
        {
            auto element = textures.find(textureRequest);
            
            if (element != textures.end())
            {
                return element->second.get(); // TEXTURE CAN BE IN A "DISCARDED" STATE
            }
        }
        
        return nullptr;
    }
    
    void TextureManager::discardTextures()
    {
        for (auto &element : textures)
        {
            element.second->discard();
        }
    }
    
    void TextureManager::reloadTextures()
    {
        for (auto &element : textures)
        {
            element.second->reload();
        }
    }
}
