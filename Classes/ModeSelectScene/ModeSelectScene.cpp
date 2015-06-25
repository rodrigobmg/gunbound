#include "ModeSelectScene\ModeSelectScene.h"

Scene* ModeSelectScene::createScene(){
	auto scene = Scene::create();
	auto layer = ModeSelectScene::create();

	scene->addChild(layer);

	return scene;
}

bool ModeSelectScene::init(){
	if (!LayerBase::init()){
		return false;
	}

	/* Khoi tao Database truoc khi select play mode */
	log("Khoi tao DB");
	
	// Create Database
	DataModel::getInstance()->createDatabase();

	// Disable back button
	_usernameBG->setVisible(false); // hide
	_backButton->setVisible(false);
	// reload sprite texture
	_bgImage->setTexture(TextureCache::getInstance()->addImage("image/screen/gunbound_bg2.png"));

	if (_titleLabel != nullptr){
		_titleLabel->setString("Select Mode");
	}

	// Add unitcharacter
	Sprite* unitCharacter = Sprite::create("image/unit/navi2.png");
	unitCharacter->setPosition(Vec2(_origin.x + _visibleSize.width / 4, _origin.y + _visibleSize.height - _backButton->getContentSize().height - unitCharacter->getContentSize().height / 2));
	addChild(unitCharacter);

	// Add button multi select
	Button* multiButton = Button::create();
	multiButton->loadTextureNormal("image/button/new/multi.png");
	multiButton->setTouchEnabled(true);
	multiButton->addTouchEventListener(CC_CALLBACK_2(ModeSelectScene::multiButtonCallback, this));
	multiButton->setPosition(Vec2(_visibleSize.width * 3 / 4, _visibleSize.height * 2 / 3));
	addChild(multiButton);


	Button* soloButton = Button::create();
	soloButton->loadTextureNormal("image/button/new/solo.png");
	soloButton->setTouchEnabled(true);
	soloButton->addTouchEventListener(CC_CALLBACK_2(ModeSelectScene::soloButtonCallback, this));
	soloButton->setPosition(Vec2(multiButton->getPositionX(), _visibleSize.height * 1 / 3));
	addChild(soloButton);


	Button* testBattleScene = Button::create();
	testBattleScene->loadTextureNormal("test.png");
	testBattleScene->setTouchEnabled(true);
	testBattleScene->addTouchEventListener(CC_CALLBACK_2(ModeSelectScene::testBattleSceneCallback, this));
	testBattleScene->setPosition(Vec2(testBattleScene->getContentSize().width/2 , testBattleScene->getContentSize().height/2 ));
	addChild(testBattleScene);


	return true;
}

void ModeSelectScene::multiButtonCallback(Ref* pSender, Widget::TouchEventType type){
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		Director::getInstance()->replaceScene(TransitionMoveInR::create(0.5f , UnitSelectScene::createScene()));
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}

}

void ModeSelectScene::soloButtonCallback(Ref* pSender, Widget::TouchEventType type){
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		Director::getInstance()->replaceScene(TransitionMoveInR::create(0.5f, UnitSelectScene::createScene()));
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

/*TEST BUTTON CALLBACK*/
void ModeSelectScene::testBattleSceneCallback(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		Director::getInstance()->replaceScene(TransitionMoveInR::create(0.5f, BattleScene::createScene()));
		break;
	}
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}
