#ifndef _SKILLSELECTSCENE_H_
#define _SKILLSELECTSCENE_H_

#include "ClassBase/LayerBase.h"
#include "ClassBase/Define.h"
#include "SkillDetailDialog.h"
#include "Multi/UnitSelectScene/UnitSelectScene.h"
#include "Multi/BattleScene/BattleScene.h"
#include "Model/SkillDataModel.h"

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
using namespace std;



class SkillSelectScene : public LayerBase
{
public:
	static SkillSelectScene* create(int unitId);
	static Scene* createScene(int unitId);
	virtual bool init(int unitId);

	void onEnter();

private:

	int _pageNum;
	int _selectedUnitId;

	//////////////////////////////////////////////////////////////////////////////
	// Get data from database
	//////////////////////////////////////////////////////////////////////////////
	
	// Mang dong chua gia tri cua tat cac skill , skillInfo la struct chua thong tin skill
	vector<SkillInfo> _allSkillInfo;
	// Mang chua id cac skill duoc lua chon
	vector<int> _allSkillSelectedId;
	

	/////////////////////////////////////
	// List all unit
	/////////////////////////////////////
	Button* _leftArrow;
	Button* _rightArrow;

	PageView* _mainPage;

	/////////////////////////////////////
	// selected unit show
	/////////////////////////////////////
	
	int _onSelectedSkillId = 0;
	int _onSelectedSlot = 1;

	Button* _skillSlotBG1;
	Button* _skillSlotBG2;

	ClippingNode* _slot1;
	ClippingNode* _slot2;

	Button* _skillTemp1;
	Button* _skillTemp2;

	LabelTTF* _selectedSkillName1;
	LabelTTF* _selectedSkillName2;


	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchMoved(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);

	//////////////////////////////////////////////////////////////////////////////
	// Show all skill using pageview
	//////////////////////////////////////////////////////////////////////////////

	// Khoi tao du lieu skillData local
	virtual void createSkillInfoData();

	// create pageview show all skill
	virtual void createAllSkillView();
	// Left, right arrow callback function
	virtual void leftArrowCallback(Ref* pSender, Widget::TouchEventType type);
	virtual void rightArrowCallback(Ref* pSender, Widget::TouchEventType type);

	// Ham select skill item trong pageview
	virtual void onTouchSkill(Ref* pSender, Widget::TouchEventType type);

	// Event duoc goi den khi paga view scroll
	virtual void pageViewEvent(Ref* pSender, PageView::EventType type);


	//////////////////////////////////////////////////////////////////////////////
	// Show selected skill after close dialog
	//////////////////////////////////////////////////////////////////////////////

	// Ham callback duoc goi lai khi decide,cancel dialog
	virtual void decideCallback(Ref* pSender, Widget::TouchEventType type);
	virtual void cancelCallback(Ref* pSender, Widget::TouchEventType type);

	// Ham khoi tao slot show unit sau khi unit duoc lua chon
	virtual void createSkillDisplay();
	// Khoi tao cac thanh phan co ban : button, clippingnode, label ,...
	virtual Button* createSlotBaseSprite(Vec2 pos);
	virtual Sprite* createSkillNameBackground(Vec2 pos);
	virtual LabelTTF* createSkillNameLabel(Vec2 pos);
	virtual ClippingNode* createSlot(Vec2 pos);

	// Ham callback duoc goi sau khi click skillTemp button
	virtual void onTouchSkillSlot(Ref* pSender, Widget::TouchEventType type, int selectSlot);
	// Ham chuyen mau background slot
	virtual void setSelectSlot(int selectedSlot);
	// Ham duoc goi sau khi decide dialog, hien thi skill duoc lua chon theo thu tu slot
	virtual void onSelectSkill(int selectedUnitId);
	// Ham show skill sau khi decide dialog(duoc goi tu onSelectSkill)
	virtual void showSkillAfterCloseDialog(Button* parent, LabelTTF* unitNameLabel, int unitId);

	/////////////////////////////////////
	// check so luong skill duoc lua chon
	// neu la 0 thi bat buoc phai lua chon
	/////////////////////////////////////

	void onBackButtonCallback(Ref* pSender , Widget::TouchEventType type);
	void nextButtonCallback(Ref* pSender, Widget::TouchEventType type);

	int _selectedSkillNum = 0;

	

};

#endif // _SKILLSELECTSCENE_H_