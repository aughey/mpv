#include <stdio.h>

#include <osgSDL/Viewer>
#include <osgSDL/SDLEventAdapter>

#include <osg/State>

#include <osgUtil/SceneView>
#include <osgUtil/DisplayRequirementsVisitor>

#include <osgGA/GUIActionAdapter>
#include <osgDB/Registry>

#include <SDL.h>

#ifdef USE_SPACEBALL
#include <libsball/sball.h>
#endif

osgSDL::Viewer::Viewer()
:	
	GUIActionAdapter()
{
	active_vp_ = -1;
	surf_ = 0;
	frame_stamp_ = new osg::FrameStamp;
	tsrc_ = new RealTimeSource;
	cont_update_ = 0;
	frametime_ = 0;
	disp_settings_ = new osg::DisplaySettings;
	spaceball_handle_ = 0;
	state_ = 0;
	force_single_state_ = true;
	planned_width_ = 800;
	planned_height_ = 600;
	planned_bpp_ = 24;
	planned_flags_ = 0;
	
	initClock();
}

osgSDL::Viewer::Viewer(const Viewer &copy)
{
	// unimplemented
}

osgSDL::Viewer::~Viewer()
{
	if (spaceball_handle_) {
#ifdef USE_SPACEBALL
		sball_close(spaceball_handle_);
#endif
	}
}

bool osgSDL::Viewer::open(SDL_Surface *surf)
{
	if (surf_) return true;

	SDL_EnableUNICODE(1);

	Viewport_list::iterator i;
	for (i=viewports_.begin(); i!=viewports_.end(); ++i) {
		osgUtil::SceneView *sv = i->get()->getSceneView();
		if (sv) {
			const osg::DisplaySettings *ds = sv->getDisplaySettings();
			if (ds) {
				if (disp_settings_.valid()) {
					disp_settings_->merge(*ds);
				}
			} else {
				sv->setDisplaySettings(disp_settings_.get());
			}
		}
	}

	osg::ref_ptr<osgUtil::DisplayRequirementsVisitor> drv = new osgUtil::DisplayRequirementsVisitor;
	drv->setDisplaySettings(disp_settings_.get());

	for (i=viewports_.begin(); i!=viewports_.end(); ++i) {
		osgUtil::SceneView *sv = i->get()->getSceneView();
		if (sv) {
			if (sv->getSceneData()) {
				sv->getSceneData()->accept(*drv);
			}
		}
	}

	GLbitfield clear_mask = 0;
	if (disp_settings_.valid()) {
		if (disp_settings_->getRGB()) clear_mask |= GL_COLOR_BUFFER_BIT;
		if (disp_settings_->getDepthBuffer()) clear_mask |= GL_DEPTH_BUFFER_BIT;
		if (disp_settings_->getStencilBuffer()) clear_mask |= GL_STENCIL_BUFFER_BIT;
	}

	for (i=viewports_.begin(); i!=viewports_.end(); ++i) {
		if (i->get()->getSceneView()) {
			osgUtil::RenderStage *rs = i->get()->getSceneView()->getRenderStage();
			if (rs) {
				rs->setClearMask(rs->getClearMask() & clear_mask);
			}
		}
	}

	if (surf) {
		surf_ = surf;
	} else {
		int clr_size = 5;
		int alpha_size = 1;
		if (planned_bpp_ == 32) {
			clr_size = 8;
			alpha_size = 8;
		}
		if (planned_bpp_ == 24) {
			clr_size = 8;
			alpha_size = 0;
		}
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, clr_size);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, clr_size);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, clr_size);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, alpha_size);
//		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32); // who has a 32-bit depth buffer?
//		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 16);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		if(char* envvar = getenv("OSG_STEREO")) {
			if(strcmp(envvar, "ON")==0) {
				SDL_GL_SetAttribute(SDL_GL_STEREO, 1);
			}
		}
		surf_ = SDL_SetVideoMode(planned_width_, planned_height_, planned_bpp_, planned_flags_ | SDL_OPENGL);
		if (!surf_) return false;
	}	

	SDL_ResizeEvent event;
	event.type = SDL_VIDEORESIZE;	
	event.w = getWidth();
	event.h = getHeight();	
	SDL_PushEvent(reinterpret_cast<SDL_Event *>(&event));

	return true;
}

void osgSDL::Viewer::init() {

	Viewport_list::iterator i;
	for (i=viewports_.begin(); i!=viewports_.end(); ++i) {
		osgUtil::SceneView *sv = i->get()->getSceneView();
		if (sv) {
			sv->init();
		}
	}

	initClock();
	terminated_ = false;

	// send INIT event
	processUserEvent(USER_INIT_EVENT, 0, 0, true, true, true);

}

void osgSDL::Viewer::runOnce() {
	SDL_Event event;

		double t = tsrc_->getTime();

		// poll the Spaceball for data
		if (spaceball_handle_) {
#ifdef USE_SPACEBALL
			poll_spaceball();
#endif
		}

		// update the frame stamp
		frame_stamp_->setFrameNumber(frame_stamp_->getFrameNumber()+1);
		frame_stamp_->setReferenceTime(t);
		frame_stamp_->setSimulationTime(t);

		// if continuous update is needed, fire a FRAME event now
		//if (cont_update_ > 0) {
			processUserEvent(USER_FRAME_EVENT, 0, 0, true, true);
		//}
		
		// check whether a SDL event has been received
		while (SDL_PollEvent(&event) && !terminated_) {
			switch (event.type) {
				case SDL_VIDEORESIZE:
					surf_->w = event.resize.w;
					surf_->h = event.resize.h;
					fire_event(event, true, true, false);
					break;

				case SDL_QUIT:
					terminated_ = true;
					fire_event(event, true, true, false);
					break;

				default:
					fire_event(event);
			}
		}

		// do something (maybe render? :)
		app_idle();

		// update the time spent for processing the last frame
		frametime_ = tsrc_->getTime() - t;

}

bool osgSDL::Viewer::run()
{
	init();

	while (!terminated_) {
		runOnce();
	}

	return true;
}

void osgSDL::Viewer::app_idle()
{
	Viewport_list::iterator i;
	int j = 0;
	// required for Terrex terrain paging
	osgDB::DatabasePager* pager =
		osgDB::Registry::instance()->getDatabasePager();
	if(pager)
	{
		// tell the DatabasePager the current frame number 
		pager->signalBeginFrame( frame_stamp_.get() );
		
		// syncronize changes required by the
		// DatabasePager thread to the scene graph
		pager->updateSceneGraph( frame_stamp_->getReferenceTime() );
	}

	for (i=viewports_.begin(); i!=viewports_.end(); ++i, ++j) {
		if ((*i)->isEnabled()) {
			update_sceneview_viewport((*i).get());
			osgUtil::SceneView *sv = (*i)->getSceneView();
			if (force_single_state_) {
				if (!state_.valid()) {
					state_ = sv->getState();
				}
				if (state_.get() != sv->getState()) {
					sv->setState(state_.get());
				}
			}
			sv->setFrameStamp(frame_stamp_.get());
			if ((*i)->getMatrixManipulator()) {
/*				if (!sv->getViewMatrix()) {
					sv->setViewMatrix(new osg::RefMatrix);
				}
				if (!sv->getProjectionMatrix()) {
					sv->setProjectionMatrix(new osg::RefMatrix(osg::Matrix::perspective(45, 1.333333f, 1, 100000)));
				}*/
				sv->getViewMatrix().set((*i)->getMatrixManipulator()->getInverseMatrix());
			}
			(*i)->setAdditionalNodesEnabled(true);

			update(sv);

			processUserEvent(USER_UPDATE_EVENT, 0, 0, true, true);

			cull(sv);
			draw(sv);

			(*i)->setAdditionalNodesEnabled(false);			
		}
	}

	SDL_GL_SwapBuffers();
}

void osgSDL::Viewer::stop()
{
	terminated_ = true;
}

void osgSDL::Viewer::initClock()
{
	tsrc_->reset();
	frame_stamp_->setFrameNumber(0);
}

bool osgSDL::Viewer::call_global_handlers(const SDLEventAdapter &adapter, bool forcebroadcast)
{
	Event_handler_list::iterator i;
	for (i=hlist_.begin(); i!=hlist_.end(); ++i) {
		if ((*i)->handle(adapter, *this) && !forcebroadcast) return true;
	}
	return false;
}

bool osgSDL::Viewer::call_viewport_handlers(const SDLEventAdapter &adapter, Viewport *vp, bool forcebroadcast)
{
	for (int i=0; i<vp->numEventHandlers(); ++i)
		if (vp->getEventHandler(i)->handle(adapter, *this) && !forcebroadcast) return true;

	if (vp->getMatrixManipulator()) {
		if (adapter.getEventType() == 0xffff) {
			if (adapter.getUserEventID() == USER_INIT_EVENT) {
				vp->getMatrixManipulator()->init(adapter, *this);
				vp->getMatrixManipulator()->home(adapter, *this);
				return true;
			}
		}
		return vp->getMatrixManipulator()->handle(adapter, *this);
	}

	return false;
}

void osgSDL::Viewer::fire_event(const SDL_Event &event, bool allviewports, bool forcebroadcast, bool skipdisabled)
{
	// get the current time and mouse state
	double t = frame_stamp_->getReferenceTime();
	int ms = SDL_GetMouseState(0, 0);

	// call per-viewport handlers, but only if there is an active
	// viewport or a full traversal is requested
	if ((active_vp_) >= 0 || allviewports) {

		// call handlers for active viewport
		Viewport_list::reverse_iterator beg;
		if (allviewports) {
			beg = viewports_.rbegin();
		} else {
			beg = viewports_.rbegin() + viewports_.size() - active_vp_ - 1;
		}
		Viewport_list::reverse_iterator end = allviewports? viewports_.rend() : beg+1;

		for (Viewport_list::reverse_iterator i=beg; i!=end; ++i) {	
			if ((*i)->isEnabled() || !skipdisabled) {
				int x = static_cast<int>((*i)->getPixelLeft(getWidth()));
				int w = static_cast<int>((*i)->getPixelWidth(getWidth()));
				int y = static_cast<int>((*i)->getPixelTop(getHeight()));
				int h = static_cast<int>((*i)->getPixelHeight(getHeight()));
				osg::ref_ptr<SDLEventAdapter> adapter = new SDLEventAdapter(event, x, y, x+w-1, y+h-1, t, ms);
				if (call_viewport_handlers(*adapter, (*i).get(), forcebroadcast) && !forcebroadcast) return;
			}
		}

	}

	// call global handlers
	osg::ref_ptr<SDLEventAdapter> adapter = new SDLEventAdapter(event, 0, 0, getWidth(), getHeight(), t, ms);
	call_global_handlers(*adapter, forcebroadcast);
}

void osgSDL::Viewer::poll_spaceball()
{
#ifdef USE_SPACEBALL
	static Spaceball_data e;
	if (sball_getstatus(spaceball_handle_, &e.tx, &e.ty, &e.tz, &e.rx, &e.ry, &e.rz, &e.buttons) > 0) {
		SDL_Event event;
		event.type = SDL_USEREVENT;
		event.user.code = USER_SPACEBALL_EVENT;
		event.user.data1 = &e;
		SDL_PushEvent(&event);
	}
#endif
}

bool osgSDL::Viewer::initSpaceball(const char *device)
{
#ifdef USE_SPACEBALL
	if (spaceball_handle_ || !device) return false;

	spaceball_handle_ = sball_open(const_cast<char *>(device));
	if (!spaceball_handle_) return false;

	return true;
#else
	return false;
#endif
}

void osgSDL::Viewer::readCommandLine(std::vector<std::string> &cmdline)
{
	if (disp_settings_.valid()) {
		//disp_settings_->readCommandLine(cmdline);
	}
}

bool osgSDL::Viewer::setDisplayMode(int width, int height, int bpp, int flags)
{
	if (surf_) {
		SDL_Surface *newsurf = SDL_SetVideoMode(width, height, bpp, flags | SDL_OPENGL);
		if (newsurf) {
			surf_ = newsurf;
			return true;
		}
	} else {
		planned_width_ = width;
		planned_height_ = height;
		planned_bpp_ = bpp;
		planned_flags_ = flags;
		return true;
	}
	return false;
}

