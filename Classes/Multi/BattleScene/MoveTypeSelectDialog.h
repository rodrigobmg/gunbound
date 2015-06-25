#ifndef _MOVETYPESELECTDIALOG_H_
#define _MOVETYPESELECTDIALOG_H_

#include "cocos2d.h"
#include "ClassBase/Define.h"
#include "ClassBase/DialogBase.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"

USING_NS_CC;
using namespace ui;


#define MOVEKEY "MOVE_MODE"
#define MOVE_CIRCLE_LOCATION "MOVE_CIRCLE_LOCATION"
#define MOVE_CIRCLE_TYPE "MOVE_CIRCLE_TYPE"
#define MOVE_AUTO 1
#define MOVE_MANUAL 2
#define MOVE_CIRCLE 3
#define MOVE_CIRCLE_LEFT 4
#define MOVE_CIRCLE_RIGHT 5
#define MOVE_CIRCLE_TIME 6
#define MOVE_CIRCLE_DISTANCE 7

class MoveTypeSelectDialog : public DialogBase
{
public:
	static MoveTypeSelectDialog* create(MyTouchEvent decideCallback, MyTouchEvent cancenCallback);
	bool init(MyTouchEvent decideCallback, MyTouchEvent cancelCallback);
	
private:

	MyTouchEvent _decideCallback;
	MyTouchEvent _cancelCallback;

	CheckBox* _mode1;
	CheckBox* _mode2;
	CheckBox* _mode3;

	CheckBox *_mode3Left;
	CheckBox *_mode3Right;
	CheckBox *_mode3Time;
	CheckBox *_mode3Distance;

	Label* _select1Txt;
	Label* _select2Txt;
	Label* _select3Txt;
	Label* _optionText3Left;
	Label* _optionText3Right;
	Label* _optionText3Time;
	Label* _optionText3Distance;

	int _selectMode;
	int _circleLocationMode;
	int _circleTypeMode;

	void createDialogContent();
	void createDefaultMode();

	void checkBox1Callback(Ref* pSender, CheckBox::EventType type);
	void checkBox2Callback(Ref* pSender, CheckBox::EventType type);
	void checkBox3Callback(Ref* pSender, CheckBox::EventType type);

	void touchMove3LeftCallback(Ref *pSender, CheckBox::EventType type);
	void touchMove3RightCallback(Ref *pSender, CheckBox::EventType type);
	void touchMove3TimeCallback(Ref *pSender, CheckBox::EventType type);
	void touchMove3DistanceCallback(Ref *pSender, CheckBox::EventType type);

	void decideButtonCallback(Ref* pSender, Widget::TouchEventType type);

};


#endif // _MOVETYPESELECTDIALOG_H_