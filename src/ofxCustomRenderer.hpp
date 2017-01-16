//
//  ofxCustomRenderer.hpp
//  ofxCustomRenderer
//
//  Created by Yuma Taesu on 2016/12/29.
//
//

#pragma once
#include "ofMain.h"
//#include "CRHelper.h"
#include "gBuffer.hpp"


#include "pointLight.h"


class gBuffer;

class ofxCustomRenderer
{
    
private:
    gBuffer *m_gbuffer;
    
    ofCamera *m_cam;
    ofMatrix4x4 m_viewMatrix;
    
    ofMesh quad;
    
    GLuint m_ssaoColorBuffer, m_ssaoColorBufferBlur;
    GLuint m_noiseTexture;
    GLuint m_ssaoFbo, m_ssaoBlurFBO;
    vector<ofVec3f> m_ssaoKernel;
    int m_KarnelSample;
    
    
    ofShader m_SSAOPass, m_SSAOBlurPass;
    GLfloat lerp(GLfloat a, GLfloat b, GLfloat f)
    {
        return a + f * (b - a);
    }
    
public:
    ofShader GeometryPass, LightingPass;
    
    ofxCustomRenderer();
    ~ofxCustomRenderer();
    void setup(int width, int height);
    
    void bindGeometry(ofCamera &cam_, PointLight &pointlight);
    void unbindGeometry();
    
    void bindLighting();
    void unbindLighting();
    void uniformLights(PointLight &pointlight);
    void uniformLights(vector<PointLight> &pointlight);
    
    void uniformTexture(const string str, const ofTexture &tex, int location);
};
