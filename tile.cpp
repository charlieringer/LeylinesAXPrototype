#include "tile.h"
Tile::Tile(): x (0), y(0), w(0), h (0){}

Tile::Tile(int _x, int _y, int _w, int _h) : x (_x), y(_y), w(_w), h (_h){}

void Tile::display()
{
	drawTexture(tex, x, y, w, h);
}

void Tile::setType(int _tileType)
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
	emptyTex->loadImage("data/images/0.jpg");

    minustwoTex->loadImage("data/images/1.jpg");
    oneTex->loadImage("data/images/2.jpg");
    twoTex->loadImage("data/images/3.jpg");
    threeTex->loadImage("data/images/4.jpg");
    fourTex->loadImage("data/images/5.jpg");

    yourWhizTex->loadImage("data/images/6.jpg");
    aiWhizTex->loadImage("data/images/7.jpg");
}

AXTexture* Tile::emptyTex = new AXTexture();
AXTexture* Tile::minustwoTex = new AXTexture();
AXTexture* Tile::oneTex = new AXTexture();
AXTexture* Tile::twoTex = new AXTexture();
AXTexture* Tile::threeTex = new AXTexture();
AXTexture* Tile::fourTex = new AXTexture();
AXTexture* Tile::yourWhizTex = new AXTexture();
AXTexture* Tile::aiWhizTex = new AXTexture();

