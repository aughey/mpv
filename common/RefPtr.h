/** <pre>
 *  The MPV Common Library
 *  
 *  Copyright (c) 2004 Abdiel Janulgue (xlptr.h)
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
 *      Initial release.  Code based on forked version of XLPtr
 *  
 *  
 *  </pre>
 */


#ifndef _REFPTR_H_
#define _REFPTR_H_

namespace mpv
{

//=========================================================
//! Reference-counting-enabled pointer.  Works in conjunction with Referenced.
//! 
template <class T>
class RefPtr
{
public:

	RefPtr() : ptr( NULL ) {}

	RefPtr( T* p )
	{
		ptr = p;
		if( ptr )
			ptr->ref();
	}

	RefPtr( const RefPtr<T> & p )
	{
		ptr = p.ptr;
		if( ptr )
			ptr->ref();
	}

	~RefPtr()
	{
		if( ptr )
			ptr->unref();
		ptr = NULL;
	}

	const RefPtr<T>& operator = ( const RefPtr<T>& p )
	{
		if( ptr == p.ptr )
			return *this;

		// The OSG folks catch an edge case here that the 
		// original XL author missed.  The new ptr should be 
		// ref()'d before the old ptr is unref()'d, to catch 
		// situations where the old ptr object has a reference 
		// to the new ptr.  If this ordering were not observed, 
		// the new ptr would be deleted before this object 
		// claimed a reference to it.
		T* tmp_ptr = ptr;

		ptr = p.ptr;

		if( ptr )
			ptr->ref();
		if( tmp_ptr )
			tmp_ptr->unref();
		return *this;
	}

	const RefPtr<T>& operator = ( T *p )
	{
		if( ptr == p )
			return *this;

		// The OSG folks catch an edge case here that the 
		// original XL author missed.  The new ptr should be 
		// ref()'d before the old ptr is unref()'d, to catch 
		// situations where the old ptr object has a reference 
		// to the new ptr.  If this ordering were not observed, 
		// the new ptr would be deleted before this object 
		// claimed a reference to it.
		T* tmp_ptr = ptr;

		ptr = p;

		if( ptr )
			ptr->ref();
		if( tmp_ptr )
			tmp_ptr->unref();
		return *this;
	}

	bool operator < (const RefPtr<T>& p) const { return (p.ptr < ptr); }
	bool operator == (const RefPtr<T>& p) const { return (p.ptr == ptr); }

	T*        get() const { return ptr; }
	T* operator->() const { return ptr; }
	T& operator *() const { return *ptr; }

	bool valid() const { return (ptr != NULL); }

private:
	
	T *ptr;

};

}

#endif
