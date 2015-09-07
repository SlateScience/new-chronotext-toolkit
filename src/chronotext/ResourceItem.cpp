//
//  ResourceItem.cpp
//  SlateMath
//
//  Created by Michael Boccara on 3/23/14.
//
//

#include "ResourceItem.h"

namespace chronotext {
    ResourceItem::ResourceItem()
    {
    }
    
    ResourceItem::ResourceItem(InputSourceRef _inputSource) :
    inputSource(_inputSource)
    {
    }
    
    ci::fs::path ResourceItem::getFilePathHint() const
    {
        return inputSource ? inputSource->getFilePathHint() : "";
    }

    std::string ResourceItem::getURI() const
    {
        return inputSource ? inputSource->getURI() : "";
    }
}