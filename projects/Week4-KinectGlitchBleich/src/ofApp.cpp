#include "ofApp.h"

void addFace(ofMesh& mesh, ofColor color, ofColor color2, ofVec3f a, ofVec3f b, ofVec3f c) {
	mesh.addVertex(a);
    mesh.addColor(color);
	mesh.addVertex(b);
    mesh.addColor(color2);
	mesh.addVertex(c);
    mesh.addColor(color);
}

void addFace(ofMesh& mesh, ofColor color, ofColor color2, ofVec3f a, ofVec3f b, ofVec3f c, ofVec3f d) {
	addFace(mesh, color, color2, a, b, c);
	addFace(mesh, color, color2, a, c, d);
}

ofVec3f getVertexFromImg(ofImage& img, int x, int y) {
	ofColor cur = img.getColor(x, y);
	if(cur.getBrightness() > 150 ) {
		float z = ofMap(cur.getBrightness(), 200, 255, -250, 250);
		return ofVec3f(x - img.getWidth() / 2, y - img.getHeight() / 2, z);
	} else {
		return ofVec3f(0, 0, 0);
	}
}

void ofApp::setup() {
	kinect.init();
	kinect.setRegistration(true);
	kinect.open();
    glitchKinect.allocate(640, 480, OF_IMAGE_COLOR);
    colorImage.allocate(640, 480, OF_IMAGE_COLOR_ALPHA);
    
    light.enable();
	light.setPosition(0, 0, 1000);

    glEnable(GL_DEPTH_TEST);
   
    ofEnableLighting();
}

void ofApp::update() {
	kinect.update();
	if(kinect.isFrameNew()) {
        
        ofPixels& depthPixels = kinect.getDepthPixelsRef();
        ofPixels& colorPixels = kinect.getPixelsRef();


        for(int y = 0; y < 480; y++) {
            for(int x = 0; x < 640; x++) {
                ofColor color = colorPixels.getColor(x, y);
                ofColor depth = depthPixels.getColor(x, y);
                glitchKinect.setColor(x, y,depth);
                colorImage.setColor(x, y,ofColor(color,depth.getBrightness()));
                
            }
		}
        glitchKinect.saveImage("out1.jpeg",OF_IMAGE_QUALITY_BEST);

        
        //if(ofGetKeyPressed('g')) {
			// this portion glitches the jpeg file
			// first loading the file (as binary)
			ofBuffer file = ofBufferFromFile("out1.jpeg", true);
			int fileSize = file.size();
			char * buffer = file.getBinaryBuffer();
            
			// pick a byte offset that is somewhere near the end of the file
			int whichByte = (int) ofRandom(fileSize * .007, fileSize);
			// and pick a bit in that byte to turn on
			int whichBit = ofRandom(8);
			char bitMask = 1 << whichBit;
			// using the OR operator |, if the bit isn't already on this will turn it on
			buffer[whichByte] = bitMask;
            
			// write the file out like nothing happened
			ofBufferToFile("out1.jpeg", file, true);
			glitchKinect.loadImage("out1.jpeg");
	//	} 
        if(ofGetKeyPressed('m')){
            
        }else{
           mesh.clear(); 
        }
        
        mesh.setMode(OF_PRIMITIVE_TRIANGLES);
        ofVec3f zero;
        zero.set(0,0,0);
        cam.setTarget(ofVec3f(0,0,0));
        for(int y = 0; y < glitchKinect.getHeight()-8; y+=8) {
            for(int x = 0; x < glitchKinect.getWidth()-8; x+=8) {
                ofColor curColor = glitchKinect.getColor(x, y);
                ofColor curColor2 = colorImage.getColor(x, y);
                ofVec3f nw = getVertexFromImg(glitchKinect, x, y);
                ofVec3f ne = getVertexFromImg(glitchKinect, x + 8, y);
                ofVec3f sw = getVertexFromImg(glitchKinect, x, y + 8);
                ofVec3f se = getVertexFromImg(glitchKinect, x + 8, y + 8);
                ofVec3f normSE = se.getNormalized();
                ofVec3f normNW = nw.getNormalized();
                ofVec3f normNE = ne.getNormalized();
                ofVec3f normSW = sw.getNormalized();
                float theDistance  = (normNW.distance(normNE) + normNE.distance(normSE) + normSE.distance(normSW) + normSW.distance(normNW) + normNW.distance(normSE) + normSW.distance(normNW)) / 6;
                
                if(nw != zero && ne != zero && sw != zero && se != zero && theDistance<.06) {
                    curColor.a=curColor2.a;
                    curColor2.a=curColor2.getBrightness();
                    addFace(mesh, curColor, curColor2, nw, ne, se, sw);
                }
                
            }
        }

        
	}
}

void ofApp::draw() {
    ofEnableAlphaBlending();
	ofBackground(0);
    
	ofSetColor(255, 255, 255);
	//kinect.drawDepth(0, 0, 640, 480);
   // glitchKinect.draw(0,0);
    //kinect.draw(0, 480, 640, 480);
       cam.begin();
	ofScale(1, -1, 1); // make y point down
    ofTranslate(0, 0,-300);

    ofSetLineWidth(2);
    mesh.draw();
	
	cam.end();
    
    ofDrawBitmapString("Press 'm' for MURDER ", 10, 20);
}

void ofApp::exit() {
	kinect.close();
}

