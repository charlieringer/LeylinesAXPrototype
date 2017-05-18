#include "text.h"

TextObject::~TextObject(){
	delete this->text; // clean up the text
}

void TextObject::display(int x, int y)
{
	drawTexture(text, x, y);
}

void TextObject::displayCentered(int x, int y)
{
	drawTexture(text, x-(text->getWidth()/2), y-(text->getHeight()/2));
}

void TextObject::setText(string _text, AXFont* font)
{
	if(text){
		delete text;
	}
	AXColour fill = AXColour(0,0,0,0);
	text = font->bakeTexture(_text, fill);
	rawString = _text;
}
        