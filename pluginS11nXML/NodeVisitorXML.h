/** <pre>
 *  MPV Serialization Framework
 *  Copyright (c) 2008 Andrew Sampson
 *  
 *  This component is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; specifically 
 *  version 2.1 of the License.
 *  
 *  This software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *  
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this software; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *  
 *  
 *  2008-12-29 Andrew Sampson
 *      Initial release
 *  
 *  
 *  </pre>
 */


#ifndef _NODEVISITORXML_H_
#define _NODEVISITORXML_H_

#include "GroupNode.h"
#include "StringNode.h"
#include "IntNode.h"
#include "FloatNode.h"
#include "DoubleNode.h"
#include "DoubleVectorNode.h"
#include "NodeVisitor.h"

//=========================================================
//! 
//! 
class NodeVisitorXML : public mpvs11n::NodeVisitor
{
public:
	//=========================================================
	//! General Constructor
	//! 
	NodeVisitorXML( std::ostream &os );
	
	//=========================================================
	//! General Destructor
	//! 
	virtual ~NodeVisitorXML();
	
	virtual void visit( mpvs11n::GroupNode  	  &n ) const;
	virtual void visit( mpvs11n::StringNode 	  &n ) const;
	virtual void visit( mpvs11n::IntNode		  &n ) const;
	virtual void visit( mpvs11n::FloatNode  	  &n ) const;
	virtual void visit( mpvs11n::DoubleNode 	  &n ) const;
	virtual void visit( mpvs11n::DoubleVectorNode &n ) const;
	
protected:
	
	//! The level of indentation.  Incremented and decremented as the 
	//! visitor descends and ascends group nodes.
	mutable int indentLevel;
	
	mutable std::string indentString;
	
	void increaseIndent() const;
	void decreaseIndent() const;
	
	void startElement( mpvs11n::Node &n ) const;
	void endElement( mpvs11n::Node &n ) const;
};

#endif
