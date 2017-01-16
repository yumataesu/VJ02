#include "ofApp.h"

ofxAlembic::Reader abc, jinnan_abc, virus_abc;

//--------------------------------------------------------------
void ofApp::setup()
{
    final_out_fbo.allocate(WIDTH, HEIGHT);
    
    ofDisableArbTex();
    customrenderer.setup(WIDTH, HEIGHT);
    
    cam.setNearClip(0.1f);
    cam.setFarClip(400.0f);
    
    abc.open("abc/metaball1.abc");
    abc.dumpNames();
    jinnan_abc.open("abc/jinnanacid.abc");
    jinnan_abc.dumpNames();
    virus_abc.open("abc/virus.abc");
    virus_abc.dumpNames();
    
    box.set(15.0f);
    vbomesh = box.getMesh();
    sphere.set(5.0f, 10.0f);
    
    gui.setup();
    
    gui.add(box_size.setup("BoxSize", 0.2, 0.0, 2.0));
    gui.add(box_rot.setup("BoxRotate", 0.2, 0.0, 1.0));
    gui.add(box_speed.setup("BoxSpeed", 0.2, 0.0, 1.0));
    
    gui.add(text_size.setup("TextSize", 0.2, 0.0, 1.0));
    gui.add(metaball_size.setup("MetaballSize", 0.2, 0.0, 1.0));
    gui.add(virus_size.setup("VirusSize", 0.2, 0.0, 1.0));
    
    gui.add(cam_fov.setup("CamFov", 80.0, 40.0, 120.0));
    gui.add(cam_dist.setup("CamDist", 0.2, 0.0, 1.0));
    
    gui.add(light_r.setup("lightColorR", 0.3, 0.0, 1.0));
    gui.add(light_g.setup("lightColorG", 0.3, 0.0, 1.0));
    gui.add(light_b.setup("lightColorB", 0.3, 0.0, 1.0));
    gui.add(cam_bang.setup("cambang"));
    gui.add(box_bang.setup("boxBang"));
    
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, WIDTH, HEIGHT);
    
    quad.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    quad.addVertex(ofVec3f(1.0, 1.0, 0.0));
    quad.addTexCoord(ofVec2f(1.0f, 1.0f));
    quad.addVertex(ofVec3f(1.0, -1.0, 0.0));
    quad.addTexCoord(ofVec2f(1.0f, 0.0f));
    quad.addVertex(ofVec3f(-1.0, -1.0, 0.0));
    quad.addTexCoord(ofVec2f(0.0f, 0.0f));
    quad.addVertex(ofVec3f(-1.0, 1.0, 0.0));
    quad.addTexCoord(ofVec2f(0.0f, 1.0f));
    
    pass.load("shaders/pass");
    blurShader.load("shaders/bloom/blur");
    bloomFinalShader.load("shaders/bloom/bloom_final");
    
    float R = 300.0f;
    for(int i = 0; i < NUM; i++)
    {
        pos[i] = ofVec3f(ofRandom(-200, 200), ofRandom(-200, 200), ofRandom(-R, R));
    }
    
    R = 1.0f;
    pointlight.resize(LightNUM);
    for(int i = 0; i < LightNUM; i++)
    {
        pointlight[i].setAmbient(0.0, 0.0, 0.0);
        pointlight[i].setDiffuse(ofRandom(0.7, 1.0), ofRandom(0.7, 1.0), ofRandom(0.7, 1.0));
        pointlight[i].setSpecular(1.0, 1.0, 1.0);
        pointlight[i].setPosition(ofRandom(-R, R), ofRandom(-R, R), ofRandom(-R, R));
        //pointlight[i].setShadowMap(1.0f, 100.0f, 1024, 1024);
    }
    
    glGenFramebuffers(1, &hdrFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    
    glGenTextures(2, colorBuffers);
    for (GLuint i = 0; i < 2; i++)
    {
        glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WIDTH, HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // attach texture to framebuffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
    }
    
    // - Create and attach depth buffer (renderbuffer)
    GLuint rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIDTH, HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
    // - Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);
    // - Finally check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    // Ping pong framebuffer for blurring
    
    glGenFramebuffers(2, pingpongFBO);
    glGenTextures(2, pingpongColorbuffers);
    for (GLuint i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
        glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WIDTH, HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // We clamp to the edge as the blur filter would otherwise sample repeated texture values!
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
        // Also check if framebuffers are complete (no need for depth buffer)
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer not complete!" << std::endl;
    }
    
    
    mainOutputSyphonServer.setName("Screen Output");
    individualTextureSyphonServer.setName("Texture Output");
    mClient.setup();
    //using Syphon app Simple Server, found at http://syphon.v002.info/
    mClient.set("","Simple Server");
    
    angle = 0;
}

//--------------------------------------------------------------
void ofApp::update()
{
    time = ofGetElapsedTimef();
    float t = fmodf(time, abc.getMaxTime());
    float jinnan_t = fmodf(time, jinnan_abc.getMaxTime());
    float virus_t = fmodf(time, virus_abc.getMaxTime());

    for(int i = 0; i < LightNUM; i++)
    {
        pointlight[i].setDiffuse(light_r, light_g, light_b);
    }
    
    
    cam.setFov(cam_fov);
    // update alemblic reader with time in sec
    abc.setTime(t);
    jinnan_abc.setTime(jinnan_t);
    virus_abc.setTime(virus_t);
    
    
    for(int i = 0; i < LightNUM; i++)
    {
        pointlight[i].setPosition(100*sin(time)*ofNoise(time*0.5+0.1),
                                  100*cos(time)*ofNoise(time*0.5+0.2),
                                  100*sin(time)*ofNoise(time*0.5+0.6));
    }
    
    
    
    //BOX Size
    if(box_bang)
    {
        for(int i = 0; i < NUM; i++)
        {
            cube_offset[i] = ofVec3f(ofRandom(4.0f), ofRandom(6.0f), ofRandom(4.0f));
        }
    }
    
    for(int i = 0; i < NUM; i++)
    {
        cube_d[i] = cube_offset[i] - cube_value[i];
        cube_value[i] += cube_d[i] * 0.3;
    }
    
    
    //CAMERA
    if(cam_bang)
    {
        cam_offset = ofMap(cam_dist, 0.0, 1.0, 30.0, 110.0);
    }
    
    cam_d = cam_offset - cam_value;
    cam_value += cam_d * 0.3;
    
    cam.orbit(time*10.0, 3.0, cam_value);
}

//--------------------------------------------------------------
void ofApp::draw()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    
    cam.begin();
    ofMatrix4x4 viewMatrix = ofGetCurrentViewMatrix();
    cam.end();
    
    
    //1 - GeometryPass--------------------------------------------------------
    customrenderer.bindGeometry(cam, pointlight[0]);
    
    angle += box_rot * 10.0;
    
    
    //CUBES
    for(int i = 0; i < NUM; i++)
    {
        pos[i].z += box_speed * 4.0;
        ofMatrix4x4 model;
        model.scale(box_size*cube_value[i].x, box_size*cube_value[i].y, box_size*cube_value[i].z);
        model.rotate(angle + i, 1.0, 0.4, 0.1);
        model.translate(pos[i]);
        
        customrenderer.GeometryPass.setUniformMatrix4f("model", model);
        
        vbomesh.draw();
        
        if(pos[i].z > 300) pos[i].z = -300;
    }
    
    //TEXT
    {
        ofMatrix4x4 model;
        model.scale(text_size * 0.5, text_size * 0.5, text_size * 0.5);
        model.translate(-50, 5.0, 0.0);
        model.rotate(time * 20.0, 1.0f, 1.0f, 0.1f);
        
        customrenderer.GeometryPass.setUniformMatrix4f("model", model);
        
        ofMesh mesh;
        jinnan_abc.get("/MoText/MoTextShape", mesh);
        mesh.draw();
    }
    
    //VIRUS
    {
        ofMatrix4x4 model;
        model.scale(virus_size * 0.4, virus_size * 0.4, virus_size * 0.4);
        model.rotate(time * 20.0, 1.0f, -1.0f, 0.1f);
        model.translate(0.0, 0.0, 0.0);
        
        customrenderer.GeometryPass.setUniformMatrix4f("model", model);
        
        ofMesh mesh;
        virus_abc.get("/Sphere/SphereShape", mesh);
        mesh.draw();
    }
    
    //METABALL
    {
        ofMatrix4x4 model;
        model.scale(metaball_size * 0.10, metaball_size * 0.10, metaball_size * 0.10);
        model.rotate(time * 20.0, 1.0f, -1.0f, 0.1f);
        model.translate(0.0, 0.0, 0.0);
        
        customrenderer.GeometryPass.setUniformMatrix4f("model", model);
        
        ofMesh mesh;
        abc.get("/Metaball/MetaballShape", mesh);
        mesh.draw();
    }
    
    
    customrenderer.GeometryPass.setUniform1i("frag_bloom", 1);
    for(int i = 0; i < LightNUM; i++)
    {
        ofMatrix4x4 model;
        model.scale(0.1, 0.1, 0.1);
        model.translate(pointlight[i].getPosition());
        
        customrenderer.GeometryPass.setUniformMatrix4f("model", model);
        
        customrenderer.GeometryPass.setUniform3f("lightColor", pointlight[i].getDiffuse());
        sphere.draw();
    }
    
    customrenderer.GeometryPass.setUniform1i("frag_bloom", 0);
    customrenderer.unbindGeometry();
    
    
    
    //2 - Lighting Pass-------------------------------------------------
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    customrenderer.bindLighting();
    customrenderer.uniformLights(pointlight[0]);
    quad.draw(OF_MESH_FILL);
    customrenderer.unbindLighting();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    
    
    //3 - BloomPass(Blur)-----------------------------------------------
    bool horizontal = true, first_iteration = true;
    int amount = 10;
    blurShader.begin();
    for (int i = 0; i < amount; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        if(first_iteration)
        {
            blurShader.setUniformTexture("image", GL_TEXTURE_2D, colorBuffers[1], 0);
        }else{
            blurShader.setUniformTexture("image", GL_TEXTURE_2D, pingpongColorbuffers[!horizontal], 0);
        }
        quad.draw(OF_MESH_FILL);
        horizontal = !horizontal;
        blurShader.setUniform1i("horizontal", horizontal);
        if (first_iteration)
            first_iteration = false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    blurShader.end();
    
    
    
    //4 - BloomPass(Composite)------------------------------------------
    final_out_fbo.begin();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    bloomFinalShader.begin();
    bloomFinalShader.setUniformTexture("scene", GL_TEXTURE_2D, colorBuffers[0], 0);
    bloomFinalShader.setUniformTexture("bloomBlur", GL_TEXTURE_2D, pingpongColorbuffers[!horizontal], 1);
    quad.draw(OF_MESH_FILL);
    bloomFinalShader.end();
    final_out_fbo.end();
    
    
    glDisable(GL_DEPTH_TEST);
    final_out_fbo.draw(0, 0);
    gui.draw();
    
    
    

//    mainOutputSyphonServer.publishFBO(&final_out_fbo);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
    
}
