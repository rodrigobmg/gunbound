#pragma execution_character_set("utf-8")
#include "DialogBase.h"

DialogBase* DialogBase::create(MyTouchEvent decideCallback , MyTouchEvent cancelCallback)
{
	DialogBase* dialog = new (std::nothrow) DialogBase();
	if(dialog && dialog->init(decideCallback , cancelCallback)){
		dialog->autorelease();
		return dialog;
	}
	CC_SAFE_DELETE(dialog);
	return nullptr;
}

bool DialogBase::init(MyTouchEvent decideCallback , MyTouchEvent cancelCallback)
{
	if(!LayerBase::init()){
		return false;
	}

	// hide parent
	//_menu->setVisible(false);
	_bgImage->setVisible(false);
	_pageTitleSprite->setVisible(false);
	_usernameBG->setVisible(false);


	// Thiet lap cac gia tri truyen vao thanh bien toan cuc
	_decideCallback = decideCallback;
	_cancelCallback = cancelCallback;


	_background->setOpacity(150); // Set do trong cua layer

	///////////////////////////////////////////////////////////////////////////////
	// Xay dung UI cua dialog
	///////////////////////////////////////////////////////////////////////////////

	// dialog background
	_dialogBG = Sprite::create("image/dialog/unitDetail/select_scene_detail.png");
	_dialogBG->setPosition(Vec2(_origin.x + _visibleSize.width / 2, _origin.y + _visibleSize.height / 2));
	addChild(_dialogBG);


	// close button
	_dialogCancelButton = Button::create();
	_dialogCancelButton->loadTextureNormal("image/button/new/button_close.png");
	_dialogCancelButton->setPosition(Vec2(_dialogBG->getContentSize().width - _dialogCancelButton->getContentSize().width/2  - 20 , _dialogBG->getContentSize().height - _dialogCancelButton->getContentSize().height/2 - 20 ));
	_dialogCancelButton->addTouchEventListener(CC_CALLBACK_2(DialogBase::cancelButtonCallback, this));
	_dialogBG->addChild(_dialogCancelButton);

	// decide button
	_dialogDecideButton = Button::create();
	_dialogDecideButton->loadTextureNormal("image/button/new/button_decide.png");
	_dialogDecideButton->setPosition(Vec2(_dialogBG->getContentSize().width - _dialogDecideButton->getContentSize().width / 2 - 25 , _dialogDecideButton->getContentSize().height / 2  + 25  ));
	_dialogDecideButton->addTouchEventListener(CC_CALLBACK_2(DialogBase::decideButtonCallback, this));
	_dialogBG->addChild(_dialogDecideButton);


	return true;
}


void DialogBase::decideButtonCallback(Ref* pSender , Widget::TouchEventType type)
{
	closeDialog();
	if( _decideCallback != nullptr ){
		_decideCallback(pSender , type);
	}
}


void DialogBase::cancelButtonCallback(Ref* pSender , Widget::TouchEventType type)
{
	closeDialog();
	if( _cancelCallback != nullptr){
		_cancelCallback(pSender , type);
	}
}

void DialogBase::closeDialog()
{
	this->runAction(RemoveSelf::create(false));
}
