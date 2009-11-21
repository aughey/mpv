/** <pre>
 *  The MPV Common Library
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
 *  2008-07-15 Andrew Sampson
 *      Initial release
 *
 * </pre>
 */


#ifndef _BIND_SLOT_H_
#define _BIND_SLOT_H_

#if defined(_MSC_VER)
    #pragma warning(disable: 4251)
#endif
#include <boost/bind.hpp>

#define BIND_SLOT0( method, obj ) boost::bind( &method, obj )
#define BIND_SLOT1( method, obj ) boost::bind( &method, obj, _1 )
#define BIND_SLOT2( method, obj ) boost::bind( &method, obj, _1, _2 )
#define BIND_SLOT3( method, obj ) boost::bind( &method, obj, _1, _2, _3 )
#define BIND_SLOT4( method, obj ) boost::bind( &method, obj, _1, _2, _3, _4 )
#define BIND_SLOT5( method, obj ) boost::bind( &method, obj, _1, _2, _3, _4, _5 )

#endif
