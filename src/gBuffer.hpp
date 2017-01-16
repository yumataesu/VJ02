//
//  gBuffer.hpp
//  ofxCustomRenderer
//
//  Created by Yuma Taesu on 2016/12/29.
//
//

#pragma once
#include "ofMain.h"

class gBuffer
{
    
private:
    GLuint m_gBuffer;
    GLuint gPosition, gNormal, gAlbedo, gDepth;
    
    int width, height;
    
public:
    gBuffer();
    ~gBuffer();
    void genAndBindGBuffer(int width, int height);
    void bindGBuffer();
    void unbindGBuffer();
    
    GLuint getPosition() const;
    GLuint getNormal() const;
    GLuint getAlbedo() const;
    GLuint getDepth() const;
};
