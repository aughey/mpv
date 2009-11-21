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
 *  FILENAME:   GenerateID.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *   manage General IDs.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  03/29/2004 Greg Basler                       MPV_CR_DR_1
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  1.0
 */

#if !defined(_GENERATE_ID_INCLUDED_)
#define _GENERATE_ID_INCLUDED_

#include <map>
#include <string>

#include "MPVCommonTypes.h"

#if defined(_MSC_VER)
   #pragma warning(push)
   #pragma warning(disable : 4251)
#endif

//=========================================================
//! The class to manage IDs.
//!
class MPVCMN_SPEC GenerateID  
{
public:


   //==> Management

   //=========================================================
   //! General Constructor
   //!
   GenerateID() { };

   //=========================================================
   //! General Destructor
   //!
	virtual ~GenerateID() { IDGroup.clear(); }



   //==> Accessing Timing Functions


   //=========================================================
   //! Generate a new ID<br>
   //! \param Type - The type of ID to create
   //!
   virtual int GetNextID(std::string Type)
   {
      std::map<std::string, int>::iterator iter;
      iter = IDGroup.find( Type );

      int id = 1;

      if(iter == IDGroup.end())
         id = 1;
      else
         id = IDGroup[Type] + 1;

      IDGroup[Type] = id;

      return(id);

   }



protected:


   //==> Member variables

   //=========================================================
   //! IDGroup<br>
   //! A map of ID types and the last ID to be used.
   //!
   std::map<std::string,int> IDGroup;



};

#if defined(_MSC_VER)
   #pragma warning(pop)
#endif

#endif // !defined(_GENERATE_ID_INCLUDED_)
