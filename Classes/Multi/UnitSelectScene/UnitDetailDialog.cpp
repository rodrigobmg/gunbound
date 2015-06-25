#pragma execution_character_set("utf-8")
#include "UnitDetailDialog.h"

UnitDetailDialog* UnitDetailDialog::create(UnitInfo unitInfo, MyTouchEvent decideCallback, MyTouchEvent cancelCallback){
	UnitDetailDialog* layer = new (std::nothrow) UnitDetailDialog();
	if (layer && layer->init(unitInfo , decideCallback , cancelCallback))
	{
		layer->autorelease();
		return layer;
	}

	CC_SAFE_DELETE(layer);
	return nullptr;
}

bool UnitDetailDialog::init(UnitInfo unitInfo, MyTouchEvent decideCallback, MyTouchEvent cancelCallback){
	if (!DialogBase::init(decideCallback , cancelCallback))
	{
		return false;
	}

	// Goi toi ham lay du lieu tu database theo _unitInfo._id duoc truyen vao
	_unitInfo = unitInfo; // Du lieu cua unit duoc chon

	// create dialog content
	createDisplayUnitInfo(getDialogBG());

	return true;
}

/*
Ham xay dung noi dung cua dialog
*/
void UnitDetailDialog::createDisplayUnitInfo(Sprite* parent){
	// Create unit image
	auto unitImage = Sprite::create(_unitInfo.imagePath);
	unitImage->setScale(1.5f);
	unitImage->setPosition(Vec2(unitImage->getContentSize().width + 80 , parent->getContentSize().height / 2 ));
	parent->addChild(unitImage);


	// status button
	_statusButton = Button::create();
	_statusButton->loadTextureNormal("image/dialog/unitDetail/status_s.png");
	_statusButton->setPosition(Vec2(parent->getContentSize().width / 2 - 10 , parent->getContentSize().height - 45));
	_statusButton->addTouchEventListener(CC_CALLBACK_2(UnitDetailDialog::statusButtonCallback, this));
	parent->addChild(_statusButton);

	// skill button
	_skillButton = Button::create();
	_skillButton->loadTextureNormal("image/dialog/unitDetail/skill.png");
	_skillButton->setPosition(_statusButton->getPosition() + Vec2( _statusButton->getContentSize().width/2 + 10 + _skillButton->getContentSize().width/2 , 0 ));
	_skillButton->addTouchEventListener(CC_CALLBACK_2(UnitDetailDialog::skillButtonCallback, this));
	parent->addChild(_skillButton);

	
	// status label
	log("Unit Name: %s", _unitInfo.name.c_str());


	std::stringstream statusInfo;
	statusInfo << "Name : " << _unitInfo.name << "\nAttack: " << _unitInfo.attack_dame << "\nDefence: " << _unitInfo.defence << "\nSpeech: " << _unitInfo.move_speech << "\nHP : " << _unitInfo.hp << "\nMana: " << _unitInfo.mp << "\nAttribute: " << _unitInfo.attribute << "\nType: " << _unitInfo.type << "\nAttack: " << _unitInfo.attack_dame << "\nDefence: " << _unitInfo.defence << "\nSpeech: " << _unitInfo.move_speech;
	_statusLabel = LabelTTF::create(statusInfo.str().c_str(), "fonts/arial.ttf", 30);
	_statusLabel->setColor(Color3B::RED);
	_statusLabel->setHorizontalAlignment(TextHAlignment::LEFT);


	// skill label
	std::stringstream skillInfo;
	skillInfo << "Skill Name : " << _unitInfo.name;
	_skillLabel = LabelTTF::create(skillInfo.str().c_str(), "fonts/arial.ttf", 30);
	_skillLabel->setColor(Color3B::RED);
	_skillLabel->setHorizontalAlignment(TextHAlignment::LEFT);
	_skillLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	_skillLabel->setPosition(Vec2(parent->getContentSize().width / 2 - 70, parent->getContentSize().height - 100));
	_skillLabel->setVisible(false);
	parent->addChild(_skillLabel);

	// create scroll view info
	if (_statusLabel->getContentSize().height > (parent->getContentSize().height - 200))
	{
		// Create scroll
		auto scrollInfo = extension::ScrollView::create();
		scrollInfo->setDirection(extension::ScrollView::Direction::VERTICAL); // cho scroll di chuyen theo chieu doc
		scrollInfo->setViewSize(Size(parent->getContentSize().width / 2, 300));
		scrollInfo->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		scrollInfo->setPosition(Vec2(parent->getContentSize().width / 2 - 70, 30));
		scrollInfo->updateInset();
		scrollInfo->setVisible(true);
		parent->addChild(scrollInfo);

		// Add layer to scroll
		auto layer = Layer::create();
		layer->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		layer->setContentSize(Size(parent->getContentSize().width / 2, _statusLabel->getContentSize().height + 50));
		layer->setPosition(Vec2(0, -(layer->getContentSize().height / 2)));

		scrollInfo->setContainer(layer);

		// Add _statusLabel to layer
		auto height = layer->getContentSize().height;
		scrollInfo->setContentOffset(scrollInfo->minContainerOffset());
		_statusLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		_statusLabel->setPosition(Vec2(0, height - 20));
		layer->addChild(_statusLabel);
	}
	else
	{
		_statusLabel->setPosition(Vec2(parent->getContentSize().width / 2 - 70, parent->getContentSize().height - 100));
		_statusLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);

		parent->addChild(_statusLabel);
	}
	

}


void UnitDetailDialog::statusButtonCallback(Ref* pSender, Widget::TouchEventType type){
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		_statusLabel->setVisible(true);
		_skillLabel->setVisible(false);
		_statusButton->loadTextureNormal("image/dialog/unitDetail/status_s.png");
		_skillButton->loadTextureNormal("image/dialog/unitDetail/skill.png");
		break;
	}
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void UnitDetailDialog::skillButtonCallback(Ref* pSender, Widget::TouchEventType type){
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		_statusLabel->setVisible(false);
		_skillLabel->setVisible(true);
		_statusButton->loadTextureNormal("image/dialog/unitDetail/status.png");
		_skillButton->loadTextureNormal("image/dialog/unitDetail/skill_s.png");
		break;
	}
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}