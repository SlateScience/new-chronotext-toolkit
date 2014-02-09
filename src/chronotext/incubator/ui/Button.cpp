/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/incubator/ui/Button.h"
#include "chronotext/text/TextHelper.h"
#include "chronotext/utils/GLUtils.h"

using namespace std;
using namespace ci;

namespace chronotext
{
    Button::Button(shared_ptr<Style> style)
    :
    Shape(style),
    Touchable(this, TYPE_CLICKABLE, style->hitExtra),
    style(style),
    delegate(NULL)
    {}

    float Button::getWidth()
    {
        if (autoWidth)
        {
            return paddingLeft + getContentWidth() + paddingRight;
        }
        else
        {
            return width;
        }
    }
    
    float Button::getHeight()
    {
        if (autoHeight)
        {
            return paddingTop + getContentHeight() + paddingBottom;
        }
        else
        {
            return height;
        }
    }
   
    vector<Touchable*> Button::getTouchables()
    {
        vector<Touchable*> touchables;
        touchables.push_back(this);
        return touchables;
    }
    
    Rectf Button::getTouchableBounds()
    {
        return getBounds();
    }
    
    void Button::draw()
    {
        Rectf bounds = getBounds();
        float inset = 0;
        
        if (style->borderColor[state])
        {
            inset = -1;
            
            gl::color(*style->borderColor[state]);
            gl::drawSolidRect(bounds);
        }
        
        if (style->backgroundColor[state])
        {
            gl::color(*style->backgroundColor[state]);
            gl::drawSolidRect(bounds.inflated(Vec2f(inset, inset)));
        }
        
        // ---
        
        if (icon)
        {
            gl::color(*style->color[state]);
            
            glPushMatrix();
            gl::translate(bounds.getCenter());
            gl::scale(icon->scale);
            
            icon->texture->begin();
            icon->texture->drawFromCenter();
            icon->texture->end();
            
            glPopMatrix();
        }
        else if (style->font)
        {
            gl::color(*style->color[state]);
            
            style->font->setSize(style->fontSize);
            TextHelper::drawTextInRect(style->font, NULL, text, bounds, style->snap);
            
            if (style->strikethrough[state])
            {
                TextHelper::drawStrikethroughInRect(style->font, text, bounds, style->snap);
            }
        }
    }
    
    void Button::touchStateChanged(Touchable *touchable, int nextState, int prevState)
    {}
    
    void Button::touchActionPerformed(Touchable *touchable, int action)
    {
        if (delegate)
        {
            if (action == Touchable::ACTION_CLICKED)
            {
                delegate->buttonClicked(tag);
            }
            else if (action == Touchable::ACTION_TOGGLED)
            {
                delegate->buttonToggled(tag);
            }
        }
    }
    
    float Button::getContentWidth()
    {
        if (icon)
        {
            return icon->getWidth();
        }
        else if (style->font)
        {
            style->font->setSize(style->fontSize);
            return TextHelper::getStringAdvance(style->font, text, style->snap);
        }
        else
        {
            return 0;
        }
    }
    
    float Button::getContentHeight()
    {
        if (icon)
        {
            return icon->getHeight();
        }
        else if (style->font)
        {
            style->font->setSize(style->fontSize);
            return style->font->getHeight();
        }
        else
        {
            return 0;
        }
    }
}
