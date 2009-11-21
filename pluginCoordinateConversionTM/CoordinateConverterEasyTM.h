/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2008 Andrew Sampson
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
 *  2008-08-03 Andrew Sampson
 *      Initial release
 *  
 *  
 *  </pre>
 */


#ifndef _COORDINATE_CONVERTER_EASYTM_H_
#define _COORDINATE_CONVERTER_EASYTM_H_

#include "CoordinateConverter.h"

#include "CoordinateConverterTM.h"

//=========================================================
//! An easier-to-use wrapper for TM coordinate conversion.
//! 
class CoordinateConverterEasyTM : public mpv::CoordinateConverter
{
public:
	//=========================================================
	//! General Constructor
	//! 
	CoordinateConverterEasyTM();
	
	virtual void performConversion( const CoordinateSet &in, CoordinateSet &out );
	
	virtual void performReverseConversion( const CoordinateSet &in, CoordinateSet &out );

	void coordSysParamsChangedCB( mpv::CoordinateConverter* );
	
protected:
	//=========================================================
	//! General Destructor
	//! 
	virtual ~CoordinateConverterEasyTM();
	
	mpv::RefPtr<CoordinateConverterTM> converter;
};

#endif
