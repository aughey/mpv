/** <pre>
 *  Plugin to add text labels to entities, for the MPV
 *  Copyright (c) 2008 Andrew Sampson
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
 *  2008-08-18 Andrew Sampson
 *      Initial release.  Split code out from LabelElement.
 *  
 *  
 *  </pre>
 */


#ifndef _LABEL_NODE_COMPONENT_IMP_H_
#define _LABEL_NODE_COMPONENT_IMP_H_


#include "Component.h"

#include "LabelNode.h"

//=========================================================
//! 
//! 
class LabelNodeComponentImp : public mpv::ComponentImp
{
public:
	//=========================================================
	//! General Constructor
	//! \param newLabel - the label node that this component is associated with
	//! 
	LabelNodeComponentImp( 
		mpv::Component *_component, 
		const std::string &defaultText, 
		LabelNode *newLabel );
	
protected:
	//=========================================================
	//! General Destructor
	//! 
	virtual ~LabelNodeComponentImp();
	
	void componentDataChanged( mpv::Component * );
	
	//=========================================================
	//! 
	std::string labelText;
	
	//=========================================================
	//! the label node that this component is associated with
	//! 
	osg::ref_ptr<LabelNode> labelNode;
	
};

#endif
