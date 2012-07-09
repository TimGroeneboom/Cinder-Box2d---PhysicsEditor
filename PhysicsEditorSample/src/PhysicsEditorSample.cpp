#include "cinder/app/AppBasic.h"
#include <b2cinder/b2cinder.h>

#include "PolyElement.h"

using namespace ci;
using namespace ci::app;
using namespace ci::box2d;

class PhysicsEditorSample : public AppBasic {
public:
	void prepareSettings(Settings *settings);
	
	void setup();
	void update();
	void draw();
	void shutdown();
	
	void mouseDown( MouseEvent event );
	
private:
	Sandbox mSandbox;

	std::vector<PolyElementRef> mPolyElements;
	ElementsRef					mElements;
};

void PhysicsEditorSample::prepareSettings(Settings *settings)
{
	settings->setWindowSize(1024, 768);
	settings->setTitle("PhysicsEditorSample");
}

void PhysicsEditorSample::setup()
{
	mSandbox.init();

	mElements = ElementsRef( new Elements() );

	mElements->setup( loadAsset( "elements.xml" ) );

	const ElementsList & elementsList = mElements->getElements();
	for( auto elementsListItr = elementsList.begin(); elementsListItr != elementsList.end() ; ++elementsListItr ){
		mPolyElements.push_back( PolyElementRef( new PolyElement(	mSandbox.getWorld(),
																	Vec2f( ci::Rand::randFloat( 0, app::getWindowWidth() ), ci::Rand::randFloat( 0, app::getWindowHeight() ) ), 
																	elementsListItr->second ) ) );
	}
}

void PhysicsEditorSample::mouseDown( MouseEvent event )
{
	mPolyElements.push_back( PolyElementRef( new PolyElement(	mSandbox.getWorld(), event.getPos(), mElements->getElement( "Mario1" ) ) ) );
}

void PhysicsEditorSample::update()
{
	mSandbox.update();
}

void PhysicsEditorSample::draw()
{
	gl::clear(Color::white());
	gl::enableAlphaBlending();
	
	mSandbox.draw();
//	mSandbox.debugDraw(true, true);
}

void PhysicsEditorSample::shutdown()
{
	
}


// This line tells Cinder to actually create the application
CINDER_APP_BASIC( PhysicsEditorSample, RendererGl )
