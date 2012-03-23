#include "testApp.h"
#include "ofMeshUtils.h"

extern "C" {
#include "macGlutfix.h"
}

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
#ifdef TARGET_OSX
	//ofSetDataPathRoot("../data/");
#endif
    ofSetFrameRate(60);
    captureWidth = 640;
    captureHeight = 480;
	image.allocate(captureWidth, captureHeight, OF_IMAGE_COLOR);
    camMemoryTexture.allocate(captureWidth, captureHeight, OF_IMAGE_COLOR);
    srcMemoryTexture.allocate(captureWidth, captureHeight, OF_IMAGE_COLOR);

	ofSetVerticalSync(true);
	cloneReady = false;
    srcCloneReady = false;
	cam.initGrabber(640, 480);
	camClone.setup(cam.getWidth(), cam.getHeight());
    srcClone.setup(captureWidth, captureHeight);
    cloneClone.setup(captureWidth, captureHeight);
	ofFbo::Settings settings;
	settings.width = cam.getWidth();
	settings.height = cam.getHeight();
	camMask.allocate(settings);
    srcMask.allocate(settings);
	srcFbo.allocate(settings);
    camFbo.allocate(settings);
	camTracker.setup();
	srcTracker.setup();
	//srcTracker.setIterations(10);
	//srcTracker.setAttempts(4);
    //camTracker.setIterations(10);
	//camTracker.setAttempts(4);
    
    camMask.begin();
    ofClear(0, 255);
    camMask.end();
    srcMask.begin();
    ofClear(0, 255);
    srcMask.end();
	srcFbo.begin();
    ofClear(0, 255);
    srcFbo.end();
    camFbo.begin();
    ofClear(0, 255);
    camFbo.end();

    srcNeedsSetting=true;
    camNeedsSetting=true;
    srcReady=true;
    camReady=true;

	faces.allowExt("jpg");
	faces.allowExt("png");
	faces.listDir("faces");
	currentFace = 0;
	if(faces.size()!=0){
		loadFace(faces.getPath(currentFace));
	}
}



void testApp::update() {
    
    faceStrength = ofMap(mouseX,0,ofGetWidth(),0,25);
    
    cam.update();
    
    captureWidth = 640;
    captureHeight = 480;
    
	unsigned char * data = pixelsBelowWindow(ofGetWindowPositionX(), ofGetWindowPositionY(), captureWidth, captureHeight);
	
	// now, let's get the R and B data swapped, so that it's all OK:
	for (int i = 0; i < captureWidth * captureHeight; i++){
		
		unsigned char r = data[i*4]; // mem A  
		
		data[i*4]   = data[i*4+1];   
		data[i*4+1] = data[i*4+2];   
		data[i*4+2] = data[i*4+3];   
		data[i*4+3] = r; 
	}
	
	if (data!= NULL) {
		image.setFromPixels(data, captureWidth, captureHeight, OF_IMAGE_COLOR_ALPHA, true);
		image.setImageType(OF_IMAGE_COLOR);
		image.update();
        src=image;
        if(src.getWidth() > 0){
            srcTracker.update(toCv(src));
        }
	}

	if(cam.isFrameNew()) {
		camTracker.update(toCv(cam));

        if(camTracker.getFound()){
            if(camReady){
                camNeedsSetting = true;
                camReady=false;
                camPoints.clear();
            }
            if(camNeedsSetting){
                camMemoryTexture.setFromPixels(cam.getPixels(), 640, 480, OF_IMAGE_COLOR);
                camPoints=camTracker.getImagePoints();
                camNeedsSetting=false;
            }
        }
        else{
            camReady=true;
        }
        
        if(srcTracker.getFound()){
            if(srcReady){
                srcNeedsSetting = true;
                srcReady=false;
                srcPoints.clear();
            }
            if(srcNeedsSetting){
                srcMemoryTexture.setFromPixels(src.getPixels(), 640, 480, OF_IMAGE_COLOR);
                srcPoints=srcTracker.getImagePoints();
                srcNeedsSetting=false;
            }
        }
        else{
            srcReady=true;
        }
        
    
        cloneReady = camTracker.getFound();
                   
        if(cloneReady){
            
            camMesh.clear();
			camMesh = camTracker.getImageMesh();
            camMesh.clearTexCoords();
			camMesh.addTexCoords(srcPoints);
            
            camMask.begin();
			ofClear(0, 255);
			camMesh.draw();
			camMask.end();
            
            if(srcCloneReady){
                srcFbo.begin();
                ofClear(0, 255);
                //src.bind();
                srcMemoryTexture.bind();
                camMesh.draw();
                srcMemoryTexture.unbind();
                //src.unbind();
                srcFbo.end();
            }
            else{
                srcFbo.begin();
                ofClear(0, 255);
                camMesh.draw();
                srcFbo.end(); 
            }

            camClone.setStrength(faceStrength);
			camClone.update(srcFbo.getTextureReference(), cam.getTextureReference(), camMask.getTextureReference());
        }
        else{
            camMesh.clear();
            camMask.begin();
			ofClear(255, 255);
			camMesh.draw();
			camMask.end();
            
            srcFbo.begin();
            ofClear(0, 255);
            camMesh.draw();
            srcFbo.end();
            
            camClone.setStrength(faceStrength);
			camClone.update(srcFbo.getTextureReference(), cam.getTextureReference(),camMask.getTextureReference());
        }
        
        
        srcCloneReady = srcTracker.getFound();
        
        if(srcCloneReady) {
            
//            srcPoints = srcTracker.getImagePoints();
//            position = srcTracker.getPosition();
//            scale = srcTracker.getScale();
//            orientation = srcTracker.getOrientation();
//            rotationMatrix = srcTracker.getRotationMatrix();
            
            
            srcMesh.clear();
           // camPoints = camTracker.getImagePoints();
            srcMesh = srcTracker.getImageMesh();
            srcMesh.clearTexCoords();
            srcMesh.addTexCoords(camPoints);
            
            srcMask.begin();
			ofClear(0, 255);
			srcMesh.draw();
			srcMask.end();
            
            camFbo.begin();
			ofClear(0, 255);
            //cam.getTextureReference().bind();
            camMemoryTexture.bind();
			srcMesh.draw();
			//cam.getTextureReference().unbind();
            camMemoryTexture.unbind();
			camFbo.end();

            srcClone.setStrength(faceStrength);
			srcClone.update(camFbo.getTextureReference(), src.getTextureReference(), srcMask.getTextureReference());
            
        }
        else{
            
            srcMesh.clear();
            srcMask.begin();
			ofClear(255, 255);
			srcMesh.draw();
			srcMask.end();
            
            camFbo.begin();
            ofClear(0, 255);
            srcMesh.draw();
            camFbo.end();
            
            srcClone.setStrength(faceStrength);
			srcClone.update(camFbo.getTextureReference(), src.getTextureReference(), srcMask.getTextureReference());
        }
	}
}

void testApp::draw() {
    ofBackground(0);
	ofSetColor(255);
    
   // cam.draw(640,0);
   // image.draw(0,0);
	
	if(srcCloneReady) {

        srcClone.draw(0,0);
        

    }
    else{
       srcClone.draw(0,0);
       //image.draw(0,0);
    }
    if(cloneReady) {
        camClone.draw(640, 0);
	}
    else{
        camClone.draw(640, 0);
    }
//    ofPushMatrix();
//    ofSetupScreenOrtho(1280, 480, OF_ORIENTATION_UNKNOWN, true, -1000, 1000);
//    ofTranslate(1280 / 2, 480 / 2);
//    applyMatrix(rotationMatrix);
//    ofScale(5,5,5);
//    src.getTextureReference().bind();
//    srcTracker.getObjectMesh().draw();
//    src.getTextureReference().unbind();
//    ofPopMatrix();
    
	if(!camTracker.getFound()) {
		drawHighlightString("camera face not found", 10, 10);
	}
	if(src.getWidth() == 0) {
		drawHighlightString("drag an image here", 10, 30);
	} else if(!srcTracker.getFound()) {
		drawHighlightString("image face not found", 10, 30);
	}
}

void testApp::loadFace(string face){
//	src.loadImage(face);
//	if(src.getWidth() > 0) {
//		srcTracker.update(toCv(src));
//		srcPoints = srcTracker.getImagePoints();
//	}
}

void testApp::dragEvent(ofDragInfo dragInfo) {
	loadFace(dragInfo.files[0]);
}

void testApp::keyPressed(int key){
	switch(key){
	case OF_KEY_UP:
		currentFace++;
        srcTracker.reset();
        camTracker.reset();
		break;
	case OF_KEY_DOWN:
		currentFace--;
        srcTracker.reset();
        camTracker.reset();
		break;
    case 'r':
        srcTracker.reset();
        camTracker.reset();
        camNeedsSetting=true;
        srcNeedsSetting=true;
        camReady=true;
        srcReady=true;
        break;
	}
	//currentFace = ofClamp(currentFace,0,faces.size());
//    if(currentFace >=faces.size()){
//        currentFace=0;
//    }
//	if(faces.size()!=0){
//		loadFace(faces.getPath(currentFace));
//	}
}
