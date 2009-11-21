//////////////////////////////////////////////////////////////////////////
//
//  $Workfile$
//
//////////////////////////////////////////////////////////////////////////

/** <pre>
 * The MPV Symbology Plugin Collection
 * Copyright (c) 2007 General Dynamics Land Systems
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <CigiHostSession.h>
#include <CigiIGCtrlV3_2.h>
#include <CigiSOFV3_2.h>
#include <CigiIO.h>
#include <CigiExceptions.h>
#include <CigiSymbolSurfaceDefV3_3.h>
#include <CigiSymbolCtrlV3_3.h>
#include <CigiSymbolTextDefV3_3.h>
#include <CigiSymbolCircleDefV3_3.h>
#include <CigiSymbolLineDefV3_3.h>


#include <iostream>
#include <cassert>
#include <cmath>
#include <cstring>

#include "Network.h"

#if WIN32
#define strcasecmp _stricmp
#define uint unsigned int
#endif


#define SURFACE_ID 0

/*
surface coordinates for surface 0 are as follows:

       *------------* 1,1
       |            |
       |            |
       |            |
       |            |
       |            |
-1,-1  *------------*

*/

using namespace std;

static int unique_symbology_id()
{
	static int usi = 0;
	return ++usi;
}

int secs_to_frames( double secs )
{
	return int( secs * 60 );
}

///
/// An abstract base class for iterating through all of the various
/// symbology tests.
///
class SymbologyTest
{
public:
	SymbologyTest( CigiOutgoingMsg &outgoing ) : outgoing( outgoing )
	{
		descriptionSymbolID = -1;
	}
	
	virtual ~SymbologyTest() {}

	virtual string name() const = 0;

	///
	/// Draw the desired scene.
	///@return number of frames to draw scene for before moving on to next scene.
	///
	virtual int draw( uint scene ) = 0;

	///
	/// Get number of scenes in the test.
	///
	virtual uint num_scenes() const = 0;
	
	void setupDescription( const string &description )
	{
		// if description has been set up already, destroy it before proceeding
		if( descriptionSymbolID != -1 )
			destroyDescription();
		
		descriptionSymbolID = unique_symbology_id();

		CigiSymbolTextDefV3_3 textDef;

		textDef.SetSymbolID( descriptionSymbolID );
		textDef.SetFontSize( 0.1 );
		textDef.SetAlignment( CigiBaseSymbolTextDef::BottomCenter );
		textDef.SetOrientation( CigiBaseSymbolTextDef::LeftToRight );
		textDef.SetText( description );
		outgoing << textDef;
		
		CigiSymbolCtrlV3_3 symbolCtrl;

		symbolCtrl.SetSymbolID( descriptionSymbolID );
		symbolCtrl.SetSymbolState( CigiBaseSymbolCtrl::Visible );
		symbolCtrl.SetSurfaceID( SURFACE_ID );
		symbolCtrl.SetFlashCtrl( CigiBaseSymbolCtrl::Continue );
		symbolCtrl.SetFlashDutyCycle( 100 );
		symbolCtrl.SetFlashPeriod( 1.0 );
		symbolCtrl.SetInheritColor( CigiBaseSymbolCtrl::NotInherit );
		symbolCtrl.SetColor( 0xff, 0xff, 0xff, 0xff );
		symbolCtrl.SetScaleU( 1.0 );
		symbolCtrl.SetScaleV( 1.0 );
		symbolCtrl.SetUPosition( 0.0 );
		symbolCtrl.SetVPosition( -0.9 );
		outgoing << symbolCtrl;
	}
	
	void destroyDescription()
	{
		CigiSymbolCtrlV3_3 symbolCtrl;

		symbolCtrl.SetSymbolID( descriptionSymbolID );
		symbolCtrl.SetSymbolState( CigiBaseSymbolCtrl::Destroyed );
		outgoing << symbolCtrl;

		descriptionSymbolID = -1;
	}

protected:
	CigiOutgoingMsg &outgoing;
	int descriptionSymbolID;
};

///
/// Creates a few text symbols
///
class HelloWorldTest : public SymbologyTest
{
public:
	HelloWorldTest( CigiOutgoingMsg &outgoing )
			: SymbologyTest( outgoing ), initialized( false )
	{
	}

	virtual ~HelloWorldTest()
	{
	}

	virtual string name() const
	{
		return "HelloWorldTest";
	}

	virtual uint num_scenes() const
	{
		return 2;
	}

	void initialize()
	{
		CigiSymbolTextDefV3_3 textDef;

		helloWorldID = unique_symbology_id();
		swCornerID = unique_symbology_id();
		seCornerID = unique_symbology_id();
		neCornerID = unique_symbology_id();
		nwCornerID = unique_symbology_id();

		textDef.SetSymbolID( helloWorldID );
		textDef.SetFontSize( 0.1 );
		textDef.SetAlignment( CigiBaseSymbolTextDef::Center );
		textDef.SetOrientation( CigiBaseSymbolTextDef::LeftToRight );
		textDef.SetText( "Hello World" );
		outgoing << textDef;

		textDef.SetSymbolID( swCornerID );
		textDef.SetText( "-1,-1" );
		outgoing << textDef;

		textDef.SetSymbolID( seCornerID );
		textDef.SetText( " 1,-1" );
		outgoing << textDef;

		textDef.SetSymbolID( neCornerID );
		textDef.SetText( " 1, 1" );
		outgoing << textDef;

		textDef.SetSymbolID( nwCornerID );
		textDef.SetText( "-1, 1" );
		outgoing << textDef;
		
		initialized = true;
	}
	
	virtual int draw( uint scene )
	{
		double num_secs_to_display = 2.0;
		
		if( !initialized )
			initialize();
		
		CigiSymbolCtrlV3_3 symbolCtrl;

		if( scene == 0 )
		{
			symbolCtrl.SetSymbolID( helloWorldID );
			symbolCtrl.SetSymbolState( CigiBaseSymbolCtrl::Visible );
			symbolCtrl.SetSurfaceID( SURFACE_ID );
			symbolCtrl.SetFlashCtrl( CigiBaseSymbolCtrl::Continue );
			symbolCtrl.SetFlashDutyCycle( 100 );
			symbolCtrl.SetFlashPeriod( 1.0 );
			symbolCtrl.SetInheritColor( CigiBaseSymbolCtrl::NotInherit );
			symbolCtrl.SetColor( 0xff, 0xff, 0xff, 0xff );
			symbolCtrl.SetScaleU( 1.0 );
			symbolCtrl.SetScaleV( 1.0 );
			symbolCtrl.SetUPosition( 0.5 );
			symbolCtrl.SetVPosition( 0.5 );
			outgoing << symbolCtrl;

			symbolCtrl.SetSymbolID( swCornerID );
			symbolCtrl.SetUPosition( -1. );
			symbolCtrl.SetVPosition( -1. );
			outgoing << symbolCtrl;

			symbolCtrl.SetSymbolID( seCornerID );
			symbolCtrl.SetUPosition(  1. );
			symbolCtrl.SetVPosition( -1. );
			outgoing << symbolCtrl;

			symbolCtrl.SetSymbolID( neCornerID );
			symbolCtrl.SetUPosition(  1. );
			symbolCtrl.SetVPosition(  1. );
			outgoing << symbolCtrl;

			symbolCtrl.SetSymbolID( nwCornerID );
			symbolCtrl.SetUPosition( -1. );
			symbolCtrl.SetVPosition(  1. );
			outgoing << symbolCtrl;

			setupDescription( "There should be a text symbol in each of \nthe 4 corners, displaying the corner's coordinate" );
		}
		else
		{
			// don't bother to initialize any of the other fields...
			symbolCtrl.SetSymbolState( CigiBaseSymbolCtrl::Destroyed );

			symbolCtrl.SetSymbolID( helloWorldID );
			outgoing << symbolCtrl;

			symbolCtrl.SetSymbolID( swCornerID );
			outgoing << symbolCtrl;

			symbolCtrl.SetSymbolID( seCornerID );
			outgoing << symbolCtrl;

			symbolCtrl.SetSymbolID( neCornerID );
			outgoing << symbolCtrl;

			symbolCtrl.SetSymbolID( nwCornerID );
			outgoing << symbolCtrl;

			destroyDescription();
		}


		return secs_to_frames( num_secs_to_display );
	}

private:
	int helloWorldID;
	int swCornerID;
	int seCornerID;
	int neCornerID;
	int nwCornerID;
	bool initialized;
};


///
/// Tests a filled rectangle...
///
class FilledRectTest : public SymbologyTest
{
public:
	FilledRectTest( CigiOutgoingMsg &outgoing )
			: SymbologyTest( outgoing ), initialized( false )
	{
	}

	virtual ~FilledRectTest()
	{
	}

	virtual string name() const
	{
		return "FilledRectTest";
	}

	virtual uint num_scenes() const
	{
		return 4;
	}

	void initialize()
	{
		filled_rectangle.SetSymbolID( unique_symbology_id() );
		filled_rectangle.SetPrimitive( CigiBaseSymbolLineDef::TriangleFan );
		CigiBaseVertexSymbolData *vertex = NULL;
		vertex = filled_rectangle.AddVertex();
		vertex->SetVertexU( 0.0 );
		vertex->SetVertexV( 0.0 );
		vertex = filled_rectangle.AddVertex();
		vertex->SetVertexU( 0.156 );
		vertex->SetVertexV( 0.0 );
		vertex = filled_rectangle.AddVertex();
		vertex->SetVertexU( 0.156 );
		vertex->SetVertexV( 0.390 );
		vertex = filled_rectangle.AddVertex();
		vertex->SetVertexU( 0.0 );
		vertex->SetVertexV( 0.390 );
		outgoing << filled_rectangle;
		
		initialized = true;
	}
	
	virtual int draw( uint scene )
	{
		double num_secs_to_display = 2.0;
		
		if( !initialized )
			initialize();
		
		CigiSymbolCtrlV3_3 symbolCtrl;

		symbolCtrl.SetSymbolID( filled_rectangle.GetSymbolID() );
		symbolCtrl.SetSurfaceID( SURFACE_ID );
		symbolCtrl.SetFlashCtrl( CigiBaseSymbolCtrl::Continue );
		symbolCtrl.SetFlashDutyCycle( 100 );
		symbolCtrl.SetFlashPeriod( 1.0 );
		symbolCtrl.SetInheritColor( CigiBaseSymbolCtrl::NotInherit );
		symbolCtrl.SetUPosition( -0.375 );
		symbolCtrl.SetVPosition( 0.0742 );
		symbolCtrl.SetRotation( 0.0 );
		symbolCtrl.SetScaleU( 1.0 );
		symbolCtrl.SetScaleV( 1.0 );

		if( scene == 0 )
		{
			symbolCtrl.SetSymbolState( CigiBaseSymbolCtrl::Visible );
			symbolCtrl.SetColor( 0xff, 0xff, 0xff, 0xff );

			setupDescription( "This is a filled white rectangle" );
		}
		else if( scene == 1 )
		{
			symbolCtrl.SetSymbolState( CigiBaseSymbolCtrl::Visible );
			symbolCtrl.SetColor( 0xff, 0x00, 0x00, 0xff );

			setupDescription( "It's now red" );
		}
		else if( scene == 2 )
		{
			symbolCtrl.SetSymbolState( CigiBaseSymbolCtrl::Visible );
			symbolCtrl.SetColor( 0xff, 0x00, 0x00, 0xff );
			symbolCtrl.SetRotation( 90.0 );

			setupDescription( "It's now rotated 90 degrees clockwise \nabout its origin (lower left hand corner)" );
			num_secs_to_display = 3.0;
		}
		else
		{
			symbolCtrl.SetSymbolState( CigiBaseSymbolCtrl::Destroyed );
			destroyDescription();
		}

		outgoing << symbolCtrl;

		return secs_to_frames( num_secs_to_display );
	}

private:
	CigiSymbolLineDefV3_3 filled_rectangle;
	bool initialized;
};



///
/// Tests an unfilled rectangle...
///
class UnfilledRectTest : public SymbologyTest
{
public:
	UnfilledRectTest( CigiOutgoingMsg &outgoing )
			: SymbologyTest( outgoing ), initialized( false )
	{
	}

	virtual ~UnfilledRectTest()
	{
	}

	virtual string name() const
	{
		return "UnfilledRectTest";
	}

	void initialize()
	{
		rectangle.SetSymbolID( unique_symbology_id() );
		rectangle.SetPrimitive( CigiBaseSymbolLineDef::LineLoop );
		CigiBaseVertexSymbolData *vertex = NULL;
		vertex = rectangle.AddVertex();
		vertex->SetVertexU( 0.0 );
		vertex->SetVertexV( 0.0 );
		vertex = rectangle.AddVertex();
		vertex->SetVertexU( 0.156 );
		vertex->SetVertexV( 0.0 );
		vertex = rectangle.AddVertex();
		vertex->SetVertexU( 0.156 );
		vertex->SetVertexV( 0.390 );
		vertex = rectangle.AddVertex();
		vertex->SetVertexU( 0.0 );
		vertex->SetVertexV( 0.390 );
		outgoing << rectangle;
		
		initialized = true;
	}
	
	virtual uint num_scenes() const
	{
		return 3;
	}

	virtual int draw( uint scene )
	{
		double num_secs_to_display = 2.0;
		
		if( !initialized )
			initialize();
		
		CigiSymbolCtrlV3_3 symbolCtrl;

		symbolCtrl.SetSymbolID( rectangle.GetSymbolID() );
		symbolCtrl.SetSurfaceID( SURFACE_ID );
		symbolCtrl.SetFlashCtrl( CigiBaseSymbolCtrl::Continue );
		symbolCtrl.SetFlashDutyCycle( 100 );
		symbolCtrl.SetFlashPeriod( 1.0 );
		symbolCtrl.SetInheritColor( CigiBaseSymbolCtrl::NotInherit );
		symbolCtrl.SetUPosition( -0.375 );
		symbolCtrl.SetVPosition( 0.0742 );
		symbolCtrl.SetRotation( 0.0 );
		symbolCtrl.SetScaleU( 1.0 );
		symbolCtrl.SetScaleV( 1.0 );

		if( scene == 0 )
		{
			symbolCtrl.SetSymbolState( CigiBaseSymbolCtrl::Visible );
			symbolCtrl.SetColor( 0xff, 0xff, 0xff, 0xff );

			setupDescription( "This is an unfilled white rectangle" );
		}
		else if( scene == 1 )
		{
			symbolCtrl.SetSymbolState( CigiBaseSymbolCtrl::Visible );
			symbolCtrl.SetColor( 0xff, 0x00, 0xff, 0xff );

			setupDescription( "The border is now purple" );
		}
		else
		{
			symbolCtrl.SetSymbolState( CigiBaseSymbolCtrl::Destroyed );
			destroyDescription();
		}

		outgoing << symbolCtrl;

		return secs_to_frames( num_secs_to_display );
	}

private:
	CigiSymbolLineDefV3_3 rectangle;
	bool initialized;
};

/*
///
/// Tests a textured rectangle...
///
class TexturedRectTest : public SymbologyTest
{
public:
	TexturedRectTest( CigiOutgoingMsg &outgoing )
			: SymbologyTest( outgoing )
	{
		textured_rectangle = new CIGI::RemoteRectSymbol( true );
		textured_rectangle->SetSymbolID( unique_symbology_id() );
		note.SetSymbolID( unique_symbology_id() );
	}

	virtual ~TexturedRectTest()
	{
		delete textured_rectangle;
	}

	virtual string name() const
	{
		return "TexturedRectTest";
	}

	virtual uint num_scenes() const
	{
		return 4;
	}

	virtual int draw( uint scene )
	{
		double num_secs_to_display = 2.0;

		if( scene == 0 )
		{
			description.enable( true );
			setupDescription( "If the file Images/clockface.JPG is in a directory in your OSG_FILE_PATH you should see it loaded." );

			note.setColor( 0.8, 1.0, 0.0, 1.0 );
			note.setPosition( 900, 300, 1.0 );
			note.setFont( "fonts/arial.ttf" );
			note.setCharacterSize( 20 );
			note.setMaximumWidth( 300 );
			note.setAlignment( CENTER_TOP );
			string msg2( "Note that this file comes with the OSG sample data available" );
			msg2 += " online at http://www.openscenegraph.org/downloads/data/OpenSceneGraph-Data-1.1.zip";
			note.setText( msg2 );

			textured_rectangle->setColor( 1.0, 1.0, 1.0, 1.0 );
			textured_rectangle->setPosition( 400, 550, 0.5 );
			textured_rectangle->setDimensions( 200, 200 );
			textured_rectangle->setTexture( "Images/clockface.JPG" );

			num_secs_to_display = 4.0;
		}
		else if( scene == 1 )
		{
			textured_rectangle->setColor( 1.0, 0.0, 0.0, 1.0 );
			setupDescription( "You can color texture images too." );
		}
		else if( scene == 2 )
		{
			textured_rectangle->setColor( 1.0, 0.0, 0.0, 0.2 );
			setupDescription( "You can also alpha blend them." );
		}
		else
		{
			textured_rectangle->enable( false );
			destroyDescription();
			note.enable( false );
		}

		send();
		return secs_to_frames( num_secs_to_display );
	}

	void send()
	{
		outgoing << textured_rectangle->getPacket();
		outgoing << note.getPacket();
	}

private:
	CIGI::RemoteRectSymbol *textured_rectangle;
	CIGI::RemoteTextSymbol note;
};
*/

///
/// Tests overlapping filled elipses...
///
class FilledEllipseTest : public SymbologyTest
{
public:
	FilledEllipseTest( CigiOutgoingMsg &outgoing )
			: SymbologyTest( outgoing ), initialized( false )
	{
		for( uint i = 0; i < 3; ++i )
		{
			CigiSymbolCircleDefV3_3 *def = new CigiSymbolCircleDefV3_3;
			def->SetSymbolID( unique_symbology_id() );
			circleDefs.push_back( def );
			
			CigiSymbolCtrlV3_3 *ctrl = new CigiSymbolCtrlV3_3;
			ctrl->SetSymbolID( def->GetSymbolID() );
			circleCtrls.push_back( ctrl );
		}
	}

	virtual ~FilledEllipseTest()
	{
		uint i;
		for( i = 0; i < circleDefs.size(); ++i )
		{
			delete circleDefs[ i ];
		}
		for( i = 0; i < circleCtrls.size(); ++i )
		{
			delete circleCtrls[ i ];
		}
	}

	virtual string name() const
	{
		return "FilledEllipseTest with order";
	}

	void initialize()
	{
		uint i;
		for( i = 0; i < circleDefs.size(); ++i )
		{
			CigiSymbolCircleDefV3_3 *circleDef = circleDefs[i];
			circleDef->SetDrawingStyle( CigiBaseSymbolCircleDef::Fill );
			CigiBaseCircleSymbolData *subcircle = circleDef->AddCircle();
			subcircle->SetCenterUPosition( 0.0 );
			subcircle->SetCenterVPosition( 0.0 );
			subcircle->SetRadius( 0.156 );
			subcircle->SetInnerRadius( 0.0 );
			subcircle->SetStartAngle( 0.0 );
			subcircle->SetEndAngle( 0.0 );
			
			outgoing << *circleDef;
		}

		for( i = 0; i < circleCtrls.size(); ++i )
		{
			CigiSymbolCtrlV3_3 *symbolCtrl = circleCtrls[i];
			symbolCtrl->SetSymbolID( circleDefs[i]->GetSymbolID() );
			symbolCtrl->SetSymbolState( CigiBaseSymbolCtrl::Visible );
			symbolCtrl->SetSurfaceID( SURFACE_ID );
			symbolCtrl->SetFlashCtrl( CigiBaseSymbolCtrl::Continue );
			symbolCtrl->SetFlashDutyCycle( 100 );
			symbolCtrl->SetFlashPeriod( 1.0 );
			symbolCtrl->SetInheritColor( CigiBaseSymbolCtrl::NotInherit );
			symbolCtrl->SetRotation( 0.0 );

			symbolCtrl->SetScaleU( 1.0 );
			symbolCtrl->SetScaleV( 0.5 );

			if( i == 0 )
			{
				symbolCtrl->SetColor( 0xff, 0x00, 0x00, 0xff );
				symbolCtrl->SetUPosition( -0.53125 );
				symbolCtrl->SetVPosition( -0.0234375 );
				symbolCtrl->SetLayer( 2 );
			}
			else if( i == 1 )
			{
				symbolCtrl->SetColor( 0xff, 0xff, 0xff, 0xff );
				symbolCtrl->SetUPosition( -0.4140625 );
				symbolCtrl->SetVPosition( -0.0234375 );
				symbolCtrl->SetLayer( 1 );
			}
			else if( i == 2 )
			{
				symbolCtrl->SetColor( 0x00, 0x00, 0xff, 0xff );
				symbolCtrl->SetUPosition( -0.296875 );
				symbolCtrl->SetVPosition( -0.0234375 );
				symbolCtrl->SetLayer( 0 );
			}
		}
		
		initialized = true;
	}
	
	virtual uint num_scenes() const
	{
		return 5;
	}

	virtual int draw( uint scene )
	{
		double num_secs_to_display = 2.0;
		
		if( !initialized )
			initialize();

		if( scene == 0 )
		{
			setupDescription( "Here are three ellipses." );
		}
		else if( scene == 1 )
		{
			setupDescription( "They are sorted in Z order with Red on top of White on top of Blue" );
		}
		else if( scene == 2 )
		{
			circleCtrls[0]->SetLayer( 1 );
			circleCtrls[1]->SetLayer( 2 );
			circleCtrls[2]->SetLayer( 0 );

			setupDescription( "White on top of Red and Blue" );
		}
		else if( scene == 3 )
		{
			circleCtrls[0]->SetLayer( 0 );
			circleCtrls[1]->SetLayer( 1 );
			circleCtrls[2]->SetLayer( 2 );

			setupDescription( "Blue on top of White on top of Red" );
		}
		else
		{
			for( uint i = 0; i < circleCtrls.size(); ++i )
			{
				circleCtrls[i]->SetSymbolState( CigiBaseSymbolCtrl::Destroyed );
			}
			destroyDescription();
		}

		sendCtrls();
		return secs_to_frames( num_secs_to_display );
	}

	void sendCtrls()
	{
		for( uint i = 0; i < circleCtrls.size(); ++i )
		{
			outgoing << *circleCtrls[i];
		}
	}

private:
	vector<CigiSymbolCircleDefV3_3*> circleDefs;
	vector<CigiSymbolCtrlV3_3*>      circleCtrls;
	bool initialized;
};


/**
Tests the various circle types:
- unfilled partial, 
- unfilled complete
- filled partial, no hole (pacman)
- filled complete, no hole (full circle)
- filled partial, hole (rainbow)
- filled complete, hole (donut)
*/
class ManyCircleTypesTest : public SymbologyTest
{
public:
	ManyCircleTypesTest( CigiOutgoingMsg &outgoing )
			: SymbologyTest( outgoing ), initialized( false )
	{
		for( uint i = 0; i < 6; ++i )
		{
			CigiSymbolCircleDefV3_3 *def = new CigiSymbolCircleDefV3_3;
			def->SetSymbolID( unique_symbology_id() );
			circleDefs.push_back( def );
			
			CigiSymbolCtrlV3_3 *ctrl = new CigiSymbolCtrlV3_3;
			ctrl->SetSymbolID( def->GetSymbolID() );
			circleCtrls.push_back( ctrl );
		}
	}

	virtual ~ManyCircleTypesTest()
	{
		uint i;
		for( i = 0; i < circleDefs.size(); ++i )
		{
			delete circleDefs[ i ];
		}
		for( i = 0; i < circleCtrls.size(); ++i )
		{
			delete circleCtrls[ i ];
		}
	}

	virtual string name() const
	{
		return "Many circle types";
	}

	void initialize()
	{
		uint i;
		for( i = 0; i < circleDefs.size(); ++i )
		{
			CigiSymbolCircleDefV3_3 *circleDef = circleDefs[i];
			circleDef->SetDrawingStyle( CigiBaseSymbolCircleDef::Fill );
			CigiBaseCircleSymbolData *subcircle = circleDef->AddCircle();
			subcircle->SetCenterUPosition( 0.0 );
			subcircle->SetCenterVPosition( 0.0 );
			subcircle->SetRadius( 0.156 );
			subcircle->SetInnerRadius( 0.0 );
			subcircle->SetStartAngle( 0.0 );
			subcircle->SetEndAngle( 0.0 );
			
			switch( i )
			{
			case 0:
				circleDef->SetDrawingStyle( CigiBaseSymbolCircleDef::Line );
				subcircle->SetStartAngle( 45.0 );
				subcircle->SetEndAngle( 315.0 );
				break;
			case 1:
				circleDef->SetDrawingStyle( CigiBaseSymbolCircleDef::Line );
				break;
			case 2:
				subcircle->SetStartAngle( 45.0 );
				subcircle->SetEndAngle( 315.0 );
				break;
			case 3:
				break;
			case 4:
				subcircle->SetStartAngle( 45.0 );
				subcircle->SetEndAngle( 315.0 );
				subcircle->SetInnerRadius( 0.07 );
				break;
			case 5:
				subcircle->SetInnerRadius( 0.07 );
				break;
			default:
				break;
			}
			
			outgoing << *circleDef;
		}

		for( i = 0; i < circleCtrls.size(); ++i )
		{
			CigiSymbolCtrlV3_3 *symbolCtrl = circleCtrls[i];
			symbolCtrl->SetSymbolID( circleDefs[i]->GetSymbolID() );
			symbolCtrl->SetSymbolState( CigiBaseSymbolCtrl::Visible );
			symbolCtrl->SetSurfaceID( SURFACE_ID );
			symbolCtrl->SetLayer( 0 );
			symbolCtrl->SetFlashCtrl( CigiBaseSymbolCtrl::Continue );
			symbolCtrl->SetFlashDutyCycle( 100 );
			symbolCtrl->SetFlashPeriod( 1.0 );
			symbolCtrl->SetInheritColor( CigiBaseSymbolCtrl::NotInherit );
			symbolCtrl->SetRotation( 0.0 );

			symbolCtrl->SetScaleU( 1.0 );
			symbolCtrl->SetScaleV( 1.0 );

			symbolCtrl->SetColor( 0xff, 0xff, 0xff, 0xff );
			
			switch( i )
			{
			case 0:
				symbolCtrl->SetUPosition( -0.6 );
				symbolCtrl->SetVPosition( 0.6 );
				break;
			case 1:
				symbolCtrl->SetUPosition( -0.6 );
				symbolCtrl->SetVPosition( -0.6 );
				break;
			case 2:
				symbolCtrl->SetUPosition( 0.0 );
				symbolCtrl->SetVPosition( 0.6 );
				break;
			case 3:
				symbolCtrl->SetUPosition( 0.0 );
				symbolCtrl->SetVPosition( -0.6 );
				break;
			case 4:
				symbolCtrl->SetUPosition( 0.6 );
				symbolCtrl->SetVPosition( 0.6 );
				break;
			case 5:
				symbolCtrl->SetUPosition( 0.6 );
				symbolCtrl->SetVPosition( -0.6 );
				break;
			default:
				break;
			}
		}
		
		initialized = true;
	}
	
	virtual uint num_scenes() const
	{
		return 2;
	}

	virtual int draw( uint scene )
	{
		double num_secs_to_display = 2.0;
		
		if( !initialized )
			initialize();

		if( scene == 0 )
		{
			setupDescription( "Here are various circle types." );
		}
		else
		{
			for( uint i = 0; i < circleCtrls.size(); ++i )
			{
				circleCtrls[i]->SetSymbolState( CigiBaseSymbolCtrl::Destroyed );
			}
			destroyDescription();
		}

		sendCtrls();
		return secs_to_frames( num_secs_to_display );
	}

	void sendCtrls()
	{
		for( uint i = 0; i < circleCtrls.size(); ++i )
		{
			outgoing << *circleCtrls[i];
		}
	}

private:
	vector<CigiSymbolCircleDefV3_3*> circleDefs;
	vector<CigiSymbolCtrlV3_3*>      circleCtrls;
	bool initialized;
};


///
/// Tests a Line
///
class LineTest : public SymbologyTest
{
public:
	LineTest( CigiOutgoingMsg &outgoing )
			: SymbologyTest( outgoing ), initialized( false )
	{
	}

	virtual ~LineTest()
	{
	}

	virtual string name() const
	{
		return "LineTest";
	}

	void initialize()
	{
		line.SetSymbolID( unique_symbology_id() );
		line.SetPrimitive( CigiBaseSymbolLineDef::Lines );
		CigiBaseVertexSymbolData *vertex = NULL;
		vertex = line.AddVertex();
		vertex->SetVertexU( -1.0 );
		vertex->SetVertexV( -1.0 );
		vertex = line.AddVertex();
		vertex->SetVertexU(  1.0 );
		vertex->SetVertexV(  1.0 );
		outgoing << line;
		
		initialized = true;
	}
	
	virtual uint num_scenes() const
	{
		return 2;
	}

	virtual int draw( uint scene )
	{
		double num_secs_to_display = 2.0;
		
		if( !initialized )
			initialize();
		
		CigiSymbolCtrlV3_3 symbolCtrl;

		symbolCtrl.SetSymbolID( line.GetSymbolID() );
		symbolCtrl.SetSymbolState( CigiBaseSymbolCtrl::Visible );
		symbolCtrl.SetSurfaceID( SURFACE_ID );
		symbolCtrl.SetFlashCtrl( CigiBaseSymbolCtrl::Continue );
		symbolCtrl.SetFlashDutyCycle( 100 );
		symbolCtrl.SetFlashPeriod( 1.0 );
		symbolCtrl.SetInheritColor( CigiBaseSymbolCtrl::NotInherit );
		symbolCtrl.SetUPosition( 0.0 );
		symbolCtrl.SetVPosition( 0.0 );
		symbolCtrl.SetRotation( 0.0 );
		symbolCtrl.SetScaleU( 1.0 );
		symbolCtrl.SetScaleV( 1.0 );
		symbolCtrl.SetColor( 0xff, 0xff, 0xff, 0xff );

		if( scene == 0 )
		{
			setupDescription( "A line drawn from the \nbottom left to the top right" );
		}
		else
		{
			symbolCtrl.SetSymbolState( CigiBaseSymbolCtrl::Destroyed );
			destroyDescription();
		}

		outgoing << symbolCtrl;

		return secs_to_frames( num_secs_to_display );
	}

private:
	CigiSymbolLineDefV3_3 line;
	bool initialized;
};


///
/// Tests a polygon.
///
class PolyTest : public SymbologyTest
{
public:
	PolyTest( CigiOutgoingMsg &outgoing ) :
		SymbologyTest( outgoing ), 
		filledPolyID( -1 ), 
		unfilledPolyID( -1 ), 
		initialized( false )
	{
		
	}

	virtual ~PolyTest()
	{
	}

	virtual string name() const
	{
		return "PolyTest";
	}

	virtual uint num_scenes() const
	{
		return 3;
	}

	void initialize()
	{
		CigiBaseVertexSymbolData *vertex = NULL;

		CigiSymbolLineDefV3_3 filled_poly;
		filledPolyID = unique_symbology_id();
		filled_poly.SetSymbolID( filledPolyID );
		filled_poly.SetPrimitive( CigiBaseSymbolLineDef::LineLoop );
		vertex = filled_poly.AddVertex();
		vertex->SetVertexU( 0.0 );
		vertex->SetVertexV( 0.0 );
		vertex = filled_poly.AddVertex();
		vertex->SetVertexU( 0.078125 );
		vertex->SetVertexV( 0.0 );
		vertex = filled_poly.AddVertex();
		vertex->SetVertexU( 0.078125 );
		vertex->SetVertexV( 0.146484375 );
		vertex = filled_poly.AddVertex();
		vertex->SetVertexU( 0.0390625 );
		vertex->SetVertexV( 0.29296875 );
		vertex = filled_poly.AddVertex();
		vertex->SetVertexU( 0.03125 );
		vertex->SetVertexV( 0.01953125 );
		
		CigiSymbolLineDefV3_3 unfilled_poly;
		unfilledPolyID = unique_symbology_id();
		unfilled_poly.SetSymbolID( unfilledPolyID );
		unfilled_poly.SetPrimitive( CigiBaseSymbolLineDef::LineLoop );
		vertex = unfilled_poly.AddVertex();
		vertex->SetVertexU( 0.0 );
		vertex->SetVertexV( 0.0 );
		vertex = unfilled_poly.AddVertex();
		vertex->SetVertexU( 0.078125 );
		vertex->SetVertexV( 0.0 );
		vertex = unfilled_poly.AddVertex();
		vertex->SetVertexU( 0.078125 );
		vertex->SetVertexV( 0.146484375 );
		vertex = unfilled_poly.AddVertex();
		vertex->SetVertexU( 0.0390625 );
		vertex->SetVertexV( 0.29296875 );
		vertex = unfilled_poly.AddVertex();
		vertex->SetVertexU( 0.03125 );
		vertex->SetVertexV( 0.01953125 );
		
		outgoing << filled_poly;
		outgoing << unfilled_poly;
		initialized = true;
	}

	virtual int draw( uint scene )
	{
		double num_secs_to_display = 2.0;

		if( !initialized )
			initialize();
		
		CigiSymbolCtrlV3_3 symbolCtrl;

		symbolCtrl.SetSymbolState( CigiBaseSymbolCtrl::Hidden );
		symbolCtrl.SetSurfaceID( SURFACE_ID );
		symbolCtrl.SetFlashCtrl( CigiBaseSymbolCtrl::Continue );
		symbolCtrl.SetFlashDutyCycle( 100 );
		symbolCtrl.SetFlashPeriod( 1.0 );
		symbolCtrl.SetInheritColor( CigiBaseSymbolCtrl::NotInherit );
		symbolCtrl.SetUPosition( 0.0 );
		symbolCtrl.SetVPosition( 0.0 );
		symbolCtrl.SetRotation( 0.0 );
		symbolCtrl.SetScaleU( 1.0 );
		symbolCtrl.SetScaleV( 1.0 );
		symbolCtrl.SetColor( 0xb2, 0xff, 0x00, 0xff );

		if( scene == 0 )
		{
			symbolCtrl.SetSymbolID( filledPolyID );
			symbolCtrl.SetSymbolState( CigiBaseSymbolCtrl::Hidden );
			outgoing << symbolCtrl;

			symbolCtrl.SetSymbolID( unfilledPolyID );
			symbolCtrl.SetSymbolState( CigiBaseSymbolCtrl::Visible );
			symbolCtrl.SetUPosition( -0.21875 );
			symbolCtrl.SetVPosition( -0.609375 );
			outgoing << symbolCtrl;

			setupDescription( "Arbitrary polygons can be created" );
		}
		else if( scene == 1 )
		{
			symbolCtrl.SetSymbolID( filledPolyID );
			symbolCtrl.SetSymbolState( CigiBaseSymbolCtrl::Visible );
			symbolCtrl.SetUPosition( -0.6875 );
			symbolCtrl.SetVPosition( -0.609375 );
			outgoing << symbolCtrl;

			setupDescription( "Note that polygons must be legal OpenGL polygons in order to be filled properly." );
		}
		else
		{
			symbolCtrl.SetSymbolID( filledPolyID );
			symbolCtrl.SetSymbolState( CigiBaseSymbolCtrl::Destroyed );
			outgoing << symbolCtrl;

			symbolCtrl.SetSymbolID( unfilledPolyID );
			symbolCtrl.SetSymbolState( CigiBaseSymbolCtrl::Destroyed );
			outgoing << symbolCtrl;

			destroyDescription();
		}

		return secs_to_frames( num_secs_to_display );
	}


private:
	int filledPolyID;
	int unfilledPolyID;
	bool initialized;
};

/*
///
/// Tests a PolyLine
///
class PolyLineTest : public SymbologyTest
{
public:
	PolyLineTest( CigiOutgoingMsg &outgoing )
			: SymbologyTest( outgoing )
	{
		polyline.SetSymbolID( unique_symbology_id() );
	}

	virtual ~PolyLineTest()
	{}

	virtual string name() const
	{
		return "PolyLineTest";
	}

	virtual uint num_scenes() const
	{
		return 2;
	}

	virtual int draw( uint scene )
	{
		double num_secs_to_display = 2.0;

		if( scene == 0 )
		{
			// Create a square
			polyline.addVertex( SymPoint( 0 , 0 ) );
			polyline.addVertex( SymPoint( 40, 0 ) );
			polyline.addVertex( SymPoint( 40, 40 ) );
			polyline.addVertex( SymPoint( 0, 40 ) );

			// Connect the square
			bool ok = true;
			ok &= polyline.connectVertices( 0, 1 );
			ok &= polyline.connectVertices( 1, 2 );
			ok &= polyline.connectVertices( 2, 3 );
			ok &= polyline.connectVertices( 3, 0 );
			// Make an X inside the square
			ok &= polyline.connectVertices( 0, 2 );
			ok &= polyline.connectVertices( 1, 3 );
			if( !ok )
			{
				cout << "ERROR connecting vertices in polyline" << endl;
				assert( false );
			}

			polyline.setColor( 1.0, 0.0, 0.0, 1.0 );
			polyline.setPosition( 400, 400, 1.0 );

			setupDescription( "This is a PolyLine comprised of a small square with an X in it." );
		}
		else
		{
			polyline.enable( false );
			destroyDescription();
		}

		send();
		return secs_to_frames( num_secs_to_display );
	}

	void send()
	{
		outgoing << polyline.getPacket();
	}

private:
	CIGI::RemotePolyLineSymbol polyline;
};

///
/// Tests groups
///
class GroupTest : public SymbologyTest
{
public:
	GroupTest( CigiOutgoingMsg &outgoing )
			: SymbologyTest( outgoing )
	{
		// Two filled rectangles in groups 1 and 2.
		rect1_group_a = new CIGI::RemoteRectSymbol( true );
		rect1_group_a->SetSymbolID( unique_symbology_id() );
		rect1_group_a->setGroupID( 1 );
		rect2_group_a = new CIGI::RemoteRectSymbol( true );
		rect2_group_a->SetSymbolID( unique_symbology_id() );
		rect2_group_a->setGroupID( 1 );

		rect1_group_b = new CIGI::RemoteRectSymbol( true );
		rect1_group_b->SetSymbolID( unique_symbology_id() );
		rect1_group_b->setGroupID( 2 );
		rect2_group_b = new CIGI::RemoteRectSymbol( true );
		rect2_group_b->SetSymbolID( unique_symbology_id() );
		rect2_group_b->setGroupID( 2 );

		// Setup manipulator packets to control the groups
		group_a_manipulator.group_id = 1;
		group_b_manipulator.group_id = 2;
	}

	virtual ~GroupTest()
	{
		delete rect1_group_a;
		delete rect2_group_a;
		delete rect1_group_b;
		delete rect2_group_b;
	}

	virtual string name() const
	{
		return "GroupTest";
	}

	virtual uint num_scenes() const
	{
		return 4;
	}

	virtual int draw( uint scene )
	{
		double num_secs_to_display = 2.0;

		if( scene == 0 )
		{
			//setup group a(1) to be red and on the left
			rect1_group_a->setColor( 1.0, 0.0, 0.0, 1.0 );
			rect1_group_a->setPosition( 200, 200, 0.5 );
			rect1_group_a->setPreRotationTranslation( -200, -200 );
			rect1_group_a->setPostRotationTranslation( 200, 200 );
			rect1_group_a->setDimensions( 150, 100 );

			rect2_group_a->setColor( 0.6, 0.0, 0.0, 1.0 );
			rect2_group_a->setPosition( 200, 500, 0.5 );
			rect2_group_a->setPreRotationTranslation( -200, -500 );
			rect2_group_a->setPostRotationTranslation( 200, 500 );
			rect2_group_a->setDimensions( 150, 100 );

			//setup group b(2) to be green and on the right
			rect1_group_b->setColor( 0.0, 1.0, 0.0, 1.0 );
			rect1_group_b->setPosition( 500, 200, 0.5 );
			rect1_group_b->setPreRotationTranslation( -500, -200 );
			rect1_group_b->setPostRotationTranslation( 500, 200 );
			rect1_group_b->setDimensions( 150, 100 );

			rect2_group_b->setColor( 0.0, 0.6, 0.0, 1.0 );
			rect2_group_b->setPosition( 500, 500, 0.5 );
			rect2_group_b->setPreRotationTranslation( -500, -500 );
			rect2_group_b->setPostRotationTranslation( 500, 500 );
			rect2_group_b->setDimensions( 150, 100 );

			setupDescription( "Group 1 is red and on the left.  Group 2 is in green and on the right." );

			outgoing << rect1_group_a->getPacket();
			outgoing << rect2_group_a->getPacket();
			outgoing << rect1_group_b->getPacket();
			outgoing << rect2_group_b->getPacket();
		}
		else if( scene == 1 )
		{
			// use the group manipulator packets to control both group 1 and group 2
			//
			// NOTE: we have to explicitly enable what parts of the packet we want the
			// group to obey.
			// First we rotate
			group_a_manipulator.rot_en = true;
			group_b_manipulator.rot_en = true;
			group_a_manipulator.rot = M_PI / 4.0;
			group_b_manipulator.rot = -M_PI / 8.0;

			setupDescription( "Rotated group 1 by 45 degrees and group 2 by -22.5 degrees" );

			outgoing << group_a_manipulator;
			outgoing << group_b_manipulator;

			group_a_manipulator.rot_en = false;
			group_b_manipulator.rot_en = false;
		}
		else if( scene == 2 )
		{
			group_a_manipulator.color_en = true;
			group_b_manipulator.color_en = true;

			// normalized RGBA for chartreuse
			float chartreuse[ 4 ] = {0.5, 1.0, 0.0, 1.0};
			// normalized RGBA for hot pink
			float hot_pink[ 4 ] = {1.0, .4117, .7058, 1.0}; // hot pink

			for( int i = 0; i < 4; ++i )
				group_a_manipulator.color[ i ] = chartreuse[ i ];
			for( int i = 0; i < 4; ++i )
				group_b_manipulator.color[ i ] = hot_pink[ i ];

			setupDescription( "Group 1 should now be an enticing chartreuse color.\nGroup 2 is colored hot pink." );

			outgoing << group_a_manipulator;
			outgoing << group_b_manipulator;

			group_a_manipulator.color_en = false;
			group_b_manipulator.color_en = false;
		}
		else
		{
			rect1_group_a->enable( false );
			rect2_group_a->enable( false );
			rect1_group_b->enable( false );
			rect2_group_b->enable( false );
			destroyDescription();

			outgoing << rect1_group_a->getPacket();
			outgoing << rect2_group_a->getPacket();
			outgoing << rect1_group_b->getPacket();
			outgoing << rect2_group_b->getPacket();
		}

		return secs_to_frames( num_secs_to_display );
	}

private:
	CIGI::RemoteRectSymbol *rect1_group_a;
	CIGI::RemoteRectSymbol *rect2_group_a;
	CIGI::RemoteRectSymbol *rect1_group_b;
	CIGI::RemoteRectSymbol *rect2_group_b;

	SymbologyControlPacket group_a_manipulator;
	SymbologyControlPacket group_b_manipulator;

};
*/




class SOFProcessor : public CigiBaseEventProcessor  
{
public:

	SOFProcessor()
		: igIsInOperateMode( false ) {}

	virtual ~SOFProcessor() {}

	virtual void OnPacketReceived( CigiBasePacket *Packet )
	{
		CigiSOFV3_2 *sof = (CigiSOFV3_2 *)(Packet);
		igIsInOperateMode = ( 
			sof->GetIGMode() == CigiBaseSOF::Operate || 
			sof->GetIGMode() == CigiBaseSOF::debug );
	}

	bool igIsInOperateMode;
};



//------------------------------------------------------------------------
// main.
//------------------------------------------------------------------------

int main( int argc, char* argv[] )
{
	bool interactive = true;
	if( argc > 1 )
	{
		if( strcasecmp( argv[ 1 ], "--timed" ) == 0 )
		{
			interactive = false;
		}
	}

	uint frame = 0;

	CigiHostSession *session = new CigiHostSession();
	session->SetCigiVersion( 3, 3 );
	session->SetSynchronous( false );

	CigiIGCtrlV3_2 IG;
	IG.SetFrameCntr( frame );
	IG.SetIGMode( CigiBaseIGCtrl::Operate );
	IG.SetDatabaseID( 0 ); // 0 = no database change requested

	CigiOutgoingMsg &outgoing = session->GetOutgoingMsgMgr();
	outgoing.BeginMsg();
	CigiIncomingMsg &incoming = session->GetIncomingMsgMgr();
	incoming.SetReaderCigiVersion( 3, 3 );
	incoming.UsingIteration( false );
	
	// set up a callback for SOF packets
	SOFProcessor sofProcessor;
	incoming.RegisterEventProcessor( 
		CIGI_SOF_PACKET_ID_V3_2, (CigiBaseEventProcessor *)&sofProcessor );

	bool success;
	Network network;
	std::string destIP( "127.0.0.1" );
	int destPort = 8004;
	int recvPort = 8005;
	success = network.openSocket( destIP.c_str(), destPort, recvPort );
	if( !success )
	{
		cout << "Unable to open socket (send: "
		<< destIP << ":" << destPort << " recv: " << recvPort << ")" << endl;
		return 1;
	}

	unsigned int cur_test_index = 0;
	unsigned int cur_scene_index = 0;
	int sleep_frames = 0;

	vector<SymbologyTest*> tests;
	tests.push_back( new HelloWorldTest( outgoing ) );
	tests.push_back( new FilledRectTest( outgoing ) );
	tests.push_back( new UnfilledRectTest( outgoing ) );
//	tests.push_back( new TexturedRectTest( outgoing ) );
	tests.push_back( new FilledEllipseTest( outgoing ) );
	tests.push_back( new ManyCircleTypesTest( outgoing ) );
	tests.push_back( new LineTest( outgoing ) );
	tests.push_back( new PolyTest( outgoing ) );
//	tests.push_back( new PolyLineTest( outgoing ) );
//	tests.push_back( new GroupTest( outgoing ) );


	// set up the symbol surface
	bool haveAddedSurfaceDef = false;
	CigiSymbolSurfaceDefV3_3 surfaceDef;

	surfaceDef.SetSurfaceID( SURFACE_ID );
	surfaceDef.SetSurfaceState( CigiBaseSymbolSurfaceDef::Active );
	surfaceDef.SetAttached( CigiBaseSymbolSurfaceDef::ViewAttached );

	// shared/"union"ed
	surfaceDef.SetViewID( 0 );
	// set up the surface so that it covers most, but not all, of the view
	surfaceDef.SetLeftEdgePosition(   0.1 );
	surfaceDef.SetRightEdgePosition(  0.9 );
	surfaceDef.SetTopEdgePosition(    0.9 );
	surfaceDef.SetBottomEdgePosition( 0.1 );

	surfaceDef.SetMinU( -1. );
	surfaceDef.SetMaxU( 1. );
	surfaceDef.SetMaxV( 1. );
	surfaceDef.SetMinV( -1. );

	/*
	Note - you can account for aspect ratio by either adjusting the 
	surface extents and leaving the min/max uv values alone, or by 
	leaving the surface extents alone and adjusting the min/max uv 
	values.  The choice is yours.  I'd recommend the first option, since 
	it isn't a great idea to mess with your uv coordinates.
	
	Above, I don't adjust for aspect ratio at all.  Unless it so happens 
	that the IG is rendering at some strange, square resolution (like 
	1024x1024 or something) then the symbols will appear stretched.  
	No big deal; it's just a test.
	*/

	
	unsigned char buf[ 4096 ];
	int size = 0;
	
	cout << "Waiting for the IG to send SOF and transition to Operate mode." << endl;
	
	// wait until the IG is ready
	bool igIsReady = false;
	while( !igIsReady )
	{
		size = network.recvBlock( buf, sizeof( buf ) );
		if( size <= 0 )
		{
			// some kind of error
			std::cerr << "There was a networking error\n";
			// bail, try again
			continue;
		}
		try
		{
			incoming.ProcessIncomingMsg( buf, size );
		}
		catch( CigiException & te )
		{
			cout << "Exception = " << te.what() << endl;
		}

		IG.SetFrameCntr( ++frame );
		try
		{
			outgoing << IG;
		}
		catch( CigiException & te )
		{
			cout << __FILE__ << ":" << __LINE__ << " " << te.what() << endl;
		}

		int msg_len;
		outgoing.LockMsg();
		unsigned char *cigi_buffer = outgoing.GetMsg( msg_len );

		int retval = network.send( cigi_buffer, msg_len );
		if( retval == -1 )
		{
			cout << "Error sending!" << endl;
		}

		outgoing.UnlockMsg();
//		outgoing.BeginMsg();

		if( sofProcessor.igIsInOperateMode )
			igIsReady = true;
	}
	
	
	cout << "IG is ready.  Beginning tests." << endl;
	
	if( interactive )
	{
		cout << "Press <ENTER> to advance through each symbology test." << endl;
	}

	while( 1 )
	{

		//
		// Process incoming messages
		//

		size = network.recvBlock( buf, sizeof( buf ) );
		if( size <= 0 )
		{
			// some kind of error
			std::cerr << "There was a networking error\n";
			// bail, try again
			continue;
		}

		try
		{
			incoming.ProcessIncomingMsg( buf, size );
		}
		catch( CigiException & te )
		{
			cout << "Exception = " << te.what() << endl;
		}

		//
		// Create outgoing messages
		//

		IG.SetFrameCntr( ++frame );
		try
		{
			outgoing << IG;
		}
		catch( CigiException & te )
		{
			cout << __FILE__ << ":" << __LINE__ << " " << te.what() << endl;
		}
		
		// set up the symbol surface on the first frame
		if( !haveAddedSurfaceDef )
		{
			outgoing << surfaceDef;
			haveAddedSurfaceDef = true;
		}

		//
		// Draw a scene of a test or sleep this frame if instructed to
		// by the last test.
		//
		if( sleep_frames > 0 && !interactive )
		{
			--sleep_frames;
		}
		else
		{
			bool advance = false;

//#if 0
			if( interactive )
			{
				// check to see if user hit key
				struct timeval ts;
				ts.tv_sec = 0;
				ts.tv_usec = 0;

				fd_set readfds;
				FD_ZERO( &readfds );
				FD_SET( 0, &readfds );

				if( select( 1, &readfds, NULL, NULL, &ts ) )
				{
					if( FD_ISSET( 0, &readfds ) )
					{
						// a key was pressed.
						string crap;
						getline( cin, crap );
						advance = true;
					}
				}
			}
			else
//#endif
//std::string dummy;
//cin >> dummy;
			{
				advance = true;
			}


			if( advance )
			{
				if( cur_test_index >= tests.size() )
				{
					cout << "Done with all tests!" << endl;
					break;
				}

				SymbologyTest *cur_test = tests[ cur_test_index ];
				sleep_frames = cur_test->draw( cur_scene_index );
				cout << cur_test->name() << " " << cur_test_index << ":" << cur_scene_index << endl;

				// Increment scene and/or test
				++cur_scene_index;
				if( cur_scene_index >= cur_test->num_scenes() )
				{
					++cur_test_index;
					cur_scene_index = 0;
				}
			}
		}

		//
		// Send outgoing messages
		//

		int msg_len;
		outgoing.LockMsg();
		unsigned char *cigi_buffer = outgoing.GetMsg( msg_len );

		int retval = network.send( cigi_buffer, msg_len );
		if( retval == -1 )
		{
			cout << "Error sending!" << endl;
		}

		outgoing.UnlockMsg();
//		outgoing.BeginMsg();
	}

	for( uint i = 0; i < tests.size(); ++i )
	{
		delete tests[ i ];
	}

	return 0;
}

