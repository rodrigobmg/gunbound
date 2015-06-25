#ifndef _BATTLESCENE_H_
#define _BATTLESCENE_H_

#include "cocos2d.h"
#include <cstdlib>
#include <time.h>

#include "ClassBase/LayerBase.h"
#include "ClassBase/Define.h"
#include "MoveTypeSelectDialog.h"
#include "ModeSelectScene/ModeSelectScene.h"

USING_NS_CC;
using namespace std;


class BattleScene : public LayerBase
{
public :
	static Scene* createScene(int selectedUnitId);
	static BattleScene* create(int selectedUnitId);
	virtual bool init(int selectedUnitId);

private :
	/*PhysicsWorld*/
	PhysicsWorld* _myWorld;

	/*UI*/
	Node* _background;
	std::string _imagePath;

	/*Character*/
	Sprite* _mainCharacter;
	vector<SkillInfo> _skillUnitList;
	vector<SkillInfo> _skillUnitSelected;

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

	/* Get time system */
	time_t timer;
	struct tm * timeInfo;
	Sprite* _timeViewSprite;
	virtual std::string convertToTimeString(int time);
	virtual void createClockBattle();
	virtual void createMiniMap();

	/* Create Debug content */
	virtual void createDebugContent();
	
	/*Create content battlescene*/
	virtual void createContent();
	virtual void createBackground();
	virtual Sprite* createBackgroundPart(Vec2 pos);
	

	/* Physicsbody */
	void createBackgroundBody();
	Node* createBodyPart(Vec2 pos, Size bodySize);

	/*Init method*/
	void setPhysicsWorldToLayer(PhysicsWorld* myWorld);
	virtual void onEnter();
	virtual void update(float dt);
	virtual void updateTimeView(float dt);
	virtual void updateMiniMap();


	/*Touch variable*/
	Vec2 _touchStartPoint;
	Sprite *_touchMoveBeginSprite;
	Sprite *_touchMoveEndSprite;
	/*onEnter : touch event call*/
	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchMoved(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);

	/*direct angle using roatate unit*/
	int _currentMoveDirectTag;

	bool getDetectAngleFlg(int offset, float angle); // Tinh goc 
	int getDirectionWithAngle(float angle); // Tinh derect theo goc
	void actionRotateWithDirectionIndex(int derect); // Xoay character theo derect
	Animation* createMoveAnimationCharacterWithImage(int imageId, std::string path);
	Animation* createAttackAnimationCharacter(int imageId, std::string path);


	/* Debug version setup */
	void debugPhysicsButtonCallback(Ref* pSender, Widget::TouchEventType type);
	void menuButtonCallback(Ref* pSender, Widget::TouchEventType type);

	/* Move type */
	int _moveMode;
	int _moveCircleLocation;
	int _moveCircleType;

	/* Dialog Callback */
	virtual void moveTypeSelectCallback(Ref* pSender, Widget::TouchEventType type);
	virtual void moveTypeSelectDecideCallback(Ref* pSender, Widget::TouchEventType type);
	virtual void moveTypeSelectCancelCallback(Ref* pSender, Widget::TouchEventType type);

	/* Skill button */
	Button* _skill1UnitBtn;
	Button* _skill2UnitBtn;
	Button* _skill3SelectedBtn;
	Button* _skill4SelectedBtn;
	virtual void createSkillButton();
};

#endif // _BATTLESCENE_H_