#pragma once
#include <Axilya/AXMain.h>
using namespace std;
using namespace AXGraphics;

class TextObject
{
	private:
		string rawString;
		SDL_Texture* text;
		AXVector2D size;
	public:
		void display(int x, int y);
		void displayCentered(int x, int y);
		void setText(string _text, AXFont* font);
};