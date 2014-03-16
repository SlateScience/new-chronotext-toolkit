/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/font/zf/VirtualFont.h"
#include <set>

namespace chronotext
{
    namespace zf
    {
        class FontLayer
        {
            int began;
            std::set<std::shared_ptr<VirtualFont> > fonts;
            
            int glyphBatchCount;
            
        public:
            FontLayer();
            void begin();
            void addFont(std::shared_ptr<VirtualFont>);
            void end();
            
            int getFontCount() const {return fonts.size();}
            int getGlyphBatchCount() const {return glyphBatchCount;}
        };
    }
}

namespace chr = chronotext;
