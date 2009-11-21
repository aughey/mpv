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
 *  FILENAME:   MPVCommonTypes.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  ...
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  12/02/2004 Greg Basler                       MPV_CR_DR_1
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  1.0
 */

// MPVCommonTypes.h: Specifies types and declspec for the common library/dll.
//
//////////////////////////////////////////////////////////////////////

#if !defined(MPV_COMMON_TYPES_TYPES_H)
#define MPV_COMMON_TYPES_TYPES_H

// Due to type problems between compilers

#if defined(_MSC_VER)
    #pragma warning(disable: 4251)
#endif

#if defined(WIN32)

   #ifdef MPVCMN_DLL

      #ifdef _EXPORT_MPVCMN_
         #define MPVCMN_SPEC __declspec(dllexport)
      #else
         #define MPVCMN_SPEC __declspec(dllimport)
      #endif

   #else

      #define MPVCMN_SPEC

   #endif



#elif defined(__GNUC__)

   #define MPVCMN_SPEC

#else

   // Use these or put your own definitions here!

   #define MPVCMN_SPEC

#endif




#endif // #if !defined(MPV_COMMON_TYPES_TYPES_H)




