#include "MiniCircle.h"

MiniCircle* MiniCircle::createMiniCircle(int circleLocation)
{
	auto layer = new MiniCircle();
	if (layer && layer->initWithLocation(circleLocation))
	{
		layer->autorelease();
		return layer;
	}
	CC_SAFE_DELETE(layer);
	return nullptr;
}


bool MiniCircle::initWithLocation(int circleLocation)
{
	if (!Sprite::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();

	_miniCircle = Sprite::create("image/screen/battle/mini_circle.png");

	// Duong kinh mong muon cua miniCircle
	float diameter = Director::getInstance()->getOpenGLView()->getFrameSize().height * MINICIRCLE_SCALE; // duong kinh moi cua miniCirlce
	// baseY la ti le cua newCircle va oldCirlce
	float base = (float)(diameter / _miniCircle->getContentSize().height);


	// Ti le kich thuoc hien thi va kich thuoc cua may
	float scaleX = Director::getInstance()->getOpenGLView()->getFrameSize().width / visibleSize.width;
	float scaleY = Director::getInstance()->getOpenGLView()->getFrameSize().height / visibleSize.height;

	// Thay doi kich thuoc theo kich thuoc cua may
	setMiniCircleScaleXRate(base * (1 / scaleX));
	setMiniCircleScaleYRate(base * (1 / scaleY));

	_miniCircle->setScale(getMiniCircleScaleXRate(), getMiniCircleScaleYRate());

	addChild(_miniCircle);

	_circleX = _miniCircle->getContentSize().width * getMiniCircleScaleXRate();
	_circleY = _miniCircle->getContentSize().height * getMiniCircleScaleYRate();

	switch (circleLocation)
	{
	case MOVE_CIRCLE_LEFT:
		this->setPosition(Vec2(visibleSize.width / 48 + _circleX / 2, visibleSize.height / 32 + _circleY / 2));
		break;
	case MOVE_CIRCLE_RIGHT:
		this->setPosition(Vec2(visibleSize.width - visibleSize.width / 48 - _circleX / 2, visibleSize.height / 32 + _circleY / 2));
		break;
	default:
		this->setPosition(Vec2(visibleSize.width / 48 + _circleX / 2, visibleSize.height / 32 + _circleY / 2));
		break;
	}


	return true;
}


Rect MiniCircle::getRect()
{
	Vec2 point = this->getPosition();

	float w = this->getMiniCircleAfterScaleX();
	float h = this->getMiniCircleAfterScaleY();

	return CCRectMake(point.x - w / 2, point.y - h / 2, w, h);
}

bool MiniCircle::isTapped(Vec2 point)
{
	Rect rect = this->getRect();

	if (rect.containsPoint(point))
	{
		return true;
	}
	else
	{
		return false;
	}
}