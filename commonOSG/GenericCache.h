/** <pre>
 *  Copyright (c) 2008 Generic Dynamics Land Systems
 *  
 *  This component is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser Generic Public
 *  License as published by the Free Software Foundation; specifically 
 *  version 2.1 of the License.
 *  
 *  This software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser Generic Public License for more details.
 *  
 *  You should have received a copy of the GNU Lesser Generic Public
 *  License along with this software; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *  
 *  
 *  2008-08-27 Philip Lowman, GDLS
 *      Initial release
 *
 * </pre>
 */

#ifndef _MPV_COMMON_OSG_GENERIC_CACHE
#define _MPV_COMMON_OSG_GENERIC_CACHE

#include <map>
#include <iterator>

#include <osg/Referenced>

//=========================================================
//! A template class for caching OSG objects by a key.
//! 
template<typename Key, typename Value>
class GenericCache : osg::Referenced
{
	public:
		typedef std::map<Key, osg::ref_ptr<Value> > CacheType;

		//=========================================================
		//! Add to the cache
		//!
		void add(Key k, Value* v)
		{
			cache[k] = v;
		}

		//=========================================================
		//! Obtain a font from the cache by ID
		//!@return The desired osgText::Font or 0 if not found
		//!
		Value* get(Key k) const
		{
			typename CacheType::const_iterator itr = cache.find(k);

			if(itr != cache.end())
			{
				return itr->second.get();
			}
			else
			{
				return 0;
			}
		}

		//=========================================================
		//! Obtain the singleton for this cache.
		//!
		static GenericCache* instance()
		{
			static GenericCache<Key, Value> c;
			return &c;
		}

	private:
		CacheType cache;
};

#endif
