#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	for (int i=0; i<14; i++) {
		ofxBone* b = new ofxBone();
		if (i>0) b->setParent(bones.back());
		b->setPosition(ofVec3f(50, 0, 50));
		bones.push_back(tr1::shared_ptr<ofxBone>(b));
	}
	
	IKchain.boneChain = bones;
	IKchain.endEffector = bones.back();
	IKchain.targetPositionInGlobalSpace = ofVec3f(0,-300);
}

//--------------------------------------------------------------
void testApp::update(){

	IKchain.solve();
	
//	auto b = bones[0];
//	
//	ofQuaternion q;
//	q.makeRotate(((ofGetElapsedTimeMillis()%10000)/10000.0) * 360, 0, 0, 1);
//	b->setOrientationAtJoint(q);
//	b = bones[2];
//	b->setOrientation(q);
////
//	b = bones[5];
//	
//	q.makeRotate(0, 0, 0, 1);
//	b->setGlobalOrientationAtJoint(q);
//	
//	bones[6]->setGlobalOrientation(q);
}

//--------------------------------------------------------------
void testApp::draw(){

	ofBackground(20);
	
	ofSetColor(255);

	// we want to draw in the centre of the screen.
	ofTranslate(ofGetWidth()/2.0, ofGetHeight()/2.0);

	for (int i = 0; i<bones.size(); ++i){
		std::tr1::shared_ptr<ofxBone>& b = bones[i];
		b->draw();
		ofDrawBitmapString(ofToString(i), b->getGlobalPosition());
		ofSphere(b->getGlobalJointPosition(), 5);
	}
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	IKchain.targetPositionInGlobalSpace = ofVec3f(x-ofGetWidth()/2.0,y-ofGetHeight()/2.0,0);

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}