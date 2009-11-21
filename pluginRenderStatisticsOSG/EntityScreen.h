/*




*/

#ifndef ENTITY_SCREEN_H
#define ENTITY_SCREEN_H

#include <list>
#include <osg/Geode>
#include <osgText/Text>

#include "Entity.h"
#include "EntityContainer.h"
#include "OverlayScreen.h"

class EntityScreen : public mpvosg::OverlayScreen
{
public:

	EntityScreen();
	virtual ~EntityScreen();
	
	virtual void resetView( const mpv::View *viewParams );
	virtual void act( double deltaT );
	
	virtual osg::Node *getNode();
	
	void setEntityList( mpv::EntityContainer *entityContainer )
	{
		allEntities = entityContainer;
	}
	
protected:

	//=========================================================
	//! 
	//! 
	osg::ref_ptr<osg::Geode> geode;
	
	//=========================================================
	//! An entity container containing all active entities. 
	//! Retrieved from the blackboard.
	//! 
	mpv::RefPtr<mpv::EntityContainer> allEntities;

	//=========================================================
	//! Text object for displaying the entity summary table.
	//! Note that geode stores a reference to this variable, so an 
	//! additional reference-counted pointer isn't needed.
	//! 
	osgText::Text *tableText;
	
	void createTextBox();

};

#endif

