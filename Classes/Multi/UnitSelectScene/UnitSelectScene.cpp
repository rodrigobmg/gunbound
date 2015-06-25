#include "UnitSelectScene.h"

/////////////////////////////////////////////////////////////////////////////
// Create Scene UnitSelectScene
/////////////////////////////////////////////////////////////////////////////

UnitSelectScene* UnitSelectScene::create(){
	auto layer = new (std::nothrow) UnitSelectScene();
	if (layer && layer->init()){
		layer->autorelease();

		return layer;
	}
	CC_SAFE_DELETE(layer);
	return nullptr;
}

Scene* UnitSelectScene::createScene(){
	auto scene = Scene::create();
	auto layer = UnitSelectScene::create();
	scene->addChild(layer);

	return scene;
}

bool UnitSelectScene::init(){
	if (!LayerBase::init()){
		return false;
	}

	_titleLabel->setString("Select Unit");

	// Get database from server
	_allUnitInfo = UnitDataModel::getInstance()->getDataUnitFromDatabase();

	//Show list unit
	createAllUnitView();

	// Create unit display after selected
	createUnitDisplay();

	// next button
	auto nextButton = Button::create();
	nextButton->loadTextureNormal("image/button/new/button_decide.png");
	nextButton->setPosition(Vec2(_visibleSize.width - nextButton->getContentSize().width / 2, _visibleSize.height - 100));
	nextButton->setTouchEnabled(true);
	nextButton->addTouchEventListener(CC_CALLBACK_2(UnitSelectScene::nextButtonCallback, this));
	addChild(nextButton);

	// back button add event
	_backButton->addTouchEventListener(CC_CALLBACK_2(UnitSelectScene::onBackButtonCallback, this));

	return true;
}


void UnitSelectScene::onEnter(){
	LayerBase::onEnter();

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(UnitSelectScene::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(UnitSelectScene::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(UnitSelectScene::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	touchListener->setSwallowTouches(false);

}


/////////////////////////////////////////////////////////////////////////////
// Show all unit using pageview
/////////////////////////////////////////////////////////////////////////////

/*
Tao pageView va hien thi unit
*/
void UnitSelectScene::createAllUnitView(){

	//createUnitInfoData();

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
	_leftArrow->addTouchEventListener(CC_CALLBACK_2(UnitSelectScene::leftArrowCallback, this));
	boxSprite->addChild(_leftArrow);

	//Add right arrow
	_rightArrow = Button::create();
	_rightArrow->loadTextureNormal("image/screen/unitSelect/right.png");
	_rightArrow->setSwallowTouches(true);
	_rightArrow->setVisible(true);
	_rightArrow->setPosition(Vec2(boxSprite->getContentSize().width - 50 - _rightArrow->getContentSize().width / 2, boxSprite->getContentSize().height / 2));
	_rightArrow->addTouchEventListener(CC_CALLBACK_2(UnitSelectScene::rightArrowCallback, this));
	boxSprite->addChild(_rightArrow);

	// set default value
	Size boxSize = boxSprite->getContentSize(); // Kich thuoc cua box
	float space = boxSize.width - 150; // Do rong cua pagaview


	// Set page view
	_mainPage = PageView::create();
	_mainPage->setContentSize(Size(space, boxSize.height - 40));
	_mainPage->setPosition(Vec2(75, 0));

	// Xac dinh pageNum theo so luong unit
	_pageNum = (_allUnitInfo.size()) / 4;
	if (_pageNum * 4 < _allUnitInfo.size()){
		_pageNum += 1;
	}

	log("Page Num: %d ", _pageNum);
	if (_pageNum == 1)
	{
		_rightArrow->setVisible(false);
	}

	// space la do rong cua pageview
	float baseX = space * 1 / 8;
	float spaceX = space * 1 / 4;

	log("So luong unit: %d ", _allUnitInfo.size());

	// Show unit in page view
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
			if ((j + i * 4) < _allUnitInfo.size()) {
				// Khoi tao unit image
				auto spriteUnit = Button::create();
				spriteUnit->setTag(_allUnitInfo[j + i * 4].id); // lay tag theo id
				log("TAG : %d", _allUnitInfo[j + i * 4].id);
				spriteUnit->loadTextureNormal(_allUnitInfo[j + i * 4].imagePath); // load texture
				spriteUnit->setScale(1.5f);
				spriteUnit->setSwallowTouches(false);
				spriteUnit->addTouchEventListener(CC_CALLBACK_2(UnitSelectScene::onTouchUnitInPageView, this));
				int yValue = hBox->getContentSize().height / 2 + 20;
				spriteUnit->setPosition(Vec2(baseX + spaceX *j, yValue));
				lay->addChild(spriteUnit);
			}
		}
		_mainPage->insertPage(hBox, i); // insert cac lop theo so luong pageNum
		_mainPage->setSwallowTouches(false);
	}

	// Event bat su kien pageview thay doi
	_mainPage->addEventListener(CC_CALLBACK_2(UnitSelectScene::pageViewEvent, this));
	boxSprite->addChild(_mainPage);

}

/*
Event thuc hien khi pageView co su thay doi
*/
void UnitSelectScene::pageViewEvent(Ref* pSender, PageView::EventType type){
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
void UnitSelectScene::leftArrowCallback(Ref* pSender, Widget::TouchEventType type){
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
	{
		//log("Current Page: %d ", _mainPage->getCurPageIndex());
		_mainPage->scrollToPage(_mainPage->getCurPageIndex() - 1);
		break;
	}
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

/*
Xu ly su kien click button right arrow(di chuyen page view ve phia ben phai)
*/
void UnitSelectScene::rightArrowCallback(Ref* pSender, Widget::TouchEventType type){
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
	{
		//log("Current Page: %d ", _mainPage->getCurPageIndex());
		_mainPage->scrollToPage(_mainPage->getCurPageIndex() + 1);
		break;
	}
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

/*
Su kien click vao unit item thi se hien thi unitDialogDetail
*/
void UnitSelectScene::onTouchUnitInPageView(Ref* pSender, Widget::TouchEventType type){
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
	{
		Button* unit = dynamic_cast<Button*>(pSender);
		int tag = unit->getTag();
		log("Unit Tag : %d ", tag);

		// Check click vao button arrow
		int currentPageIndex = _mainPage->getCurPageIndex();
		log("Current Page: %d ", currentPageIndex);
		if (tag > (currentPageIndex + 1) * 4) return;
		//if (tag != 0 && tag == (currentPageIndex * 4)) return;

		// show dialog unit detail
		//_onSelectedUnitId = tag;
		_selectTemp = tag;
		// Tag lay theo id nen bat dau tu 1, mang bat dau tu 0 nen can tru di 1
		auto dialogDetail = UnitDetailDialog::create(_allUnitInfo[tag-1], CC_CALLBACK_2(UnitSelectScene::decideCallback, this), CC_CALLBACK_2(UnitSelectScene::cancelCallback, this));
		this->getParent()->addChild(dialogDetail);

		break;
	}
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// Show unit selected with Clipping Node
//////////////////////////////////////////////////////////////////////////////

void UnitSelectScene::createUnitDisplay(){

	// ClippingNode hien thi unit duoc lua chon
	_unitSlotBG1 = createSlotBaseSprite(Vec2(_visibleSize.width / 2 - 150, _visibleSize.height - 150));
	addChild(_unitSlotBG1);
	_unitSlotBG1->loadTextureNormal("image/screen/unitSelect/active.png"); // default cai mau vang la cai dau tien
	_unitSlotBG1->addChild(createUnitNameBackground(Vec2(_unitSlotBG1->getContentSize().width / 2, 0)));
	_selectedUnitName1 = createUnitNameLabel(Vec2(_unitSlotBG1->getContentSize().width / 2, 0));
	_unitSlotBG1->addChild(_selectedUnitName1);

	_unitSlotBG2 = createSlotBaseSprite(Vec2(_visibleSize.width / 2, _visibleSize.height - 150));
	addChild(_unitSlotBG2);
	_unitSlotBG2->loadTextureNormal("image/screen/unitSelect/inactive.png");
	_unitSlotBG2->addChild(createUnitNameBackground(Vec2(_unitSlotBG2->getContentSize().width / 2, 0)));
	_selectedUnitName2 = createUnitNameLabel(Vec2(_unitSlotBG2->getContentSize().width / 2, 0));
	_unitSlotBG2->addChild(_selectedUnitName2);

	_unitSlotBG3 = createSlotBaseSprite(Vec2(_visibleSize.width / 2 + 150, _visibleSize.height - 150));
	addChild(_unitSlotBG3);
	_unitSlotBG3->loadTextureNormal("image/screen/unitSelect/inactive.png");
	_unitSlotBG3->addChild(createUnitNameBackground(Vec2(_unitSlotBG3->getContentSize().width / 2, 0)));
	_selectedUnitName3 = createUnitNameLabel(Vec2(_unitSlotBG3->getContentSize().width / 2, 0));
	_unitSlotBG3->addChild(_selectedUnitName3);


	// khoi tao cac doi tuong clippingnode
	_slot1 = createSlot(Vec2(_visibleSize.width / 2 - 150, _visibleSize.height - 150));
	_slot2 = createSlot(Vec2(_visibleSize.width / 2, _visibleSize.height - 150));
	_slot3 = createSlot(Vec2(_visibleSize.width / 2 + 150, _visibleSize.height - 150));

	addChild(_slot1);
	addChild(_slot2);
	addChild(_slot3);

	// create select button , sau khi unit duoc lua chon thi se load lai texture theo unit
	_unitTemp1 = Button::create();
	_unitTemp1->loadTextureNormal("image/screen/unitSelect/slot.png");
	_unitTemp1->setTouchEnabled(true);
	_unitTemp1->addTouchEventListener(CC_CALLBACK_2(UnitSelectScene::onTouchUnitSlot, this, 1));
	_unitTemp1->setPosition(Vec2(_visibleSize.width / 2 - 150, _visibleSize.height - 150));
	_slot1->addChild(_unitTemp1, 5);

	_unitTemp2 = Button::create();
	_unitTemp2->loadTextureNormal("image/screen/unitSelect/slot.png");
	_unitTemp2->setTouchEnabled(true);
	_unitTemp2->addTouchEventListener(CC_CALLBACK_2(UnitSelectScene::onTouchUnitSlot, this, 2));
	_unitTemp2->setPosition(Vec2(_visibleSize.width / 2, _visibleSize.height - 150));
	_slot2->addChild(_unitTemp2, 5);

	_unitTemp3 = Button::create();
	_unitTemp3->loadTextureNormal("image/screen/unitSelect/slot.png");
	_unitTemp3->setTouchEnabled(true);
	_unitTemp3->addTouchEventListener(CC_CALLBACK_2(UnitSelectScene::onTouchUnitSlot, this, 3));
	_unitTemp3->setPosition(Vec2(_visibleSize.width / 2 + 150, _visibleSize.height - 150));
	_slot3->addChild(_unitTemp3, 5);

}


Button* UnitSelectScene::createSlotBaseSprite(Vec2 pos){
	auto sprite = Button::create();
	sprite->loadTextureNormal("image/screen/unitSelect/inactive.png");
	sprite->setEnabled(false);
	sprite->setPosition(pos);

	return sprite;
}

Sprite* UnitSelectScene::createUnitNameBackground(Vec2 pos){
	auto unitNameBG = Sprite::create("image/screen/unitSelect/name.png");
	unitNameBG->setPosition(pos);

	return unitNameBG;
}

LabelTTF* UnitSelectScene::createUnitNameLabel(Vec2 pos){
	auto unitNameLabel = LabelTTF::create("", "fonts/arial.ttf", 20);
	unitNameLabel->setPosition(pos);
	unitNameLabel->setHorizontalAlignment(TextHAlignment::CENTER);
	unitNameLabel->setColor(Color3B::BLUE);

	return unitNameLabel;
}

ClippingNode* UnitSelectScene::createSlot(Vec2 pos){
	auto clippingUnit = ClippingNode::create();
	clippingUnit->setAlphaThreshold(0);
	clippingUnit->setPosition(Vec2::ZERO);
	clippingUnit->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);


	auto mask = Sprite::create("image/navigator/selct_scene_circle.png");
	mask->setPosition(pos);
	clippingUnit->setStencil(mask);

	return clippingUnit;
}

void UnitSelectScene::onTouchUnitSlot(Ref* pSender, Widget::TouchEventType type, int selectSlot){
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
void UnitSelectScene::setSelectSlot(int selectedSlot){
	switch (selectedSlot)
	{
	case 1:
		_unitSlotBG1->loadTextureNormal("image/screen/unitSelect/active.png");
		_unitSlotBG2->loadTextureNormal("image/screen/unitSelect/inactive.png");
		_unitSlotBG3->loadTextureNormal("image/screen/unitSelect/inactive.png");
		break;
	case 2:
		_unitSlotBG1->loadTextureNormal("image/screen/unitSelect/inactive.png");
		_unitSlotBG2->loadTextureNormal("image/screen/unitSelect/active.png");
		_unitSlotBG3->loadTextureNormal("image/screen/unitSelect/inactive.png");
		break;
	case 3:
		_unitSlotBG1->loadTextureNormal("image/screen/unitSelect/inactive.png");
		_unitSlotBG2->loadTextureNormal("image/screen/unitSelect/inactive.png");
		_unitSlotBG3->loadTextureNormal("image/screen/unitSelect/active.png");
		break;
	default:
		break;
	}
}


void UnitSelectScene::onSelectUnit(int selectedUnitId){
	switch (_onSelectedSlot)
	{
	case 1:
		showUnitAfterCloseDialog(_unitTemp1, _selectedUnitName1, selectedUnitId);
		break;
	case 2:
		showUnitAfterCloseDialog(_unitTemp2, _selectedUnitName2, selectedUnitId);
		break;
	case 3:
		showUnitAfterCloseDialog(_unitTemp3, _selectedUnitName3, selectedUnitId);
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
void UnitSelectScene::showUnitAfterCloseDialog(Button* parent, LabelTTF* unitNameLabel, int unitId){
	// Load lai texture va labelName voi unit duoc lua chon
	parent->loadTextureNormal(_allUnitInfo[unitId - 1].imagePath);
	unitNameLabel->setString(_allUnitInfo[unitId - 1].name);

	// Tu dong chuyen slot lua chon len button tiep theo
	log("_onSelectSlot : %d ", _onSelectedSlot);
	if (_onSelectedSlot < 3)
	{
		_onSelectedSlot++;
		setSelectSlot(_onSelectedSlot);
	}
}

//////////////////////////////////////////////////////////////////////////////
// Thuc hien touch
//////////////////////////////////////////////////////////////////////////////

bool UnitSelectScene::onTouchBegan(Touch* touch, Event* event){
	return true;
}
void UnitSelectScene::onTouchMoved(Touch* touch, Event* event){

}
void UnitSelectScene::onTouchEnded(Touch* touch, Event* event){

}

//////////////////////////////////////////////////////////////////////////////
// Thuc hien ham callback sau khi tat dialog
//////////////////////////////////////////////////////////////////////////////

void UnitSelectScene::decideCallback(Ref* pSender, Widget::TouchEventType type){
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
	{
		log("Decide Callback da duoc goi");
		_selectedUnitNum++;
		_onSelectedUnitId = _selectTemp; // id = tag nen se lay gia tri id theo tag
		onSelectUnit(_onSelectedUnitId);
		log("Selected UnitID: %d ", _onSelectedUnitId);
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

void UnitSelectScene::cancelCallback(Ref* pSender, Widget::TouchEventType type){
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
void UnitSelectScene::onBackButtonCallback(Ref* pSender, Widget::TouchEventType type){

	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		log("===== BACK BUTTON =====");
		Director::getInstance()->replaceScene(TransitionMoveInR::create(0.25f, ModeSelectScene::createScene()));
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
void UnitSelectScene::nextButtonCallback(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		log("===== NEXT BUTTON =====");
		if (_onSelectedUnitId == 0)
		{
			break;
		}
		log("UnitID Selected: %d ", _onSelectedUnitId);
		if (_selectedUnitNum == 0) return;
		Director::getInstance()->replaceScene(TransitionMoveInR::create(0.25f, SkillSelectScene::createScene(_onSelectedUnitId)));
		break;
	}
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////
//Tao du lieu local(Version Old)
//////////////////////////////////////////////
/*
Thiet lap hinh anh va cac thong so cho unit , xay dung local bang tay
Sau nay thi createUnitImage se chuyen qua lay du lieu tu db
Update: Da khong duoc su dung ma thay vao do la su dung ham getDataFromDatabase()
de lay du lieu tu sever
*/
void UnitSelectScene::createUnitInfoData(){
	for (int i = 1; i < 16; i++)
	{
		// Create path name
		std::stringstream pathImage;
		pathImage << "image/unit/" << i << ".png";

		// Create unit info
		UnitInfoOld temp;

		temp._imagePath = pathImage.str().c_str();
		temp._attack = 100;
		temp._defence = 100;
		temp._hp = 100;
		temp._name = pathImage.str().c_str();
		temp._unitId = i;

		// Add unit vao mang _allUnitInfo
		_allUnitInfoOld.push_back(temp);
	}
}