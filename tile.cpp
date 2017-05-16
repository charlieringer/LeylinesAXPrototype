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
			//tex = new AXTexture("images/0.jpg");
			break;

		case(MINUSTWO):
			tex = Tile::minustwoTex;
			//tex = new AXTexture("images/0.jpg");
			break;

		case(ONE):
			tex = Tile::oneTex;
			//tex = new AXTexture("images/0.jpg");
			break;

		case(TWO):
			tex = Tile::twoTex;
			//tex = new AXTexture("images/0.jpg");
			break;

		case(THREE):
			tex = Tile::threeTex;
			//tex = new AXTexture("images/0.jpg");
			break;

		case(FOUR):
			tex = Tile::fourTex;
			//tex = new AXTexture("images/0.jpg");
			break;

		case(YOURWIZ):
			tex = Tile::yourWhizTex;
			//tex = new AXTexture("images/0.jpg");
			break;

		case(AIWIZ):
			tex = Tile::aiWhizTex;
			//tex = new AXTexture("images/0.jpg");
			break;
	}
}

void Tile::setDraggable(bool _dragable)
{
	dragable = _dragable;
}

void Tile::loadTextures()
{
	emptyTex->loadImage("images/0.jpg");

    minustwoTex->loadImage("images/1.jpg");
    oneTex->loadImage("images/2.jpg");
    twoTex->loadImage("images/3.jpg");
    threeTex->loadImage("images/4.jpg");
    fourTex->loadImage("images/5.jpg");

    yourWhizTex->loadImage("images/6.jpg");
    aiWhizTex->loadImage("images/7.jpg");
}

AXTexture* Tile::emptyTex = new AXTexture();
AXTexture* Tile::minustwoTex = new AXTexture();
AXTexture* Tile::oneTex = new AXTexture();
AXTexture* Tile::twoTex = new AXTexture();
AXTexture* Tile::threeTex = new AXTexture();
AXTexture* Tile::fourTex = new AXTexture();
AXTexture* Tile::yourWhizTex = new AXTexture();
AXTexture* Tile::aiWhizTex = new AXTexture();

