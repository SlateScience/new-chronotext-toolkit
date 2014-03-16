/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "FontLayer.h"

using namespace std;

namespace chronotext
{
    namespace zf
    {
        FontLayer::FontLayer() :
        began(0),
        glyphBatchCount(0)
        {}
        
        void FontLayer::begin()
        {
            if(began == 0)
            {
                fonts.clear();
            }
            began++;
        }
        
        void FontLayer::addFont(shared_ptr<VirtualFont> font)
        {
            if(!began)
                return;
            
            if(fonts.find(font) == fonts.end())
            {
                font->beginSequence(true);
                fonts.insert(font);
            }
        }

        void FontLayer::end()
        {
            began--;
            if(began == 0)
            {
                glyphBatchCount = 0;
                
                for(auto font : fonts)
                {
                    glyphBatchCount += font->getSequenceBatchCount();
                    font->endSequence();
                }
            }
        }
    }
}
