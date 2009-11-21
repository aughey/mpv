/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2006 Andrew Sampson
 *  
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *  
 *  
 *  2006-10-29 Andrew Sampson
 *      Initial release
 *  
 *  2008-08-17 Andrew Sampson
 *      Ported to new Component interface
 *  
 */


#ifndef _SCALE_COMPONENT_IMP_H_
#define _SCALE_COMPONENT_IMP_H_

#include <stdio.h>
#include <iostream>
#include <osg/MatrixTransform>

#include "Component.h"

//=========================================================
//! 
//! 
class ScaleComponentImp : public mpv::ComponentImp
{
public:
	
	//=========================================================
	//! Constructor
	//! \param newTransform - the transform node that this component is associated with
	//! 
	ScaleComponentImp( mpv::Component *_component, osg::MatrixTransform *newTransform );
	
	
private:
	virtual ~ScaleComponentImp();
	
	void componentDataChanged( mpv::Component * );
	
	//=========================================================
	//! The transform node that this component is associated with
	//! 
	osg::ref_ptr<osg::MatrixTransform> transform;
	
};

#endif
