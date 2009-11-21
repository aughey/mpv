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


#ifndef _DOUBLEVECTORNODE_H_
#define _DOUBLEVECTORNODE_H_

#include <vector>

#include "Vect2.h"
#include "Vect3.h"
#include "Vect4.h"

#include "Node.h"

namespace mpvs11n
{

//=========================================================
//! 
//! 
class DoubleVectorNode : public Node
{
public:
	//=========================================================
	//! General Constructor
	//! \param n node's name
	//! 
	explicit DoubleVectorNode( const std::string &n );
	
	//! Visitor pattern: called by visitor, turns around and calls 
	//! visitor.visit( something )
	virtual void accept( const NodeVisitor &visitor );
	
	std::vector<double> get() const;
	
	void set( const mpv::Vect2 &v );
	void set( const mpv::Vect3 &v );
	void set( const mpv::Vect4 &v );
	
protected:

	//=========================================================
	//! General Destructor
	//! 
	virtual ~DoubleVectorNode();
	
	std::vector<double> value;
};

}

#endif
