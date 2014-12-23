/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/cinder/CinderApp.h"
#include "chronotext/utils/Utils.h"

using namespace std;
using namespace ci;
using namespace app;

namespace chr
{
    void CinderApp::setup()
    {
        sketch->setIOService(io_service());
        sketch->setup(false);
    }
    
    void CinderApp::shutdown()
    {
        stop(CinderSketch::FLAG_FOCUS_LOST);
        sketch->shutdown();
        delete sketch;
    }
    
    void CinderApp::resize()
    {
        sketch->resize();
        
        if (startCount == 0)
        {
            start(CinderSketch::FLAG_FOCUS_GAINED);
            startCount++;
        }
    }
    
    void CinderApp::update()
    {
        double now = getElapsedSeconds();
        
        if (ticks == 0)
        {
            t0 = now;
        }
        
        ticks++;
        elapsed = now - t0;
        
        // ---
        
        sketch->clock().update();
        sketch->update();
        updateCount++;
    }
    
    void CinderApp::draw()
    {
        if (updateCount == 0)
        {
            update(); // HANDLING CASES WHERE draw() IS INVOKED BEFORE update()
        }
        
        sketch->draw();
    }
    
    void CinderApp::mouseDown(MouseEvent event)
    {
        sketch->addTouch(0, event.getX(), event.getY());
    }
    
    void CinderApp::mouseDrag(MouseEvent event)
    {
        sketch->updateTouch(0, event.getX(), event.getY());
    }
    
    void CinderApp::mouseUp(MouseEvent event)
    {
        sketch->removeTouch(0, event.getX(), event.getY());
    }
    
    void CinderApp::touchesBegan(TouchEvent event)
    {
        for (vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt)
        {
            sketch->addTouch(touchIt->getId() - 1, touchIt->getPos().x, touchIt->getPos().y);
        }
    }
    
    void CinderApp::touchesMoved(TouchEvent event)
    {
        for (vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt)
        {
            sketch->updateTouch(touchIt->getId() - 1, touchIt->getPos().x, touchIt->getPos().y);
        }
    }
    
    void CinderApp::touchesEnded(TouchEvent event)
    {
        for (vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt)
        {
            sketch->removeTouch(touchIt->getId() - 1, touchIt->getPos().x, touchIt->getPos().y);
        }
    }
    
    void CinderApp::accelerated(AccelEvent event)
    {
        sketch->accelerated(event);
    }
    
    void CinderApp::sendMessageToSketch(int what, const string &body)
    {
        sketch->sendMessage(Message(what, body));
    }
    
    void CinderApp::start(int flags)
    {
        ticks = 0;

        sketch->clock().start();
        sketch->start(flags);
    }
    
    void CinderApp::stop(int flags)
    {
        sketch->clock().stop();
        sketch->stop(flags);

        LOGI << "AVERAGE FRAME-RATE: " << ticks / elapsed << " FPS" << endl;
    }
}
