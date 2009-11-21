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
 *  03/29/2004 Andrew Sampson
 *      Initial Release.
 *  
 *  2009-11-10 Andrew Sampson
 *      Removed original MPVException.  Reworked exception classes.
 *  
 * </pre>
 */

#ifndef _MPV_EXCEPTIONS_INCLUDED_
#define _MPV_EXCEPTIONS_INCLUDED_
    
    
#include <exception>
#include <string.h>
#include <stdio.h>
#include <string>

#include "MPVCommonTypes.h"


//=========================================================
//! The base exception class for some of the exceptions in the MPV
//!
class MPVCMN_SPEC MPVException : public std::exception
{
   public:
      MPVException(std::string const &reason) : reason(reason)
      {
      }
   
      virtual ~MPVException(void) throw() { };

      //=========================================================
      //! Explains why the exception occured.
      //!
      virtual const char *what() const throw()
      {
         return reason.c_str();
      }

   protected:
      std::string reason;
};


//=========================================================
//! This is the exception thrown when a file io error occurs (a read error, 
//! specifically).
//!
class MPVCMN_SPEC MPVFileIOReadException : public MPVException
{
public:

	MPVFileIOReadException( std::string const &reason ) : 
		MPVException( std::string( "Exception while reading from a file: " ) + reason )
	{
	}
};


//=========================================================
//! This is the exception thrown when a Plugin cannot be
//! initialized for some reason.
//!
class MPVCMN_SPEC MPVPluginInitException : public MPVException
{
public:
	MPVPluginInitException( std::string const &reason ) : 
		MPVException( std::string( "Exception while initializing a plugin: " ) + reason )
	{
	}
};


//=========================================================
//! This is the exception thrown when something goes wrong when accessing 
//! the blackboard.  For example, it might be thrown when a blackboard entry 
//! cannot be found, or if a plugin attempts to post or retrieve from the 
//! blackboard when doing so is not permitted.
//!
class MPVCMN_SPEC MPVBlackboardException : public MPVException
{
public:
	MPVBlackboardException( const std::string &reason ) : 
		MPVException( std::string( "Exception related to the blackboard: " ) + reason )
	{
	}
};

#endif
