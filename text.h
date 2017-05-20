#pragma once
#include <Axilya/AXMain.h>
using namespace std;
using namespace AXGraphics;

class TextObject
{
	private:
		string rawString;
		AXTexture* text = nullptr;

	public:
		~TextObject();
		void display(int x, int y);
		void displayCentered(int x, int y);
		void setText(string _text, AXFont* font);
};