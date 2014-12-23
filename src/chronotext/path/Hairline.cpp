#include "Hairline.h"

using namespace std;

namespace chr
{
    Hairline::Hairline()
    :
    type(TYPE_UNDEFINED)
    {}
    
    Hairline::Hairline(TextureManager &textureManager, Type type)
    :
    type(type)
    {
        string resourceName;
        
        switch (type)
        {
            case TYPE_NORMAL:
                resourceName = "hairline.png";
                break;
                
            case TYPE_DASHED:
                resourceName = "hairline_dashed.png";
                break;
                
            default:
                assert(false);
        }
        
        TextureRequest request(InputSource::getResource(resourceName), false, TextureRequest::FLAGS_TRANSLUCENT);
        request.setWrap(GL_REPEAT, GL_CLAMP_TO_EDGE);
        
        texture = textureManager.preloadTexture(request);
    }
    
    void Hairline::stroke(const FollowablePath &path, float scale, float uOffset)
    {
        if (type != TYPE_UNDEFINED)
        {
            StrokeHelper::stroke(path, strip, 4 / scale, (type == TYPE_NORMAL) ? 1 : 0.5f, uOffset);
        }
    }
    
    void Hairline::stroke(const FollowablePath &path, const vector<float> &offsets, float scale, float uOffset)
    {
        if (type != TYPE_UNDEFINED)
        {
            int size = offsets.size();
            
            if (size % 2 == 0)
            {
                strip.clear();
                
                for (int i = 0; i < size / 2; i++)
                {
                    float offsetBegin = offsets[i * 2];
                    float offsetEnd = offsets[i * 2 + 1];
                    
                    StrokeHelper::stroke(path, offsetBegin, offsetEnd, strip, 4 / scale, (type == TYPE_NORMAL) ? 1 : 0.5f, uOffset);
                }
            }
        }
    }
    
    void Hairline::draw()
    {
        if (type != TYPE_UNDEFINED)
        {
            texture->begin();
            strip.draw();
            texture->end();
        }
    }
}
