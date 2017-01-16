#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main(){
    ofGLFWWindowSettings settings;
    settings.setGLVersion(3, 3);
    settings.width = 1280;
    settings.height = 720;
    settings.numSamples = 0;
    settings.resizable = false;
    ofCreateWindow(settings);
    
    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    ofRunApp(new ofApp());

}
