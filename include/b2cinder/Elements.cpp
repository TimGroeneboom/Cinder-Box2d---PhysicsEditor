#include "Elements.h"

using namespace ci;
using namespace box2d;

Elements* Elements::get(){
	static Elements * instance = NULL;

	if(!instance){
		instance = new Elements();
	}

	return instance;
}

Elements::Elements(){}
Elements::~Elements(){}

void Elements::setup(std::string file){
	XmlTree doc( loadFile( file ) );

	auto bodies = doc.begin("bodydef/bodies");
	for( XmlTree::Iter body = bodies->begin("body"); body != bodies->end(); ++body ){
		Body element;
		element.name = body->getAttributeValue<std::string>("name", "unknown");

		std::vector<Fixture> bodyFixtures;
		for( XmlTree::Iter fixture = body->begin("fixture") ; fixture != body->end(); ++fixture ){
			Fixture bodyFixture;

			bodyFixture.maskBits		= fixture->getAttributeValue<uint16>( "filter_maskBits" );
			bodyFixture.catagoryBits	= fixture->getAttributeValue<uint16>( "filter_categoryBits" );
			bodyFixture.density			= fixture->getAttributeValue<float>( "density" );
			bodyFixture.friction		= fixture->getAttributeValue<float>( "friction" );
			bodyFixture.restitution		= fixture->getAttributeValue<float>( "restitution" );
			bodyFixture.type			= fixture->getAttributeValue<std::string>( "type" );

			for( XmlTree::Iter polygon = fixture->begin("polygon"); polygon != fixture->end(); ++polygon){
				Polygon fixturePolygon;

				for( XmlTree::Iter vertex = polygon->begin("vertex"); vertex != polygon->end(); ++vertex)
					fixturePolygon.vertexes.push_back( Vec2f( vertex->getAttributeValue<float>("x"), vertex->getAttributeValue<float>("y") ) );

				bodyFixture.polygons.push_back(fixturePolygon);
			}
			for( XmlTree::Iter circle = fixture->begin("circle"); circle != fixture->end(); ++circle ){
				Circle fixtureCircle;

				fixtureCircle.radius = circle->getAttributeValue<float>( "r" );
				fixtureCircle.position = Vec2f( circle->getAttributeValue<float>( "x" ), circle->getAttributeValue<float>( "y" ) );

				bodyFixture.circles.push_back( fixtureCircle );
			}

			element.fixtures.push_back(bodyFixture);
		}

		mElements[element.name] = element;
	}
}

const Elements::Body& Elements::getElement(std::string id){
	return mElements[id];
}
