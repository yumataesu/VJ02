//
//  ofxCustomRenderer.cpp
//  ofxCustomRenderer
//
//  Created by Yuma Taesu on 2016/12/29.
//
//

#include "ofxCustomRenderer.hpp"


ofxCustomRenderer::ofxCustomRenderer()
: m_cam(m_cam)
, m_viewMatrix(m_viewMatrix)
, m_gbuffer(m_gbuffer)
{
}

ofxCustomRenderer::~ofxCustomRenderer()
{
}


void ofxCustomRenderer::setup(int width, int height)
{
    m_gbuffer = new gBuffer();
    
    GeometryPass.load("shaders/gBuffer");
    LightingPass.load("shaders/lighting");
    
    m_gbuffer->genAndBindGBuffer(width, height);
    
    quad.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    quad.addVertex(ofVec3f(1.0, 1.0, 0.0));
    quad.addTexCoord(ofVec2f(1.0f, 1.0f));
    quad.addVertex(ofVec3f(1.0, -1.0, 0.0));
    quad.addTexCoord(ofVec2f(1.0f, 0.0f));
    quad.addVertex(ofVec3f(-1.0, -1.0, 0.0));
    quad.addTexCoord(ofVec2f(0.0f, 0.0f));
    quad.addVertex(ofVec3f(-1.0, 1.0, 0.0));
    quad.addTexCoord(ofVec2f(0.0f, 1.0f));
}


void ofxCustomRenderer::bindGeometry(ofCamera &cam_, PointLight &pointlight)
{
    m_cam = &cam_;
    m_cam->begin();
    m_viewMatrix = ofGetCurrentViewMatrix();
    m_cam->end();
    
    m_gbuffer->bindGBuffer();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    GeometryPass.begin();
    GeometryPass.setUniformMatrix4f("view", m_viewMatrix);
    GeometryPass.setUniformMatrix4f("projection", m_cam->getProjectionMatrix());
    GeometryPass.setUniform3f("lightPos", pointlight.getPosition());
    GeometryPass.setUniform1f("near", m_cam->getNearClip());
    GeometryPass.setUniform1f("far", m_cam->getFarClip());
    //GeometryPass.setUniformTexture("depthCubemap", GL_TEXTURE_CUBE_MAP, pointlight.getdepthCubemapID(), 3);
}


void ofxCustomRenderer::unbindGeometry()
{
    GeometryPass.end();
    m_gbuffer->unbindGBuffer();
}



void ofxCustomRenderer::uniformTexture(const string str, const ofTexture &tex, int location)
{
    GeometryPass.setUniformTexture(str, tex, location);
}


void ofxCustomRenderer::bindLighting()
{
    LightingPass.begin();
    LightingPass.setUniformTexture("gPosition", GL_TEXTURE_2D, m_gbuffer->getPosition(), 0);
    LightingPass.setUniformTexture("gNormal", GL_TEXTURE_2D, m_gbuffer->getNormal(), 1);
    LightingPass.setUniformTexture("gAlbedo", GL_TEXTURE_2D, m_gbuffer->getAlbedo(), 2);
    LightingPass.setUniformTexture("ssao", GL_TEXTURE_2D, m_ssaoColorBufferBlur, 3);
    LightingPass.setUniform3f("ViewPos", m_cam->getPosition());
}


void ofxCustomRenderer::uniformLights(PointLight &pointlight)
{
    ofVec3f lightPos =  pointlight.getPosition() * m_viewMatrix;
    
    LightingPass.setUniform3f("light[0].lposition", lightPos);
    LightingPass.setUniform3f("light[0].ambient", pointlight.getAmbient());
    LightingPass.setUniform3f("light[0].diffuse", pointlight.getDiffuse());
    LightingPass.setUniform3f("light[0].specular", pointlight.getSpecular());
    LightingPass.setUniform3f("light[0].position", pointlight.getPosition());
}


void ofxCustomRenderer::uniformLights(vector<PointLight> &pointlight)
{
    int lightNum = pointlight.size();
    
    for(int i = 0; i < lightNum; i++)
    {
        ofVec3f lightPos =  pointlight[i].getPosition() * m_viewMatrix;
        
        LightingPass.setUniform3fv("light["+to_string(i)+"].position", &lightPos[0]);
        LightingPass.setUniform3fv("light["+to_string(i)+"].ambient", &pointlight[i].getAmbient()[0], 1);
        LightingPass.setUniform3fv("light["+to_string(i)+"].diffuse", &pointlight[i].getDiffuse()[0], 1);
        LightingPass.setUniform3fv("light["+to_string(i)+"].specular", &pointlight[i].getSpecular()[0], 1);
    }
}


void ofxCustomRenderer::unbindLighting()
{
    LightingPass.end();
}
