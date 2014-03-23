#include "Hairline.h"

using namespace std;

namespace chronotext
{
    Hairline::Hairline()
    {}
    
    Hairline::Hairline(TextureManager &textureManager, Type type, const WindowInfo &windowInfo)
    :
    type(type),
    contentScale(windowInfo.contentScale)
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
                throw;
        }
        
        texture = textureManager.getTexture(TextureRequest(InputSource::getResource(resourceName), false, TextureRequest::FLAGS_TRANSLUCENT).setWrap(GL_REPEAT, GL_CLAMP_TO_EDGE));
    }
    
    void Hairline::stroke(const chr::FollowablePath &path, float scale, float uOffset)
    {
        StrokeHelper::stroke(path, strip, 4 / contentScale / scale, (type == TYPE_NORMAL) ? 1 : 0.5f, uOffset);
    }
    
    void Hairline::stroke(const chr::FollowablePath &path, const vector<float> &offsets, float scale, float uOffset)
    {
        int size = offsets.size();
        
        if (size % 2 == 0)
        {
            strip.clear();
            
            for (int i = 0; i < size / 2; i++)
            {
                float offsetStart = offsets[i * 2];
                float offsetEnd = offsets[i * 2 + 1];
                
                StrokeHelper::stroke(path, offsetStart, offsetEnd, strip, 4 / contentScale / scale, (type == TYPE_NORMAL) ? 1 : 0.5f, uOffset);
            }
        }
    }

    void Hairline::draw()
    {
        texture->begin();
        strip.draw();
        texture->end();
    }
}
