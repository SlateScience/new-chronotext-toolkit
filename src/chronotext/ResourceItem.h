//
//  ResourceItem.cpp
//  SlateMath
//
//  Created by Michael Boccara on 3/23/14.
//
//

#pragma once

#include "chronotext/InputSource.h"

namespace chronotext
{
    class ResourceItem
    {
    protected:
        chr::InputSource::Ref inputSource;
        unsigned int memoryUsed;
        
    public:
        typedef std::shared_ptr<class ResourceItem> Ref;

        ResourceItem(InputSource::Ref _inputSource);
        
        ci::fs::path getFilePathHint() const;
        std::string getURI() const;
        size_t getMemoryUsed() const { return memoryUsed; }
    };
}

namespace chr = chronotext;
