#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "Clone.h"
#include "ofxFaceTracker.h"
#include "ofxFaceTrackerThreaded.h"

class testApp : public ofBaseApp {
public:
   
	void setup();
	void update();
	void draw();
	void dragEvent(ofDragInfo dragInfo);
	void loadFace(string face);
	void keyPressed(int key);

	ofxFaceTrackerThreaded camTracker;
	ofVideoGrabber cam;
	ofxFaceTracker srcTracker;
	ofImage src;
    ofImage camMemoryTexture;
    ofImage srcMemoryTexture;
	vector<ofVec2f> srcPoints;
    vector<ofVec2f> camPoints;

	bool cloneReady;
    bool srcCloneReady;
	Clone camClone;
    Clone srcClone;
	ofFbo srcFbo, srcMask, camFbo, camMask;

	ofDirectory faces;
	int currentFace;
    
    ofEasyCam easyCam;
    
    ofMesh camMesh;
    ofMesh srcMesh;
    
    float depthScale;
    int faceStrength;
    
    ofImage		image;
    
    int captureWidth;
    int captureHeight;
    
    ofVec2f position;
	float scale;
	ofVec3f orientation;
	ofMatrix4x4 rotationMatrix;

    bool srcNeedsSetting;
    bool camNeedsSetting;
    
    bool srcReady;
    bool camReady;


};
