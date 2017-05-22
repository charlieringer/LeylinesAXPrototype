#pragma once
#include <Axilya/AXMain.h>
using namespace AXGraphics;

#ifndef TILE_H
#define TILE_H
enum{
	//These are the actual tile values
	MINUSFOUR = -4,
	MINUSTHREE = -3,
	MINUSTWO = -2,
	MINUSONE = -1,
	EMPTY = 0,
	ONE = 1,
	TWO = 2,
	THREE = 3,
	FOUR = 4,
	FIVE = 5,
	
	//Anything above 50 is a special tile
	TREE = 50,
	ROCK = 51,
	//Anything 100-199 is unknown data
	UNKNOWN = 100,
	RANDOM = 150,
	//200 is your wizard
	YOURWIZ = 200,
	//300 is ai wizard
	AIWIZ = 300,
};

class Tile
{
	private:
		int x = 0;
		int y = 0;
		int w = 80; 
		int h = 80;
		bool beingDragged = false;
		bool dragable;
		int tileType;
		AXTexture* tex = nullptr;

		static AXTexture* emptyTex;
		static AXTexture* minusoneTex;
		static AXTexture* minustwoTex;
		static AXTexture* minusthreeTex;
		static AXTexture* minusfourTex;
		static AXTexture* oneTex;
		static AXTexture* twoTex;
		static AXTexture* threeTex;
		static AXTexture* fourTex;
		static AXTexture* fiveTex;

		static AXTexture* treeTex;
		static AXTexture* rockTex;
		
		static AXTexture* yourWhizTex;
		static AXTexture* aiWhizTex;

	public:
		Tile();
		Tile(int x, int y, int w, int h);
		void display();
		void setType(int type);
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
		static void loadTextures();
};

#endif