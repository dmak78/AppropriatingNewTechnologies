#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "Clone.h"
#include "ofxFaceTracker.h"
#include "ofxFaceTrackerThreaded.h"

using namespace ofxCv;
using namespace cv;



class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);

	void loadFace(string face);

	
	ofVideoGrabber cam;
	ofxFaceTrackerThreaded tracker;
    ofxFaceTracker imgTracker;
    ofImage imgSrc;
    ofImage img1;
    ofImage img2;
    ofImage img3;
    
    ofDirectory faces;
	int currentFace;
    
	ofVec2f position;
	float scale;
	ofVec3f orientation;
	ofMatrix4x4 rotationMatrix;
	
	Mat translation, rotation;
	ofMatrix4x4 pose;
    
    Clone clone1;
    Clone clone2;
    Clone clone3;
    ofFbo img1Fbo, img2Fbo, img3Fbo, clone1Fbo, clone2Fbo, clone3Fbo, camMask, camFbo;
    
    ofMesh camMesh, img1Mesh, img2Mesh, img3Mesh;
    vector<ofVec2f> imagePoints[3];

	ofEasyCam easyCam;
    
    int newPositionX;
    int newPositionY;
    
    bool followMouse;
    
    ofLight light;
    
    vector<float> meltSpeed;
    
    bool isMelting;
    
    bool timeToMelt;
    
};
