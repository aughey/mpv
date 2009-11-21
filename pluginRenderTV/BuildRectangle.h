/* copyright (c) 2006 all rights reserved by
 * David Fries <dfries@mail.win.org>
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
 */
#ifndef _BUILD_RECTANGLE_H
#define _BUILD_RECTANGLE_H 1

#include <osg/StateAttribute>

namespace osg
{
	class Node;
}

// Usage: Create a callback object that updates an osg::Texture2D's image,
// then pass it to this function.
osg::StateSet* GenerateUpdatingStateSet(osg::StateAttribute::Callback
	*callback);

class BuildRectangle
{
public:
	/* Specify the width and height of the resulting rectangle.
	 * Zero will be in the center of the rectangle.
	 */
	BuildRectangle(float width=4, float height=3);
	/* This allows the texture to be expanded to show less than
	 * the full fexture.  By default the texture is scaled to fill
	 * up the entire rectangle.  Arguments are expected in the range
	 * 0.0 to 1.0.
	 */
	void ScaleTexture(float left, float right, float top, float bottom);
	/* Calculates the texture coordinates to stretch a texture of
	 * resolution width by height to display the given columns and
	 * lines.
	 */
	void ScaleTexture(float width, float height,
		float startLine, float endLine,
		float startColumn, float endColumn);
	/* Build a rectangle from the current set of parameters.
	 */
	osg::Node *GetRectangle();
private:
	// dimensions in GL Units
	float Width, Height;
	// Texture coordinates
	float Left, Right, Top, Bottom;
};

#endif // _BUILD_RECTANGLE_H 
