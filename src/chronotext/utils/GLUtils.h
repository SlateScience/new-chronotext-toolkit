/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Camera.h"

namespace chr
{
    void bindTexture(ci::gl::Texture *texture);
    void unbindTexture();
    
    void beginTexture(ci::gl::Texture *texture);
    void endTexture();
    
    void drawTextureFromCenter(ci::gl::Texture *texture);
    void drawTexture(ci::gl::Texture *texture, float rx = 0, float ry = 0);
    void drawTextureInRect(ci::gl::Texture *texture, const ci::Rectf &rect, float ox = 0, float oy = 0);
    
    ci::Matrix44f getPerspectiveMatrix(float fovy, float aspect, float zNear, float zFar);
    ci::Matrix44f getPerspectiveMatrix(float fovy, float zNear, float zFar, float width, float height, float panX = 0, float panY = 0, float zoom = 1);
    ci::Matrix44f getFrustumMatrix(float left, float right, float bottom, float top, float znear, float zfar);
    
    void drawGrid(const ci::Rectf &bounds, float size, const ci::Vec2f &offset = ci::Vec2f::zero());
    void drawGrid(const ci::Rectf &bounds, float sx, float sy, const ci::Vec2f &offset = ci::Vec2f::zero());
    
    void drawFullScreenQuad();
    void drawVerticalGradient(const ci::Rectf &bounds, const ci::ColorA &color1, const ci::ColorA &color2, float v1 = 0, float v2 = 1);
    
    void dumpCamera(const ci::Camera &cam, const std::string &name = "cam");
}

namespace cinder
{
    namespace gl
    {
        inline void scale(float s) { glScalef(s, s, 1); }
    }
}


