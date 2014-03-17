/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/app/AppAndroid.h"

#include "chronotext/cinder/CinderSketchBase.h"

namespace chronotext
{
    class CinderDelegate;
    
    class CinderSketchComplex : public CinderSketchBase
    {
    public:
        CinderSketchComplex(void *context, void *delegate = NULL);
        
        void enableAccelerometer(float updateFrequency = 30, float filterFactor = 0.1f);
        void disableAccelerometer();
        
        std::ostream& console();
        boost::asio::io_service& io_service() const;
        
        double getElapsedSeconds() const;
        uint32_t getElapsedFrames() const;
        
        float getWindowDensity() const;
        int getWindowWidth() const;
        int getWindowHeight() const;
        ci::Vec2f getWindowCenter() const;
        ci::Vec2i getWindowSize() const;
        float getWindowAspectRatio() const;
        ci::Area getWindowBounds() const;
        float getWindowContentScale() const { return 1; }
        int getWindowAALevel() const { return 0; }
        
        void sendMessageToDelegate(int what, const std::string &body = "");
        
    protected:
        CinderDelegate *context;
        CinderDelegate *delegate;
    };
}

namespace chr = chronotext;
