#pragma execution_character_set("utf-8")
#include "SkillSelectScene.h"


Scene* SkillSelectScene::createScene(int unitId)
{
	auto scene = Scene::create();
	auto layer = SkillSelectScene::create(unitId);

	scene->addChild(layer);

	return scene;
}

SkillSelectScene* SkillSelectScene::create(int unitId){
	auto layer = new (std::nothrow) SkillSelectScene();
	if (layer && layer->init(unitId))
	{
		layer->autorelease();
		return layer;
	}
	CC_SAFE_DELETE(layer);

	return nullptr;
}

bool SkillSelectScene::init(int unitId)
{
	if (!LayerBase::init()){
		return false;
	}

	_selectedUnitId = unitId;

	_titleLabel->setString("Select Skill");

	// get data from database
	_allSkillInfo = SkillDataModel::getInstance()->getDataSkillFromDatabase();


	// Show list skill
	createAllSkillView();

	// Show selected skill
	createSkillDisplay();

	// next button
	auto nextButton = Button::create();
	nextButton->loadTextureNormal("image/button/new/button_decide.png");
	nextButton->setPosition(Vec2(_visibleSize.width - nextButton->getContentSize().width / 2, _visibleSize.height - 100));
	nextButton->setTouchEnabled(true);
	nextButton->addTouchEventListener(CC_CALLBACK_2(SkillSelectScene::nextButtonCallback, this));
	addChild(nextButton);

	// back button add event
	_backButton->addTouchEventListener(CC_CALLBACK_2(SkillSelectScene::onBackButtonCallback, this));

	return true;
}

void SkillSelectScene::onEnter(){
	LayerBase::onEnter();

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(SkillSelectScene::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(SkillSelectScene::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(SkillSelectScene::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	touchListener->setSwallowTouches(false);

}

/////////////////////////////////////////////////////////////////////////////
// Show all skill using pageview
/////////////////////////////////////////////////////////////////////////////

/*
Tao pageView va hien thi unit
*/
void SkillSelectScene::createAllSkillView()
{
	//createSkillInfoData();

	//Add Box Sprite
	auto boxSprite = Sprite::create("image/screen/unitSelect/back.png");
	boxSprite->setPosition(Vec2(_origin.x + _visibleSize.width / 2, _origin.y + _visibleSize.height / 2 - _pageTitleSprite->getContentSize().height / 2 - 70));
	addChild(boxSprite);

	// Add left arrow 
	_leftArrow = Button::create();
	_leftArrow->loadTextureNormal("image/screen/unitSelect/left.png");
	_leftArrow->setSwallowTouches(true);
	_leftArrow->setVisible(false);
	_leftArrow->setPosition(Vec2(50 + _leftArrow->getContentSize().width / 2, boxSprite->getContentSize().height / 2));
	_leftArrow->addTouchEventListener(CC_CALLBACK_2(SkillSelectScene::leftArrowCallback, this));
	boxSprite->addChild(_leftArrow);

	//Add right arrow
	_rightArrow = Button::create();
	_rightArrow->loadTextureNormal("image/screen/unitSelect/right.png");
	_rightArrow->setSwallowTouches(true);
	_rightArrow->setVisible(true);
	_rightArrow->setPosition(Vec2(boxSprite->getContentSize().width - 50 - _rightArrow->getContentSize().width / 2, boxSprite->getContentSize().height / 2));
	_rightArrow->addTouchEventListener(CC_CALLBACK_2(SkillSelectScene::rightArrowCallback, this));
	boxSprite->addChild(_rightArrow);

	// set default value
	Size boxSize = boxSprite->getContentSize(); // Kich thuoc cua box
	float space = boxSize.width - 150; // Do rong cua pagaview


	// Set page view
	_mainPage = PageView::create();
	_mainPage->setContentSize(Size(space, boxSize.height - 40));
	_mainPage->setPosition(Vec2(75, 0));

	// Xac dinh pageNum theo so luong unit
	_pageNum = (_allSkillInfo.size()) / 4;
	if (_pageNum * 4 < _allSkillInfo.size()){
		_pageNum += 1;
	}

	// space la do rong cua pageview
	float baseX = space * 1 / 8;
	float spaceX = space * 1 / 4;
	for (int i = 0; i < _pageNum; i++)
	{
		// 1 layout dai dien cho 1 page cua page view
		Layout* hBox = Layout::create();
		hBox->setContentSize(Size(boxSize.width - 150, boxSize.height - 40));
		hBox->setSwallowTouches(false);

		// Su dung layout de thiet lap vi tri cho item unit
		auto lay = Layer::create();
		lay->setContentSize(hBox->getContentSize());
		lay->setSwallowsTouches(false);

		hBox->addChild(lay);


		for (int j = 0; j < 4; j++)
		{
			if ((j + i * 4) < _allSkillInfo.size()) {
				// Khoi tao unit image
				auto spriteUnit = Button::create();
				spriteUnit->setTag(_allSkillInfo[j + i * 4].id); // lay tag theo id
				spriteUnit->loadTextureNormal(_allSkillInfo[j + i * 4].imagePath); // load texture
				spriteUnit->setSwallowTouches(false);
				spriteUnit->addTouchEventListener(CC_CALLBACK_2(SkillSelectScene::onTouchSkill, this));
				int yValue = hBox->getContentSize().height / 2 + 20;
				spriteUnit->setPosition(Vec2(baseX + spaceX *j, yValue));
				lay->addChild(spriteUnit);
			}
		}
		_mainPage->insertPage(hBox, i); // insert cac lop theo so luong pageNum
		_mainPage->setSwallowTouches(false);
	}

	// Event bat su kien pageview thay doi
	_mainPage->addEventListener(CC_CALLBACK_2(SkillSelectScene::pageViewEvent, this));
	boxSprite->addChild(_mainPage);
}

/*
Event duoc thuc thi khi pageview scroll
*/
void SkillSelectScene::pageViewEvent(Ref* pSender, PageView::EventType type)
{
	switch (type)
	{
	case cocos2d::ui::PageView::EventType::TURNING:
	{
		PageView* pageView = dynamic_cast<PageView*>(pSender);
		int pageIndex = pageView->getCurPageIndex();

		_leftArrow->setVisible(true);
		_rightArrow->setVisible(true);

		if (pageIndex == 0){
			_leftArrow->setVisible(false);
		}
		if (pageIndex == (_pageNum - 1)){
			_rightArrow->setVisible(false);
		}
		break;
	}
	default:
		break;
	}
}

/*
Xu ly su kien click button left arrow(di chuyen page view ve phia ben trai)
*/
void SkillSelectScene::leftArrowCallback(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		//log("Current Page: %d ", _mainPage->getCurPageIndex());
		_mainPage->scrollToPage(_mainPage->getCurPageIndex() - 1);
		break;
	}
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}
/*
Xu ly su kien click button right arrow(di chuyen page view ve phia ben phai)
*/
void SkillSelectScene::rightArrowCallback(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		//log("Current Page: %d ", _mainPage->getCurPageIndex());
		_mainPage->scrollToPage(_mainPage->getCurPageIndex() + 1);
		break;
	}
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}


/*
Su kien click vao skill item thi se hien thi skillDialogDetail
*/
void SkillSelectScene::onTouchSkill(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		Button* skill = dynamic_cast<Button*>(pSender);
		int tag = skill->getTag();
		log("Unit Tag : %d ", tag);

		// Check click vao button arrow
		int currentPageIndex = _mainPage->getCurPageIndex();
		log("Current Page: %d ", currentPageIndex);
		if (tag > (currentPageIndex + 1) * 4) return;
		if (tag == (currentPageIndex * 4)) return;

		// show dialog unit detail
		_onSelectedSkillId = tag;
		auto dialogDetail = SkillDetailDialog::create(_allSkillInfo[tag - 1], CC_CALLBACK_2(SkillSelectScene::decideCallback, this), CC_CALLBACK_2(SkillSelectScene::cancelCallback, this));
		this->getParent()->addChild(dialogDetail);

		break;
	}
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// Show unit selected with Clipping Node
//////////////////////////////////////////////////////////////////////////////
void SkillSelectScene::createSkillDisplay()
{
	// ClippingNode hien thi unit duoc lua chon
	_skillSlotBG1 = createSlotBaseSprite(Vec2(_visibleSize.width / 2 - 80, _visibleSize.height - 150));
	addChild(_skillSlotBG1);
	_skillSlotBG1->loadTextureNormal("image/screen/skillSelect/frame.png"); // default cai mau vang la cai dau tien
	_skillSlotBG1->addChild(createSkillNameBackground(Vec2(_skillSlotBG1->getContentSize().width / 2, -20)));
	_selectedSkillName1 = createSkillNameLabel(Vec2(_skillSlotBG1->getContentSize().width / 2, -20));
	_skillSlotBG1->addChild(_selectedSkillName1);

	_skillSlotBG2 = createSlotBaseSprite(Vec2(_visibleSize.width / 2 + 80, _visibleSize.height - 150));
	addChild(_skillSlotBG2);
	_skillSlotBG2->loadTextureNormal("image/screen/skillSelect/frame_inactive.png");
	_skillSlotBG2->addChild(createSkillNameBackground(Vec2(_skillSlotBG2->getContentSize().width / 2, -20)));
	_selectedSkillName2 = createSkillNameLabel(Vec2(_skillSlotBG2->getContentSize().width / 2, -20));
	_skillSlotBG2->addChild(_selectedSkillName2);

	// khoi tao cac doi tuong clippingnode
	_slot1 = createSlot(Vec2(_visibleSize.width / 2 - 80, _visibleSize.height - 150));
	_slot2 = createSlot(Vec2(_visibleSize.width / 2 + 80, _visibleSize.height - 150));

	addChild(_slot1);
	addChild(_slot2);

	// create select button , sau khi unit duoc lua chon thi se load lai texture theo unit
	_skillTemp1 = Button::create();
	_skillTemp1->loadTextureNormal("image/screen/skillSelect/face_button.png");
	_skillTemp1->setTouchEnabled(true);
	_skillTemp1->addTouchEventListener(CC_CALLBACK_2(SkillSelectScene::onTouchSkillSlot, this, 1));
	_skillTemp1->setPosition(Vec2(_visibleSize.width / 2 - 80, _visibleSize.height - 150));
	_slot1->addChild(_skillTemp1, 5);

	_skillTemp2 = Button::create();
	_skillTemp2->loadTextureNormal("image/screen/skillSelect/face_button.png");
	_skillTemp2->setTouchEnabled(true);
	_skillTemp2->addTouchEventListener(CC_CALLBACK_2(SkillSelectScene::onTouchSkillSlot, this, 2));
	_skillTemp2->setPosition(Vec2(_visibleSize.width / 2 + 80, _visibleSize.height - 150));
	_slot2->addChild(_skillTemp2, 5);
}

Button* SkillSelectScene::createSlotBaseSprite(Vec2 pos){
	auto sprite = Button::create();
	sprite->loadTextureNormal("image/screen/skillSelect/frame_inactive.png");
	sprite->setEnabled(false);
	sprite->setPosition(pos);

	return sprite;
}

Sprite* SkillSelectScene::createSkillNameBackground(Vec2 pos){
	auto unitNameBG = Sprite::create("image/screen/unitSelect/name.png");
	unitNameBG->setPosition(pos);

	return unitNameBG;
}

LabelTTF* SkillSelectScene::createSkillNameLabel(Vec2 pos){
	auto unitNameLabel = LabelTTF::create("", "fonts/arial.ttf", 20);
	unitNameLabel->setPosition(pos);
	unitNameLabel->setHorizontalAlignment(TextHAlignment::CENTER);
	unitNameLabel->setColor(Color3B::BLUE);

	return unitNameLabel;
}

ClippingNode* SkillSelectScene::createSlot(Vec2 pos){
	auto clippingUnit = ClippingNode::create();
	clippingUnit->setAlphaThreshold(0);
	clippingUnit->setPosition(Vec2::ZERO);
	clippingUnit->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);


	auto mask = Sprite::create("image/screen/skillSelect/slot_rect.png");
	mask->setPosition(pos);
	clippingUnit->setStencil(mask);

	return clippingUnit;
}

void SkillSelectScene::onTouchSkillSlot(Ref* pSender, Widget::TouchEventType type, int selectSlot){
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		log("SelectSlot: %d ", selectSlot);
		setSelectSlot(selectSlot); // Chuyen button duoc chon thanh mau vang
		_onSelectedSlot = selectSlot;
		break;
	}
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

/*
Chuyen mau button duoc lua chon thanh vang
*/
void SkillSelectScene::setSelectSlot(int selectedSlot){
	switch (selectedSlot)
	{
	case 1:
		_skillSlotBG1->loadTextureNormal("image/screen/skillSelect/frame.png");
		_skillSlotBG2->loadTextureNormal("image/screen/skillSelect/frame_inactive.png");
		break;
	case 2:
		_skillSlotBG1->loadTextureNormal("image/screen/skillSelect/frame_inactive.png");
		_skillSlotBG2->loadTextureNormal("image/screen/skillSelect/frame.png");
		break;
	default:
		break;
	}
}

void SkillSelectScene::onSelectSkill(int selectedSkillId){
	switch (_onSelectedSlot)
	{
	case 1:
		showSkillAfterCloseDialog(_skillTemp1 , _selectedSkillName1, selectedSkillId);
		break;
	case 2:
		showSkillAfterCloseDialog(_skillTemp2 , _selectedSkillName2, selectedSkillId);
		break;
	default:
		break;
	}
}

/*
* Ham show unit sau khi dong dialog
* @param Button* parent: Button hien thi unit (_unitTemp1, _unitTemp2 , _unitTemp3)
* @param LabelTTF* unitNameLabel: label hien thi ten cua unit duoc lua chon
* @param int unitId : id cua unit duoc lua chon
*/
void SkillSelectScene::showSkillAfterCloseDialog(Button* parent, LabelTTF* skillNameLabel, int skillId){
	// Load lai texture va labelName voi unit duoc lua chon
	parent->loadTextureNormal(_allSkillInfo[skillId - 1].imagePath);
	skillNameLabel->setString(_allSkillInfo[skillId - 1].name);

	// Tu dong chuyen slot lua chon len button tiep theo
	log("_onSelectSlot : %d ", _onSelectedSlot);
	if (_onSelectedSlot < 2)
	{
		_onSelectedSlot++;
		setSelectSlot(_onSelectedSlot);
	}
}

//////////////////////////////////////////////////////////////////////////////
// Thuc hien touch
//////////////////////////////////////////////////////////////////////////////
bool SkillSelectScene::onTouchBegan(Touch* touch, Event* event)
{
	return true;
}

void SkillSelectScene::onTouchMoved(Touch* touch, Event* event)
{

}
void SkillSelectScene::onTouchEnded(Touch* touch, Event* event)
{

}

//////////////////////////////////////////////////////////////////////////////
// Thuc hien ham callback sau khi tat dialog
//////////////////////////////////////////////////////////////////////////////

void SkillSelectScene::decideCallback(Ref* pSender, Widget::TouchEventType type){
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
	{
		log("Decide Callback da duoc goi");
		_selectedSkillNum++;
		onSelectSkill(_onSelectedSkillId);
		break;
	}
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

void SkillSelectScene::cancelCallback(Ref* pSender, Widget::TouchEventType type){
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
	{
		break;
	}
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

//////////////////////////////////////////////////////////////////////////////
// Backbutton callback
//////////////////////////////////////////////////////////////////////////////
void SkillSelectScene::onBackButtonCallback(Ref* pSender, Widget::TouchEventType type){

	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		Director::getInstance()->replaceScene(TransitionMoveInR::create(0.25f, UnitSelectScene::createScene()));
		break;
	}
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////
// Nextbutton callback
//////////////////////////////////////////////////////////////////////////////
void SkillSelectScene::nextButtonCallback(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		Director::getInstance()->replaceScene(TransitionMoveInR::create(0.5f, BattleScene::createScene(_selectedUnitId)));
		break;
	}
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}


/*
Thiet lap hinh anh va cac thong so cho unit , xay dung local bang tay
Sau nay thi createUnitImage se chuyen qua lay du lieu tu db
*/

void SkillSelectScene::createSkillInfoData()
{
	for (int i = 1; i < 34; i++)
	{
		std::stringstream pathImage;
		pathImage << "image/skillnew/skill/" << i << ".png";

		// create skill info
		SkillInfo temp;
		temp.imagePath = pathImage.str().c_str();
		temp.id = i;
		temp.name = pathImage.str().c_str();

		// add info to array
		_allSkillInfo.push_back(temp);
	}
}
