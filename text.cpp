#include "text.h"

TextObject::~TextObject(){
	delete this->text; // clean up the text
}

void TextObject::display(int x, int y)
{
	if(text) drawTexture(text, x, y);
	else AXLog::error("Invalid text texture being drawn.");
}

void TextObject::displayCentered(int x, int y)
{
	if(text) drawTexture(text, x-(text->getWidth()/2), y-(text->getHeight()/2));
	else AXLog::error("Invalid text texture being drawn.");
}

void TextObject::setText(string _text, AXFont* font)
{
<<<<<<< HEAD
	if(text) delete text;
	AXColour fill = AXColour(0);
=======
	if(text){
		delete text;
	}
	AXColour fill = AXColour(0,0,0,0);
>>>>>>> 8ff21d5e2200320ea6c19150f957edbd4a1d921a
	text = font->bakeTexture(_text, fill);
	rawString = _text;
}