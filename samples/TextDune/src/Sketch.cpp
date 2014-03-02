/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "Sketch.h"

#include "chronotext/utils/GLUtils.h"
#include "chronotext/utils/MathUtils.h"
#include "chronotext/font/xf/TextHelper.h"
#include "chronotext/path/SplinePath.h"

using namespace std;
using namespace ci;
using namespace chr;
using namespace chr::xf;

const float REFERENCE_H = 768;
const float TEXT_SIZE = 20;
const float GAP = 7;

const wstring text = L"if two dimensions are not enough for interaction, how about a single one?";

Sketch::Sketch(void *context, void *delegate)
:
CinderSketch(context, delegate)
{}

void Sketch::setup(bool renewContext)
{
    if (renewContext)
    {
        textureManager.discard();
        fontManager.discardTextures();
        
        textureManager.reload();
        fontManager.reloadTextures(); // NOT MANDATORY
    }
    else
    {
        strokeTexture = textureManager.getTexture("line.png", true, TextureRequest::FLAGS_TRANSLUCENT);
        font = fontManager.getCachedFont(InputSource::getResource("Georgia_Regular_64.fnt"), XFont::Properties2d());
    }
    
    // ---
    
    scale = getWindowHeight() / REFERENCE_H;
    
    path.setMode(FollowablePath::MODE_TANGENT);
    createDune(Vec2f(getWindowSize()) / scale);

    // ---
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
}

void Sketch::start(int flags)
{
    clock.start();
}

void Sketch::stop(int flags)
{
    clock.stop();
}

void Sketch::update()
{
    position = 600 + 325 * math<float>::sin(clock.getTime() * 1.75f);
}

void Sketch::draw()
{
    gl::clear(Color::white(), false);
    
    gl::setMatricesWindow(getWindowSize(), true);
    gl::scale(scale);

    // ---
    
    drawDune();
    
    // ---
    
    font->setSize(TEXT_SIZE);
    font->setColor(0, 0, 0, 0.85f);
    
    TextHelper::drawTextOnPath(*font, text, path, position, -GAP);
}

void Sketch::addTouch(int index, float x, float y)
{
    clock.stop();
}

void Sketch::removeTouch(int index, float x, float y)
{
    clock.start();
}

void Sketch::createDune(const Vec2f &size)
{
    const float coefs[] = {1.0f / 2, 1.0f / 4, 1.0f / 4 * 3, 1.0f / 2};
    const int slotCount = sizeof(coefs) / sizeof(float);
    
    float slotSize = size.x / (slotCount - 1);
    SplinePath spline;

    for (int i = 0; i < slotCount; i++)
    {
        spline.add(slotSize * i, coefs[i] * size.y);
    }

    path.clear();
    spline.flush(SplinePath::TYPE_BSPLINE, path, 3); // USING A TOLERANCE OF 3: REDUCING POINTS WHILE PRESERVING SMOOTHNESS
    
    // ---
    
    StrokeHelper::stroke(path, stroke, 8); // SEE line.png, USED FOR PSEUDO-ANTIALISING
    
    // ---
    
    vertices.clear();
    vertices.reserve(path.size() * 2);
    
    for (auto &point : path.getPoints())
    {
        vertices.emplace_back(point);
        vertices.emplace_back(point.x, REFERENCE_H);
    }
}

void Sketch::drawDune()
{
    gl::color(1, 0, 0, 0.5f);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vertices.data());
    glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size());
    glDisableClientState(GL_VERTEX_ARRAY);
    
    // ---
    
    gl::color(1, 1, 1, 1);
    
    strokeTexture->begin();
    stroke.draw();
    strokeTexture->end();
}