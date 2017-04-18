#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCv.h"

class ofApp : public ofBaseApp{
    enum AppMode {
        AppMode_Release,
        AppMode_Debug
    };
    
    AppMode mode = AppMode_Debug;


	//
	ofVideoGrabber camera;
	ofxCv::FlowFarneback fb;
	ofxCv::FlowPyrLK lk;
	ofxCv::Flow* curFlow;
	cv::Mat lastImage;
	cv::Mat currentImage;
	


	ofxPanel gui;
	ofParameter<float> fbPyrScale, lkQualityLevel, fbPolySigma;
	ofParameter<int> fbLevels, lkWinSize, fbIterations, fbPolyN, fbWinSize, lkMaxLevel, lkMaxFeatures, lkMinDistance;
	ofParameter<bool> fbUseGaussian, usefb;
    
    //--------------------------------------------------------------
    void setup(){
        ofBackground(0);
        ofSetFrameRate(60);
        ofSetVerticalSync(true);


		camera.setup(320, 240);

		curFlow = &fb;
        setupGUI();

		ofImage last = ofImage("01.png");
		ofImage current = ofImage("02.png");
		//lastImage.
    }
    
    void setupGUI() {
		gui.setup();

		gui.add(lkMaxLevel.set("lkMaxLevel", 3, 0, 8));
		gui.add(lkMaxFeatures.set("lkMaxFeatures", 200, 1, 1000));
		gui.add(lkQualityLevel.set("lkQualityLevel", 0.01, 0.001, .02));
		gui.add(lkMinDistance.set("lkMinDistance", 4, 1, 16));
		gui.add(lkWinSize.set("lkWinSize", 8, 4, 64));
		gui.add(usefb.set("Use Farneback", true));
		gui.add(fbPyrScale.set("fbPyrScale", .5, 0, .99));
		gui.add(fbLevels.set("fbLevels", 4, 1, 8));
		gui.add(fbIterations.set("fbIterations", 2, 1, 8));
		gui.add(fbPolyN.set("fbPolyN", 7, 5, 10));
		gui.add(fbPolySigma.set("fbPolySigma", 1.5, 1.1, 2));
		gui.add(fbUseGaussian.set("fbUseGaussian", false));
		gui.add(fbWinSize.set("winSize", 32, 4, 64));
    }
    
    //--------------------------------------------------------------
    void update(){
		camera.update();

		if (camera.isFrameNew()) {

			if (usefb) {
				curFlow = &fb;
				fb.setPyramidScale(fbPyrScale);
				fb.setNumLevels(fbLevels);
				fb.setWindowSize(fbWinSize);
				fb.setNumIterations(fbIterations);
				fb.setPolyN(fbPolyN);
				fb.setPolySigma(fbPolySigma);
				fb.setUseGaussian(fbUseGaussian);
			}
			else {
				curFlow = &lk;
				lk.setMaxFeatures(lkMaxFeatures);
				lk.setQualityLevel(lkQualityLevel);
				lk.setMinDistance(lkMinDistance);
				lk.setWindowSize(lkWinSize);
				lk.setMaxLevel(lkMaxLevel);
			}

			// you can use Flow polymorphically
			curFlow->calcOpticalFlow(camera);
//			curFlow->calcOpticalFlow()
		}
    }
    
    //--------------------------------------------------------------
    void draw() {
		ofPushMatrix();
		ofTranslate(0, 0);
		camera.draw(0, 0, 640, 480);
		curFlow->draw(0, 0, 640, 480);
		ofDrawBitmapStringHighlight(ofToString((int)ofGetFrameRate()) + "fps", 10, 20);
		ofPopMatrix();

        if(mode == AppMode_Debug) {
            drawDebug();
        }
    }
    
    void drawDebug() {
        ofSetColor(255);
        gui.draw();
    }
    
    void exit() {
        gui.saveToFile("settings.xml");
    }
    
    void switchDebug() {
        if (mode == AppMode_Debug) {
            mode = AppMode_Release;
        }else{
            mode = AppMode_Debug;
        }
    }
    
    void keyPressed(int key){
        if(key == 'f'){
            ofToggleFullscreen();
        }else if(key == 'd') {
            switchDebug();
        }
    }
    
    void updateFloatParam(float &value) {
        //do something.
        cout << "updateFloatParam : " << value << endl;
    }
    
    void updateIntParam(int &value) {
        //do something.
        cout << "updateIntParam : " << value << endl;
    }
};

//========================================================================
int main( ){
	ofSetupOpenGL(640,480, OF_WINDOW);
	ofRunApp( new ofApp());
}
