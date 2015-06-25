#pragma execution_character_set("utf-8")
#include "SkillDetailDialog.h"

SkillDetailDialog* SkillDetailDialog::create(SkillInfo skillInfo, MyTouchEvent decideCallback, MyTouchEvent cancelCallback)
{
	SkillDetailDialog* layer = new (std::nothrow) SkillDetailDialog();
	if (layer && layer->init(skillInfo , decideCallback , cancelCallback))
	{
		layer->autorelease();
		return layer;
	}
	CC_SAFE_DELETE(layer);
	return nullptr;
}

bool SkillDetailDialog::init(SkillInfo skillInfo, MyTouchEvent decideCallback, MyTouchEvent cancelCallback)
{
	if (!DialogBase::init(decideCallback, cancelCallback))
	{
		return false;
	}

	_skillInfo = skillInfo;

	createDisplaySkillInfo(getDialogBG());

	return true;
}

/*
Ham xay dung noi dung cua dialog
*/
void SkillDetailDialog::createDisplaySkillInfo(Sprite* parent)
{
	// Create skill image
	auto skillImage = Sprite::create(_skillInfo.imagePath);
	skillImage->setPosition(Vec2(skillImage->getContentSize().width + 40, parent->getContentSize().height / 2));
	skillImage->setScale(2.0f);
	parent->addChild(skillImage);

	std::stringstream info;
	info << "Name : " << _skillInfo.name << "\nId: " << _skillInfo.id << "\nMP: " << _skillInfo.mp_cost;
	_statusLabel = LabelTTF::create(info.str().c_str(), "fonts/arial.ttf", 30);
	_statusLabel->setColor(Color3B::RED);
	_statusLabel->setHorizontalAlignment(TextHAlignment::LEFT);

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