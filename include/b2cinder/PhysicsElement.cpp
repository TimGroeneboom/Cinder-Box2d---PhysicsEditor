/*
*  DynamicBody.cpp
*  BasicBox2D
*
*  Created by David Wicks on 6/9/10.
*  Copyright 2010 David Wicks. All rights reserved.
*
*/

#include "PhysicsElement.h"

namespace cinder
{
	namespace box2d 
	{
		PhysicsElement::~PhysicsElement(){
			if( mWorldPtr && mBody ){
				mWorldPtr->DestroyBody(mBody);
				mBody = NULL;
			}
		}

		PhysicsElement::PhysicsElement(b2World * world)
		{
			mWorldPtr = world;
			mBody = NULL;

		}

		void PhysicsElement::setGravityScale( float scale ){
			if( mBody ){
				mBody->SetGravityScale( scale );
			}
		}
	}
}
