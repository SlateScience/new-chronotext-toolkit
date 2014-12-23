/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/os/Handler.h"

using namespace std;

namespace chr
{
    Handler::Handler()
    :
    io(NULL)
    {}

    void Handler::setIOService(boost::asio::io_service &io)
    {
        this->io = &io;
    }
    
    bool Handler::sendMessage(const Message &message)
    {
        return post(bind(&Handler::handleMessage, this, message));
    }
}
