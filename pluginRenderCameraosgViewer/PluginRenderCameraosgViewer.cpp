#include <iostream>

#include <osgGA/StateSetManipulator>

#include "PluginRenderCameraosgViewer.h"
#include <osgViewer/ViewerEventHandlers>

using namespace mpv;

// PLUGINRENDERCAMERAOSG //////////////////////////////////////////////

EXPORT_DYNAMIC_CLASS( PluginRenderCameraosgViewer )

  class EventProxyHandler : public osgGA::GUIEventHandler
{
  public:
    EventProxyHandler( std::list< osg::ref_ptr<osgGA::GUIEventAdapter> > *_eventList ) 
    {
      eventList = _eventList;
    }

    bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &)
    {
      // the cast is to remove the const-ness
      eventList->push_back( (osgGA::GUIEventAdapter *)(&ea) );

      return false;
    }

  private:
    std::list< osg::ref_ptr<osgGA::GUIEventAdapter> > *eventList;
};


// ================================================
// PluginRenderCameraosgViewer
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderCameraosgViewer::PluginRenderCameraosgViewer() : Plugin() 
{
  name_ = "PluginRenderCameraosgViewer";
  licenseInfo_.setLicense( LicenseInfo::LicenseGPL );
  licenseInfo_.setOrigin( "Community" );
  dependencies_.push_back( "PluginRenderCameraOSGNode" );

  viewer = new osgViewer::Viewer;
  viewer->addEventHandler(new osgViewer::StatsHandler);
  viewer->addEventHandler(new osgViewer::ThreadingHandler);
  viewer->addEventHandler(new osgViewer::LODScaleHandler);
  viewer->addEventHandler(new osgViewer::ScreenCaptureHandler);
  viewer->addEventHandler(new osgViewer::HelpHandler);
  viewer->addEventHandler(new osgGA::StateSetManipulator(viewer->getCamera()->getOrCreateStateSet()));

  scene = 0;

  std::cout << "PluginRenderCameraosgViewer created\n";
}


// ================================================
// ~PluginRenderCameraosgViewer
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderCameraosgViewer::~PluginRenderCameraosgViewer() throw() 
{


}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderCameraosgViewer::act( SystemState::ID state, StateContext &stateContext )
{
  switch( state )
  {
    case SystemState::BlackboardRetrieve:
      std::list< osg::ref_ptr<osgGA::GUIEventAdapter> > *eventList;
      bb_->get( "GUIEventList", eventList );
      //
      // Create a handler that will bounce the GUI events emitted by
      // this viewer into the eventHandlerList
      viewer->addEventHandler(new EventProxyHandler(eventList));
      break;

    case SystemState::Standby:
    case SystemState::DatabaseLoad:
    case SystemState::Operate:
    case SystemState::Debug:

      if(scene == 0) {
        // Look for the scene data in the BB
        bb_->get( "OSGNode", scene );
        if(scene == 0) {
          std::cout << "PluginRenderCameraosgViewer didn't find scene data in bb\n";
          return;
        }
        viewer->setSceneData(scene);
        viewer->realize();
      }
      viewer->frame();

      break;

    case SystemState::Shutdown:
      break;

    default:
      break;
  }
}

