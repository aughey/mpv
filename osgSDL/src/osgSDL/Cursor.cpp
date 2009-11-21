#include <osgSDL/Cursor>

using namespace osgSDL;

Cursor::Cursor()
:	osg::Referenced(),
	sdl_cursor_(0)
{
}

Cursor::~Cursor()
{
	if (sdl_cursor_) {
		SDL_FreeCursor(sdl_cursor_);
	}
}

WaitCursor::WaitCursor()
:	Cursor()
{
	const int w = 32;
	const int h = 32;
	static const char image[] = {
		"                                "
		"                                "
		"                                "
		"                                "
		"                                "
		"                                "
		"      **        ***        **   "
		"     .**       .***       .**   "
		"     ..**      .***       **    "
		"      .**      **.**      .*    "
		"      .**     .**.**      .*    "
		"      .***    .**.**     ***    "
		"      ..**    .**.**    .**     "
		"       .**    ** ..**   .**     "
		"       .**   .**  .**   .**     "
		"       .***  .**  .**   ***     "
		"       ..**  .**  .**  .**      "
		"        .**  **   ..** .**      "
		"        .***.**    .** .**      "
		"        .***.**    .** ***      "
		"        ..**.**    .**.**       "
		"         .**.*     ..****       "
		"         .****      .****       "
		"         ..***      .****       "
		"          .***      .***        "
		"          ...        ..         "
		"                                "
		"                                "
		"                                "
		"                                "
		"                                "
		"                                "
	};

	Uint8 data[w*h/8];
	Uint8 mask[w*h/8];

	for (int i=0; i<w*h/8; ++i) {
		data[i] = 0; mask[i] = 0;
	}

	for (int i=0; i<w*h; ++i) {
		int sp = 7-i%8;
		if (image[i] == '*') {
			data[i/8] |= 1 << sp; 
			mask[i/8] |= 1 << sp;
		}
		if (image[i] == '.') {
			mask[i/8] |= 1 << sp;
		}
	}

	setSDLCursor(SDL_CreateCursor(data, mask, w, h, w/2, h/2));
}

