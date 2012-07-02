
#include "PolyElement.h"

#include "cinder/ImageIo.h"

using namespace ci;
using namespace box2d;

PolyElement::PolyElement( b2World * world, Vec2f pos, Elements::Body body ) 
	: PhysicsElement( world )
{
	mBodyDef.position.Set(	Conversions::toPhysics( pos.x ), Conversions::toPhysics( pos.y ) );		
	mFixtureDef.shape = &mShape;
	mFixtureDef.friction = 0.3f;
	mFixtureDef.restitution = 0.15f;
	mFixtureDef.density = 1.0f;

	mBodyDef.type = b2_dynamicBody;

	// keep around the pointers for deletion later on, doesn't get the nobel prize but works
	std::vector<b2Shape*> shapes_collection;
	std::vector<b2Vec2*> vertexes_collection;

	for(size_t i = 0 ; i < body.fixtures.size() ; i++){
		Elements::Fixture fixture = body.fixtures[i];

		for(size_t j = 0 ; j < fixture.polygons.size() ; j++){
			b2PolygonShape *shape = new b2PolygonShape();
			shapes_collection.push_back(shape);
				
			Elements::Polygon polygon = fixture.polygons[j];
			b2Vec2 *vertexes = new b2Vec2[polygon.vertexes.size()];
			vertexes_collection.push_back(vertexes);

			b2FixtureDef b2Fixture = mFixtureDef;
				
			for(size_t k = 0 ; k < polygon.vertexes.size() ; k++){
				vertexes[k] = b2Vec2( Conversions::toPhysics( polygon.vertexes[k].x ), Conversions::toPhysics( polygon.vertexes[k].y ) );
			}
			shape->Set(vertexes, polygon.vertexes.size());

			b2Fixture.shape = shape;
			b2Fixture.filter.categoryBits = fixture.catagoryBits;
			b2Fixture.filter.maskBits = fixture.maskBits;
			b2Fixture.restitution = fixture.restitution;
			b2Fixture.friction = fixture.friction;
			b2Fixture.density = fixture.density;

			mFixtureDefs.push_back( b2Fixture );
		}
		for( size_t j = 0 ; j < fixture.circles.size() ; j++ ){
			b2CircleShape *shape = new b2CircleShape();
			shapes_collection.push_back(shape);

			b2FixtureDef b2Fixture = mFixtureDef;

			Elements::Circle circle = fixture.circles[j];
			shape->m_p = b2Vec2( Conversions::toPhysics( circle.position.x ), Conversions::toPhysics( circle.position.y ) );
			shape->m_radius = Conversions::toPhysics( circle.radius );

			b2Fixture.shape = shape;
			b2Fixture.filter.categoryBits = fixture.catagoryBits;
			b2Fixture.filter.maskBits = fixture.maskBits;
			b2Fixture.restitution = fixture.restitution;
			b2Fixture.friction = fixture.friction;
			b2Fixture.density = fixture.density;

			mFixtureDefs.push_back(	b2Fixture );
		}
	}

	// get the associated texture
	mTexture = gl::Texture( loadImage( "../" + body.name + ".png" ) );

	mBody = mWorldPtr->CreateBody( &mBodyDef );

	// add all fixture defs
	std::list<b2FixtureDef>* defs = &mFixtureDefs;
	for( std::list<b2FixtureDef>::iterator iter = defs->begin(); iter != defs->end(); ++iter )
	{
		mBody->CreateFixture( &*(iter) );
	}

	// make a circular reference between PhysicsElement and b2Body
	mBody->SetUserData(this);

	// delete any shapes 
	for( size_t i = 0 ; i < shapes_collection.size(); i++ )
		delete shapes_collection[i];

	// delete any vertex arrays
	for( size_t i = 0 ; i < vertexes_collection.size(); i++ )
		delete vertexes_collection[i];	
}

void PolyElement::draw()
{
	Vec2f pos = Conversions::toScreen( mBody->GetPosition() );
	float t = Conversions::radiansToDegrees( mBody->GetAngle() );

	gl::color( Color::white() );

	glPushMatrix();
	gl::translate( pos );
	gl::rotate( t );
	gl::translate( (float)(-mTexture.getWidth()/2), (float)(-mTexture.getHeight()/2) );

	gl::draw( mTexture );

	glPopMatrix();
}

void PolyElement::moveTo( Vec2f pos ){
	b2Vec2 newPos = Conversions::toPhysics( pos );
	b2Vec2 dis	= newPos - mBody->GetPosition();
	dis			*= 30.0f;

	mBody->SetLinearVelocity( dis );
	mStopVelocity = true;
}

void PolyElement::moveX( float x ){
	float dis	=	Conversions::toPhysics(x) - mBody->GetPosition().x;
	dis			*= 30.0f;

	mBody->SetLinearVelocity( b2Vec2( dis, mBody->GetLinearVelocity().y ) );
	mStopXVelocity = true;
}

void PolyElement::rotateTo( float angle ){
	float totalRotation = angle - mBody->GetAngle();
	while ( totalRotation < -180.0f * DEGTORAD ) totalRotation += 360.0f * DEGTORAD;
	while ( totalRotation >  180.0f * DEGTORAD ) totalRotation -= 360.0f * DEGTORAD;

	mBody->SetAngularVelocity( totalRotation * 30.0f );
	mStopAngularVelocity = true;
}