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
	drawTexture(tex, x, y, w, h);
}

void Tile::setTileType(int _tileType)
{
	tileType = _tileType;
	switch(tileType)
	{
		case(EMPTY):
			tex = Tile::emptyTex;
			break;

		case(MINUSTWO):
			tex = Tile::minustwoTex;
			break;

		case(ONE):
			tex = Tile::oneTex;
			break;

		case(TWO):
			tex = Tile::twoTex;
			break;

		case(THREE):
			tex = Tile::threeTex;
			break;

		case(FOUR):
			tex = Tile::fourTex;
			break;

		case(YOURWIZ):
			tex = Tile::yourWhizTex;
			break;

		case(AIWIZ):
			tex = Tile::aiWhizTex;
			break;
	}
}

void Tile::setDraggable(bool _dragable)
{
	dragable = _dragable;
}

void Tile::loadTextures()
{
	emptyTex = new AXTexture("images\\0.jpg");

    minustwoTex = new AXTexture("images\\1.jpg");
    oneTex = new AXTexture("images\\2.jpg");
    twoTex = new AXTexture("images\\3.jpg");
    threeTex = new AXTexture("images\\4.jpg");
    fourTex = new AXTexture("images\\5.jpg");

    yourWhizTex = new AXTexture("images\\6.jpg");
    aiWhizTex = new AXTexture("images\\7.jpg");
}

AXTexture* Tile::emptyTex = NULL;
AXTexture* Tile::minustwoTex = NULL;
AXTexture* Tile::oneTex = NULL;
AXTexture* Tile::twoTex = NULL;
AXTexture* Tile::threeTex = NULL;
AXTexture* Tile::fourTex = NULL;
AXTexture* Tile::yourWhizTex = NULL;
AXTexture* Tile::aiWhizTex = NULL;

