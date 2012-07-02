#pragma once
#include "b2cinder/Conversions.h"
#include "b2cinder/PhysicsElement.h"

#include "cinder/Vector.h"
#include "cinder/Color.h"
#include "cinder/app/AppBasic.h"
#include "cinder/Xml.h"

namespace cinder{
	namespace box2d {

	class Elements {
	public:
		struct Polygon{
			std::vector<Vec2f> vertexes;
		};
		struct Circle{
			float		radius;
			ci::Vec2f	position;
		};
		struct Fixture{
			std::vector<Polygon>	polygons;
			std::vector<Circle>		circles;

			uint16 maskBits;
			uint16 catagoryBits;

			float density;
			float restitution;
			float friction;

			std::string type;
		};
		struct Body{
			std::string name;
			std::vector<Fixture> fixtures;
		};
	public:
		static Elements* get();

		~Elements();

		void setup(std::string file);
		const Body& getElement(std::string id);
	private:
		Elements();
		std::map<std::string, Body> mElements;
	};
}

}