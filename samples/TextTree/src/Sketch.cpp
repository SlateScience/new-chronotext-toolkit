#include "Sketch.h"

#include "chronotext/utils/GLUtils.h"
#include "chronotext/utils/MathUtils.h"
#include "chronotext/font/xf/TextHelper.h"

using namespace std;
using namespace ci;
using namespace chr;
using namespace chr::xf;

const float SCALE = 768;
const float TEXT_SIZE = 20;
const float GROW_FACTOR = 1.133f;

Sketch::Sketch(void *context, void *delegate)
:
CinderSketch(context, delegate)
{}

void Sketch::setup(bool renewContext)
{
    if (renewContext)
    {
        textureManager.unload();
        fontManager.discardTextures();
        
        textureManager.reload();
        fontManager.reloadTextures(); // NOT MANDATORY
    }
    else
    {
        dot = textureManager.getTexture(InputSource::getResource("dot.png"), true, TextureRequest::FLAGS_TRANSLUCENT);
        font = fontManager.getCachedFont(InputSource::getResource("Georgia_Regular_64.fnt"), XFont::Properties2d());
    }
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
}

void Sketch::update()
{
    double now = getElapsedSeconds();

    r1 = 3 * math<float>::sin(now * 0.75f); // OSCILLATING BETWEEN -3 AND +3
    r2 = 15 + 9 * math<float>::sin(now * 1.5f); // OSCILLATING BETWEEN 6 AND 24
    r3 = 25 + 10 * math<float>::sin(now * 3); // OSCILLATING BETWEEN 15 AND 35
    r4 = 6 * math<float>::sin(now * 1.5f); // OSCILLATING BETWEEN -6 AND +6
    r5 = 30 + 9 * math<float>::sin(now * 1.25f); // OSCILLATING BETWEEN 21 AND 39
}

void Sketch::draw()
{
    gl::clear(Color::gray(1.0f), false);
    gl::setMatricesWindow(getWindowSize(), true);
    
    gl::translate(getWindowCenter());
    gl::scale(getWindowHeight() / SCALE);
    
    // ---
    
    vector<FontMatrix::Values> M;
    
    auto matrix = font->getMatrix();
    
    font->setSize(TEXT_SIZE);
    font->setColor(0, 0, 0, 0.85f);
    
    font->beginSequence();
    
    /*
     * THE BASE OF THE TREE IS AT THE BASE OF THE SCREEN
     */
    matrix->setTranslation(0, 384);
    
    matrix->rotateZ((-90 + r1) * D2R);
    TextHelper::drawTransformedText(*font, L"2 dimensions");
    
    matrix->rotateZ((r4) * D2R);
    matrix->scale(GROW_FACTOR);
    TextHelper::drawTransformedText(*font, L" are");
    
    matrix->push();
    matrix->rotateZ(+r2 * D2R);
    matrix->scale(GROW_FACTOR);
    TextHelper::drawTransformedText(*font, L" not enough");
    
    matrix->rotateZ(r3 * D2R);
    matrix->scale(GROW_FACTOR);
    TextHelper::drawTransformedText(*font, L" for");
    
    matrix->push();
    matrix->rotateZ(r2 * D2R);
    matrix->scale(GROW_FACTOR);
    TextHelper::drawTransformedText(*font, L" text trees ");
    M.push_back(matrix->m);

    matrix->pop();
    matrix->rotateZ(-r5 * D2R);
    matrix->scale(GROW_FACTOR);
    TextHelper::drawTransformedText(*font, L" interaction ");
    M.push_back(matrix->m);

    matrix->pop();
    matrix->rotateZ(-r5 * D2R);
    matrix->scale(GROW_FACTOR);
    TextHelper::drawTransformedText(*font, L" fine");
    
    matrix->rotateZ(r4 * D2R);
    matrix->scale(GROW_FACTOR);
    TextHelper::drawTransformedText(*font, L" for");
    
    matrix->push();
    matrix->rotateZ(-r3 * D2R);
    matrix->scale(GROW_FACTOR);
    TextHelper::drawTransformedText(*font, L" text trees ");
    M.push_back(matrix->m);
    
    matrix->pop();
    matrix->rotateZ(+r3 * D2R);
    matrix->scale(GROW_FACTOR);
    TextHelper::drawTransformedText(*font, L" fiction ");
    M.push_back(matrix->m);
    
    font->endSequence();
    
    // ---
    
    gl::color(1, 0, 0, 0.75f);
    dot->begin();
    
    for (auto &m : M)
    {
        glPushMatrix();
        glMultMatrixf(m.data());
        gl::scale(0.20f);
        
        dot->drawFromCenter();
        glPopMatrix();
    }
    
    dot->end();
}
