#pragma execution_character_set("utf-8")
#include "MoveTypeSelectDialog.h"

MoveTypeSelectDialog* MoveTypeSelectDialog::create(MyTouchEvent decideCallback, MyTouchEvent cancelCallback)
{
	auto layer = new MoveTypeSelectDialog();
	if (layer && layer->init(decideCallback, cancelCallback))
	{
		layer->autorelease();
		return layer;
	}
	CC_SAFE_DELETE(layer);
	return nullptr;
}

bool MoveTypeSelectDialog::init(MyTouchEvent decideCallback, MyTouchEvent cancelCallback)
{
	if (!DialogBase::init(decideCallback, cancelCallback))
	{
		return false;
	}


	/* Thiet lap lai background va cac thanh phan co san cho dialog */
	_dialogCancelButton->setVisible(false);
	_dialogBG->setTexture(TextureCache::sharedTextureCache()->addImage("image/screen/battle/dialogbg.png"));
	_backButton->setVisible(false);
	_dialogDecideButton->setPosition(_dialogDecideButton->getPosition() + Vec2( -20 , 10));


	_selectMode = UserDefault::getInstance()->getIntegerForKey(MOVEKEY);

	if (_selectMode == MOVE_CIRCLE)
	{
		_circleLocationMode = UserDefault::getInstance()->getIntegerForKey(MOVE_CIRCLE_LOCATION);
		_circleTypeMode = UserDefault::getInstance()->getIntegerForKey(MOVE_CIRCLE_TYPE);
	}
	

	createDialogContent();
	createDefaultMode();


	return true;
}

void MoveTypeSelectDialog::createDialogContent()
{
	////////////////////////////////////////////////////////////////////////////
	// MOVE_AUTO 
	////////////////////////////////////////////////////////////////////////////

	Size size = _dialogBG->getContentSize();

	_mode1 = CheckBox::create("image/navigator/checkbox/check_box_normal.png",
		"image/navigator/checkbox/check_box_normal_press.png",
		"image/navigator/checkbox/check_box_active.png",
		"image/navigator/checkbox/check_box_normal_disable.png",
		"image/navigator/checkbox/check_box_normal_disable.png");
	_mode1->setPosition(Vec2(size.width * 1 / 6, size.height * 5 / 6));
	_mode1->addEventListener(CC_CALLBACK_2(MoveTypeSelectDialog::checkBox1Callback, this));

	_select1Txt = Label::createWithSystemFont("Di chuyen bang cach tap den vi tri muon den", JAPANESE_FONT_1_BOLD, 30);
	_select1Txt->setColor(Color3B::BLACK);
	_select1Txt->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	_select1Txt->setHorizontalAlignment(TextHAlignment::LEFT);
	_select1Txt->setPosition(_mode1->getPosition() + Vec2(20, 0));

	////////////////////////////////////////////////////////////////////////////
	// MOVE_MANUAL
	////////////////////////////////////////////////////////////////////////////

	_mode2 = CheckBox::create("image/navigator/checkbox/check_box_normal.png",
		"image/navigator/checkbox/check_box_normal_press.png",
		"image/navigator/checkbox/check_box_active.png",
		"image/navigator/checkbox/check_box_normal_disable.png",
		"image/navigator/checkbox/check_box_normal_disable.png");
	_mode2->setPosition(Vec2(size.width * 1 / 6, size.height * 2 / 3));
	_mode2->addEventListener(CC_CALLBACK_2(MoveTypeSelectDialog::checkBox2Callback, this));

	_select2Txt = Label::createWithSystemFont("Di chuyen bang cach touch move man hinh", JAPANESE_FONT_1_BOLD, 30);
	_select2Txt->setColor(Color3B::BLACK);
	_select2Txt->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	_select2Txt->setHorizontalAlignment(TextHAlignment::LEFT);
	_select2Txt->setPosition(_mode2->getPosition() + Vec2(20, 0));

	////////////////////////////////////////////////////////////////////////////
	// MOVE_CIRCLE
	////////////////////////////////////////////////////////////////////////////

	_mode3 = CheckBox::create("image/navigator/checkbox/check_box_normal.png",
		"image/navigator/checkbox/check_box_normal_press.png",
		"image/navigator/checkbox/check_box_active.png",
		"image/navigator/checkbox/check_box_normal_disable.png",
		"image/navigator/checkbox/check_box_normal_disable.png");
	_mode3->setPosition(Vec2(size.width * 1 / 6, size.height * 1 / 2));
	_mode3->addEventListener(CC_CALLBACK_2(MoveTypeSelectDialog::checkBox3Callback, this));

	_select3Txt = Label::createWithSystemFont("Di chuyen trong vong tron", JAPANESE_FONT_1_BOLD, 30);
	_select3Txt->setColor(Color3B::BLACK);
	_select3Txt->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	_select3Txt->setHorizontalAlignment(TextHAlignment::LEFT);
	_select3Txt->setPosition(_mode3->getPosition() + Vec2(20, 0));

	//////////////////////
	// LEFT
	//////////////////////
	_mode3Left = CheckBox::create("image/navigator/checkbox/check_box_normal.png",
		"image/navigator/checkbox/check_box_normal_press.png",
		"image/navigator/checkbox/check_box_active.png",
		"image/navigator/checkbox/check_box_normal_disable.png",
		"image/navigator/checkbox/check_box_active_disable.png");

	_mode3Left->setPosition(Vec2(size.width * 1 / 4, size.height * 1 / 3));
	_mode3Left->addEventListener(CC_CALLBACK_2(MoveTypeSelectDialog::touchMove3LeftCallback, this));

	_optionText3Left = Label::createWithSystemFont("Trai", JAPANESE_FONT_1_BOLD, 30);

	_optionText3Left->setHorizontalAlignment(TextHAlignment::LEFT);
	_optionText3Left->setColor(Color3B::BLACK);
	_optionText3Left->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	_optionText3Left->setPosition(_mode3Left->getPosition() + Vec2(20, 0));

	//////////////////////
	// RIGHT
	//////////////////////
	_mode3Right = CheckBox::create("image/navigator/checkbox/check_box_normal.png",
		"image/navigator/checkbox/check_box_normal_press.png",
		"image/navigator/checkbox/check_box_active.png",
		"image/navigator/checkbox/check_box_normal_disable.png",
		"image/navigator/checkbox/check_box_active_disable.png");

	_mode3Right->setPosition(Vec2(size.width * 1 / 2, size.height * 1 / 3));
	_mode3Right->addEventListener(CC_CALLBACK_2(MoveTypeSelectDialog::touchMove3RightCallback, this));

	_optionText3Right = Label::createWithSystemFont("Phai", JAPANESE_FONT_1_BOLD, 30);

	_optionText3Right->setHorizontalAlignment(TextHAlignment::LEFT);
	_optionText3Right->setColor(Color3B::BLACK);
	_optionText3Right->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	_optionText3Right->setPosition(_mode3Right->getPosition() + Vec2(20, 0));

	//////////////////////
	// TIME
	//////////////////////
	_mode3Time = CheckBox::create("image/navigator/checkbox/check_box_normal.png",
		"image/navigator/checkbox/check_box_normal_press.png",
		"image/navigator/checkbox/check_box_active.png",
		"image/navigator/checkbox/check_box_normal_disable.png",
		"image/navigator/checkbox/check_box_active_disable.png");

	_mode3Time->setPosition(Vec2(size.width * 1 / 4, size.height * 1 / 6));
	_mode3Time->addEventListener(CC_CALLBACK_2(MoveTypeSelectDialog::touchMove3TimeCallback, this));

	_optionText3Time = Label::createWithSystemFont("Thoi gian", JAPANESE_FONT_1_BOLD, 30);

	_optionText3Time->setHorizontalAlignment(TextHAlignment::LEFT);
	_optionText3Time->setColor(Color3B::BLACK);
	_optionText3Time->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	_optionText3Time->setPosition(_mode3Time->getPosition() + Vec2(20, 0));

	//////////////////////
	// DISTANCE
	//////////////////////
	_mode3Distance = CheckBox::create("image/navigator/checkbox/check_box_normal.png",
		"image/navigator/checkbox/check_box_normal_press.png",
		"image/navigator/checkbox/check_box_active.png",
		"image/navigator/checkbox/check_box_normal_disable.png",
		"image/navigator/checkbox/check_box_active_disable.png");

	_mode3Distance->setPosition(Vec2(size.width * 1 / 2, size.height * 1 / 6));
	_mode3Distance->addEventListener(CC_CALLBACK_2(MoveTypeSelectDialog::touchMove3DistanceCallback, this));

	_optionText3Distance = Label::createWithSystemFont("Khoang cach", JAPANESE_FONT_1_BOLD, 30);

	_optionText3Distance->setHorizontalAlignment(TextHAlignment::LEFT);
	_optionText3Distance->setColor(Color3B::BLACK);
	_optionText3Distance->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	_optionText3Distance->setPosition(_mode3Distance->getPosition() + Vec2(20, 0));

	////////////////////////////////////////////////////////////////////////////
	// ADD TO BACKGROUND
	////////////////////////////////////////////////////////////////////////////
	_dialogBG->addChild(_mode1);
	_dialogBG->addChild(_mode2);
	_dialogBG->addChild(_mode3);

	_dialogBG->addChild(_select1Txt);
	_dialogBG->addChild(_select2Txt);
	_dialogBG->addChild(_select3Txt);

	_dialogBG->addChild(_mode3Left);
	_dialogBG->addChild(_optionText3Left);
	_dialogBG->addChild(_mode3Right);
	_dialogBG->addChild(_optionText3Right);

	_dialogBG->addChild(_mode3Time);
	_dialogBG->addChild(_optionText3Time);
	_dialogBG->addChild(_mode3Distance);
	_dialogBG->addChild(_optionText3Distance);
}

void MoveTypeSelectDialog::createDefaultMode()
{
	////////////////////////////////////////////////////////////////////////////
	// SET CHECKBOX DEFAULT AND ENABLE 
	////////////////////////////////////////////////////////////////////////////

	switch (_selectMode)
	{
	case MOVE_AUTO:
		_mode1->setSelected(true);
		_mode1->setTouchEnabled(false); // Khong cho tu click neu da o trang thai selected

		_mode3Left->setVisible(false);
		_mode3Right->setVisible(false);
		_optionText3Left->setVisible(false);
		_optionText3Right->setVisible(false);

		_mode3Time->setVisible(false);
		_mode3Distance->setVisible(false);
		_optionText3Time->setVisible(false);
		_optionText3Distance->setVisible(false);

		break;
	case MOVE_MANUAL:
		_mode2->setSelected(true);
		_mode2->setTouchEnabled(false);

		_mode3Left->setVisible(false);
		_mode3Right->setVisible(false);
		_optionText3Left->setVisible(false);
		_optionText3Right->setVisible(false);

		_mode3Time->setVisible(false);
		_mode3Distance->setVisible(false);
		_optionText3Time->setVisible(false);
		_optionText3Distance->setVisible(false);

		break;

	case MOVE_CIRCLE:
		_mode3->setSelected(true);
		_mode3->setTouchEnabled(false);

		_mode3Left->setVisible(true);
		_mode3Right->setVisible(true);

		_mode3Time->setVisible(true);
		_mode3Distance->setVisible(true);


		switch (_circleLocationMode)
		{
		case MOVE_CIRCLE_LEFT:
			_mode3Left->setSelected(true);
			_mode3Left->setTouchEnabled(false);

			_mode3Right->setSelected(false);
			_mode3Right->setTouchEnabled(true);

			_circleLocationMode = MOVE_CIRCLE_LEFT;
			break;
		case MOVE_CIRCLE_RIGHT:
			_mode3Left->setSelected(false);
			_mode3Left->setTouchEnabled(true);

			_mode3Right->setSelected(true);
			_mode3Right->setTouchEnabled(false);

			_circleLocationMode = MOVE_CIRCLE_RIGHT;
			break;
		default:
			break;
		}

		switch (_circleTypeMode)
		{
		case MOVE_CIRCLE_TIME:
			_mode3Time->setSelected(true);
			_mode3Time->setTouchEnabled(false);
			_mode3Distance->setSelected(false);
			_mode3Distance->setTouchEnabled(true);
			_circleTypeMode = MOVE_CIRCLE_TIME;
			break;
		case MOVE_CIRCLE_DISTANCE:
			_mode3Time->setSelected(false);
			_mode3Time->setTouchEnabled(true);
			_mode3Distance->setSelected(true);
			_mode3Distance->setTouchEnabled(false);
			_circleTypeMode = MOVE_CIRCLE_DISTANCE;
			break;
		default:
			break;
		}

		_optionText3Left->setVisible(true);
		_optionText3Right->setVisible(true);
		_optionText3Time->setVisible(true);
		_optionText3Distance->setVisible(true);

		break;
	default:
		break;
	}

}

void MoveTypeSelectDialog::checkBox1Callback(Ref* pSender, CheckBox::EventType type)
{
	switch (type)
	{
	case cocos2d::ui::CheckBox::EventType::SELECTED:
	{
		_mode1->setSelected(true);
		_mode1->setTouchEnabled(false);
		_mode2->setSelected(false);
		_mode2->setTouchEnabled(true);
		_mode3->setSelected(false);
		_mode3->setTouchEnabled(true);

		_mode3Left->setVisible(false);
		_mode3Right->setVisible(false);
		_optionText3Left->setVisible(false);
		_optionText3Right->setVisible(false);

		_mode3Time->setVisible(false);
		_mode3Distance->setVisible(false);
		_optionText3Time->setVisible(false);
		_optionText3Distance->setVisible(false);

		_selectMode = MOVE_AUTO;
		_circleLocationMode = MOVE_CIRCLE_LEFT;
		_circleTypeMode = MOVE_CIRCLE_TIME;

		break;
	}
	case cocos2d::ui::CheckBox::EventType::UNSELECTED:
		break;
	default:
		break;
	}
}

void MoveTypeSelectDialog::checkBox2Callback(Ref* pSender, CheckBox::EventType type)
{
	switch (type)
	{
	case cocos2d::ui::CheckBox::EventType::SELECTED:
	{
		_mode1->setSelected(false);
		_mode1->setTouchEnabled(true);
		_mode2->setSelected(true);
		_mode2->setTouchEnabled(false);
		_mode3->setSelected(false);
		_mode3->setTouchEnabled(true);

		// Neu mode 3 chua duoc lua chon thi se an cac thuoc tinh cua mode3 di
		_mode3Left->setVisible(false);
		_mode3Right->setVisible(false);
		_optionText3Left->setVisible(false);
		_optionText3Right->setVisible(false);

		_mode3Time->setVisible(false);
		_mode3Distance->setVisible(false);
		_optionText3Time->setVisible(false);
		_optionText3Distance->setVisible(false);

		_selectMode = MOVE_MANUAL;
		_circleLocationMode = MOVE_CIRCLE_LEFT;
		_circleTypeMode = MOVE_CIRCLE_TIME;
		break;
	}
	case cocos2d::ui::CheckBox::EventType::UNSELECTED:
		break;
	default:
		break;
	}
}

void MoveTypeSelectDialog::checkBox3Callback(Ref* pSender, CheckBox::EventType type)
{
	switch (type)
	{
	case cocos2d::ui::CheckBox::EventType::SELECTED:
	{
		_mode1->setSelected(false);
		_mode1->setTouchEnabled(true);
		_mode2->setSelected(false);
		_mode2->setTouchEnabled(true);
		_mode3->setSelected(true);
		_mode3->setTouchEnabled(false);

		_mode3Left->setVisible(true);
		_mode3Right->setVisible(true);
		_optionText3Left->setVisible(true);
		_optionText3Right->setVisible(true);

		_mode3Time->setVisible(true);
		_mode3Distance->setVisible(true);
		_optionText3Time->setVisible(true);
		_optionText3Distance->setVisible(true);

		_selectMode = MOVE_CIRCLE;

		switch (_circleLocationMode)
		{
		case MOVE_CIRCLE_LEFT:
			_mode3Left->setSelected(true);
			_mode3Left->setTouchEnabled(false);

			_circleLocationMode = MOVE_CIRCLE_LEFT;

			break;
		case MOVE_CIRCLE_RIGHT:

			_mode3Right->setSelected(true);
			_mode3Right->setTouchEnabled(false);
			_circleLocationMode = MOVE_CIRCLE_RIGHT;

			break;
		default:

			_mode3Left->setSelected(true);
			_mode3Left->setTouchEnabled(false);

			_circleLocationMode = MOVE_CIRCLE_LEFT;

			break;
		}

		switch (_circleTypeMode)
		{
		case MOVE_CIRCLE_TIME:
			_mode3Time->setSelected(true);
			_mode3Time->setTouchEnabled(false);

			_circleTypeMode = MOVE_CIRCLE_TIME;

			break;
		case MOVE_CIRCLE_DISTANCE:
			_mode3Distance->setSelected(true);
			_mode3Distance->setTouchEnabled(false);

			_circleTypeMode = MOVE_CIRCLE_DISTANCE;

			break;
		default:

			_mode3Time->setSelected(true);
			_mode3Time->setTouchEnabled(false);

			_circleTypeMode = MOVE_CIRCLE_TIME;

			break;
		}

		break;
	}
	case cocos2d::ui::CheckBox::EventType::UNSELECTED:
		break;
	default:
		break;
	}
}

void MoveTypeSelectDialog::touchMove3LeftCallback(Ref *pSender, CheckBox::EventType type)
{
	switch (type)
	{
	case cocos2d::ui::CheckBox::EventType::SELECTED:
	{
		_mode1->setSelected(false);
		_mode1->setTouchEnabled(true);
		_mode2->setSelected(false);
		_mode2->setTouchEnabled(true);
		_mode3->setSelected(true);
		_mode3->setTouchEnabled(false);

		_mode3Left->setSelected(true);
		_mode3Left->setTouchEnabled(false);
		_mode3Right->setSelected(false);
		_mode3Right->setTouchEnabled(true);

		_selectMode = MOVE_CIRCLE;
		_circleLocationMode = MOVE_CIRCLE_LEFT;

		break;

	}case cocos2d::ui::CheckBox::EventType::UNSELECTED:
		break;
	default:
		break;
	}
}

void MoveTypeSelectDialog::touchMove3RightCallback(Ref *pSender, CheckBox::EventType type)
{
	switch (type)
	{
	case cocos2d::ui::CheckBox::EventType::SELECTED:
	{
		_mode1->setSelected(false);
		_mode1->setTouchEnabled(true);
		_mode2->setSelected(false);
		_mode2->setTouchEnabled(true);
		_mode3->setSelected(true);
		_mode3->setTouchEnabled(false);

		_mode3Left->setSelected(false);
		_mode3Left->setTouchEnabled(true);
		_mode3Right->setSelected(true);
		_mode3Right->setTouchEnabled(false);

		_selectMode = MOVE_CIRCLE;
		_circleLocationMode = MOVE_CIRCLE_RIGHT;
		break;

	}case cocos2d::ui::CheckBox::EventType::UNSELECTED:
		break;
	default:
		break;
	}
}

void MoveTypeSelectDialog::touchMove3TimeCallback(Ref *pSender, CheckBox::EventType type)
{
	switch (type)
	{
	case cocos2d::ui::CheckBox::EventType::SELECTED:
	{
		_mode1->setSelected(false);
		_mode1->setTouchEnabled(true);
		_mode2->setSelected(false);
		_mode2->setTouchEnabled(true);
		_mode3->setSelected(true);
		_mode3->setTouchEnabled(false);

		_mode3Time->setSelected(true);
		_mode3Time->setTouchEnabled(false);
		_mode3Distance->setSelected(false);
		_mode3Distance->setTouchEnabled(true);

		_selectMode = MOVE_CIRCLE;
		_circleTypeMode = MOVE_CIRCLE_TIME;

		break;

	}case cocos2d::ui::CheckBox::EventType::UNSELECTED:
		break;
	default:
		break;
	}
}

void MoveTypeSelectDialog::touchMove3DistanceCallback(Ref *pSender, CheckBox::EventType type)
{
	switch (type)
	{
	case cocos2d::ui::CheckBox::EventType::SELECTED:
	{
		_mode1->setSelected(false);
		_mode1->setTouchEnabled(true);
		_mode2->setSelected(false);
		_mode2->setTouchEnabled(true);
		_mode3->setSelected(true);
		_mode3->setTouchEnabled(false);

		_mode3Time->setSelected(false);
		_mode3Time->setTouchEnabled(true);
		_mode3Distance->setSelected(true);
		_mode3Distance->setTouchEnabled(false);

		_selectMode = MOVE_CIRCLE;
		_circleTypeMode = MOVE_CIRCLE_DISTANCE;
		break;

	}case cocos2d::ui::CheckBox::EventType::UNSELECTED:
		break;
	default:
		break;
	}
}

void MoveTypeSelectDialog::decideButtonCallback(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		UserDefault::getInstance()->setIntegerForKey(MOVEKEY, _selectMode);
		if (_selectMode == MOVE_CIRCLE)
		{
			UserDefault::getInstance()->setIntegerForKey(MOVE_CIRCLE_LOCATION, _circleLocationMode);
			UserDefault::getInstance()->setIntegerForKey(MOVE_CIRCLE_TYPE, _circleTypeMode);
		}

		DialogBase::decideButtonCallback(pSender, type);

		break;
	}
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}

}
