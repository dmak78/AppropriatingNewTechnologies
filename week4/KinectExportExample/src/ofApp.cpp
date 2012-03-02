#include "ofApp.h"

void addFace(ofMesh& mesh, ofColor colorA, ofColor colorB, ofColor colorC, ofVec3f a, ofVec3f b, ofVec3f c) {
    ofVec3f normal = a.getCrossed(b);
   // mesh.addNormal(normal);
	mesh.addVertex(a);
   
    mesh.addColor(colorA);
    
	mesh.addVertex(b);
  
    mesh.addColor(colorB);
    
	mesh.addVertex(c);
   
    mesh.addColor(colorC);
    
    
    
}

void addFace(ofMesh& mesh, ofColor colorA, ofColor colorB, ofColor colorC, ofColor colorD, ofVec3f a, ofVec3f b, ofVec3f c, ofVec3f d) {
	addFace(mesh, colorA, colorB, colorC, a, b, c);
	addFace(mesh, colorA, colorC, colorD, a, c, d);
}



void ofApp::setup() {
    //ofSetVerticalSync(true);
	kinect.init();
	kinect.setRegistration(true);
	kinect.open();
    step=4;
    rotationY=0;
    rotationX=0;
    rotationZ=0;
    takeSnap=false;

	
	shader.load("DOFCloud");	
	focusDistance = 300;
	aperture = .0015;
    
}

void ofApp::update() {
	//focusDistance = ofMap(mouseY,0,ofGetHeight(),0,500);
	//aperture = ofMap(mouseX,0,ofGetWidth(),0,.1);
    
	kinect.update();
    rotationY+=PI/60;
    rotationX+=PI/10;
    rotationZ+=PI/4;
    ofVec3f zero(0, 0, 0);
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	if(kinect.isFrameNew()) {
        mesh.clear();
        for(int y = 0; y < 480-step; y+=step) {
            for(int x = 0; x < 640-step; x+=step) {
                ofVec3f nw = kinect.getWorldCoordinateAt(x, y);
                ofVec3f ne =kinect.getWorldCoordinateAt( x + step, y);
                ofVec3f sw = kinect.getWorldCoordinateAt(x, y + step);
                ofVec3f se = kinect.getWorldCoordinateAt(x + step, y + step);
                ofColor curColorNW = kinect.getColorAt(x, y);
                ofColor curColorNE = kinect.getColorAt( x + step, y);
                ofColor curColorSW = kinect.getColorAt(x, y + step);
                ofColor curColorSE = kinect.getColorAt(x + step, y + step);
                if(nw != zero && ne != zero && sw != zero && se != zero) {
                    addFace(mesh, curColorNW,curColorNE,curColorSW,curColorSE,nw, ne, se, sw);
                    
                }
                //mesh.addVertex(cur);
                //mesh.addColor(curColor);
            }
        }
        
        if(takeSnap==true){
            meshSnaps.push_back(mesh);
            meshSnapsRotationY.push_back(rotationY);
            meshSnapsRotationX.push_back(rotationX);
            meshSnapsRotationZ.push_back(rotationZ);
            takeSnap=false;
            
        }

        
        
//		if(ofGetKeyPressed(' ')) {
//			ofPixels& depthPixels = kinect.getDepthPixelsRef();
//			ofPixels& colorPixels = kinect.getPixelsRef();
//			ofImage img;
//			img.allocate(640, 480, OF_IMAGE_COLOR_ALPHA);
//			for(int y = 0; y < 480; y++) {
//				for(int x = 0; x < 640; x++) {
//					ofColor color = colorPixels.getColor(x, y);
//					ofColor depth = depthPixels.getColor(x, y);
//					img.setColor(x, y, ofColor(color, depth.getBrightness()));
//				}
//			}
			//img.saveImage("out.png");
//		}
	}
    //glEnable(GL_DEPTH_TEST);
    ofEnableAlphaBlending();
   // light.enable();
	//light.setPosition(640/2+100, 480/2, -600);
}

void ofApp::draw() {
	ofBackground(0);
	//ofSetColor(255, 255, 255);
    glPointSize(1);


    cam.begin();
	ofScale(1, -1, -1); // make y point down
    ofTranslate(0,0,0);
    shader.begin();
	shader.setUniform1f("focusDistance", focusDistance);
	shader.setUniform1f("aperture", aperture);
    glEnable(GL_POINT_SMOOTH); // makes circular points
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE_ARB);	// allows per-point size
   // ofRotateY(rotationY);
   // ofRotateX(rotationX);
   // ofRotateZ(rotationZ);
    mesh.drawVertices();
   
    ofPushMatrix();
    if(meshSnaps.size()>0){
        for(int i = 0; i < meshSnaps.size();i++){
            ofRotateY(meshSnapsRotationY[i]);
            ofRotateX(meshSnapsRotationX[i]);
            ofRotateZ(meshSnapsRotationZ[i]);
            meshSnaps[i].drawWireframe();
        }
    }
    ofPopMatrix();
    shader.end();
    cam.end();
  

}

void ofApp::exit() {
	kinect.close();
}

void ofApp::keyPressed(int key){
    if(key=='s'){
        takeSnap=true;
    }

}

