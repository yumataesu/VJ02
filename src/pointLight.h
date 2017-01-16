//
//  pointLight.h
//  test
//
//  Created by Yuma Taesu on 2017/01/12.
//
//

//
//  pointLight.h
//  ofxCustomRenderer
//
//  Created by Yuma Taesu on 2016/12/29.
//
//

#pragma once
#include "ofMain.h"


class PointLight : public ofNode {
public:
    PointLight() :
    intensity(1.0f)
    {
        sphere.set(10, 10);
        ambient = ofVec3f(0.0, 0.0, 0.0);
        diffuse = ofVec3f(0.0, 0.0, 0.0);
        specular = ofVec3f(0.0, 0.0, 0.0);
        attenuation = ofVec3f(0.0, 0.0);
    }
    
    
    //Memo::WIP

//    void setShadowMap(float near, float far, const int SHADOW_WIDTH = 1024, const int SHADOW_HEIGHT = 1024)
//    {
//        m_near = near;
//        m_far = far;
//        m_shadowMapshader.load("shaders/shadow/point_shadows_depth.vert",
//                               "shaders/shadow/point_shadows_depth.frag",
//                               "shaders/shadow/point_shadows_depth.geom");
//        
//        glGenFramebuffers(1, &m_depthMapFBO);
//        // Create depth cubemap texture
//        
//        
//        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
//        glGenTextures(1, &m_depthCubemap);
//        glBindTexture(GL_TEXTURE_CUBE_MAP, m_depthCubemap);
//        for (GLuint i = 0; i < 6; ++i)
//        {
//            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
//        }
//        
//        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//        // Attach cubemap as depth map FBO's color buffer
//        glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
//        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthCubemap, 0);
//        glDrawBuffer(GL_NONE);
//        glReadBuffer(GL_NONE);
//        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//            cout << "Framebuffer not complete!" << endl;
//        glBindFramebuffer(GL_FRAMEBUFFER, 0);
//        
//        
//        m_lightProjectionMatrix.makePerspectiveMatrix(90, (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near, far);
//    }
//    
//    
//    void CulcViewProjectionMatrix()
//    {
//        ofVec3f lightPos = this->getPosition();
//        
//        m_lightViewMatrix[0].makeLookAtViewMatrix(lightPos, lightPos + ofVec3f( 1.0,  0.0,  0.0), ofVec3f(0.0, -1.0,  0.0));
//        m_lightViewMatrix[1].makeLookAtViewMatrix(lightPos, lightPos + ofVec3f(-1.0,  0.0,  0.0), ofVec3f(0.0, -1.0,  0.0));
//        m_lightViewMatrix[2].makeLookAtViewMatrix(lightPos, lightPos + ofVec3f( 0.0,  1.0,  0.0), ofVec3f(0.0,  0.0,  1.0));
//        m_lightViewMatrix[3].makeLookAtViewMatrix(lightPos, lightPos + ofVec3f( 0.0, -1.0,  0.0), ofVec3f(0.0,  0.0, -1.0));
//        m_lightViewMatrix[4].makeLookAtViewMatrix(lightPos, lightPos + ofVec3f( 0.0,  0.0,  1.0), ofVec3f(0.0, -1.0,  0.0));
//        m_lightViewMatrix[5].makeLookAtViewMatrix(lightPos, lightPos + ofVec3f( 0.0,  0.0, -1.0), ofVec3f(0.0, -1.0,  0.0));
//        
//        for(int i = 0; i < 6; i++)
//            m_lightViewProjectionMatrix[i] = m_lightViewMatrix[i] * m_lightProjectionMatrix;
//    }
//    
//    
//    void beginShadow()
//    {
//        glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
//        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
//        glViewport(0.0, 0.0, 1024, 1024);
//        
//        m_shadowMapshader.begin();
//        m_shadowMapshader.setUniform1f("far", m_far);
//        m_shadowMapshader.setUniform3f("lightPos", this->getPosition());
//        for(int i = 0; i < 6; i++)
//            m_shadowMapshader.setUniformMatrix4f("light["+ to_string(i) +"].viewProjectionMatirx", m_lightViewProjectionMatrix[i]);
//        
//    }
//    
//    void endShadow()
//    {
//        m_shadowMapshader.end();
//        glBindFramebuffer(GL_FRAMEBUFFER, 0);
//        
//    }
    
    
    void setAmbient(float r, float g, float b)
    {
        ambient = ofVec3f(r, g, b);
    }
    
    
    void setDiffuse(float r, float g, float b, float a=1.0f)
    {
        diffuse = ofVec3f(r, g, b);
    }
    
    
    void setSpecular(float r, float g, float b, float a=1.0f)
    {
        specular = ofVec3f(r, g, b);
    }
    
    
    void setAttenuation(float constant, float linear, float exponential)
    {
        attenuation = ofVec3f(constant, linear, exponential);
    }
    
//    Memo::WIP
//    GLuint getdepthCubemapID() const
//    {
//        return m_depthCubemap;
//    }
    
    
    ofVec3f getAmbient() const
    {
        return ambient;
    }
    
    
    ofVec3f getDiffuse() const
    {
        return diffuse;
    }
    
    
    ofVec3f getSpecular() const
    {
        return specular;
    }
    
    
    ofVec3f getAttenuation() const
    {
        return attenuation;
    }
    
    
    void draw() const
    {
        sphere.draw();
    }
    
    ofShader m_shadowMapshader;
    
private:
    
    ofVec3f ambient;
    ofVec3f diffuse;
    ofVec3f specular;
    ofVec3f attenuation;
    
    float intensity;
    ofSpherePrimitive sphere;
    
    //Memo::WIP
//    GLuint m_depthMapFBO, m_depthCubemap;
//    ofMatrix4x4 m_lightViewProjectionMatrix[6], m_lightViewMatrix[6], m_lightProjectionMatrix;
//    float m_near, m_far;
    
    ofVec3f orbitAxis;
};

