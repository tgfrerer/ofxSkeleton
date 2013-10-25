//
//     _____    ___     
//    /    /   /  /     ofxSkeleton
//   /  __/ * /  /__    (c) ponies & light, 2012, 2013. All rights reserved.
//  /__/     /_____/    poniesandlight.co.uk
//
//  ofxSkeleton.cpp
//  Created by tim on 12/11/2012.
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

#include "ofxSkeleton.h"

using namespace pal::ofxSkeleton;

// ----------------------------------------------------------------------

ofVboMesh & sBoneMesh() {
	static ofVboMesh mesh(ofConePrimitive( 1.f, 1.f, 4, 2, 2 ).getConeMesh());
	return mesh;
}

// ----------------------------------------------------------------------

ofVboMesh & sJointMesh(){
	 static ofVboMesh mesh(ofSpherePrimitive(1,6).getMesh());
	 return mesh;
}

// ----------------------------------------------------------------------

ofMesh getAxisMesh(){
	ofMesh mesh;

	mesh.setMode(OF_PRIMITIVE_LINES);
	
	ofVec3f vertices[6] = {
		ofVec3f(0,0,0),
		ofVec3f(1,0,0),
		ofVec3f(0,0,0),
		ofVec3f(0,1,0),
		ofVec3f(0,0,0),
		ofVec3f(0,0,1),
	};
	ofFloatColor colors[6] = {
		ofColor::red,
		ofColor::red,
		ofColor::green,
		ofColor::green,
		ofColor::blue,
		ofColor::blue,
	};
	
	mesh.addVertices(vertices, 6);
	mesh.addColors(colors, 6);
	return mesh;
}

// ----------------------------------------------------------------------
/// returns a Mesh representing an XYZ coordinate system.
ofVboMesh & sAxisMesh(){
	// mesh only available as wireframe //
	static ofVboMesh mesh(getAxisMesh());
	return mesh;
}

// ----------------------------------------------------------------------

void ofxJoint::customDraw(float r_) const {

	ofVec3f p1(0);
	ofVec3f p2(0);

	if (shared_ptr<ofxJoint> p = parent.lock()){
		// joint has a parent. draw bone connection from parent joint to this joint.
		p1 = p1 * p->getGlobalTransformMatrix() * ofMatrix4x4::getInverseOf(getGlobalTransformMatrix());

		ofMatrix4x4 mat;
		mat.makeRotationMatrix(p1-p2, ofVec3f(0,1,0));

		ofPushMatrix();
		ofMultMatrix(mat.getInverse());
		ofTranslate(0, (p2-p1).length() * 0.5, 0);
		ofScale(r_,(p2-p1).length(),r_);
		sBoneMesh().drawWireframe();
		ofPopMatrix();
	}
	
	ofPushMatrix();
	ofScale(r_,r_,r_);
	sJointMesh().drawWireframe();
	ofPopMatrix();
}


// ----------------------------------------------------------------------

void ofxJoint::draw(float r_, const ofColor& colour_) const {
	ofPushStyle();
	ofPushMatrix();
	ofMultMatrix(getGlobalTransformMatrix());
	ofSetColor(colour_);
	customDraw(r_);
	ofPopMatrix();
	ofPopStyle();
};

// ----------------------------------------------------------------------

void ofxJoint::drawAxes(float scale_) const {
	ofPushStyle();
	ofPushMatrix();
	ofMultMatrix(getGlobalTransformMatrix());
	ofScale(scale_, scale_, scale_);
	sAxisMesh().draw();
	ofPopMatrix();
	ofPopStyle();
}

void ofxJoint::drawName(const ofColor& colour_) const {
	ofPushStyle();
	ofSetColor(colour_);
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
	ofPushMatrix();
	ofMultMatrix(getGlobalTransformMatrix());
	ofDrawBitmapString(mName, 0, 0);
	ofPopMatrix();
	ofPopStyle();
}

// ----------------------------------------------------------------------

shared_ptr<ofxJoint> ofxJoint::setParent(shared_ptr<ofxJoint> parent_){
	parent = weak_ptr<ofxJoint>(parent_);
	localTransformMatrix =  localTransformMatrix  * ofMatrix4x4::getInverseOf( parent_->getGlobalTransformMatrix());
	return parent_;
};

// ----------------------------------------------------------------------
/// Alias for setParent()
///@detail		Allows you to do the following:	mFoot->bone(mKnee)->bone(mHip);
shared_ptr<ofxJoint> ofxJoint::bone(shared_ptr<ofxJoint> parent_){
	return setParent(parent_);
}

// ----------------------------------------------------------------------

const string& ofxJoint::getName() const {
	return mName;
}

// ----------------------------------------------------------------------

void ofxJoint::setName(string name_) {
	mName = name_;
}

// ----------------------------------------------------------------------

shared_ptr<ofxJoint> ofxJoint::getParent(){
	shared_ptr<ofxJoint> p = parent.lock();
	return (p);
};

// ----------------------------------------------------------------------

void ofxJoint::setTransformMatrix(ofMatrix4x4 m_){
	localTransformMatrix = m_;
};

// ----------------------------------------------------------------------

void ofxJoint::setOrientation(ofQuaternion orientation_){
	localTransformMatrix.setRotate(orientation_);
};

// ----------------------------------------------------------------------

void ofxJoint::setParentGlobalOrientation(ofQuaternion orientation_){
	if (shared_ptr<ofxJoint> p = parent.lock()) {
		p->setGlobalOrientation(orientation_);
		localParentTransformMatrix.makeRotationMatrix(p->getOrientation());
	} else {
		setParentOrientation(orientation_);
		localParentTransformMatrix.makeRotationMatrix(orientation_);
	}
};


// ----------------------------------------------------------------------

const ofQuaternion ofxJoint::getParentOrientation() const{
		return localParentTransformMatrix.getRotate();
};

// ----------------------------------------------------------------------

void ofxJoint::setParentOrientation(ofQuaternion orientation_){
	if (shared_ptr<ofxJoint> p = parent.lock()) {
		p->setOrientation(orientation_);
		localParentTransformMatrix.makeRotationMatrix(p->getOrientation());
	} else {
		localParentTransformMatrix.makeRotationMatrix(orientation_);
	}
};

// ----------------------------------------------------------------------

void ofxJoint::setPosition(ofVec3f position_){
	if (shared_ptr<ofxJoint> p = parent.lock()) {
		localTransformMatrix.setTranslation(position_);
	} else {
		localTransformMatrix.setTranslation(position_);
	}
};

// ----------------------------------------------------------------------

void ofxJoint::setGlobalOrientation(ofQuaternion orientation_){
	ofQuaternion orientationInParentSpace;
	if (shared_ptr<ofxJoint> p = parent.lock()) {
		orientationInParentSpace = orientation_ * p->getGlobalOrientation().inverse();
	} else {
		orientationInParentSpace = orientation_ ;
	}
	localTransformMatrix.setRotate(orientationInParentSpace);
};

// ----------------------------------------------------------------------

void ofxJoint::setGlobalPosition(ofVec3f position_){
	ofVec3f positionInParentSpace;
	if (shared_ptr<ofxJoint> p = parent.lock()){
		positionInParentSpace = position_ * ofMatrix4x4::getInverseOf(p->getGlobalTransformMatrix());
	} else {
		positionInParentSpace = position_ ;
	}
	setPosition(positionInParentSpace);
};

// ----------------------------------------------------------------------

const ofMatrix4x4 ofxJoint::getGlobalTransformMatrix() const {
	if (shared_ptr<ofxJoint> p = parent.lock()) {
		return getLocalTransformMatrix() * p->getGlobalTransformMatrix();
	} else {
		return getLocalTransformMatrix() * localParentTransformMatrix;
	}
}

// ----------------------------------------------------------------------

const ofMatrix4x4 ofxJoint::getLocalTransformMatrix() const {
	return localTransformMatrix;
}

// ----------------------------------------------------------------------

const ofQuaternion ofxJoint::getGlobalOrientation() const {
	return getGlobalTransformMatrix().getRotate();
}

// ----------------------------------------------------------------------

const ofQuaternion ofxJoint::getOrientation() const {
	return getLocalTransformMatrix().getRotate();
}

// ----------------------------------------------------------------------

const ofVec3f ofxJoint::getGlobalPosition() const {
	return getGlobalTransformMatrix().getTranslation();
}

// ----------------------------------------------------------------------

const ofVec3f ofxJoint::getParentGlobalPosition() const {
	if (shared_ptr<ofxJoint> p = parent.lock()){
		return (getParentTransformMatrix() * getGlobalTransformMatrix()).getTranslation();
	} else {
		return ofVec3f(0);
	}
}

// ----------------------------------------------------------------------

const ofVec3f ofxJoint::getParentPosition() const {
	return getParentTransformMatrix().getTranslation();
}

// ----------------------------------------------------------------------

const ofMatrix4x4 ofxJoint::getParentTransformMatrix() const {
	ofMatrix4x4 m;
	
	if (shared_ptr<ofxJoint> p = parent.lock()){
		m = ofMatrix4x4::getInverseOf(getLocalTransformMatrix());
	} else {
		m = localParentTransformMatrix;
	}
	
	return m;
}

// ----------------------------------------------------------------------
const ofMatrix4x4 ofxJoint::getParentGlobalTransformMatrix() const {
	ofMatrix4x4 m;
	if (shared_ptr<ofxJoint> p = parent.lock()){
		m = getParentTransformMatrix() * getGlobalTransformMatrix();
	} else {
		m.setTranslation(0, 0, 0);
		m = m * localParentTransformMatrix;
	}
	return m;
}

// ----------------------------------------------------------------------

const ofQuaternion ofxJoint::getParentGlobalOrientation() const {
	return getParentGlobalTransformMatrix().getRotate();
}

// ----------------------------------------------------------------------
// INVERSE KINEMATICS
// ----------------------------------------------------------------------

void ofxIKchain::solve(){
	ofVec3f targetPosition = targetPositionInGlobalSpace;
	
	for (int i=0; i<1; i++){	// increase loop count to solve with greater precision.
		for (int j=bones.size()-1; j>=0; --j){
			std::shared_ptr<ofxJoint> bone = bones[j];
			// shoot ray from current parent joint to target
			// find relative angle between end effector and target
			
			const ofVec3f effectorPosition = endEffector->getGlobalPosition();
			const ofMatrix4x4 worldToBone = (ofMatrix4x4::getInverseOf(bone->getParentGlobalTransformMatrix()));
			
			ofVec3f localTarget =  targetPosition * worldToBone;
            ofVec3f localEffector = effectorPosition * worldToBone;
			
			if (localEffector.distanceSquared(localTarget) < 0.02) continue;
			
			float angle = localTarget.angle(localEffector);
			if (angle != angle ){
				// this is where we test whether the angle is NaN
				//				ofLogWarning() << "angle is NAN" ;
				continue;
			}
			if (fabs(angle) < 0.001) continue;
			ofVec3f axis = -(localTarget.getCrossed(localEffector)).getNormalized();
			
			ofQuaternion q, p;
			p = bone->getParentOrientation();

			ofVec3f parentOldAxis;
			float parentOldAngle;
			
			// we generate a quaternion with the new orientation
			q.makeRotate(angle, axis);
			q = p * q;	// this rotates by (adds the new angle to) the current orientation
			
			// calculate the total angle
			q.getRotate(parentOldAngle, parentOldAxis);
			// clamp to the max angle.
			q.makeRotate(ofClamp(parentOldAngle,-60,80), parentOldAxis);
			bone->setParentOrientation(q);
		}
	}
}

// ----------------------------------------------------------------------




