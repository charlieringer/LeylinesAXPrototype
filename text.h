#pragma once
#include <Axilya/AXMain.h>
using namespace std;
using namespace AXGraphics;

class TextObject
{
	private:
		string rawString;
<<<<<<< HEAD
		AXTexture* text = nullptr;
=======
		AXTexture* text;
>>>>>>> 8ff21d5e2200320ea6c19150f957edbd4a1d921a
	public:
		~TextObject();
		void display(int x, int y);
		void displayCentered(int x, int y);
		void setText(string _text, AXFont* font);
};