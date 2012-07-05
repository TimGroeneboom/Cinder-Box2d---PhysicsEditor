/*
 *  DynamicBody.h
 *  BasicBox2D
 *
 *  Created by David Wicks on 6/9/10.
 *  Copyright 2010 David Wicks. All rights reserved.
 *
 *	Base class for elements added to Box2D simulation
 *	Sandbox casts b2Body::UserData to this type for rendering
 *	
 *
 */

#pragma once

#include <Box2D/Box2D.h>
#include <list>
#include "b2cinder/Conversions.h"
#include "cinder/app/App.h"

namespace cinder {
	namespace box2d {
		class PhysicsElement {
			
		public:
			PhysicsElement(b2World * world);
			virtual ~PhysicsElement();

			// things necessary for defining objects in physics simulation
			b2BodyDef* getBodyDef(){ return &mBodyDef; }
			
			// careful with this one; mBody is undefined until set
			b2Body* getBody(){ return mBody; }

			inline void applyForce( Vec2f force, Vec2f pos ){ mBody->ApplyForce( Conversions::toPhysics(force), Conversions::toPhysics(pos) ); }
			
			// update any settings applied to the body
			virtual void update(){}

			// render the body on screen
			virtual void draw(){}

			void setGravityScale( float scale );

			//
			Vec2f getScreenPos(){ return Conversions::toScreen( mBody->GetPosition() ); }
			float getAngleDegrees(){ return Conversions::radiansToDegrees( mBody->GetAngle() ); }
			
			// add special attributes
			inline bool getStopVelocity(){ return mStopVelocity; };
			inline void setStopVelocity(bool val = false){ mBody->SetLinearVelocity( b2Vec2(0,0) ); mStopVelocity = val; };

			inline bool getStopAngularVelocity(){ return mStopAngularVelocity; };
			inline void setStopAngularVelocity(bool val = false){ mBody->SetAngularVelocity( 0.0f ); mStopAngularVelocity = val; };

			inline bool getStopXVelocity(){ return mStopXVelocity; };
			inline void setStopXVelocity(bool val = false){ mBody->SetLinearVelocity( b2Vec2(0,mBody->GetLinearVelocity().y) ); mStopXVelocity = val; };
		private:
			
			
		protected:
			b2BodyDef mBodyDef;
			
			// temporary fixture def and list for all defs
			b2FixtureDef mFixtureDef;
			std::list<b2FixtureDef> mFixtureDefs;

			// only gets set when the body is added to the b2World
			b2Body* mBody; 

			//
			b2World* mWorldPtr;

			// these values indicate if the velocity has been changed artificially for player movement
			bool mStopVelocity;
			bool mStopAngularVelocity;
			bool mStopXVelocity;
		};
	}
}

