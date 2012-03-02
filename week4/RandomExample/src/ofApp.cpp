#include "ofApp.h"

// https://en.wikipedia.org/wiki/Linear_feedback_shift_register#Galois_LFSRs
unsigned int lfsr = 1;
unsigned bit;
unsigned period = 0;
unsigned int glitch = 1;
unsigned int galois() {
  //lfsr = (lfsr >> 1) ^ (-(lfsr & 1u) & 0xD0000001u);
   lfsr += (lfsr >> 1) ^ (-(lfsr & 4u) & 0xD0000001u);

        bit  += ((lfsr >> 0) ^ (lfsr >> glitch) ^ (lfsr >> glitch*2) ^ (lfsr >> 1) ) & 1;
        lfsr +=  (lfsr >> 10) | (bit << 15);
    glitch++;
	return lfsr;
}

void ofApp::setup() {
	img.allocate(1000, 1000, OF_IMAGE_COLOR_ALPHA);
}  

void ofApp::update() {
    
	unsigned char* pixels = img.getPixels();
	int n = 3 * img.getWidth() * img.getHeight();
	for(int i = 0; i < n; i++) {
		int cur = galois();
		pixels[i] = cur;
	}
	img.update();
}

void ofApp::draw() {
	img.draw(0, 0);
}