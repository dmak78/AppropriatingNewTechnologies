#include "testApp.h"

using namespace ofxCv;

void testApp::setup() {
#ifdef TARGET_OSX
	//ofSetDataPathRoot("../data/");
#endif
	ofSetVerticalSync(true);
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
	
	cam.initGrabber(640, 480);
    clone1.setup(cam.getWidth(), cam.getHeight());
    clone2.setup(cam.getWidth(), cam.getHeight());
    clone3.setup(cam.getWidth(), cam.getHeight());
    ofFbo::Settings settings;
	settings.width = cam.getWidth();
	settings.height = cam.getHeight();
    
    img1Fbo.allocate(settings);
    img2Fbo.allocate(settings);
    img3Fbo.allocate(settings);
    clone1Fbo.allocate(settings);
    clone2Fbo.allocate(settings);
    clone3Fbo.allocate(settings);
    camMask.allocate(settings);
    camFbo.allocate(settings);
    
	tracker.setup();
    imgTracker.setup();
    imgTracker.setIterations(25);
	imgTracker.setAttempts(4);
    //faces.allowExt("jpg");
	//faces.allowExt("png");
	//faces.listDir("../data/faces");
    
    followMouse = false;
    
    img1.loadImage("images/me01.jpg");
    imgTracker.update(toCv(img1));
    if(imgTracker.getFound()){
        imagePoints[0] = imgTracker.getImagePoints();
    }
    img2.loadImage("images/me00.jpg");
    imgTracker.update(toCv(img2));
    if(imgTracker.getFound()){
        imagePoints[1] = imgTracker.getImagePoints();
    }
    img3.loadImage("images/me02.jpg");
    imgTracker.update(toCv(img3));
    if(imgTracker.getFound()){
        imagePoints[2] = imgTracker.getImagePoints();
    }
}

void testApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		tracker.update(toCv(cam));
		position = tracker.getPosition();
		scale = tracker.getScale();
		orientation = tracker.getOrientation();
		rotationMatrix = tracker.getRotationMatrix();
        
        if(followMouse){
            position.x = mouseX;
            position.y = mouseY;
        }
        
        if(tracker.getFound()){
            
            camMesh.clear();
			camMesh = tracker.getObjectMesh();
            
            img1Mesh.clear();
			img1Mesh = tracker.getObjectMesh();
            img1Mesh.clearTexCoords();
			img1Mesh.addTexCoords(imagePoints[0]);
            
            img2Mesh.clear();
			img2Mesh = tracker.getObjectMesh();
            img2Mesh.clearTexCoords();
			img2Mesh.addTexCoords(imagePoints[1]);
            
            img3Mesh.clear();
			img3Mesh = tracker.getObjectMesh();
            img3Mesh.clearTexCoords();
			img3Mesh.addTexCoords(imagePoints[2]);
            
            camMask.begin();
            ofClear(0, 255);
                ofPushMatrix();
                ofSetupScreenOrtho(640, 480, OF_ORIENTATION_UNKNOWN, false , -1000, 1000);
                ofTranslate(position.x,position.y);
                applyMatrix(rotationMatrix);
                ofScale(scale,scale,scale);
                camMesh.draw();
                ofPopMatrix();
			camMask.end();
            
            camFbo.begin();
                ofClear(0, 255);
                cam.draw(0,0);
                ofPushMatrix();
                ofSetupScreenOrtho(640, 480, OF_ORIENTATION_UNKNOWN, false , -1000, 1000);
                ofTranslate(position.x,position.y);
                applyMatrix(rotationMatrix);
                ofScale(scale,scale,scale);
                cam.getTextureReference().bind();
                camMesh.draw();
                cam.getTextureReference().unbind();
                ofPopMatrix();
			camFbo.end();
            
            img1Fbo.begin();
            ofClear(0, 255);
                ofPushMatrix();
                ofSetupScreenOrtho(640, 480, OF_ORIENTATION_UNKNOWN, false, -1000, 1000);
                ofTranslate(position.x,position.y);
                applyMatrix(rotationMatrix);
                ofScale(scale,scale,scale);
                img1.bind();
                img1Mesh.draw();
                img1.unbind();
                ofPopMatrix();
            img1Fbo.end();
            
            img2Fbo.begin();
            ofClear(0, 255);
                ofPushMatrix();
                ofSetupScreenOrtho(640, 480, OF_ORIENTATION_UNKNOWN, false, -1000, 1000);
                ofTranslate(position.x,position.y);
                applyMatrix(rotationMatrix);
                ofScale(scale,scale,scale);
                img2.getTextureReference().bind();
                img2Mesh.draw();
                img2.getTextureReference().unbind();
                ofPopMatrix();
            img2Fbo.end();
            
            img3Fbo.begin();
            ofClear(0,255);
                ofPushMatrix();
                ofSetupScreenOrtho(640, 480, OF_ORIENTATION_UNKNOWN, false , -1000, 1000);
                ofTranslate(position.x,position.y);
                applyMatrix(rotationMatrix);
                ofScale(scale,scale,scale);
                img3.getTextureReference().bind();
                img3Mesh.draw();
                img3.getTextureReference().unbind();
                ofPopMatrix();
            img3Fbo.end();
            
            clone1.setStrength(int(ofMap(mouseX,0,ofGetWidth()/3,0,25, true)));
            clone1.update(img1Fbo.getTextureReference(), camFbo.getTextureReference(), camMask.getTextureReference());
            
            clone1Fbo.begin();
            ofClear(0, 255);
            clone1.draw(0,0);
            clone1Fbo.end();
            
            clone2.setStrength(int(ofMap(mouseX,ofGetWidth()/3,(ofGetWidth()/3)*2,0,25, true)));
            clone2.update(img2Fbo.getTextureReference(), clone1Fbo.getTextureReference(), camMask.getTextureReference());
            
            clone2Fbo.begin();
            ofClear(0, 255);
            clone2.draw(0,0);
            clone2Fbo.end();
            
            clone3.setStrength(int(ofMap(mouseX,(ofGetWidth()/3)*2,ofGetWidth(),0,25, true)));
            clone3.update(img3Fbo.getTextureReference(), clone2Fbo.getTextureReference(), camMask.getTextureReference());
        }
	}
}

void testApp::draw() {
	ofSetColor(255);
	//cam.draw(0, 0);
	
	
	if(tracker.getFound()) {
		ofSetLineWidth(1);
        
        if(mouseX > 0 && mouseX < ofGetWidth()/3){
            clone1.draw(0,0);
        }
        if(mouseX > ofGetWidth()/3 && mouseX < (ofGetWidth()/3)*2){
            clone2.draw(0,0);
        }
        if(mouseX > (ofGetWidth()/3)*2 && mouseX < ofGetWidth()){
            clone3.draw(0,0);
        }
        if(mouseX < 0){
            clone1.draw(0,0);
        }
        if(mouseX > ofGetWidth()){
            clone3.draw(0,0);
        }
        

        
        //ofDrawBitmapString(ofToString((float) mouthWide) , 10, 20);
	}
    else{
        cam.draw(0,0);
        //img3.draw(0,0);
    }
}

void testApp::keyPressed(int key) {
	if(key == 'r') {
		tracker.setup();
	}
}

void testApp::mouseDragged(int x, int y, int button) {

}

void testApp::mousePressed(int x, int y, int button) {
    followMouse=true;
}

void testApp::mouseReleased(int x, int y, int button) {
    followMouse=false;
}




