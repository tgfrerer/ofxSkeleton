//
//     _____    ___
//    /    /   /  /     ofxSkeleton
//   /  __/ * /  /__    (c) ponies & light, 2012, 2013. All rights reserved.
//  /__/     /_____/    poniesandlight.co.uk
//
//  ofxSkeleton.h
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

#pragma once

#include "ofMain.h"

namespace pal {
	namespace ofxSkeleton {
		
		/**
		 * @brief   A connector of bones in 3d space.
		 *
		 * @detail  A bone is what links two joints.
		 *          A joint is linked to its parent joint through a bone.
		 *          A joint can have many children, but only one parent.
		 *          A joint that has no parents is a root joint.
		 *
		 * @note    By default, any position / orientation operations on joints
		 *          are relative to its parent joint. These are local
		 *          operations. If you want to set the global orientaion /
		 *          position of a joint, use the equivalent setGlobal*
		 *          methods.
		 *
		 * @author tig
		 *
		 * @date   2013-10-25
		 */
		class ofxJoint {
			
		private:
			
			weak_ptr<ofxJoint>					parent;
			ofMatrix4x4							localTransformMatrix;
			ofMatrix4x4							localParentTransformMatrix;
			string								mName;

			virtual void						customDraw(float r)												const;
			
		public:
			
			
			void								setTransformMatrix(ofMatrix4x4 m_);
			void								setOrientation(ofQuaternion orientation_);
			void								setParentOrientation(ofQuaternion orientation_);
			
			void								setPosition(ofVec3f position_);
			void								setGlobalOrientation(ofQuaternion orientation_);
			void								setParentGlobalOrientation(ofQuaternion orientation_);
			
			void								setGlobalPosition(ofVec3f position_);
			shared_ptr<ofxJoint>				setParent(shared_ptr<ofxJoint> parent_);
			shared_ptr<ofxJoint>				bone(shared_ptr<ofxJoint> parent_);
			
			void								setName(string name_);
			
			void								draw(float r_ = 4.0, const ofColor& colour_ = ofColor::white)	const;
			void								drawAxes(float scale_ = 10.0)									const;
			void								drawName(const ofColor& colour_ = ofColor::yellow)				const;
			
			const string&						getName()														const;
			shared_ptr<ofxJoint>				getParent();
			
			const ofQuaternion					getOrientation()												const;
			const ofQuaternion					getParentOrientation()											const;
			const ofMatrix4x4					getLocalTransformMatrix()										const;
			const ofMatrix4x4					getParentTransformMatrix()										const;
			const ofVec3f						getParentPosition()												const;
			
			const ofMatrix4x4					getGlobalTransformMatrix()										const;
			const ofMatrix4x4					getParentGlobalTransformMatrix()								const;
			
			const ofQuaternion					getParentGlobalOrientation()									const;
			const ofQuaternion					getGlobalOrientation()											const;
			const ofVec3f						getGlobalPosition()												const;
			const ofVec3f						getParentGlobalPosition()										const;
			
			ofxJoint()
			:parent()	// will automatically initialize as a NULL shared ptr.
			{};
			
		};

		// ----------------------------------------------------------------------
		// Human skeleton helper methods.
		
		/**
		 * @brief   Calculates hip and knee orientation
		 *
		 * @param   vHip_    global position of hip joint
		 * @param   vKnee_   global position of knee joint
		 * @param   vFoot_   global position of foot joint
		 *
		 * @return  qHip_    global orientation at hip joint
		 * @return  qKnee_   global orientation at knee joint
		 *
		 * @detail  Orientations are calculated based on the global position of
		 *          hip, knee, foot, and the assumption that the knee joint
		 *          has only one degree of freedom (around its local x-axis).
		 *
		 * @note    After applying these orientations, you need to make sure to
		 *          reset the joint positions, since they might have changed.
		 *
		 * @author tig
		 */
		static void calculateHipKneeOrientation(const ofVec3f& vHip_, const ofVec3f& vKnee_, const ofVec3f& vFoot_, ofQuaternion& qKnee_, ofQuaternion& qHip_) {
			ofVec3f uHK = (vKnee_ - vHip_).getNormalized();			// e
			ofVec3f uKF = (vFoot_ - vKnee_).getNormalized();		// f
			
			ofMatrix4x4 rotM;
			
			// If Hip, knee, Foot are in one straight line, we cannot calculate axes from cross products.
			// but it is safe to assume zero rotation, so we initialize them with zero rotation and then
			// check if they are *not* in a straight line.
			
			qKnee_ = ofQuaternion();
			qHip_  = ofQuaternion();
			
			if (fabs(uHK.dot(uKF)) < 0.9999 ){
				
				// ----------| invariant: hip, knee, foot *not* in a straight line
				
				// this means, Hip-Knee-Foot span a plane. We know that our knee's local x axis must be perpendicular to
				// this plane, since the knee can only rotate around it's local x axis.
				// because their twists are linked, the hip shares the knee's local x-axis.
				// the local y-axis at a joint always points in the opposite direction of the bone.
				// with this, we can calculate the local z-coordinate as a cross product of local x and local y.
				
				ofVec3f ax  = uHK.getCrossed(uKF);	// local x-axis
				ofVec3f ayK = -uKF;					// local y-axis (points in the opposite direction of the bone)
				ofVec3f azK = ax.getCrossed(ayK);	// local z-axis
				
				rotM.set( ax.x,  ax.y,  ax.z, 0,
						 ayK.x, ayK.y, ayK.z, 0,
						 azK.x, azK.y, azK.z, 0,
						 0,     0,     0, 1);	// generates a rotation matrix.
				qKnee_ = rotM.getRotate();
				
				// for ax, we re-use local x-axis
				ofVec3f ayH = -uHK;					// local y-axis
				ofVec3f azH = ax.getCrossed(ayH);	// local z-axis
				
				rotM.set( ax.x,  ax.y,  ax.z, 0,
						 ayH.x, ayH.y, ayH.z, 0,
						 azH.x, azH.y, azH.z, 0,
						 0,     0,     0, 1);	// generates a rotation matrix.
				
				qHip_ = rotM.getRotate();
			}
		}
		
		// --------------------------------------------------------
		// this is currently only in prototype stage:
		
		class ofxIKchain {
		public:
			vector<shared_ptr<ofxJoint> >		bones;
			shared_ptr<ofxJoint>				endEffector;
			ofVec3f								targetPositionInGlobalSpace;
			
			void								solve();
		};
		
	}	// namespace ofxSkeleton
}	// namespace pal
