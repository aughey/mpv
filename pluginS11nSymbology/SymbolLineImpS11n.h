/** <pre>
 *  MPV Serialization Framework
 *  Copyright (c) 2009 Andrew Sampson
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
 *  2009-09-06 Andrew Sampson
 *      Initial release
 *
 * </pre>
 */


#ifndef SYMBOLLINEIMPS11N_H
#define SYMBOLLINEIMPS11N_H

#include <string>

#include "SymbolImpS11n.h"
#include "SymbolLine.h"

//=========================================================
//! 
//! 
class SymbolLineImpS11n : public mpvs11n::SymbolImpS11n
{
public:

	SymbolLineImpS11n( mpv::SymbolLine *symbol );

	virtual void update( double timeElapsed );

	void lineStyleChanged( mpv::SymbolLine *symbol );

	void verticesChanged( mpv::SymbolLine *symbol );

protected:

	virtual ~SymbolLineImpS11n();
	
	mpv::RefPtr<mpvs11n::StringNode> primitiveType;
	mpv::RefPtr<mpvs11n::FloatNode> lineWidth;
	mpv::RefPtr<mpvs11n::IntNode> stipplePattern;
	mpv::RefPtr<mpvs11n::FloatNode> stipplePatternLength;
	mpv::RefPtr<mpvs11n::GroupNode> vertices;
};

#endif
