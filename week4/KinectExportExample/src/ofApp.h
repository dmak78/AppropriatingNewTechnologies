#pragma once

#include "ofMain.h"
#include "ofxKinect.h"


class ofApp : public ofBaseApp {
public:
	
	void setup();
	void update();
	void draw();
	void exit();
    void keyPressed(int key);
    
    vector<ofVboMesh> meshSnaps;
    vector<float> meshSnapsRotationY;
    vector<float> meshSnapsRotationX;
    vector<float> meshSnapsRotationZ;
    
    ofMesh mesh;
    ofEasyCam cam;
    float rotationY;
    float rotationX;
    float rotationZ;
    
    int step;
	ofLight light;
	ofxKinect kinect;
    
    bool takeSnap;
    
    ofShader shader;
	float focusDistance, aperture;

};
