#pragma once

#include "ofMain.h"
#include "ofxCustomRenderer.hpp"
#include "pointLight.h"
#include "ofxAlembic.h"
#include "ofxSyphon.h"
#include "ofxGui.h"

#define NUM 1250
#define LightNUM 1

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void keyReleased(int key);
    
    const int WIDTH = ofGetWidth();
    const int HEIGHT = ofGetHeight();
    
    ofxCustomRenderer customrenderer;
    
    ofxPanel gui;
    ofxFloatSlider box_rot, box_size, box_speed;
    ofxFloatSlider metaball_size, text_size, virus_size;
    ofxFloatSlider light_r, light_g, light_b;
    ofxButton cam_bang, box_bang;
    
    ofxFloatSlider cam_fov, cam_dist;
    
    float angle;
    float time;
    
    
    vector<PointLight> pointlight;
    ofMesh quad;
    ofEasyCam cam;
    ofBoxPrimitive box;
    ofVboMesh vbomesh;
    ofSpherePrimitive sphere;
    ofVec3f pos[NUM];
    
    GLuint hdrFBO;
    GLuint colorBuffers[2];
    GLuint pingpongFBO[2];
    GLuint pingpongColorbuffers[2];
    ofShader blurShader, bloomFinalShader;
    ofShader pass;
    
    
    
    ofxSyphonServer mainOutputSyphonServer;
    ofxSyphonServer individualTextureSyphonServer;
    ofxSyphonClient mClient;
    ofFbo final_out_fbo;
    
    
    //easing
    float cam_value, cam_offset, cam_d;
    ofVec3f cube_value[NUM];
    ofVec3f cube_offset[NUM];
    ofVec3f cube_d[NUM];
    
};
