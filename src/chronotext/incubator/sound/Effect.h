/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/InputSource.h"

#include "FMOD.hpp"

#include <memory>

class Effect
{
public:
    typedef std::shared_ptr<Effect> Ref;

    chr::InputSource::Ref inputSource;
    uint32_t effectId;
    FMOD::Sound *sound;
    
    Effect(chr::InputSource::Ref inputSource, uint32_t effectId, FMOD::Sound *sound);
    ~Effect();
    
    uint32_t getId() const;
    
    double getDuration();
    size_t getMemoryUsage();
};
