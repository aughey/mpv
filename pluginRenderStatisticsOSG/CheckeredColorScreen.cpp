/** <pre>
 *  Plugin for the MPV to add statistics displays, such as frame rate
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
 *  2008-03-15 Andrew Sampson
 *      Initial release
 *
 * </pre>
 */

#include "CheckeredColorScreen.h"

CheckeredColorScreen::CheckeredColorScreen( 
	const osg::Vec4 &colorA, const osg::Vec4 &colorB, 
	int numGridCellsWide, int numGridCellsTall ) : 
	GridColorScreen()
{
	std::vector< osg::Vec4 > checkerColors;
	checkerColors.push_back( colorA );
	checkerColors.push_back( colorB );
	
	int numCheckerColors = checkerColors.size();
	int numGridCells = numGridCellsWide * numGridCellsTall;
	bool rowWidthIsEvenMultiple = ((numGridCellsWide % numCheckerColors) == 0);
	
	// If numGridCellsWide is an even multiple of the number of colors (the 
	// typical checkerboard pattern uses 2 colors), then additional steps must 
	// be taken to prevent the first cell in every row from having the same 
	// color.  If this is not accounted for, then we'll end up with stripes 
	// instead of checkers.
	
	setNumGridCellsWide( numGridCellsWide );

	int currentRow = 0;
	for( int i = 0; i < numGridCells; i++ )
	{
		if( i && (i % numGridCellsWide) == 0 )
			currentRow++;
		
		int colorIndex = i;
		if( rowWidthIsEvenMultiple )
		{
			// force rows to alternate for their initial color
			colorIndex += currentRow % numCheckerColors;
		}
		
		addColor( checkerColors[colorIndex % numCheckerColors] );
	}
	
}


CheckeredColorScreen::~CheckeredColorScreen()
{

}

