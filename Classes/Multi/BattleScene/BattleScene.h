#ifndef _BATTLESCENE_H_
#define _BATTLESCENE_H_

#include "cocos2d.h"
#include <cstdlib>
#include <time.h>

#include "ClassBase/LayerBase.h"
#include "ClassBase/Define.h"
#include "MoveTypeSelectDialog.h"
#include "ModeSelectScene/ModeSelectScene.h"
#include "Object/Character/Character.h"
#include "Object/MiniCircle/MiniCircle.h"
#include "Object/Background/Background.h"
#include "ClassBase/MyBodyParser.h"


USING_NS_CC;
using namespace std;


class BattleScene : public LayerBase
{
public :
	static Scene* createScene(int selectedUnitId);
	static BattleScene* create(int selectedUnitId);
	virtual bool init(int selectedUnitId);

private :

	////////////////////////////////////////////////////////////////////////
	// XAY DUNG THUOC TINH
	////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////
	// CREATE DATABASE
	///////////////////////////////////////////////

	/* Get data from database */
	vector<SkillInfo> _allSkillSelectedInfo; // Cac skill duoc unit lua chon
	// Thuc ra character duoc lua chon chinh la tuong trong tran dau
	UnitInfo _allUnitSelectedInfo; // Unit duoc lua chon

	/////////////////////////////////////////////////
	// CREATE BACKGROUND AND CONTENT
	/////////////////////////////////////////////////
	
	/*PhysicsWorld*/
	PhysicsWorld* _myWorld;

	/*UI*/
	Node* _background;
	std::string _imagePath;


	/*Status bar*/
	Sprite* _statusCharacterBar;
	ClippingNode* _characterMiniIcon;
	Sprite* _icon;
	Slider* _hpBar;
	Label* _timeViewLabel;

	Button* _moveTypeSelectMenu;

	/* Mini Map */
	Sprite* _miniMap;
	Sprite* _selectRect;
	Sprite* _miniIcon;

	/* Battle Clock */
	time_t timer;
	struct tm * timeInfo;
	Sprite* _timeViewSprite;


	////////////////////////////////////////////////
	// CREATE CHARACTER
	////////////////////////////////////////////////

	/*Character*/
	Character* _mainCharacter;
	vector<SkillInfo> _skillUnitList;
	vector<SkillInfo> _skillUnitSelected;
	void createCharacterImageById(int unitId);

	int _selectedCharacterId;


	////////////////////////////////////////////////////////////////////////
	// METHOD
	////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////
	// BACKGROUND AND CONTENT
	////////////////////////////////////////////////

	/* Get time system */
	virtual std::string convertToTimeString(int time);
	virtual void createClockBattle();
	virtual void createMiniMap();

	/* Create Debug content */
	virtual void createDebugContent();
	/* Debug version setup */
	void debugPhysicsButtonCallback(Ref* pSender, Widget::TouchEventType type);
	void menuButtonCallback(Ref* pSender, Widget::TouchEventType type);
	
	/*Create content battlescene*/
	virtual void createContent();
	virtual void createBackground();
	virtual Sprite* createBackgroundPart(Vec2 pos);
	
	virtual void createMenu();

	/* Physicsbody */
	void createBackgroundBody();
	Node* createBodyPart(Vec2 pos, Size bodySize);

	/* Create background with TitledMap */
	TMXTiledMap* _myMap;
	TMXLayer* _mapLayer;
	TMXLayer* _blockLayer;

	Size _myMapSize;
	

	void createBackgroundWithTiledMap();
	void createMapBorder();

	/*Init method*/
	void setPhysicsWorldToLayer(PhysicsWorld* myWorld);
	virtual void onEnter();
	virtual void update(float dt);
	virtual void updateTimeView(float dt);
	virtual void updateMiniMap();


	////////////////////////////////////////////////
	// TOUCH EVENT
	////////////////////////////////////////////////

	Vec2 _touchStartPoint;
	Sprite *_touchMoveBeginSprite;
	Sprite *_touchMoveEndSprite;
	/*onEnter : touch event call*/
	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchMoved(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);

	
	////////////////////////////////////////////////
	// CHARACTER MOVE MODE
	////////////////////////////////////////////////
	/* Move type */
	int _moveMode;
	int _moveCircleLocation;
	int _moveCircleType;

	/* Dialog Callback */
	virtual void moveTypeSelectCallback(Ref* pSender, Widget::TouchEventType type);
	virtual void moveTypeSelectDecideCallback(Ref* pSender, Widget::TouchEventType type);
	virtual void moveTypeSelectCancelCallback(Ref* pSender, Widget::TouchEventType type);


	////////////////////////////////////////////////
	// CHARACTER SKILL
	////////////////////////////////////////////////

	/* Skill button */
	Button* _skill1UnitBtn;
	Button* _skill2UnitBtn;
	Button* _skill3SelectedBtn;
	Button* _skill4SelectedBtn;
	virtual void createSkillButton();
	virtual void skillButtonCallback(Ref* pSender, Widget::TouchEventType type);


	////////////////////////////////////////////////
	// COLLISITION EVENT
	////////////////////////////////////////////////

	bool onContactBegin(PhysicsContact &contact);


	////////////////////////////////////////////////
	// CREATE CIRCLE MINI
	////////////////////////////////////////////////

	MiniCircle* _miniCircle;
	Character* _miniUnit;
	void createMiniCircleAndMiniUnit(int circleLocation);
	// Flag check lan dau tao miniCircle, 
	bool _checkFirstCreateMiniCircleFlg = false;
	bool _checkOneTapMoveFlg = false;
	bool _checkLongTapMoveFlg = false;

	////////////////////////////////////////////////
	// CREATE RANDOM ROCK, TREE
	////////////////////////////////////////////////
	vector<Sprite*> _allRock;
	vector<Sprite*> _allTree;

	void createRandomRock();
	void createRandomTree();
	void createTower();
	void createHurdle();

	////////////////////////////////////////////////
	// CREATE WORM HOLE
	////////////////////////////////////////////////

	void createWormHole();

};

#endif // _BATTLESCENE_H_