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
    typedef std::shared_ptr<class ResourceItem> ResourceItemRef;
    
    class ResourceItem
    {
    protected:
        chr::InputSourceRef inputSource;
    public:
        ResourceItem();
        ResourceItem(InputSourceRef _inputSource);
        ci::fs::path getFilePathHint() const;
        std::string getURI() const;
    };
}

namespace chr = chronotext;
