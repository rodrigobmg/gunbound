#ifndef _BITMAPFONTBASE_H_
#define _BITMAPFONTBASE_H_

#include "cocos2d.h"

USING_NS_CC;

enum TypeFont
{
	Font_Yellow = 1 ,
	Font_Green = 2
};


class BitmapFontBase : public Sprite
{
public:
	static BitmapFontBase* createLabelWithBitmapFont(int value, int typeFont);
	virtual bool initLabelWithBitmapFont(int value, int typeFont);

	void onEnter();

private:

	int _value;
	int _typeFont;
};


#endif //_BITMAPFONTBASE_H_