#include "ofApp.h"

// bunny comes from http://graphics.stanford.edu/data/3Dscanrep/
// specifically ftp://graphics.stanford.edu/pub/3Dscanrep/bunny.tar.gz

void ofApp::setup() {
	// otherwise the app will run too fast
	ofSetVerticalSync(true);
    

    
    shader.load("DOFCloud");	
	focusDistance = 200;
	aperture = .1;
	
	// assimp can load all kinds of formats, and supports texture mapping and
	// even animations. use getMesh(0) to get a mesh from assimp. if you have
	// a bunch of meshes inside your 3d model file, change 0 to something else.
	ofxAssimpModelLoader loader;
	loader.loadModel("dog3.obj");
	center = loader.getSceneCenter();
	mesh = loader.getMesh(0);
	
	// when you scale things to make them bigger or smaller, this also scales
	// their normals. because the light is calculated using the normal, this
	// will change how bright the surface is. since we don't want this, we 
	// ask opengl to normalize the normals for us before it does the lighting
	// calculations.
	glEnable(GL_NORMALIZE);
	
	// when we're drawing a 3d model that has faces, we want to make sure that
	// things obscure each other. normally when you draw 2d it's the opposite:
	// you want whatever the most recent thing you've drawn to be the thing in
	// front.
	glEnable(GL_DEPTH_TEST);

    
    ofEnableAlphaBlending();
    
//    for(int i = 1; i < mesh.getNumVertices(); i++){
//        ofVec3f cur = mesh.getVertex(i);
//        
//        cur.x += ofRandom(-.5,.5);
//        cur.y += ofRandom(-.5,.5);
//        cur.z += ofRandom(-.51,.51);
//        mesh.setVertex(i,cur);
//        
//    }
    
    
    meshColor.r=ofRandom(0,255);
    meshColor.g=ofRandom(0,255);
    meshColor.b=ofRandom(0,255);
    meshColor.a=ofRandom(0,255);

    amplitude=.1;
	// in openFrameworks you enable lighting by creating and enabling a light
	light.enable();
//    ofSetColor(255, 255, 255);
//    stringstream reportStream;
//	reportStream << "indices " << ofToString(mesh.getNumIndices(), 2) << endl;
//	ofDrawBitmapString(reportStream.str(),10,10);
}

void ofApp::update() {

    float inc = TWO_PI/175.0;
    frequency = 1.5;
    amplitude+=inc;
    
    float inflate = sin(amplitude)*frequency;
    
    int zSpacing = .5;
    processedMesh = mesh;
    for(int i = 0; i < processedMesh.getNumVertices(); i++){
        ofVec3f cur = processedMesh.getVertex(i);
        //meshColor.a=0;
       cur.x += ofRandom(-.2,.1);
     cur.y += ofRandom(-.2,.1);
       cur.z += ofRandom(-.2,.1)+zSpacing*(i/processedMesh.getNumVertices());
//        cur.x+=(center.x*inflate);
//       cur.y+=(center.y*inflate);
//        cur.z+=(center.z*inflate);

       //processedMesh.addColor(meshColor);
        processedMesh.setVertex(i,cur);
        
    }
    for(int i = 0; i < processedMesh.getNumNormals(); i++){
        ofVec3f curNorm = processedMesh.getNormal(i);
        ofFloatColor colorNormal;
        colorNormal.r = curNorm.x;
        colorNormal.g = curNorm.y;
        colorNormal.b = curNorm.z;
        colorNormal.a = ofRandom(.5,1);
        processedMesh.addColor(colorNormal);
        
        
    }
    
    //focusDistance++;

}

void ofApp::draw() {
	ofBackground(0);
    glPointSize(8);
   // glEnable(GL_POINT_SMOOTH); // makes circular points
	//glEnable(GL_VERTEX_PROGRAM_POINT_SIZE_ARB);	// allows per-point size
 
	cam.begin();
    shader.begin();
	shader.setUniform1f("focusDistance", focusDistance);
	shader.setUniform1f("aperture", aperture);
	float s = 65;
	ofScale(s,s,s);
	ofTranslate(-center.x, -center.y, -center.z);
	//mesh.draw();
    processedMesh.draw();
    shader.end();
	cam.end();

    

}
