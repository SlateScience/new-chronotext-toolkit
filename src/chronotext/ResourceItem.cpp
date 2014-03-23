//
//  ResourceItem.cpp
//  SlateMath
//
//  Created by Michael Boccara on 3/23/14.
//
//

#include "ResourceItem.h"

namespace chronotext {
    ResourceItem::ResourceItem(InputSourceRef _inputSource) :
        inputSource(_inputSource)
    {
    }
    
    ci::fs::path ResourceItem::getFilePathHint() const
    {
        return inputSource->getFilePathHint();
    }

    std::string ResourceItem::getURI() const
    {
        return inputSource->getURI();
    }
}