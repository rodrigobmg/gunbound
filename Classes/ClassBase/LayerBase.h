#ifndef _LAYERBASE_H_
#define _LAYERBASE_H_

#include "cocos2d.h"
#include "Define.h"
#include "ui/CocosGUI.h"


USING_NS_CC;
using namespace ui;

class LayerBase : public cocos2d::Layer
{
public:

	bool init();
	virtual void onEnter();
	virtual void onExit();

	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchMoved(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);

protected :

	Size _visibleSize;
	Vec2 _origin;
	LayerColor* _background;

	Sprite* _bgImage;
	Sprite* _pageTitleSprite;
	Label* _titleLabel;

	Sprite* _usernameBG;
	Label* _usernameLabel;

	//Menu* _menu;
	//MenuItemImage* _backButton;
	Button* _backButton;

private:




};


#endif // _LAYERBASE_H_
