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
    public:
        typedef std::shared_ptr<class ResourceItem> Ref;

        chr::InputSource::Ref inputSource;
        unsigned int memoryUsed;

        ResourceItem(InputSource::Ref _inputSource);
        
        ci::fs::path getFilePathHint() const;
        std::string getURI() const;
    };
}

namespace chr = chronotext;
