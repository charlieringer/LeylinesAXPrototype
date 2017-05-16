#include "tile.h"
Tile::Tile()
{
	x = 0;
	y = 0;
	w = 0;
	h = 0;
}

Tile::Tile(int _x, int _y, int _w, int _h)
{
	x = _x;
	y = _y;
	w = _w;
	h = _h;
}

void Tile::display()
{
	fill(fillColour);
	drawRect(x, y, w, h);
}

void Tile::setTileType(int _tileType)
{
	tileType = _tileType;
	switch(tileType)
	{
		case(EMPTY):
			fillColour.set(128,128,128,255);
			break;

		case(MINUSTWO):
			fillColour.set(0,0,0,255);
			break;

		case(ONE):
			fillColour.set(50,255,255,255);
			break;

		case(TWO):
			fillColour.set(100,255,255,255);
			break;

		case(THREE):
			fillColour.set(150,255,255,255);
			break;

		case(FOUR):
			fillColour.set(200,255,255,255);
			break;

		case(YOURWIZ):
			fillColour.set(0,255,0,255);
			break;

		case(AIWIZ):
			fillColour.set(0,0,255,255);
			break;
	}
}

void Tile::setDraggable(bool _dragable)
{
	dragable = _dragable;
}

