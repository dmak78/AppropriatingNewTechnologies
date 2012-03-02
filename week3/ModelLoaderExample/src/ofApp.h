#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"



class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	ofEasyCam cam;
	ofVboMesh mesh;
    ofVboMesh processedMesh;
	ofVec3f center;
	ofLight light;
    ofColor meshColor;
    float inflate;
    float frequency;
    float amplitude;
    ofShader shader;
	float focusDistance, aperture;
  

};
