#include "chronotext/texture/FboTexture.h"
#include <cinder/gl/Fbo.h>

using namespace std;
using namespace ci;

namespace chronotext
{
    FboTexture::FboTexture(int width, int height)
    {
        fboTarget = make_shared<gl::Fbo>(width, height);

        ci::gl::Texture& texture = fboTarget->getTexture();
        setTarget(&texture);
    }

    void FboTexture::bindFramebuffer()
    {
        if (fboTarget) {
            fboTarget->bindFramebuffer();

            gl::setMatricesWindow(width, height, false);
            
        }
    }
    
    void FboTexture::unbindFramebuffer()
    {
        if (fboTarget) {
            fboTarget->unbindFramebuffer();
        }
    }
}


