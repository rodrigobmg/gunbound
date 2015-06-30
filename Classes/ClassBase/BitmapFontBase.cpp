#include "BitmapFontBase.h"

BitmapFontBase* BitmapFontBase::createLabelWithBitmapFont(int value, int typeFont)
{
	auto label = new BitmapFontBase();
	if (label && label->initLabelWithBitmapFont(value, typeFont))
	{
		label->autorelease();
		return label;
	}

	CC_SAFE_DELETE(label);
	return nullptr;
}

bool BitmapFontBase::initLabelWithBitmapFont(int value, int typeFont)
{
	if (!Sprite::init())
	{
		return false;
	}

	_value = value;
	_typeFont = typeFont;


	std::stringstream str;
	str << _value;

	std::string fontPath;
	switch (_typeFont)
	{
	case Font_Green:
		fontPath = "font/fontGreen.fnt";	
		break;
	case Font_Yellow:
		fontPath = "font/fontYellow.fnt";
		break;
	default:
		fontPath = "font/fontGreen.fnt";
		break;
	}

	auto label = Label::createWithBMFont(fontPath, str.str(), TextHAlignment::CENTER);
	label->setPosition(Vec2::ZERO);

	this->addChild(label);

	return true;
}

void BitmapFontBase::onEnter()
{
	Sprite::onEnter();
}