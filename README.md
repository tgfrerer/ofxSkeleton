
	   _____    ___     
	  /    /   /  /     ofxSkeleton
	 /  __/ * /  /__    (c) ponies & light, 2012, 2013. All rights reserved.
	/__/     /_____/    poniesandlight.co.uk

	ofxSkeleton.cpp
	Created by tim on 12/11/2012.

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.

# ofxSkeleton

This addon allows you to build (and debug-draw) skeleton structures, useful for skeletal animation and skinning. 

## Use case:

	typedef shared_ptr<pal::ofxSkeleton::ofxJoint>  JointP_t;
		
	for (int i = 0; i < 4; i++){
		mSkeleton.push_back(JointP_t(new ofxJoint));
	}
	
	mFoot	= mSkeleton[0];
	mKnee	= mSkeleton[1];
	mHip	= mSkeleton[2];
	mRoot	= mSkeleton[3];

	mFoot->setName("Foot");
	mKnee->setName("Knee");
	mHip->setName("Hip");
	mRoot->setName("Root");

	// build skeleton based on joints.
	mFoot->bone(mKnee)->bone(mHip)->bone(mRoot);

	mRoot->setGlobalPosition(ofVec3f(0));
	
	mHip ->setGlobalPosition(ofVec3f(0,100,0));
	mKnee->setGlobalPosition(ofVec3f(0,50,0));
	mFoot->setGlobalPosition(ofVec3f(0,-50,0));

Now, you can rotate the hip joint, for example, as in: 
