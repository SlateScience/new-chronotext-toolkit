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

namespace chronotext
{
    Texture::Ref TextureManager::getTexture(const string &resourceName, bool useMipmap, TextureRequest::Flags flags)
    {
        return getTexture(InputSource::getResource(resourceName), useMipmap, flags);
    }
    
    Texture::Ref TextureManager::getTexture(InputSource::Ref inputSource, bool useMipmap, TextureRequest::Flags flags)
    {
        return getTexture(TextureRequest(inputSource, useMipmap, flags));
    }
    
    Texture::Ref TextureManager::getTexture(const TextureRequest &textureRequest, bool forceLoad)
    {
        auto it = textures.find(textureRequest);
        
        if (it != textures.end())
        {
            if (forceLoad)
            {
                it->second->reload();
            }
            
            return it->second;
        }
        else if (forceLoad)
        {
            auto texture = make_shared<Texture>(textureRequest);
            textures[textureRequest] = texture;
            return texture;
        }
        
        return Texture::Ref();
    }
    
    bool TextureManager::remove(const TextureRequest &textureRequest)
    {
        auto it = textures.find(textureRequest);
        
        if (it != textures.end())
        {
            textures.erase(it);
            return true;
        }
        
        return false;
    }
    
    void TextureManager::clear()
    {
        textures.clear();
    }
    
    void TextureManager::discard()
    {
        for (auto &entry : textures)
        {
            entry.second->discard();
        }
    }
    
    void TextureManager::reload()
    {
        for (auto &entry : textures)
        {
            entry.second->reload();
        }
    }
}
