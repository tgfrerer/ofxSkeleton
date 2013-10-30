//
//     _____    ___
//    /    /   /  /     ofxSkeleton Example
//   /  __/ * /  /__    (c) ponies & light, 2012, 2013. All rights reserved.
//  /__/     /_____/    poniesandlight.co.uk
//
//  ofxSkeleton.h
//  Created by tim on 31/10/2013.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
#include "ofApp.h"

using namespace pal::ofxSkeleton;

u_long animCurrentFrame;
bool animIsPaused = false;
bool shouldDrawLabels = false;
bool shouldDrawInfo = true;

//--------------------------------------------------------------
void ofApp::setup(){
	
	
	animIsPaused = false;
	animCurrentFrame = 0;
	
	ofDisableSmoothing();
	ofDisableAlphaBlending();
	
	mCam1.setupPerspective(false, 60, 0.1, 3000);
//	mCam1.setPosition(500, 800, 800);
	mCam1.lookAt(ofVec3f(0));

	
	mSkeleton["Root"] = JointP_t(new ofxJoint());
	mSkeleton["Root"]->setGlobalPosition(ofVec3f(0));

	string names[4] = {"Hip_", "Knee_", "Foot_", "Toe_"};
	
	for (int j = 0; j < 4; ++j){
		
		string sIndex = ofToString(j);
		
		for (int i = 0; i < 4; i++){
			mSkeleton["L_" +names[i] + sIndex] = JointP_t(new ofxJoint());
			mSkeleton["R_" + names[i] + sIndex] = JointP_t(new ofxJoint());
		}
		// apply a limb's hierarchy
		mSkeleton["L_Toe_" + sIndex]->bone(mSkeleton["L_Foot_" + sIndex])->bone(mSkeleton["L_Knee_" + sIndex])->bone(mSkeleton["L_Hip_" + sIndex])->bone(mSkeleton["Root"]);
		mSkeleton["R_Toe_" + sIndex]->bone(mSkeleton["R_Foot_" + sIndex])->bone(mSkeleton["R_Knee_" + sIndex])->bone(mSkeleton["R_Hip_" + sIndex])->bone(mSkeleton["Root"]);

		// set the limb's joints positions
	
		mSkeleton["L_Hip_" + sIndex]->setGlobalPosition(ofVec3f(-100, 100 , -120 + j * 60 ));
		mSkeleton["L_Hip_" + sIndex]->setOrientation(ofQuaternion(j * 20 - 40, ofVec3f(0,1,0)));
		mSkeleton["L_Knee_" + sIndex]->setPosition(ofVec3f(-150, 30, 0));
		mSkeleton["L_Foot_" + sIndex]->setPosition(ofVec3f(-80, -80, 0));
		mSkeleton["L_Toe_" + sIndex]->setPosition(ofVec3f(-120, -60, 0));

		mSkeleton["R_Hip_" + sIndex]->setGlobalPosition(ofVec3f(100, 100 , -120 + j * 60 ));
		mSkeleton["R_Hip_" + sIndex]->setOrientation(ofQuaternion(j * 20 - 40, ofVec3f(0,-1,0)));
		mSkeleton["R_Knee_" + sIndex]->setPosition(ofVec3f(150, 30, 0));
		mSkeleton["R_Foot_" + sIndex]->setPosition(ofVec3f(80, -80, 0));
		mSkeleton["R_Toe_" + sIndex]->setPosition(ofVec3f(120, -60, 0));
	}

	// add a tail
	
	for (int i=0; i<10; i++){
		JointP_t tailSegment = JointP_t(new ofxJoint());
		if (i>0) {
			tailSegment->setParent(mTail.back());
		}
		tailSegment->setPosition(ofVec3f(0,0,-100));
		mTail.push_back(tailSegment);
		mSkeleton["TailSegment_" + ofToString(i)] = tailSegment;
	}

	mTail[0]->setParent(mSkeleton["Root"]);
	
	mIKchainTail.bones.assign(mTail.begin()+2, mTail.end());
	mIKchainTail.endEffector = mTail.back();
	mIKchainTail.targetPositionInGlobalSpace = ofVec3f(0,-300);
	
	// add a head.
	mSkeleton["Head"] = JointP_t(new ofxJoint());
	mSkeleton["Head"]->setParent(mSkeleton["Root"]);
	mSkeleton["Head"]->setPosition(ofVec3f(0, 70 , 70 ));

	// add some fangs
	
	mSkeleton["L_Fang_Hinge"] = JointP_t(new ofxJoint());
	mSkeleton["L_Fang_Hinge"]->bone(mSkeleton["Root"]);
	mSkeleton["L_Fang_Hinge"]->setPosition(ofVec3f(-50, 0, 70 ));

	mSkeleton["L_Fang_A"] = JointP_t(new ofxJoint());
	mSkeleton["L_Fang_A"]->bone(mSkeleton["L_Fang_Hinge"]);
	mSkeleton["L_Fang_A"]->setPosition(ofVec3f(-50, 0, 90 ));

	mSkeleton["L_Fang_AA"] = JointP_t(new ofxJoint());
	mSkeleton["L_Fang_AA"]->bone(mSkeleton["L_Fang_A"]);
	mSkeleton["L_Fang_AA"]->setPosition(ofVec3f(40, 0, 120 ));

	mSkeleton["L_Fang_B"] = JointP_t(new ofxJoint());
	mSkeleton["L_Fang_B"]->bone(mSkeleton["L_Fang_Hinge"]);
	mSkeleton["L_Fang_B"]->setPosition(ofVec3f(20, 0, 90 ));

	mSkeleton["L_Fang_BB"] = JointP_t(new ofxJoint());
	mSkeleton["L_Fang_BB"]->bone(mSkeleton["L_Fang_B"]);
	mSkeleton["L_Fang_BB"]->setPosition(ofVec3f(-20, 0, 120 ));

	mSkeleton["R_Fang_Hinge"] = JointP_t(new ofxJoint());
	mSkeleton["R_Fang_Hinge"]->bone(mSkeleton["Root"]);
	mSkeleton["R_Fang_Hinge"]->setPosition(ofVec3f( 50, 0, 70 ));
	
	mSkeleton["R_Fang_A"] = JointP_t(new ofxJoint());
	mSkeleton["R_Fang_A"]->bone(mSkeleton["R_Fang_Hinge"]);
	mSkeleton["R_Fang_A"]->setPosition(ofVec3f( 50, 0, 90 ));
	
	mSkeleton["R_Fang_AA"] = JointP_t(new ofxJoint());
	mSkeleton["R_Fang_AA"]->bone(mSkeleton["R_Fang_A"]);
	mSkeleton["R_Fang_AA"]->setPosition(ofVec3f(-40, 0, 120 ));
	
	mSkeleton["R_Fang_B"] = JointP_t(new ofxJoint());
	mSkeleton["R_Fang_B"]->bone(mSkeleton["R_Fang_Hinge"]);
	mSkeleton["R_Fang_B"]->setPosition(ofVec3f(-20, 0, 90 ));
	
	mSkeleton["R_Fang_BB"] = JointP_t(new ofxJoint());
	mSkeleton["R_Fang_BB"]->bone(mSkeleton["R_Fang_B"]);
	mSkeleton["R_Fang_BB"]->setPosition(ofVec3f( 20, 0, 120 ));
	

	// set joint names according to their map indices.
	
	for (map<string, JointP_t>::iterator it = mSkeleton.begin(); it != mSkeleton.end(); ++it){
		it->second->setName(it->first);
	}
	
	
}


//--------------------------------------------------------------
void ofApp::update(){
	
	if (!animIsPaused) {
		++animCurrentFrame;

		
		mIKchainTail.solve();
	}

	ofQuaternion q = mSkeleton["L_Hip_0"]->getOrientation();
	
	ofQuaternion p( sin((animCurrentFrame % 200/200.f) * TWO_PI), ofVec3f(1,0,0));
	
	q *= p;
	
	if (!animIsPaused) {
		
		for (int i = 0; i<4; i++){
			ofQuaternion q(i * 20 - 40, ofVec3f(0,1,0));
			ofQuaternion q2(ofSignedNoise(
										  (cos(TWO_PI * (i) / 4.0f) + 1.0 ) / 2.0,
										  (sin(TWO_PI * (animCurrentFrame % 200) / 200.f)) + 1.0 ) / 2.0
							* 50.f, ofVec3f(1,0,0));
			
			mSkeleton["L_Hip_" + ofToString(i)]->setOrientation(q*q2);
		}

		for (int i = 0; i<4; i++){
			ofQuaternion q(i * 20 - 40, ofVec3f(0,-1,0));
			ofQuaternion q2(ofSignedNoise(
										  (sin(TWO_PI * (i) / 4.0f) + 1.0 ) / 2.0,
										  (cos(TWO_PI * (animCurrentFrame % 200) / 200.f)) + 1.0 ) / 2.0
							* 50.f, ofVec3f(1,0,0));
			
			mSkeleton["R_Hip_" + ofToString(i)]->setOrientation(q*q2);
		}

	
	}

}

//--------------------------------------------------------------
void ofApp::draw(){
	
	ofEnableDepthTest();
	
	ofBackgroundGradient(ofColor::fromHex(0x323232), ofColor::black);
	
	mCam1.begin();

	{
		ofVec3f mouseInWindowSpace(ofGetMouseX(),ofGetMouseY(), mCam1.worldToScreen(ofVec3f(0)).z);
		mIKchainTail.targetPositionInGlobalSpace = mCam1.screenToWorld(mouseInWindowSpace);

	}

	ofPushMatrix();

	for (map<string, JointP_t>::iterator it = mSkeleton.begin(); it != mSkeleton.end(); ++it){
		it->second->draw(10);
	}

	ofPopMatrix();
	
	mCam1.end();
	
	ofSetColor(ofColor::yellow);

	if (shouldDrawLabels) {
		for (map<string, JointP_t>::iterator it = mSkeleton.begin(); it != mSkeleton.end(); ++it){
			ofDrawBitmapString(it->second->getName(), mCam1.worldToScreen(it->second->getGlobalPosition()) * ofVec3f(1,1,0) + ofVec3f(10,10));
		}
	}
	
	ofSetColor(ofColor::white);

	if (shouldDrawInfo){
		ofDrawBitmapString("     _____    ___\n"
						   "    /    /   /  /     ofx(Exo)Skeleton Example\n"
						   "   /  __/ * /  /__    (c) ponies & light, 2013. All rights reserved.\n"
						   "  /__/     /_____/    poniesandlight.co.uk\n\n"
						   "<SPACE> toggle animation | <i> toggle info text | <f> foggle fullscreen | <l> toggle labels", 10, ofGetHeight() - 12 * 7);
	}
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	
	
	switch (key) {
		case ' ':
			animIsPaused ^= true;
			break;
		case 'i':
			shouldDrawInfo ^= true;
			break;
		case 'l':
			shouldDrawLabels ^= true;
			break;
		case 'f':
			ofToggleFullscreen();
			break;
		default:
			break;
	}
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	
	
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
