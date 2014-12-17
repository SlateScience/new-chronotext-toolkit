/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/Exception.h"
#include "chronotext/texture/TextureData.h"

namespace chronotext
{
    class Texture
    {
    public:
        typedef std::shared_ptr<Texture> Ref;

        TextureRequest request;

        Texture(InputSource::Ref inputSource, bool useMipmap = false, TextureRequest::Flags flags = TextureRequest::FLAGS_NONE);
        Texture(const TextureRequest &textureRequest);
        Texture(const TextureData &textureData);

        Texture();
        ~Texture();

        void discard();
        void reload();
        
        TextureData fetchTextureData();
        void uploadTextureData(const TextureData &textureData);
        
        uint32_t getId() const;

        int getWidth() const;
        int getHeight() const;
        ci::Vec2i getSize() const;

        int getCleanWidth() const;
        int getCleanHeight() const;
        ci::Vec2i getCleanSize() const;

        float getMaxU() const;
        float getMaxV() const;
        ci::Vec2f getMaxUV() const;
        
        size_t getMemoryUsage() const; // XXX: TEMPORARY, UNTIL A MORE RECENT VERSION OF new-chronotext-toolkit IS MERGED
        
        void bind(bool forceload = true);
        void unbind();
        
        void begin(bool forceload = true);
        void end();
        
        void drawFromCenter();
        void draw(float rx = 0, float ry = 0);
        void drawInRect(const ci::Rectf &rect, float ox = 0, float oy = 0);
        
    protected:
        ci::gl::TextureRef target;
        
        uint32_t textureId;
        int width;
        int height;
        float maxU;
        float maxV;
        
        void setTarget(ci::gl::TextureRef texture);
        void resetTarget();
    };
}

namespace chr = chronotext;
