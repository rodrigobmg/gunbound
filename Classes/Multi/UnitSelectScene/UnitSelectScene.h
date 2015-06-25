#ifndef _UNIT_SELECT_SCENE_H_
#define _UNIT_SELECT_SCENE_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "ClassBase/LayerBase.h"
#include "UnitDetailDialog.h"
#include "ModeSelectScene/ModeSelectScene.h"
#include "Multi/SkillSelectScene/SkillSelectScene.h"
#include "Database/SqlUtils.h"
#include "Database/DataUtils.h"
#include "Model/UnitDataModel.h"


class UnitSelectScene : public LayerBase
{
public:
	static Scene* createScene();
	static UnitSelectScene* create();
	bool init();

	virtual void onEnter();

private:
	int _pageNum;

	/////////////////////////////////////
	// List all unit
	/////////////////////////////////////

	Button* _leftArrow;
	Button* _rightArrow;

	PageView* _mainPage;

	/////////////////////////////////////
	// selected unit show
	/////////////////////////////////////

	int _onSelectedUnitId = 0;
	int _onSelectedSlot = 1;


	Button* _unitSlotBG1;
	Button* _unitSlotBG2;
	Button* _unitSlotBG3;

	ClippingNode* _slot1;
	ClippingNode* _slot2;
	ClippingNode* _slot3;

	Button* _unitTemp1;
	Button* _unitTemp2;
	Button* _unitTemp3;

	LabelTTF* _selectedUnitName1;
	LabelTTF* _selectedUnitName2;
	LabelTTF* _selectedUnitName3;


	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchMoved(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);

	//////////////////////////////////////////////////////////////////////////////
	// Show all unit using pageview
	//////////////////////////////////////////////////////////////////////////////

	// Khoi tao du kieu unitData local
	virtual void createUnitInfoData();


	// create pageview show all unit
	virtual void createAllUnitView();
	// Left, right arrow callback function
	virtual void leftArrowCallback(Ref* pSender, Widget::TouchEventType type);
	virtual void rightArrowCallback(Ref* pSender, Widget::TouchEventType type);

	// Ham select unit item trong pageview
	virtual void onTouchUnitInPageView(Ref* pSender, Widget::TouchEventType type);

	// Event duoc goi den khi pageview scroll
	virtual void pageViewEvent(Ref* pSender, PageView::EventType type);

	//////////////////////////////////////////////////////////////////////////////
	// Show selected unit after close dialog
	//////////////////////////////////////////////////////////////////////////////

	// Ham callback duoc goi lai khi decide,cancel dialog
	virtual void decideCallback(Ref* pSender, Widget::TouchEventType type);
	virtual void cancelCallback(Ref* pSender, Widget::TouchEventType type);

	// Ham khoi tao slot hien thi unit duoc lua chon
	virtual void createUnitDisplay();
	// Khoi tao cac thanh phan co ban nhu button, clippingnode , label,...
	virtual Button* createSlotBaseSprite(Vec2 pos);
	virtual Sprite* createUnitNameBackground(Vec2 pos);
	virtual LabelTTF* createUnitNameLabel(Vec2 pos);
	virtual ClippingNode* createSlot(Vec2 pos);

	// Ham callback duoc goi khi click unitTemp button
	virtual void onTouchUnitSlot(Ref* pSender, Widget::TouchEventType type, int selectSlot);
	// Ham chuyen mau slot duoc lua chon
	virtual void setSelectSlot(int selectedSlot);
	// Ham duoc goi sau khi decide dialog, hien thi unit theo thu tu slot
	virtual void onSelectUnit(int selectedUnitId);
	// Ham show unit sau khi decide dialog(duoc goi tu onSelectUnit)
	virtual void showUnitAfterCloseDialog(Button* parent, LabelTTF* unitNameLabel, int unitId);

	// Ham callback khi click button back
	virtual void onBackButtonCallback(Ref* pSender, Widget::TouchEventType type);
	// Ham callback khi click button decide
	virtual void nextButtonCallback(Ref* pSender, Widget::TouchEventType type);

	/////////////////////////////////////
	// check so luong unit duoc lua chon
	// neu la 0 thi bat buoc phai lua chon
	/////////////////////////////////////

	int _selectTemp = 0;
	int _selectedUnitNum = 0;


	//////////////////////////////////////////////
	// Lay du lieu tu database
	//////////////////////////////////////////////
	vector<UnitInfo> _allUnitInfo;
	vector<UnitInfoOld> _allUnitInfoOld;
	void getDataFromDatabase();

};

#endif // _UNIT_SELECT_SCENE_H_