#pragma execution_character_set("utf-8")

#include "LayerBase.h"

bool LayerBase::init(){
	if(!Layer::init()){
		return false;
	}


	// Default
	_visibleSize = Director::getInstance()->getVisibleSize();
	_origin = Director::getInstance()->getVisibleOrigin();

	// Create layer background
	_background = LayerColor::create(Color4B(0,0,0,255));
	addChild(_background);

	_bgImage = Sprite::create("image/screen/gunbound_bg1.png");
	_bgImage->setScale((float)(_visibleSize.width / _bgImage->getContentSize().width) , (float)(_visibleSize.height / _bgImage->getContentSize().height));
	_bgImage->setPosition(Vec2(_origin.x + _visibleSize.width / 2, _origin.y + _visibleSize.height / 2));
	addChild(_bgImage);

	// Create layer title
	_pageTitleSprite = Sprite::create("image/navigator/title.png");
	_pageTitleSprite->setPosition(Vec2( _origin.x + _visibleSize.width /2 , _origin.y + _visibleSize.height - _pageTitleSprite->getContentSize().height/2));
	addChild(_pageTitleSprite);

	_titleLabel = Label::createWithSystemFont("", JAPANESE_FONT_1_BOLD, 30);
	_titleLabel->setPosition(Vec2(_pageTitleSprite->getContentSize().width / 2, _pageTitleSprite->getContentSize().height / 2));
	_titleLabel->setColor(Color3B::BLACK);
	_pageTitleSprite->addChild(_titleLabel);

	// Create back button
	_backButton = Button::create();
	_backButton->loadTextureNormal("image/button/new/button_return.png");
	_backButton->loadTextureDisabled("image/button/new/button_return_disable.png");
	_backButton->setPosition(Vec2(_origin.x + _backButton->getContentSize().width / 2, _origin.y + _visibleSize.height - _backButton->getContentSize().height / 2));
	addChild(_backButton);

	// Create user name background
	_usernameBG = Sprite::create("image/navigator/user_name.png");
	_usernameBG->setScaleX(0.5f);
	_usernameBG->setScaleY(0.8f);
	_usernameBG->setPosition(Vec2( _origin.x + _visibleSize.width - _usernameBG->getContentSize().width / 2 * 0.5f , _origin.y + _visibleSize.height - _usernameBG->getContentSize().height / 2 * 0.8f ));
	addChild(_usernameBG);

	_usernameLabel = Label::createWithSystemFont("User Name", JAPANESE_FONT_1_BOLD  , 30);
	_usernameLabel->setPosition(Vec2(_usernameBG->getContentSize().width / 2 , _usernameBG->getContentSize().height/2));
	_usernameLabel->setColor(Color3B::BLACK);
	_usernameBG->addChild(_usernameLabel);

	return true;
}

void LayerBase::onEnter(){
	Layer::onEnter();

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);

	touchListener->onTouchBegan = CC_CALLBACK_2(LayerBase::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(LayerBase::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(LayerBase::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

}


void LayerBase::onExit(){
	Layer::onExit();
}

bool LayerBase::onTouchBegan(Touch* touch, Event* event){
	return true; // if return false, onTouchMove and onTouchEnd not call
}

void LayerBase::onTouchMoved(Touch* touch, Event* event){

}

void LayerBase::onTouchEnded(Touch* touch, Event* event){

}