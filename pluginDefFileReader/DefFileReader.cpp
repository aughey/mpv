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
 *  FILENAME:   PluginDefFileReader.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *   read the definition files and organize the definition data.
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

// directory entry related stuff
#ifdef WIN32
#include <windows.h>
#include <direct.h>
#define chdir _chdir
#else
#include <dirent.h>
#include <unistd.h>
#endif

#include <iostream>

#include "DefFileReader.h"

#include "MPVExceptions.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

EXPORT_DYNAMIC_CLASS( PluginDefFileReader )

// ================================================
// PluginDefFileReader
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginDefFileReader::PluginDefFileReader() : Plugin()
{
	name_ = "PluginDefFileReader";
	licenseInfo_.setLicense( LicenseInfo::LicenseGPL );
	licenseInfo_.setOrigin( "Boeing" );

	root = NULL;
	defFileDirectory = NULL;
}


// ================================================
// PluginDefFileReader
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginDefFileReader::~PluginDefFileReader() throw()
{
   if(root != NULL)
      delete root;
}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginDefFileReader::act( SystemState::ID state, StateContext &stateContext )
{
	switch( state )
	{

	case SystemState::BlackboardPost:
		// this stage is for posting things to the blackboard
		bb_->put( "DefinitionData", &root );
		break;

	case SystemState::BlackboardRetrieve:
		// this stage is for retrieving things from the blackboard
		bb_->get( "DefFileDirectory", defFileDirectory );
		break;

	case SystemState::ConfigurationLoad:
		// this stage is specifically for (re)loading configuration files
		{

		if(root != NULL)
		{
			delete root;
			root = NULL;
		}

		// Initialize the DefFileGroup
		root = new DefFileGroup;

		std::list< std::string > defFileList = getDefFileList( *defFileDirectory );
		if( defFileList.empty() )
		{
			printf( "Error - the def file reader plugin was unable to find any def files.\n" );
			printf( "If your def files are missing, then you will need to find some or create new \n" );
			printf( "ones.  If the def files are not missing, then you probably forgot to set the \n" );
			printf( "DEF_FILE_PATH environmental variable.  Just set the variable to the name of the \n" );
			printf( "directory where you keep your MPV configuration files.\n" );
			exit( 1 );
		}
		else
		{
			std::cout << "Loading the following def files:\n";
			std::list< std::string >::iterator iter;
			for( iter = defFileList.begin(); iter != defFileList.end(); iter++ )
			{
				try {
					std::cout << "\t" << *iter << std::endl;
					loadDefFile( *defFileDirectory + *iter );
				}
				catch( std::exception &e )
				{
					std::cerr << "Exception - " << e.what() << std::endl;
					std::cerr << "Error - the def file reader plugin was unable to process one of your def files.\n"
						<< "If the def file is missing, then you will need to find a copy or create a new \n"
						<< "one.  If the def file is not missing, then you probably forgot to set the \n"
						<< "DEF_FILE_PATH environmental variable.  Just set the variable to the name of the \n"
						<< "directory where you keep your MPV configuration files.\n";
					exit( 1 );
				}
			}
			printf( "\n" );
		}
		}
		break;
	
	default:
		break;
	}
	
}

// ================================================
// loadDefFile
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginDefFileReader::loadDefFile( const std::string &filename )
{
	DefFileParser parser;
	DefFileGroup *fileGroup = parser.parse( filename.c_str() );
	
	if( fileGroup == NULL )
	{
		throw MPVFileIOReadException( std::string( "Couldn't parse \"" ) + filename + "\"" );
		return;
	}
	
	// move child groups from fileGroup to root
	while( !fileGroup->children.empty() )
	{
		DefFileGroup *child = *fileGroup->children.begin();
		fileGroup->removeChild( child );
		root->addChild( child );
	}
	
	// move attributes from fileGroup to root
	while( !fileGroup->attributes.empty() )
	{
		DefFileAttrib *attr = *fileGroup->attributes.begin();
		fileGroup->removeAttribute( attr );
		root->addAttribute( attr );
	}
		
	delete fileGroup;
}


// ================================================
// getDefFileList
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
std::list< std::string > 
PluginDefFileReader::getDefFileList( const std::string &defFilePathString )
{
	std::list< std::string > result;
	
#ifdef WIN32
	HANDLE hDirHandle;
	WIN32_FIND_DATA sDirEntry;
	bool finished = false;
	
	hDirHandle = FindFirstFile((defFilePathString + "/*").c_str(), &sDirEntry);

	if( hDirHandle == INVALID_HANDLE_VALUE )
	{
		fprintf( stderr, "Error: Failed to open directory \"%s\"\n", defFilePathString.c_str() );
		finished = true;
	}

	while( !finished )
	{
      std::string strFile(sDirEntry.cFileName);

      if( (strFile.find(".def", strFile.size() - 4) > 0) && (strFile.find(".def", strFile.size() - 4) < strFile.size()) ||
          (strFile.find(".DEF", strFile.size() - 4) > 0) && (strFile.find(".DEF", strFile.size() - 4) < strFile.size()) )
			result.push_back( strFile );

		if( !FindNextFile(hDirHandle, &sDirEntry) )
			finished = true;
	}
#else
	
	struct dirent **dirEntryList;
	int dirEntryCount;
	
	dirEntryCount = scandir( defFilePathString.c_str(), &dirEntryList, 0, 0 );
	if( dirEntryCount < 0 )
	{
		fprintf( stderr, "Error: Failed to open directory \"%s\"\n", defFilePathString.c_str() );
	}
	else
	{
		for( int i = 0; i < dirEntryCount; i++ )
		{
			// check to see if the last 3 chars are 'd' 'e' 'f'
			int len = strlen( dirEntryList[i]->d_name );
			if( len > 4 && strncmp( dirEntryList[i]->d_name + len - 3, "def", 3 )  == 0 )
			{
				result.push_back( std::string( dirEntryList[i]->d_name ) );
			}
			free( dirEntryList[i] );
		}
		free( dirEntryList );
	}
	
#endif

	return result;
}
