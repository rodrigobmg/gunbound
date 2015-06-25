#include "Multi/BattleScene/BattleScene.h"

/*
create() la khi khoi tao cac doi tuong va cap phat bo nho cho chung
*/
Scene* BattleScene::createScene(int selectedUnitId)
{
	auto scene = Scene::createWithPhysics();
	// Set physics world for scene
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
	scene->getPhysicsWorld()->setGravity(Vect::ZERO); // Xet trong luong bang 0


	auto layer = BattleScene::create(selectedUnitId);
	layer->setPhysicsWorldToLayer(scene->getPhysicsWorld());
	scene->addChild(layer);
	return scene;
}


BattleScene* BattleScene::create(int selectedUnitId)
{
	auto layer = new BattleScene();
	if (layer && layer->init(selectedUnitId))
	{
		layer->autorelease();
		return layer;
	}
	CC_SAFE_DELETE(layer);
	return nullptr;
}

void BattleScene::setPhysicsWorldToLayer(PhysicsWorld* myWorld)
{
	_myWorld = myWorld;
}

/*
init() la khi khoi tao trang thai cho nhung doi tuong vua duoc khoi tao tai create
*/
bool BattleScene::init(int selectedUnitId)
{
	if (!LayerBase::init())
	{
		return false;
	}

	//////////////////////////////////////////////////////////////////////////////////
	// Hide cac thanh phan cua LayerBase va thiet lap cac thanh phan moi
	//////////////////////////////////////////////////////////////////////////////////

	_pageTitleSprite->setVisible(false);
	_usernameBG->setVisible(false);
	_backButton->setVisible(false);

	//////////////////////////////////////////////////////////////////////////////////
	// Thiet lap kieu di chuyen cho _mainCharacter
	//////////////////////////////////////////////////////////////////////////////////

	// Khoi tao gia tri moveMode mac dinh la move_auto
	log("Default  moveKey: %d ", UserDefault::getInstance()->getIntegerForKey(MOVEKEY));
	// Neu chua co gia tri thi UserDefautl::getInstance()->getIntergerForKey se duoc mac dinh la 0
	// Nhung sau khi nhan gia tri thi no sex duoc luu vao file xml theo key. 
	// Tru khi duoc thay doi, neu khong no se nhan gia tri do
	if (UserDefault::getInstance()->getIntegerForKey(MOVEKEY) == 0)
	{
		UserDefault::getInstance()->setIntegerForKey(MOVEKEY, MOVE_MANUAL);
		_moveMode = MOVE_MANUAL;
	}
	// Neu no da nhan gia tri tu dialog truyen vao, thi thiet lap lai gia tri cho no
	else
	{
		_moveMode = UserDefault::getInstance()->getIntegerForKey(MOVEKEY);
		if (_moveMode == MOVE_CIRCLE)
		{
			_moveCircleLocation = UserDefault::getInstance()->getIntegerForKey(MOVE_CIRCLE_LOCATION);
			_moveCircleType = UserDefault::getInstance()->getIntegerForKey(MOVE_CIRCLE_TYPE);

			log("Move Mode: %d , Local(4:left , 5:right) : %d , Type(6:Time, 7:Distance): %d ", _moveMode, _moveCircleLocation, _moveCircleType);
		}
	}

	//  Thiet lap hinh anh cho di chuyen manual
	auto action = RotateBy::create(1, 180);

	// Khoi tao doi tuong touchBegin va touchEnd doi voi di chuyen theo type manual
	_touchMoveBeginSprite = Sprite::create("image/screen/battle/ui_move.png");
	_touchMoveBeginSprite->setVisible(false); // Hide neu khong phai type manual
	addChild(_touchMoveBeginSprite , 10);
	_touchMoveBeginSprite->runAction(RepeatForever::create(action));

	_touchMoveEndSprite = Sprite::create("image/screen/battle/ui_move_end.png");
	_touchMoveEndSprite->setVisible(false);
	addChild(_touchMoveEndSprite , 10);
	_touchMoveEndSprite->runAction(RepeatForever::create(action->clone()));

	//////////////////////////////////////////////////////////////////////////////////
	// Xay dung noi dung cua scene
	//////////////////////////////////////////////////////////////////////////////////

	// Create BattleScene content
	createContent();

	// Create debug button
	createDebugContent();


	return true;
}

/*
onEnter() duoc goi khi cac doi tuong duoc dua len man hinh, hoac khi no duoc them vao cac container khac
Ca init() va onEnter() deu co the duoc su dung de khoi tao cac doi tuong nam trong layer
Nhung theo thu tu thuc hien thi init() se duoc thuc hien truoc onEnter
*/
void BattleScene::onEnter()
{
	LayerBase::onEnter();

	// Tinh thoi gian cua he thong - TEST
	time( &timer );
	timeInfo = localtime(&timer);
	log("Thoi gian cua he thong : %s", asctime(timeInfo));

	
	// Cu load 1frame se goi toi ham update 1 lan, nhu vay tgian update duoc goi la 1/60(s)
	this->scheduleUpdate();
	// Cu 1.0s thi se goi lai ham updateTimeView 1 lan
	this->schedule(schedule_selector(BattleScene::updateTimeView), 1.0f );
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Debug Cotnent
//////////////////////////////////////////////////////////////////////////////////////////////

void BattleScene::createDebugContent()
{
	auto physicsBodyDebugBtn = Button::create();
	physicsBodyDebugBtn->loadTextureNormal("test.png");
	physicsBodyDebugBtn->setPosition(Vec2(_visibleSize.width - 50, 70));
	physicsBodyDebugBtn->setTouchEnabled(true);
	physicsBodyDebugBtn->addTouchEventListener(CC_CALLBACK_2(BattleScene::debugPhysicsButtonCallback, this));
	addChild(physicsBodyDebugBtn, 10);

	auto menuDebugBtn = Button::create();
	menuDebugBtn->loadTextureNormal("test.png");
	menuDebugBtn->setPosition(Vec2(_visibleSize.width - 100, 70));
	menuDebugBtn->setTouchEnabled(true);
	menuDebugBtn->addTouchEventListener(CC_CALLBACK_2(BattleScene::menuButtonCallback, this));
	addChild(menuDebugBtn, 10);
}

// Button debug physics callback
void BattleScene::debugPhysicsButtonCallback(Ref* pSender, Widget::TouchEventType type)
{
	log("Debug Physics Callback");
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		// Neu on -> off , off -> on
		if (_myWorld->getDebugDrawMask() == PhysicsWorld::DEBUGDRAW_ALL)
		{
			_myWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
		}
		else
		{
			_myWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
		}
		break;

	}case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

// Chuyen ve trang ModeSelectScene
void BattleScene::menuButtonCallback(Ref* pSender, Widget::TouchEventType type)
{
	log("Debug Menu Callback");
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		Director::getInstance()->replaceScene(ModeSelectScene::createScene());
		break;

	}case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////
// BattleScene Content
//////////////////////////////////////////////////////////////////////////////////////////////

void BattleScene::createContent()
{
	// Create background, su dung _background la grobal
	createBackground();

	// create clock battle
	createClockBattle();

	// create minimap
	createMiniMap();

	createSkillButton();

	// image Path
	_imagePath = "image/unit_new/move/red/";


	////////////////////////////////////////////////////////////////
	// Create main character sprite
	////////////////////////////////////////////////////////////////
	_mainCharacter = Sprite::create("image/unit_new/move/red/unit_00_08_1.png");
	//_mainCharacter->setPosition(Vec2(_visibleSize.width / 2, _visibleSize.height / 2));
	_mainCharacter->setPosition(Vec2(100, 100));
	_mainCharacter->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_mainCharacter->setPhysicsBody(PhysicsBody::createCircle(50 , PhysicsMaterial(1, 0, 1)));
	_mainCharacter->getPhysicsBody()->setRotationEnable(false);
	_mainCharacter->setScale(0.6f);

	_background->addChild(_mainCharacter, 2);

	// create follow background

	auto follow = Follow::create(_mainCharacter);
	follow->setTag(101);
	_background->runAction(follow);

	/////////////////////////////////////////////////////////////////
	// create Menu
	/////////////////////////////////////////////////////////////////

	// create status unit clipping node
	_statusCharacterBar = Sprite::create("image/screen/battle/status_parts.png");
	_statusCharacterBar->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	_statusCharacterBar->setPosition(Vec2(0, _visibleSize.height));
	addChild(_statusCharacterBar);

	_characterMiniIcon = ClippingNode::create();
	_characterMiniIcon->setAlphaThreshold(0); // Quan trong
	_characterMiniIcon->setPosition(Vec2::ZERO);

	/* Mat na cua clipping node , hinh anh se duoc hien thi theo khuon dang cua mask */
	auto mask = Sprite::create("image/screen/battle/icon_slot.png"); 
	mask->setPosition(Vec2(50, _statusCharacterBar->getContentSize().height / 2));
	_characterMiniIcon->setStencil(mask);

	_icon = Sprite::create("image/unit_new/move/red/unit_00_02_1.png"); // Hinh anh muon hien thi
	_icon->setPosition(Vec2(50, _statusCharacterBar->getContentSize().height / 2 - 10));
	_icon->setScale(0.6f);
	_characterMiniIcon->addChild(_icon);
	_statusCharacterBar->addChild(_characterMiniIcon);

	// character status bar
	_hpBar = Slider::create();
	_hpBar->loadBarTexture("image/screen/battle/hp_base.png");
	_hpBar->setPercent(100);
	//_hpBar->loadSlidBallTextureNormal("image/screen/battle/test.png");
	_hpBar->loadProgressBarTexture("image/screen/battle/hp.png");
	_hpBar->setPosition(Vec2(_statusCharacterBar->getContentSize().width / 2 + 25, _visibleSize.height - 22));
	addChild(_hpBar);


	// create menu select move type
	_moveTypeSelectMenu = Button::create();
	_moveTypeSelectMenu->loadTextureNormal("image/screen/battle/menu_btn.png");
	_moveTypeSelectMenu->setTouchEnabled(true);
	_moveTypeSelectMenu->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	_moveTypeSelectMenu->setPosition(Vec2(_moveTypeSelectMenu->getContentSize().width / 2 + 10, _visibleSize.height - _statusCharacterBar->getContentSize().height - 5));
	_moveTypeSelectMenu->addTouchEventListener(CC_CALLBACK_2(BattleScene::moveTypeSelectCallback, this));

	addChild(_moveTypeSelectMenu);


}

//////////////////////////////////////////////////////////////////////////////////////////////////
// BACKGROUND
//////////////////////////////////////////////////////////////////////////////////////////////////

void BattleScene::createBackground()
{
	/////////////////////////////////////////////////////////////////////////////
	// Version 1: Thiet lap background bang 4 background con
	/////////////////////////////////////////////////////////////////////////////

	// create background chinh
	_background = Node::create();
	_background->setPosition(Vec2::ZERO);
	addChild(_background);
	_background->setPhysicsBody(PhysicsBody::createEdgeBox(Size(1, 1), PhysicsMaterial(1, 1, 1)));

	// Xay dung background bang 4 thanh phan chinh
	auto part1BG = createBackgroundPart(Vec2(_visibleSize.width, _visibleSize.height));
	part1BG->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_background->addChild(part1BG);
	auto part2BG = createBackgroundPart(Vec2(_visibleSize.width, _visibleSize.height));
	part2BG->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
	_background->addChild(part2BG);
	auto part3BG = createBackgroundPart(Vec2(_visibleSize.width, _visibleSize.height));
	part3BG->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	_background->addChild(part3BG);
	auto part4BG = createBackgroundPart(Vec2(_visibleSize.width, _visibleSize.height));
	part4BG->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	_background->addChild(part4BG);

	// Create physicsbody for background
	createBackgroundBody();


	/////////////////////////////////////////////////////////////////////////////
	// Version 2: Xay dung background bang tilde map
	/////////////////////////////////////////////////////////////////////////////



}

/* Khoi tao 1 doi tuong Sprite chinh la 1 phan cua background */
Sprite* BattleScene::createBackgroundPart(Vec2 pos)
{
	auto part = Sprite::create("image/screen/battle/bg.png");
	part->setPosition(pos);

	return part;
}

/* Khoi tao phusics body cho background */
void BattleScene::createBackgroundBody()
{
	Size hSize = Size(_visibleSize.width * 2, 100);
	Size vSize = Size(100, _visibleSize.height * 2);


	auto bottomBody = createBodyPart(Vec2(_visibleSize.width, 0), hSize);
	_background->addChild(bottomBody);
	auto topBody = createBodyPart(Vec2(_visibleSize.width, _visibleSize.height * 2), hSize);
	_background->addChild(topBody);
	auto ringhtBody = createBodyPart(Vec2(_visibleSize.width * 2, _visibleSize.height), vSize);
	_background->addChild(ringhtBody);
	auto leftBody = createBodyPart(Vec2(0, _visibleSize.height), vSize);
	_background->addChild(leftBody);

}

/* Khoi tao tung phan cua physics body */
Node* BattleScene::createBodyPart(Vec2 pos, Size bodySize)
{
	auto node = Node::create();

	PhysicsBody* nodeBody = PhysicsBody::createBox(bodySize, PhysicsMaterial(1, 0, 0));
	nodeBody->setGravityEnable(false); // Khong chiu tac dung cua trong luc
	nodeBody->setDynamic(false); // Khong chiu tac dung cua dong luc(ko co ma sat, ko co van toc)
	nodeBody->setContactTestBitmask(0x1);

	node->setPhysicsBody(nodeBody);
	node->setPosition(pos);

	return node;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// CLOCK AND MINIMAP
//////////////////////////////////////////////////////////////////////////////////////////////////

/* Khoi tao dong ho hien thi thoi gian tran dau */
void BattleScene::createClockBattle()
{
	// create clock
	_timeViewSprite = Sprite::create("image/screen/battle/time_parts.png");
	_timeViewSprite->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	_timeViewSprite->setPosition(Vec2(_visibleSize.width - 20, _visibleSize.height - 20));
	addChild(_timeViewSprite);

	_timeViewLabel = Label::create("00:00:00", "fonts/arial.ttf", 25);
	_timeViewLabel->setHorizontalAlignment(TextHAlignment::CENTER);
	_timeViewLabel->setPosition(Vec2(_timeViewSprite->getContentSize().width / 2, _timeViewSprite->getContentSize().height / 2));
	_timeViewSprite->addChild(_timeViewLabel);
}

/* Khoi tao minimap chinh la map battle thu nho */
void BattleScene::createMiniMap()
{
	// create minimap
	_miniMap = Sprite::create("image/screen/battle/map.png");
	_miniMap->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	_miniMap->setPosition(_timeViewSprite->getPosition() - Vec2(0, _timeViewSprite->getContentSize().height + 20));
	addChild(_miniMap);

	// khung do danh dau vi tri cua miniIcon
	_selectRect = Sprite::create("image/screen/battle/map_frame.png");
	_selectRect->setPosition(Vec2(_miniMap->getContentSize().width / 2, _miniMap->getContentSize().height / 2 + _selectRect->getContentSize().height/2 ));
	_miniMap->addChild(_selectRect);

	// create miniIcon hien thi vi tri cua mainCharacter tren miniMap
	_miniIcon = Sprite::create("image/screen/battle/mini_icon.png");
	_miniIcon->setPosition(Vec2(_selectRect->getContentSize().width / 2, _selectRect->getContentSize().height / 2));
	_selectRect->addChild(_miniIcon);
}


//////////////////////////////////////////////////////////////////////////////////////////////
// SKILL BUTTON
//////////////////////////////////////////////////////////////////////////////////////////////

void BattleScene::createSkillButton()
{
	// Tao 4 loai skill trong do co 2 loai unit co san va 2 loai duoc lua chon

	_skillUnitList = SkillDataModel::getInstance()->getDataSkillBuUnitId(1);


}

/////////////////////////////////////////////////////////////////////////////////////////////////
// UPDATE EVENT (su dung update de load theo dinh ky cac thanh phan can update lien tuc)
/////////////////////////////////////////////////////////////////////////////////////////////////

/* Ham update duoc goi tu scheduleUpdate trong onEnter() */
void BattleScene::update(float dt)
{
	updateMiniMap();
}

/* Ham update cu 1s se duoc goi 1 lan, su dung de update time cua he thong */
void BattleScene::updateTimeView(float dt)
{
	time_t currTimer;
	time(&currTimer);

	int seconds = ceil(difftime(currTimer, timer));
	_timeViewLabel->setString(convertToTimeString(seconds));
}

/* Tra ve chuoi string co dang hh::mm::ss de hien thi thoi gian cua he thong bat dau tu luc vao tran */
std::string BattleScene::convertToTimeString(int time)
{
	std::stringstream timeString;
	int hous = time / 3600;
	int minute = time / 60;
	int second = time % 60;

	if (hous >= 10){
		timeString << hous << ":";
	}else{
		timeString << "0" << hous << ":";
	}

	if (minute >= 10){
		timeString << minute << ":";
	}else{
		timeString << "0" << minute << ":";
	}

	if (second >= 10)
	{
		timeString << second;
	}else{
		timeString << "0" << second;
	}

	return timeString.str().c_str();
}

// update vi tri cua _mainCharacter tren miniMap
void BattleScene::updateMiniMap()
{
	if (_mainCharacter == nullptr) return;
	auto mainPos = _mainCharacter->getPosition();

	float posXScaleRate = (float)(_miniMap->getContentSize().width / (_visibleSize.width * 2));
	float posYScaleRate = (float)(_miniMap->getContentSize().height / (_visibleSize.height * 2));

	// Cho rect di chuyen theo _mainCharacter, miniIcon la con cua _selectRect nen se di theo cha no
	_selectRect->setPosition(Vec2(mainPos.x * posXScaleRate, mainPos.y * posYScaleRate));
}


//////////////////////////////////////////////////////////////////////////////////////////////
// TOUCH EVENT
//////////////////////////////////////////////////////////////////////////////////////////////

bool BattleScene::onTouchBegan(Touch* touch, Event* event)
{
	_touchStartPoint = touch->getLocation();
	//if (_moveMode == MOVE_MANUAL)
	//{
		_touchMoveBeginSprite->setPosition(_touchStartPoint);
		_touchMoveBeginSprite->setVisible(true);
	//}

	return true;
}

void BattleScene::onTouchMoved(Touch* touch, Event* event)
{
	

	/* Lay vector tu diem touch den diem move*/
	auto vectorMove = touch->getLocation() - _touchStartPoint;

	// Add 2 vong tron vao che do chuyen dong manual
	//if (_moveMode == MOVE_MANUAL)
	//{
		_touchMoveEndSprite->setVisible(true);
		if (vectorMove.length() < 200) {
			_touchMoveEndSprite->setPosition(touch->getLocation());
		}
		else {
			// Gioi han chi cho endSprite di chuyen cach beginSprite 1 khoang toi da la 200
			_touchMoveEndSprite->setPosition(_touchStartPoint + Vec2(200 * cos(vectorMove.getAngle()), 200 * sin(vectorMove.getAngle())));
		}

		// Neu touch vao trong beginSprite thi se dung lai
		if (vectorMove.length() < _touchMoveBeginSprite->getContentSize().width / 6)
		{
			_mainCharacter->getPhysicsBody()->setVelocity(Vect::ZERO);
			_mainCharacter->stopAllActionsByTag(_currentMoveDirectTag);
			
			return;

		}
	//}



	// Di chuyen theo diem touch move
	_mainCharacter->getPhysicsBody()->setVelocity(Vect(250 * cos(vectorMove.getAngle()), 250 * sin(vectorMove.getAngle())));

	/* Thuc hien tinh direction theo goc vua lay*/
	int direct = getDirectionWithAngle(-(vectorMove.getAngle() * RAD_DEG) + 90);
	_miniIcon->setRotation(-(vectorMove.getAngle() * RAD_DEG) + 90);
	if (direct != 0)
	{
		// Thuc hien xoay _mainCharacter
		actionRotateWithDirectionIndex(direct);
	}
}

void BattleScene::onTouchEnded(Touch* touch, Event* event)
{
	//if (_moveMode == MOVE_MANUAL)
	//{
		_touchMoveBeginSprite->setVisible(false);
		_touchMoveEndSprite->setVisible(false);
	//}
	_mainCharacter->getPhysicsBody()->setVelocity(Vect::ZERO);
	_mainCharacter->stopAllActionsByTag(_currentMoveDirectTag);
}


//////////////////////////////////////////////////////////////////////////////////////////////
//XU LY MOVE & ATTACK ANIMATION
//////////////////////////////////////////////////////////////////////////////////////////////

/* Lay chi so direction tuong ung voi goc */
int BattleScene::getDirectionWithAngle(float angle)
{
	if (getDetectAngleFlg(0, angle)) return 8; // if(angle < 22 && angle > -22) return 8
	if (getDetectAngleFlg(45, angle)) return 9; // if(angle < 45+22 && angle > 45-22) return 9
	if (getDetectAngleFlg(90, angle)) return 6;
	if (getDetectAngleFlg(135, angle)) return 3;
	if (getDetectAngleFlg(180, angle)) return 2;
	if (getDetectAngleFlg(-45, angle)) return 7;
	if (getDetectAngleFlg(-90, angle)) return 4;
	if (getDetectAngleFlg(255, angle)) return 1;

	return 0;
	
}

/* Flag dung de xac dinh goc */
bool BattleScene::getDetectAngleFlg(int offset, float angle)
{
	if ((angle < (offset + 22)) && (angle > (offset - 22)) )
	{
		return true;
	}

	return false;
}

void BattleScene::actionRotateWithDirectionIndex(int directionIndex)
{
	log("Direct: %d ", directionIndex);
	/* Kiem tra neu van la action cu thi ko stop no lai*/
	if (_mainCharacter->getNumberOfRunningActions() > 0) {
		if (_mainCharacter->getActionByTag(directionIndex) != nullptr) {
			return;
		}
	}
	// Thuc hien stop action cu khi chuyen huong(direct thay doi)
	_mainCharacter->stopAllActionsByTag(_currentMoveDirectTag);
	log("Stop action with tag: %d ", _currentMoveDirectTag);
	// thuc hien animation repeat forever
	
	auto action = Animate::create(createMoveAnimationCharacterWithImage(directionIndex, _imagePath));
	auto repeat = RepeatForever::create(action);
	_currentMoveDirectTag = directionIndex;
	repeat->setTag(directionIndex);

	_mainCharacter->runAction(repeat);

}

// Tao 1 doi tuong animation thuc hien hanh dong di chuyen duoc xay dung bang anh
Animation* BattleScene::createMoveAnimationCharacterWithImage(int imageId, std::string path)
{
	// Tao animation bang sprite sheet
	auto moveAnimation = Animation::create();

	for (int i = 1 ; i < 3 ; i++)
	{
		char szName[100] = { 0 };
		sprintf(szName, "unit_00_0%d_%d.png", imageId, i);
		std::string p = path;
		p.append(szName);
		moveAnimation->addSpriteFrameWithFile(p.c_str());
	}

	// Thoi gian animation thuc hien
	moveAnimation->setDelayPerUnit(0.2f);
	// Restore lai diem dat cua cac frame moi khi animate thuc hien
	moveAnimation->setRestoreOriginalFrame(true);
	// Thuc hien lap
	moveAnimation->setLoops(true);

	return moveAnimation;

}

// create attack animation object using sprite sheet
Animation* BattleScene::createAttackAnimationCharacter(int imageId, std::string path)
{
	auto attackAnimation = Animation::create();
	for (int i = 0; i < 3 ; i++)
	{
		char szName[100] = { 0 };
		sprintf(szName, "unit_00_0%d_attack_%d.png", imageId, i);
		string p = path;
		p.append(szName);
		attackAnimation->addSpriteFrameWithFile(p.c_str());
	}

	attackAnimation->setDelayPerUnit(0.25f);
	attackAnimation->setRestoreOriginalFrame(true);
	attackAnimation->setLoops(true);

	return attackAnimation;
}


//////////////////////////////////////////////////////////////////////////////////////////////
// Dialog select move type: button callback , decide callback and cancel callback
//////////////////////////////////////////////////////////////////////////////////////////////

void BattleScene::moveTypeSelectCallback(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		auto dialog = MoveTypeSelectDialog::create(CC_CALLBACK_2(BattleScene::moveTypeSelectDecideCallback, this), CC_CALLBACK_2(BattleScene::moveTypeSelectCancelCallback, this));
		addChild(dialog, 10);
		break;
	}
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void BattleScene::moveTypeSelectDecideCallback(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		// Thuc hien lay nhung gia tri moveKey da duoc xet trong dialog
		_moveMode = UserDefault::getInstance()->getIntegerForKey(MOVEKEY);
		log("Move Key(1-Auto , 2-Manual ,3-Circle): %d ", _moveMode);
		if (_moveMode == MOVE_CIRCLE)
		{
			_moveCircleLocation = UserDefault::getInstance()->getIntegerForKey(MOVE_CIRCLE_LOCATION);
			log("Circle location: %d(4-Left , 5-Right) ", _moveCircleLocation);
			_moveCircleType = UserDefault::getInstance()->getIntegerForKey(MOVE_CIRCLE_TYPE);
			log("Circle location: %d(6-Time , 7-Distance) ", _moveCircleType);
		}
		break;
	}
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void BattleScene::moveTypeSelectCancelCallback(Ref* pSender, Widget::TouchEventType type)
{
	// Khong thuc hien gi ca vi no chi duoc goi khi co su kien click button cancel(tat dialog)
	// Nhung trong moveTypeSelectDialog thi button cancel da duoc visialbe
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}