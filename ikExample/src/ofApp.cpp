#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	for (int i=0; i<14; i++) {
		ofxJoint * j = new ofxJoint();
		if (i>0) j->bone(bones.back());
		j->setPosition(ofVec3f(50, 0, 50));
		bones.push_back(shared_ptr<ofxJoint>(j));
	}
	
	mIKchain.bones = bones;
	mIKchain.endEffector = bones.back();
	mIKchain.targetPositionInGlobalSpace = ofVec3f(0,-300);
}

//--------------------------------------------------------------
void ofApp::update(){
	mIKchain.solve();
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofBackground(20);
	
	ofSetColor(255);

	// we want to draw in the centre of the screen.
	ofTranslate(ofGetWidth()/2.0, ofGetHeight()/2.0);

	for (int i = 0; i<bones.size(); ++i){
		bones[i]->draw(15.0f);
		ofDrawBitmapString(ofToString(i), bones[i]->getGlobalPosition());
	}
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	mIKchain.targetPositionInGlobalSpace = ofVec3f(x-ofGetWidth()/2.0,y-ofGetHeight()/2.0,0);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}