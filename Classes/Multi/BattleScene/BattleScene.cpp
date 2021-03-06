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

	_selectedCharacterId = selectedUnitId;

	//////////////////////////////////////////////////////////////////////////////////
	// Hide cac thanh phan cua LayerBase va thiet lap cac thanh phan moi
	//////////////////////////////////////////////////////////////////////////////////

	_pageTitleSprite->setVisible(false);
	_usernameBG->setVisible(false);
	_backButton->setVisible(false);

	//////////////////////////////////////////////////////////////////////////////////
	// Lay data tu database
	//////////////////////////////////////////////////////////////////////////////////

	// Skill co san cua unit
	_allSkillUnitInfo = SkillDataModel::getInstance()->getDataSkillByUnitId(_selectedCharacterId);
	// Data cua unit
	_allUnitSelectedInfo = UnitDataModel::getInstance()->getDataUnitById(_selectedCharacterId);
	// Data cua skill duoc unit lua chon
	_allSkillSelectedInfo = SkillDataModel::getInstance()->getAllSkillSelected();

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


			if (_moveCircleLocation != 0)
			{
				if (_checkFirstCreateMiniCircleFlg == true && _miniCircle != nullptr)
				{
					_miniCircle->removeFromParentAndCleanup(true);
					_miniUnit->removeFromParentAndCleanup(true);
					_checkFirstCreateMiniCircleFlg = false;
				}
				
				createMiniCircleAndMiniUnit(_moveCircleLocation);
			}

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
// BattleScene Content
//////////////////////////////////////////////////////////////////////////////////////////////

void BattleScene::createContent()
{
	// Create background
	//createBackground();

	// create map with tiledMap
	createBackgroundWithTiledMap();

	/* Neu su dung background thi nhung thiet lap vat ly tai background se khong su dung
	duoc khi su dung no tai battlescene*/
	//_background = Background::createBackground();
	//addChild(_background);

	// create clock battle
	createClockBattle();

	// create minimap
	createMiniMap();

	// create skill button
	createSkillButton();

	
	////////////////////////////////////////////////////////////////
	// Create main character sprite
	////////////////////////////////////////////////////////////////
	_mainCharacter = Character::createCharacterWithId(_selectedCharacterId);
	_mainCharacter->setPosition(Vec2(_myMapSize.width/2 , 100));
	_mainCharacter->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_mainCharacter->setPhysicsBody(PhysicsBody::createCircle(50, PhysicsMaterial(1, 0, 1)));
	_mainCharacter->getPhysicsBody()->setRotationEnable(false);
	_mainCharacter->setScale(0.6f);

	_background->addChild(_mainCharacter, 2);


	/////////////////////////////////////////////////////////////////
	// Background follow character
	/////////////////////////////////////////////////////////////////
	// create follow background

	auto follow = Follow::create(_mainCharacter);
	follow->setTag(101);
	_background->runAction(follow);


	/////////////////////////////////////////////////////////////////
	// create Menu
	/////////////////////////////////////////////////////////////////

	createMenu();

	/////////////////////////////////////////////////////////////////
	// create tree and rock
	/////////////////////////////////////////////////////////////////

	//createRandomRock();
	//createRandomTree();

	/////////////////////////////////////////////////////////////////
	// Tao chuong ngai vat
	/////////////////////////////////////////////////////////////////

	createHurdle();

	/////////////////////////////////////////////////////////////////
	// Tao dich chuyen tuc thoi
	/////////////////////////////////////////////////////////////////
	createWormHole();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// MENU
//////////////////////////////////////////////////////////////////////////////////////////////////

/* Khoi tao thanh MenuBar o goc trai man hinh */
void BattleScene::createMenu()
{
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

/* Khoi tao background bang titledMap */
void BattleScene::createBackgroundWithTiledMap()
{

	/////////////////////////////////////////////////////////////////////////////
	// Version 2: Xay dung background bang tilde map
	/////////////////////////////////////////////////////////////////////////////
	
	// create background parent
	_background = Node::create();
	_background->setPosition(Vec2(2100 , 1000));
	addChild(_background);
	_background->setPhysicsBody(PhysicsBody::createEdgeBox(Size(1, 1), PhysicsMaterial(1 , 1 , 1)));

	// create map with tiledmap
	string filePath = FileUtils::getInstance()->fullPathForFilename("map/map.tmx");
	_myMap = TMXTiledMap::create(filePath.c_str());

	_background->addChild(_myMap);

	// Lay kich thuoc cua tiledMap

	_myMapSize = _myMap->getContentSize();
	log("Map Size( %f , %f )", _myMapSize.width, _myMapSize.height);

	for (const auto& child : _myMap->getChildren())
	{
		static_cast<SpriteBatchNode*>(child)->getTexture()->setAntiAliasTexParameters();
	}

	// Map chinh
	_mapLayer = _myMap->getLayer("main_layer");

	// Su dung de tao body cho map
	_blockLayer = _myMap->getLayer("block_layer");

	_blockLayer->setVisible(false);

	log("Map BoundingBox (%f , %f)", _myMap->getBoundingBox().size.width, _myMap->getBoundingBox().size.height);

	createMapBorder();

}

/* Tao borderPhysics cho map */
void BattleScene::createMapBorder()
{
	Size blockSize = _blockLayer->getLayerSize();
	for (int i = 0; i < blockSize.width ; i++)
	{
		for (int j = 0; j < blockSize.height ; j++)
		{
			auto tiled = _blockLayer->getTileAt(Vec2(i, j));
			
			if (tiled != nullptr)
			{
				auto tiledBody = PhysicsBody::createBox(tiled->getBoundingBox().size, PhysicsMaterial(1, 1, 0));
				tiledBody->setRotationEnable(false);
				tiledBody->setDynamic(false);
				tiledBody->setGravityEnable(false);
				tiledBody->setContactTestBitmask(0x1);
				tiledBody->setCollisionBitmask(0x1);
				tiled->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
				tiled->setPhysicsBody(tiledBody);
				tiled->setTag(102);
			}
		}
	}
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

//////////////////////////////////////////////////////////////////////////////////////////////////
// TAO VUNG XOAY DI CHUYEN TUC THOI
//////////////////////////////////////////////////////////////////////////////////////////////////

/* Tao ho den di chuyen tuc thoi o 4 goc cua map */
void BattleScene::createWormHole()
{
	// Todo
	auto redHole = TextureCache::getInstance()->addImage("image/screen/battle/wormhole_red.png");
	auto blueHole = TextureCache::getInstance()->addImage("image/screen/battle/wormhole_blue.png");

	auto action = RepeatForever::create(RotateBy::create(1, 180));
	/*
	     ____________________________
		 |                           |
	    _|Gate1                 Gate2|_
       |                               |
	   |                               |
	   |_                             _|
	     |Gate0                Gate3 |
	     |___________________________|
	*/
	for (int i = 0; i < 2 ; i++)
	{
		// Create Red WormHole
		// R0(600 , 200) R3(mW - 600 , mH - 200)
		auto parent1 = Node::create();
		parent1->setScaleY(0.5f);
		parent1->setPosition((Vec2(_myMap->getBoundingBox().size) - Vec2(1200, 400)) * i + Vec2(600, 200));
		_background->addChild(parent1);

		auto redHoleSprite = Sprite::createWithTexture(redHole);
		auto action1 = action->clone();
		action1->setTag(WORMHOLE_ROTATE_ACTION);
		redHoleSprite->runAction(action1);
		redHoleSprite->setName("OPENGATE");
		_wormHoleList.push_back(redHoleSprite);
		parent1->addChild(redHoleSprite);

		// B1(600 , mH - 200) B3(mW - 600 , 200) 
		auto parent2 = Node::create();
		parent2->setScaleY(0.5f);
		parent2->setPosition(Vec2( i * (_myMapSize.width - 1200) + 600, ((i+1) % 2) * (_myMapSize.height - 400) + 200));
		_background->addChild(parent2);

		auto blueHoleSprite = Sprite::createWithTexture(blueHole);
		auto action2 = action->clone();
		action2->setTag(WORMHOLE_ROTATE_ACTION);
		blueHoleSprite->runAction(action2);
		blueHoleSprite->setName("OPENGATE");
		_wormHoleList.push_back(blueHoleSprite);
		parent2->addChild(blueHoleSprite);

	}
}

/* Thuc hien viec stop action rotate 
@param inGate int: Cong vao
@param outGate int: Cong ra
*/
void BattleScene::closeWormHole(int inGate, int outGate)
{
	//Todo
	log("CLOSE WORMHOLE GATE");

	_wormHoleList[inGate]->stopActionByTag(WORMHOLE_ROTATE_ACTION);
	_wormHoleList[inGate]->setName("CLOSEGATE");

	_wormHoleList[outGate]->stopActionByTag(WORMHOLE_ROTATE_ACTION);
	_wormHoleList[outGate]->setName("CLOSEGATE");

}

/* Thuc hien action rotate cua wormhole
@param inGate int: Cong vao
@param outGate int: Cong ra
*/
void BattleScene::openWormHole(int inGate , int outGate)
{
	//Todo
	log("OPEN WORMHOLE GATE");

	auto action1 = RepeatForever::create(RotateBy::create(1, 180));
	action1->setTag(WORMHOLE_ROTATE_ACTION);
	_wormHoleList[inGate]->runAction(action1);
	_wormHoleList[inGate]->setName("OPENGATE");

	auto action2 = action1->clone();
	action2->setTag(WORMHOLE_ROTATE_ACTION);
	_wormHoleList[outGate]->runAction(action2);
	_wormHoleList[outGate]->setName("OPENGATE");

}

/* Thuc hien doi cong cho character */
void BattleScene::checkWormHoleWorking()
{
	for (int i = 0; i <  _wormHoleList.size() ; i++)
	{
		Vec2 characterPos = _mainCharacter->getPosition();
		Vec2 wormHolePos = _wormHoleList[i]->getParent()->getPosition();

		if ((wormHolePos - characterPos).length() < 80 && strcmp(_wormHoleList[i]->getName().c_str(), "OPENGATE") == 0)
		{
			log("GATE: %d", i);
			// Neu character di chuyen vao khu vuc wormHole thi se thuc hien viec di chuyen
			int outGate = -1;
			switch (i)
			{
			case 0:
				outGate = 2;
				break;
			case 1:
				outGate = 3;
				break;
			case 2:
				outGate = 0;
				break;
			case 3:
				outGate = 1;
				break;
			default:
				break;
			}

			_mainCharacter->setPosition(_wormHoleList[outGate]->getParent()->getPosition());

			closeWormHole(i, outGate);
			log("OUTGATE: %d ", outGate);
			auto actionOpenGate = Sequence::create(DelayTime::create(20.0f) , CallFuncN::create(CC_CALLBACK_0(BattleScene::openWormHole , this , i , outGate)) , nullptr);
			_wormHoleList[i]->runAction(actionOpenGate);

			return;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
// SKILL BUTTON
//////////////////////////////////////////////////////////////////////////////////////////////

void BattleScene::createSkillButton()
{
	// Tao 4 loai skill trong do co 2 loai unit co san va 2 loai duoc lua chon

	string skill1ImagePath = _allSkillUnitInfo[0].imagePath;
	string skill2ImagePath = _allSkillUnitInfo[1].imagePath;

	string skill3ImagePath = _allSkillSelectedInfo[0].imagePath;
	string skill4ImagePath = _allSkillSelectedInfo[1].imagePath;

	_skill1UnitBtn = Button::create();
	_skill1UnitBtn->setTag(SKILL_TAG_1);
	_skill1UnitBtn->loadTextureNormal(skill1ImagePath.c_str());
	_skill1UnitBtn->setPosition(Vec2(_visibleSize.width / 2 - 180 , _skill1UnitBtn->getContentSize().height/2 + 10));
	_skill1UnitBtn->addTouchEventListener(CC_CALLBACK_2(BattleScene::skillButtonCallback, this));
	addChild(_skill1UnitBtn);

	_skill2UnitBtn = Button::create();
	_skill2UnitBtn->setTag(SKILL_TAG_2);
	_skill2UnitBtn->loadTextureNormal(skill2ImagePath.c_str());
	_skill2UnitBtn->setPosition(Vec2(_visibleSize.width / 2 - 60, _skill2UnitBtn->getContentSize().height / 2 + 10));
	_skill2UnitBtn->addTouchEventListener(CC_CALLBACK_2(BattleScene::skillButtonCallback, this));
	addChild(_skill2UnitBtn);

	_skill3SelectedBtn = Button::create();
	_skill3SelectedBtn->setTag(SKILL_TAG_3);
	_skill3SelectedBtn->loadTextureNormal(skill3ImagePath.c_str());
	_skill3SelectedBtn->setPosition(Vec2(_visibleSize.width / 2 + 60, _skill3SelectedBtn->getContentSize().height / 2 + 10));
	_skill3SelectedBtn->addTouchEventListener(CC_CALLBACK_2(BattleScene::skillButtonCallback, this));
	addChild(_skill3SelectedBtn);

	_skill4SelectedBtn = Button::create();
	_skill4SelectedBtn->setTag(SKILL_TAG_4);
	_skill4SelectedBtn->loadTextureNormal(skill4ImagePath.c_str());
	_skill4SelectedBtn->addTouchEventListener(CC_CALLBACK_2(BattleScene::skillButtonCallback, this));
	_skill4SelectedBtn->setPosition(Vec2(_visibleSize.width / 2 + 180, _skill4SelectedBtn->getContentSize().height / 2 + 10));
	addChild(_skill4SelectedBtn);


}

void BattleScene::skillButtonCallback(Ref* pSender, Widget::TouchEventType type)
{
	Button* button = dynamic_cast<Button*>(pSender);
	int buttonTag = button->getTag();
	log("Button %d ", buttonTag);

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

//////////////////////////////////////////////////////////////////////////////////////////////////
// CREATE ENERMY
//////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////////
// CREATE RANDOM ROCK AND TREE
//////////////////////////////////////////////////////////////////////////////////////////////////

void BattleScene::createTower()
{
	// Todo
}

/* Tao vat can tren duong */
void BattleScene::createHurdle()
{
	// Wall ngang texture
	auto wallHorizontal = TextureCache::getInstance()->addImage("map/wall_horizontal.png");
	// Wall doc texture
	auto wallVertical = TextureCache::getInstance()->addImage("map/wall_vertical.png");

	auto wall1 = createHurdlePart(wallHorizontal, Vec2(_myMapSize.width /2 , _myMapSize.height * 2 / 3));
	wall1->setScaleX((_myMapSize.width * 3 / 5) / wall1->getContentSize().width);

	auto wall2 = createHurdlePart(wallHorizontal, Vec2(_myMapSize.width / 2, _myMapSize.height * 1 / 3));
	wall2->setScaleX((_myMapSize.width * 3 / 5) / wall1->getContentSize().width);

	auto wall3 = createHurdlePart(wallVertical, Vec2(_myMapSize.width / 5, _myMapSize.height / 3));
	wall3->setScaleY(1.5f);

	auto wall4 = createHurdlePart(wallVertical, Vec2(_myMapSize.width * 4 / 5, _myMapSize.height * 2 / 3));
	wall4->setScaleY(1.5f);

	auto wall5 = createHurdlePart(wallVertical, Vec2(_myMapSize.width * 2 / 5, _myMapSize.height * 5 / 9));
	auto wall6 = createHurdlePart(wallVertical, Vec2(_myMapSize.width * 3 / 5, _myMapSize.height * 4 / 9));

	auto wall7 = createHurdlePart(wallVertical, Vec2(_myMapSize.width * 1 / 5, _myMapSize.height - wall6->getContentSize().height / 2));
	wall7->setScaleY(0.8f);

	auto wall8 = createHurdlePart(wallVertical, Vec2(_myMapSize.width * 4 / 5, wall7->getContentSize().height / 2));
	wall8->setScaleY(0.8f);
}


Sprite* BattleScene::createHurdlePart(Texture2D* wallTexture, Vec2 pos)
{
	auto wall = Sprite::createWithTexture(wallTexture);
	auto wallBody = PhysicsBody::createBox(wall->getContentSize(), PhysicsMaterial(1, 1, 0));
	wallBody->setDynamic(false);
	wallBody->setCollisionBitmask(0x1);
	wallBody->setContactTestBitmask(0x1);
	wall->setPhysicsBody(wallBody);
	wall->setPosition(pos);
	_background->addChild(wall);

	return wall;

}

/////////////////////////////////////////////////////////////////////////////////////////////////
// UPDATE EVENT (su dung update de load theo dinh ky cac thanh phan can update lien tuc)
/////////////////////////////////////////////////////////////////////////////////////////////////

/* Ham update duoc goi tu scheduleUpdate trong onEnter() */
void BattleScene::update(float dt)
{
	updateMiniMap();

	checkWormHoleWorking();
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

	float posXScaleRate = (float)(_miniMap->getContentSize().width / _myMapSize.width);
	float posYScaleRate = (float)(_miniMap->getContentSize().height / _myMapSize.height);

	// Cho rect di chuyen theo _mainCharacter, miniIcon la con cua _selectRect nen se di theo cha no
	_selectRect->setPosition(Vec2(mainPos.x * posXScaleRate, mainPos.y * posYScaleRate));
}


//////////////////////////////////////////////////////////////////////////////////////////////
// TOUCH EVENT
//////////////////////////////////////////////////////////////////////////////////////////////

bool BattleScene::onTouchBegan(Touch* touch, Event* event)
{
	_touchStartPoint = touch->getLocation();

	if (_moveMode == MOVE_MANUAL)
	{
		_touchMoveBeginSprite->setPosition(_touchStartPoint);
		_touchMoveBeginSprite->setVisible(true);
	}

	if (_moveMode == MOVE_CIRCLE)
	{
		log("TH3 : Move Circle");
		 // Kiem tra truong hop touch ngoai vong tron thi khong thuc hien
		if (_miniCircle->isTapped(touch->getLocation()))
		{
			_touchMoveBeginSprite->setPosition(_miniCircle->getPosition());
			_touchMoveBeginSprite->setVisible(true);

			_touchMoveEndSprite->setPosition(touch->getLocation());
			_touchMoveEndSprite->setVisible(true);
		}
		else
		{
			// Neu truong hop touch circle khong duoc thuc hien trong vong tron thi se ko thuc hien duoc
			log("++++++++++++++++++++++++");
			return false;
		}

		// Thuc hien touch long_move
		// Su dung lambda cho callfunc, doi tuong su dung callfunc trong truong hop nay la _mainCharacter
		// [&] la cho phep doi tuong goi ham duoc su dung chinh ban than no trong ham
		// [=] cho phep doi tuong su dung ban sao cua no
		auto longTapMoveAction = Sequence::create(DelayTime::create(0.15f), CallFuncN::create([& , touch](Ref* pSender){
			// Ref* pSender se duoc su dung dynamic_cast
			// Touch duoc truyen them vao de lay vi tri touch tren miniCircle
			//auto mainObj = dynamic_cast<Character*>(pSender);
			_mainCharacter->setMoveMode(4);
			auto moveVector = touch->getLocation() - _miniCircle->getPosition();

			_mainCharacter->setCharacterMoveSpeed(250);
			_mainCharacter->actionMoveByVector(moveVector);

			_checkOneTapMoveFlg = true;
			_checkLongTapMoveFlg = true;

			_miniIcon->setRotation(-(moveVector.getAngle() * RAD_DEG) + 90);

			int direct = _mainCharacter->getDirectionWithAngle(-(moveVector.getAngle() * RAD_DEG) + 90);

			if (direct != 0)
			{
				_miniUnit->actionRotateWithDirection(direct);
			}

		}) , nullptr);

		longTapMoveAction->setTag(LONGTAP_MOVE_ACTION);
		_mainCharacter->runAction(longTapMoveAction);

	}

	return true;
}

void BattleScene::onTouchMoved(Touch* touch, Event* event)
{
	

	/* Lay vector tu diem touch den diem move*/
	Vec2 vectorMove;
	if (_moveMode == MOVE_MANUAL)
	{
		vectorMove = touch->getLocation() - _touchStartPoint;
	}
	if (_moveMode == MOVE_CIRCLE)
	{
		vectorMove = touch->getLocation() - _miniCircle->getPosition();
	}


	// Xu ly truong hop ngon tay khi cham,se chuyen touchBegin thanh touchMove
	// Test tren Win32 thi se khong gap truong hop nay neu touchOne , nhung neu test tren tanmatsu thi se nhay vao day
	// Nen can check do rong cua ngon tay touc
	if ((touch->getLocation() - _touchStartPoint).length() < 20) {
		_checkOneTapMoveFlg = false;
		return;
	}
	
	/////////////////////////////////////////////////////////////////////////
	// Xu ly hien thi vong tron di chuyen cho cac truong hop touch and move
	/////////////////////////////////////////////////////////////////////////
	if (_moveMode == MOVE_MANUAL) {
		_touchMoveEndSprite->setVisible(true);
		if (vectorMove.length() < 200) {
			_touchMoveEndSprite->setPosition(touch->getLocation());
		}
		else {
			_touchMoveEndSprite->setPosition(_touchStartPoint + Vec2(200 * cos(vectorMove.getAngle()), 200 * sin(vectorMove.getAngle())));
		}
	}

	if (_moveMode == MOVE_CIRCLE) {
		_touchMoveEndSprite->setVisible(true);

		if (_miniCircle->isTapped(touch->getLocation())) {
			_touchMoveEndSprite->setPosition(touch->getLocation());
		}
		else {
			_touchMoveEndSprite->setPosition(_miniCircle->getPosition() + Vec2(_miniCircle->getMiniCircleAfterScaleX() / 2 * cos(vectorMove.getAngle()), _miniCircle->getMiniCircleAfterScaleY() / 2 * sin(vectorMove.getAngle())));
		}
	}


	/////////////////////////////////////////////////////////////////////////
	// Xu ly logic di chuyen cho cac truong hop touch and move
	/////////////////////////////////////////////////////////////////////////

	// Di vao ben trong vong tron thi se ko di chuyen tiep,su dung cho ca move_manual && move_circle_manual
	if (vectorMove.length() < _touchMoveBeginSprite->getContentSize().width / 6 && _moveMode != MOVE_CIRCLE) {
		_mainCharacter->actionStopMove();
		return;
	}

	/////////////////MOVE WITH TESTOBJECT
	_mainCharacter->stopAllActionsByTag(LONGTAP_MOVE_ACTION);
	if (_moveMode == MOVE_MANUAL || _moveMode == MOVE_CIRCLE) {
		_checkOneTapMoveFlg = true;
		
		_mainCharacter->setMoveMode(_moveMode);
		
		// Di chuyen theo diem touch move
		_mainCharacter->setCharacterMoveSpeed(250);
		_mainCharacter->actionMoveByVector(vectorMove);

		int direct = _mainCharacter->getDirectionWithAngle(-(vectorMove.getAngle() * RAD_DEG) + 90);
		_miniIcon->setRotation(-(vectorMove.getAngle() * RAD_DEG) + 90);
		if (direct != 0)
		{
			if (_moveMode == MOVE_CIRCLE)
			{
				_miniUnit->actionRotateWithDirection(direct); // Xoay miniUnit
			}
		}
	}
	
}

void BattleScene::onTouchEnded(Touch* touch, Event* event)
{
	_touchMoveBeginSprite->setVisible(false);
	_touchMoveEndSprite->setVisible(false);

	/////////////////////////////////////////////////////////////////////////
	// LOGIC
	/////////////////////////////////////////////////////////////////////////
	_mainCharacter->stopAllActionsByTag(LONGTAP_MOVE_ACTION);
	if ((_moveMode == MOVE_MANUAL) || (_moveMode == MOVE_CIRCLE && _checkOneTapMoveFlg == true) || (_moveMode == MOVE_CIRCLE && _checkLongTapMoveFlg == true))
	{
		/* Day la truong hop MOVE_MANUAL || MOVE_CIRCLE_MANUAL || MOVE_CIRCLE_LONGTAP */
		_mainCharacter->actionStopMove();
		if (_moveMode == MOVE_CIRCLE)
		{
			_miniUnit->actionStopMove();

		}

		_checkOneTapMoveFlg = false;
		_checkLongTapMoveFlg = false;

		return;
	}
	else
	{
		if (_moveMode == MOVE_AUTO)
		{
			_mainCharacter->setMoveMode(_moveMode);
			Vec2 moveVector = touch->getLocation() - Vec2(_visibleSize / 2);

			_mainCharacter->actionStopMove();
			Vect force = Vect(250 * cos(moveVector.getAngle()), 250 * sin(moveVector.getAngle()));
			_mainCharacter->getPhysicsBody()->applyImpulse(force * 9000);

		}

		
		if (_moveMode == MOVE_CIRCLE)
		{
			if (_miniCircle->isTapped(touch->getLocation()))
			{

				log("Day la oneTap circle");

				_mainCharacter->setMoveMode(5);
				_miniUnit->setMoveMode(5);

				_mainCharacter->setCharacterMoveSpeed(250);

				float moveTime;
				// Moi lan Tap di chuyen trong 1s
				if (_moveCircleType == MOVE_CIRCLE_TIME)
				{
					moveTime = 1;
				}
				// Moi lan tap di chuyen duoc 1 doan distance
				if (_moveCircleType == MOVE_CIRCLE_DISTANCE)
				{
					moveTime = (float)(MOVE_DISTANCE /_mainCharacter->getCharacterMoveSpeed());
				}

				_mainCharacter->setMoveOneTapTime(moveTime);
				_miniUnit->setMoveOneTapTime(moveTime);
				Vec2 space = (touch->getLocation() - _miniCircle->getPosition());

				_miniIcon->setRotation(-(space.getAngle() * RAD_DEG) + 90);

				_mainCharacter->actionMoveByVector(space);
				_miniUnit->actionMoveByVector(space);

			}
			else
			{
				return;
			}
		}
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////
// COLIISITION EVENT
//////////////////////////////////////////////////////////////////////////////////////////////
bool BattleScene::onContactBegin(PhysicsContact &contact)
{
	PhysicsBody* bodyA = contact.getShapeA()->getBody();
	PhysicsBody* bodyB = contact.getShapeB()->getBody();

	return true;
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

			if (_moveCircleLocation != 0)
			{
				if (_checkFirstCreateMiniCircleFlg == true && _miniCircle != nullptr)
				{
					_miniCircle->removeFromParentAndCleanup(true);
					_miniUnit->removeFromParentAndCleanup(true);
					_checkFirstCreateMiniCircleFlg = false;
				}
				
				createMiniCircleAndMiniUnit(_moveCircleLocation);
			}
		}
		else
		{
			if (_checkFirstCreateMiniCircleFlg == true && _miniCircle != nullptr)
			{
				_miniCircle->removeFromParentAndCleanup(true);
				_miniUnit->removeFromParentAndCleanup(true);
				_checkFirstCreateMiniCircleFlg = false;
			}
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

//////////////////////////////////////////////////////////////////////////////////////////////
// CREATE MINI_CIRCLE AND MINI_UNIT
//////////////////////////////////////////////////////////////////////////////////////////////

void BattleScene::createMiniCircleAndMiniUnit(int circleLocation)
{
	// Create miniCircle
	_miniCircle = MiniCircle::createMiniCircle(circleLocation);
	addChild(_miniCircle, 100);

	// Su dung flag nay de khong remove miniCircle lan dau tien khoi tao
	_checkFirstCreateMiniCircleFlg = true;

	// Create miniUnit
	_miniUnit = Character::createCharacterWithId(_selectedCharacterId);
	_miniUnit->setScale(0.25f);
	_miniUnit->setPosition(_miniCircle->getPosition());
	addChild(_miniUnit , 100);
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



/////////////////////////////////////////////////////////////////////////////////////////////////////
// CODE OLD VERSION
////////////////////////////////////////////////////////////////////////////////////////////////////


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


/*
Tao va xu ly body phuc tap:
http://laptrinhgamecocos2dx.blogspot.jp/2014/05/bai-13-tao-khung-vat-ly-cua-doi-tuong-phuc-tap-bang-phan-mem-physicbody-editor.html
*/

void BattleScene::createRandomRock()
{
	Texture2D* textureRock = Director::getInstance()->getTextureCache()->addImage("map/stone.png");

	for (int i = 0; i < 5; i++)
	{
		auto rock = Sprite::createWithTexture(textureRock);
		MyBodyParser::getInstance()->parseJsonFile("json/stone.json");
		auto rockBody = MyBodyParser::getInstance()->bodyFormJson(rock, "stone");

		if (rockBody != nullptr)
		{
			rockBody->setDynamic(false);
			rockBody->setGravityEnable(false);
			rockBody->setContactTestBitmask(0x1);
			rock->setPhysicsBody(rockBody);

			rock->setTag(ROCK_TAG);
			rock->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			rock->setPosition(Vec2(random(0.1f, 0.9f) * _visibleSize.width * 2, random(0.1f, 0.9f) * _visibleSize.height * 2));

			_allRock.push_back(rock);
			_background->addChild(_allRock.back(), 2);
		}

	}
}

void BattleScene::createRandomTree()
{
	Texture2D* textureTree = Director::getInstance()->getTextureCache()->addImage("map/tree.png");

	for (int i = 0; i < 5; i++)
	{
		auto tree = Sprite::createWithTexture(textureTree);
		MyBodyParser::getInstance()->parseJsonFile("json/tree.json");
		auto treeBody = MyBodyParser::getInstance()->bodyFormJson(tree, "tree");

		if (treeBody != nullptr)
		{
			treeBody->setDynamic(false);
			treeBody->setGravityEnable(false);
			treeBody->setContactTestBitmask(0x1);
			tree->setPhysicsBody(treeBody);

			tree->setTag(TREE_TAG);
			tree->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			tree->setPosition(Vec2(random(0.1f, 0.9f) * _visibleSize.width * 2, random(0.1f, 0.9f) * _visibleSize.height * 2));

			_allTree.push_back(tree);
			_background->addChild(_allTree.back(), 2);
		}

	}
}