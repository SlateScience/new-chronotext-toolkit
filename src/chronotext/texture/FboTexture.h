#include "chronotext/texture/Texture.h"
#include <cinder/gl/Fbo.h>

namespace chronotext
{
    typedef std::shared_ptr<class FboTexture> FboTextureRef;
    typedef std::shared_ptr<ci::gl::Fbo> FboRef;
    
    class FboTexture : public Texture
    {
    public:
        FboTexture(int width, int height);
        
        void bindFramebuffer();
        void unbindFramebuffer();
        
    protected:
        FboRef fboTarget;
    };
}

namespace chr = chronotext;
