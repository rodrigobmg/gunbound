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
	createDatabase();

	// Disable back button
	_usernameBG->setVisible(false); // hide
	_backButton->setVisible(false);
	// reload sprite texture
	_bgImage->setTexture(TextureCache::sharedTextureCache()->addImage("image/screen/gunbound_bg2.png"));

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
/////////////////////////////////////////////////////////////////////////
// CONNECT DATABASE
////////////////////////////////////////////////////////////////////////

/* Thuc hien create va connect database de khoi tao du lieu dau vao cho game */
void ModeSelectScene::createDatabase()
{
	auto request = new HttpRequest();
	std::string url = "";
	
	if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32){
		// Lay du lieu tu localhost
		url = "http://127.0.0.1/cocos2dx/database.db3";
	}
	if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID){
		/*Them port cho firewall va chuyen ServerName thanh dia chi ip cua may
		 * Lay data tu localhost nay(device va server phai dat cung 1 mang LAN)
		 */
		url = "http://192.168.0.208:80/cocos2dx/database.db3";
	}
	log("URL: %s " , url.c_str());
	request->setUrl(url.c_str());
	// Thuc hien gui request len server theo phuong thuc GET
	request->setRequestType(HttpRequest::Type::GET); 
	// Thiet lap callback khi request duoc gui toi server va server tra lai du lieu 
	/*Cocos2dx V2.2*/
	//request->setResponseCallback(this, httpresponse_selector(ModeSelectScene::serverCallback));
	/*Cocos2dx V3.4*/
	request->setResponseCallback(CC_CALLBACK_2(ModeSelectScene::serverCallback, this));
	// Gui request len server
	auto client = HttpClient::getInstance();
	client->enableCookies(NULL);
	client->send(request);
	request->release();
}

void ModeSelectScene::serverCallback(HttpClient* client, HttpResponse* response)
{
	if (response->getResponseCode() == 200)
	{
		auto fileUtils = FileUtils::getInstance();
		std::string filePath = fileUtils->getWritablePath() + "gunbound.db3";
		auto responseData = response->getResponseData();

		/* Log response data */
		/*
		for (unsigned int i = 0; i < responseData->size(); i++){
			log("%c", (*responseData)[i]);
		}
		*/

		/*
		ModeType:
		W:Tao file moi hoac neu file co san thi se duoc ghi moi hoan toan
		filePath: ten file hoac duong dan den file can mo
		Trong truong hop nay no la duong dan den file can mo
		WB: ghi vao file nhi phan chu khong phai file txt nhu binh thuong
		*/
		
		FILE* file = fopen(filePath.c_str(), "wb");
		if (file == NULL) // Neu fopen() tra ve NULL nghia la ko mo duoc file hoac file ko ton tai
		{
			log("Can not create file %s ", filePath.c_str());
			return;
		}
		// Ghi du lieu tu responseData vao tap tin file
		// Ham fwrite(du lieu muon ghi vao file , kich thuoc du lieu, so luong can ghi , file duoc ghi vao )
		fwrite(responseData->data(), 1, responseData->size(), file);

		fclose(file);
		log("Succesfull !!! Save file into %s ", filePath.c_str());


	}else{
		log("Not connect database");
	}
}

/* Them ky tu ket thuc file "\0"
if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
{
// Neu build tren win thi can them \0(ky tu ket thuc chuoi trong C) vao cuoi file db3, neu ko se ko build duoc
char* endFile = "\0";
fwrite(endFile, strlen(endFile), 1, file);
}
*/
