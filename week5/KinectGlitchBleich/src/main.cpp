#include "ofApp.h"
#include "ofAppGlutWindow.h"

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1000, 500, OF_WINDOW);
	ofRunApp(new ofApp());
}
