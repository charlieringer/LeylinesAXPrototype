#include "tile.h"
Tile::Tile(): x (0), y(0), w(0), h (0){}

Tile::Tile(int _x, int _y, int _w, int _h) : x (_x), y(_y), w(_w), h (_h){}

void Tile::display()
{
	if(tex) drawTexture(tex, x, y, w, h);
}

void Tile::setType(int _tileType)
{
	tileType = _tileType;
	switch(tileType)
	{
		case(EMPTY):
			tex = Tile::emptyTex;
			break;
		case(MINUSONE):
			tex = Tile::minusoneTex;
			break;
		case(MINUSTWO):
			tex = Tile::minustwoTex;
			break;
		case(MINUSTHREE):
			tex = Tile::minusthreeTex;
			break;
		case(MINUSFOUR):
			tex = Tile::minusfourTex;
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
		case(FIVE):
			tex = Tile::fiveTex;
			break;
		case(YOURWIZ):
			tex = Tile::yourWhizTex;
			break;
		case(AIWIZ):
			tex = Tile::aiWhizTex;
			break;
		case(ROCK):
			tex = Tile::rockTex;
			break;
		case(TREE):
			tex = Tile::treeTex;
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

    minusoneTex->loadImage("data/images/minus1.png");
    minustwoTex->loadImage("data/images/minus2.png");
    minusthreeTex->loadImage("data/images/minus3.png");
    minusfourTex->loadImage("data/images/minus4.png");
    oneTex->loadImage("data/images/1.png");
    twoTex->loadImage("data/images/2.png");
    threeTex->loadImage("data/images/3.png");
    fourTex->loadImage("data/images/4.png");
    fiveTex->loadImage("data/images/5.png");

    treeTex->loadImage("data/images/tree.png");
    rockTex->loadImage("data/images/rock.png");

    yourWhizTex->loadImage("data/images/6.png");
    aiWhizTex->loadImage("data/images/7.png");
}

AXTexture* Tile::emptyTex = new AXTexture();
AXTexture* Tile::minusoneTex = new AXTexture();
AXTexture* Tile::minustwoTex = new AXTexture();
AXTexture* Tile::minusthreeTex = new AXTexture();
AXTexture* Tile::minusfourTex = new AXTexture();
AXTexture* Tile::oneTex = new AXTexture();
AXTexture* Tile::twoTex = new AXTexture();
AXTexture* Tile::threeTex = new AXTexture();
AXTexture* Tile::fourTex = new AXTexture();
AXTexture* Tile::fiveTex = new AXTexture();
AXTexture* Tile::treeTex = new AXTexture();
AXTexture* Tile::rockTex = new AXTexture();
AXTexture* Tile::yourWhizTex = new AXTexture();
AXTexture* Tile::aiWhizTex = new AXTexture();

