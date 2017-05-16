#pragma once
#include <Axilya/AXMain.h>
using namespace AXGraphics;

enum{
	MINUSTWO = -2,
	ONE = 1,
	TWO = 2,
	THREE = 3,
	FOUR = 4,
	EMPTY = 0,

	YOURWIZ = 200,
	AIWIZ = 300,
};

class Tile
{
	public:
		Tile();
		Tile(int x, int y, int w, int h);
		void display();
		void setTileType(int type);
		void setDraggable(bool draggable);
		void setX(int _x){ x = _x;};
		void setY(int _y){ y = _y;};
		void setXCenteredOn(int _x){ x = _x-w/2;};
		void setYCenteredOn(int _y){ y = _y-h/2;};
		void setWH(int _wh){w = h = _wh;};
		int getX(){return x;};
		int getY(){return y;};
		int getType(){return tileType;};
		bool containsPoint(int oX, int oY){return(oX > x && oX < x+w && oY > y && oY < y+h);};

	private:
		int x = 0;
		int y = 0;
		int w = 80; 
		int h = 80;
		bool beingDragged = false;
		bool dragable;
		int tileType;
		AXColour fillColour;
};