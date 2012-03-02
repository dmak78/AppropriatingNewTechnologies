#pragma once

#include "ofMain.h"
#include "LineArt.h"
#include "ofxKinect.h"
#include "ofxOpenCv.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
    void drawPointCloud();
	
	ofEasyCam cam;
	ofMesh mesh;
    //ofMesh kinectMesh;
	ofImage bg;
    ofxKinect kinect;
};
