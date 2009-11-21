/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2004 The Boeing Company
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
 *  FILENAME:   PluginDefFileReader.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  Reads and parses the definition files and places the needed data
 *    onto the blackboard
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  03/29/2004 Greg Basler
 *      Initial Release.
 *  2007-07-01 Andrew Sampson
 *      Ported to new plugin API
 * </pre>
 *  The Boeing Company
 *  1.0
 */

#if !defined(_DEFINITION_FILE_READER_INCLUDED_)
#define _DEFINITION_FILE_READER_INCLUDED_


#include "Plugin.h"

#include <string>
#include <map>
#include <list>

#include "DefFileParser.h"



//=========================================================
//! The class encapsulating the Definition File Reader plugin.
//!
class PluginDefFileReader : public Plugin
{
public:


   //==> Management

   //=========================================================
   //! General Constructor
   //!
	PluginDefFileReader();

   //=========================================================
   //! General Destructor
   //!
	virtual ~PluginDefFileReader() throw();


   //==> Basic plugin functionality

	//=========================================================
	//! The per-frame processing that this plugin performs
	//! \param state - The current system state
	//! \param stateContext - an object containing all the variables which 
	//!     influence state transitions
	//!
	virtual void act( SystemState::ID state, StateContext &stateContext );



protected:


   //=========================================================
   //! A pointer to the root Definition File Group
   //!
   DefFileGroup *root;

	//=========================================================
	//! The directory to search for .def files.  Retrieved from blackboard.
	//!
	std::string *defFileDirectory;

   //=========================================================
   //! load the definition file
   //! \param filename - The name of the definition file to load.
   //!
   void loadDefFile( const std::string &filename );

	//=========================================================
	//! builds a list of files in the directory defFilePathString 
	//! with a name ending in "def"
	//! \param defFilePathString - the directory to look in for def files
	//! \returns a list of file names
	//! 
	std::list< std::string > 
		getDefFileList( const std::string &defFilePathString );

};

#endif
