/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/texture/Texture.h"

namespace chronotext
{
    class TextureManager;

    typedef std::shared_ptr<class Sprite> SpriteRef;
    
    class Sprite
    {
        TextureRef texture;
        InputSourceRef inputSource;
        TextureManager& textureManager;
        
    public:
        
        float w;
        float h;
        float ox;
        float oy;
        float ow;
        float oh;
        bool rotated;
        
        float tx1;
        float ty1;
        float tx2;
        float ty2;
        
        Sprite(InputSourceRef inputSource, TextureManager& textureManager); // IMAGE-BASED
        Sprite(InputSourceRef inputSource, TextureManager& textureManager,
               float w, float h, float ox, float oy, float ow, float oh, bool rotated, float tx1, float ty1, float tx2, float ty2); // ATLAS-BASED
        
        ci::Vec2f getSize() const;
        float getWidth() const;
        float getHeight() const;
        
        void beginTexture();
        void endTexture();
        
        void drawFromCenter();
        void draw(float rx = 0, float ry = 0);
        
        bool isLoaded() const;
        void purgeTexture();
        void loadTexture();
        InputSourceRef getInputSource() const;
        
    };
}

namespace chr = chronotext;
