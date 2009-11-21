#include <osgSDL/Viewport>
#include <osgSDL/ViewportFrame>

#include <osg/Geode>
#include <osg/Projection>
#include <osg/StateSet>
#include <osg/Depth>
#include <osg/MatrixTransform>
#include <osgDB/Registry>

using namespace osgSDL;

Viewport::Viewport(osgUtil::SceneView *sv)
:	osg::Referenced(),
	left_(0), 
	top_(0), 
	width_(1), 
	height_(1), 
	sceneview_(sv), 
	enabled_(true), 
	add_nodes_(new osg::Group), 
	allow_focus_(true),	
	focused_(false)
{
	printf( "Warning in osgSDL::Viewport - getCullVisitor must be called after setDefaults.\n"
	"For this reason, is was not possible to set up the database pager \n"
	"in the Viewport constructor.  The effects of this are unknown, but I \n"
	"would guess that this means that the paging stuff won't work right.\n" );
}

Viewport::Viewport(osg::Node *node)
:	osg::Referenced(),
	left_(0), 
	top_(0), 
	width_(1), 
	height_(1), 
	sceneview_(new osgUtil::SceneView), 
	enabled_(true), 
	add_nodes_(new osg::Group), 
	allow_focus_(true),
	focused_(false)
{
	sceneview_->setDefaults();
	sceneview_->setSceneData(node);

	SetupDatabasePager();
}

Viewport::Viewport(float l, float t, float w, float h, osgUtil::SceneView *sv)
:	osg::Referenced(),
	left_(l), 
	top_(t), 
	width_(w), 
	height_(h), 
	sceneview_(sv), 
	enabled_(true), 
	add_nodes_(new osg::Group), 
	allow_focus_(true),
	focused_(false)
{
	printf( "Warning in osgSDL::Viewport - getCullVisitor must be called after setDefaults.\n"
	"For this reason, is was not possible to set up the database pager \n"
	"in the Viewport constructor.  The effects of this are unknown, but I \n"
	"would guess that this means that the paging stuff won't work right.\n" );
}	

Viewport::Viewport(float l, float t, float w, float h, osg::Node *node)
:	osg::Referenced(),
	left_(l), 
	top_(t), 
	width_(w), 
	height_(h), 
	sceneview_(new osgUtil::SceneView), 
	enabled_(true), 
	add_nodes_(new osg::Group), 
	allow_focus_(true),
	focused_(false)
{
	sceneview_->setDefaults();
	sceneview_->setSceneData(node);

	SetupDatabasePager();
}

Viewport::~Viewport()
{
	// get the pager
	osgDB::DatabasePager* pager =
		osgDB::Registry::instance()->getOrCreateDatabasePager();
	// Delete the old pager which will allow the paging thread to
	// cleanly exit.
	sceneview_->getCullVisitor()->setDatabaseRequestHandler(NULL);
	osgDB::Registry::instance()->setDatabasePager(NULL);
}


void Viewport::SetupDatabasePager()
{
	// setup the database pager
	osgDB::DatabasePager* pager =
		osgDB::Registry::instance()->getOrCreateDatabasePager();
	
	// Set whether the DatabasePager thread should be blocked while
	// the scene graph is being used to render a frame.  Setting
	// frame block to true can help achieve constant frame rates on
	// single CPU systems.
//	pager->setUseFrameBlock(false);
	// currently it is set per txp terrain instead of the entire scene
	// getCullVisitor must be called after setDefaults()
	sceneview_->getCullVisitor()->setDatabaseRequestHandler(pager);
	// how long to hold onto tiles that haven't been displayed
	pager->setExpiryDelay(2);
}


void Viewport::createViewportFrame()
{
	float w = 480;
	float h = 640;
	
	osg::Projection *proj = new osg::Projection;
	proj->setMatrix(osg::Matrix::ortho2D(0, w, 0, h));

	osg::MatrixTransform *xform = new osg::MatrixTransform;
	proj->addChild(xform);
	
	xform->setReferenceFrame(osg::MatrixTransform::ABSOLUTE_RF);
	xform->setMatrix(osg::Matrix::identity());

	osg::StateSet *ss = new osg::StateSet;
	xform->setStateSet(ss);

	osg::Depth *depth = new osg::Depth;
	depth->setFunction(osg::Depth::ALWAYS);
	depth->setRange(0, 0);
	ss->setAttribute(depth);

	ss->setMode(GL_LIGHTING, osg::StateAttribute::OFF);	
	
	osg::Geode *geode = new osg::Geode;
	geode->addDrawable(new ViewportFrame(w, h, this));
	
	xform->addChild(geode);

	addAdditionalNode(proj);	
}

