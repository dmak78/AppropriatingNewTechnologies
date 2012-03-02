#include "ofApp.h"
#include "ofAppGlutWindow.h"

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1000, 1000, OF_WINDOW);
	ofRunApp(new ofApp());
}
