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

#include "BuildRectangle.h"

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Texture2D>

osg::StateSet* GenerateUpdatingStateSet(osg::StateAttribute::Callback *callback)
{
	osg::Texture2D *texture=new osg::Texture2D();
	// protect from being optimized away as static
	// (this is from osgtexture2D example, but what does it do?).
	texture->setDataVariance(osg::Object::DYNAMIC);
	osg::StateSet *stateset=new osg::StateSet();
	stateset->setTextureAttributeAndModes(0, texture,
		osg::StateAttribute::ON);
	texture->setUpdateCallback(callback);
	/*  Not required?
	Image=new osg::Image();
	Texture->setImage(Image.get());
	*/
	return stateset;
}

BuildRectangle::BuildRectangle(float width, float height)
{
	Width=width;
	Height=height;
	Left=0;
	Right=1;
	Top=0;
	Bottom=1;
}

void BuildRectangle::ScaleTexture(float left, float right,
	float top, float bottom)
{
	Left=left;
	Right=right;
	Top=top;
	Bottom=bottom;
}

void BuildRectangle::ScaleTexture(float width, float height,
	float startLine, float endLine, float startColumn, float endColumn)
{
	Top=startLine/height;
	Bottom=endLine/height;
	Left=startColumn/width;
	Right=endColumn/width;
}

osg::Node *BuildRectangle::GetRectangle()
{
	osg::Geometry *geometry=new osg::Geometry;

	// generate a width by height display
	osg::Vec3Array *vertices=new osg::Vec3Array(4);
	const float hw=Width/2, hh=Height/2;
	(*vertices)[0].set(-hw,hh,0);
	(*vertices)[1].set(hw,hh,0);
	(*vertices)[2].set(hw,-hh,0);
	(*vertices)[3].set(-hw,-hh,0);
	geometry->setVertexArray(vertices);
	geometry->addPrimitiveSet(new osg::DrawArrays(GL_TRIANGLE_FAN,0,4));

	// stretch the texture to the edges (or to the requested amount)
	osg::Vec2Array *texc=new osg::Vec2Array(4);
	(*texc)[0].set(Left,Top);
	(*texc)[1].set(Right,Top);
	(*texc)[2].set(Right,Bottom);
	(*texc)[3].set(Left,Bottom);
	geometry->setTexCoordArray(0,texc);

	// one normal pointing up
	osg::Vec3Array *normal=new osg::Vec3Array(1);
	(*normal)[0].set(0,0,1);
	geometry->setNormalArray(normal);
	geometry->setNormalBinding(osg::Geometry::BIND_OVERALL);

	// Solid white.
	osg::Vec4Array *color=new osg::Vec4Array(1);
	(*color)[0].set(1,1,1,1);
	geometry->setColorArray(color);
	geometry->setColorBinding(osg::Geometry::BIND_OVERALL);

	osg::Geode *geode=new osg::Geode;

	// Add the geometry drawable
	geode->addDrawable(geometry);
	return geode;
}
