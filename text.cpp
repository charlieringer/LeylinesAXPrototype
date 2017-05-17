#include "text.h"

void TextObject::display(int x, int y)
{
	drawSDLTexture(text, x, y, size.x, size.y);
}

void TextObject::displayCentered(int x, int y)
{
	drawSDLTexture(text, x-(size.x/2), y-(size.y/2), size.x, size.y);
}

void TextObject::setText(string _text, AXFont* font)
{
	AXColour fill = AXColour(0,0,0,0);
	text = font->bakeTexture(_text, fill);
	size = font->getStringSize(_text);
	rawString = _text;
}
        