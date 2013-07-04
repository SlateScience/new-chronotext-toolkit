#include "chronotext/texture/TextureAtlas.h"

#include "cinder/Xml.h"

using namespace ci;
using namespace std;

TextureAtlas::TextureAtlas(const string &resourceName, bool useMipmap)
{
    init(InputSource::getResource(resourceName), useMipmap);    
}

TextureAtlas::TextureAtlas(InputSourceRef inputSource, bool useMipmap)
{
    init(inputSource, useMipmap);
}

void TextureAtlas::init(InputSourceRef inputSource, bool useMipmap)
{
    XmlTree doc(inputSource->loadDataSource());
    
    string resourceName = doc.getChild("TextureAtlas").getAttributeValue<string>("imagePath");
    texture = make_shared<Texture>(InputSource::getResource(resourceName), useMipmap); // FIXME: WE SHOULD NOT ASSERT THAT inputSource IS OF "RESOURCE" TYPE
    
    float width = texture->getWidth();
    float height = texture->getHeight();
    
    // ---
    
    for (XmlTree::Iter spriteElement = doc.begin("TextureAtlas/sprite"); spriteElement != doc.end(); ++spriteElement)
    {
        string spritePath = spriteElement->getAttributeValue<string>("n");
        
        float x = spriteElement->getAttributeValue<float>("x");
        float y = spriteElement->getAttributeValue<float>("y");
        float w = spriteElement->getAttributeValue<float>("w");
        float h = spriteElement->getAttributeValue<float>("h");
        
        bool rotated = spriteElement->hasAttribute("r");
        
        float ox;
        float oy;
        float ow;
        float oh;
        
        if (rotated)
        {
            oy = spriteElement->getAttributeValue<float>("oX", 0);
            ox = spriteElement->getAttributeValue<float>("oY", 0);
            ow = spriteElement->getAttributeValue<float>("oW", h);
            oh = spriteElement->getAttributeValue<float>("oH", w);
        }
        else
        {
            ox = spriteElement->getAttributeValue<float>("oX", 0);
            oy = spriteElement->getAttributeValue<float>("oY", 0);
            ow = spriteElement->getAttributeValue<float>("oW", w);
            oh = spriteElement->getAttributeValue<float>("oH", h);
        }
        
        float tx1 = x / width;
        float ty1 = y / height;
        
        float tx2 = (x + w) / width;
        float ty2 = (y + h) / height;
        
        sprites[spritePath] = new Sprite(texture, w, h, ox, oy, ow, oh, rotated, tx1, ty1, tx2, ty2);
    }
}

TextureAtlas::~TextureAtlas()
{
    for (map<string, Sprite*>::iterator it = sprites.begin(); it != sprites.end(); ++it)
    {
        delete it->second;
    }
}

void TextureAtlas::unload()
{
    texture->unload();
}

void TextureAtlas::reload()
{
    texture->reload();
}

Sprite* TextureAtlas::getSprite(const string &path)
{
    map<string, Sprite*>::iterator it = sprites.find(path);
    
    if (it == sprites.end())
    {
        return NULL;
    }
    else
    {
        return it->second;
    }
}

vector<Sprite*> TextureAtlas::getAnimationSprites(const string &path) const
{
    vector<Sprite*> animationSprites;
    string pattern = path + "%d";
    
    for (map<string, Sprite*>::const_iterator it = sprites.begin(); it != sprites.end(); ++it)
    {
        int i = -1;
        sscanf((it->first).c_str(), pattern.c_str(), &i);
        
        if (i != -1)
        {
            animationSprites.push_back(it->second);
        }
    }
    
    return animationSprites;
}

void TextureAtlas::beginTexture()
{
    texture->begin();
}

void TextureAtlas::endTexture()
{
    texture->end();
}

void TextureAtlas::drawSprite(const string &path, float rx, float ry)
{
    sprites[path]->draw(rx, ry);
}

void TextureAtlas::drawSpriteFromCenter(const std::string &path)
{
    sprites[path]->drawFromCenter();
}
