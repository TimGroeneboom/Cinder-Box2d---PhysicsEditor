#pragma once
#include "b2cinder/Conversions.h"
#include "b2cinder/PhysicsElement.h"
#include "cinder/Vector.h"
#include "cinder/Color.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/Texture.h"

#include "Elements.h"

namespace cinder{
	namespace box2d {

	typedef boost::shared_ptr<class PolyElement> PolyElementRef;

	class PolyElement : public ci::box2d::PhysicsElement{
	public:
		virtual ~PolyElement(){}
		PolyElement( b2World * world, ci::Vec2f pos, Elements::Body body );

		virtual void draw();
		void destroyBody(){ delete this; }
			
		void moveTo( ci::Vec2f pos );
		void moveX( float x );
		void rotateTo( float angle );
	private:
		b2PolygonShape mShape;
		ci::gl::Texture mTexture;
	};
}

}