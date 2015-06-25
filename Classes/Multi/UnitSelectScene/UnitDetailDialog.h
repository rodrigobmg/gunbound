#ifndef _UNITDETAILDIALOG_H_
#define _UNITDETAILDIALOG_H_

#include "ClassBase/Define.h"
#include "ClassBase/DialogBase.h"

class UnitDetailDialog : public DialogBase
{
public:
	static UnitDetailDialog* create(UnitInfo unitInfo, MyTouchEvent decideCallback, MyTouchEvent cancelCallback);
	bool init(UnitInfo unitInfo, MyTouchEvent decideCallback, MyTouchEvent cancelCallback);


private:

	virtual void createDisplayUnitInfo(Sprite* parent);

	virtual void statusButtonCallback(Ref* pSender, Widget::TouchEventType type);
	virtual void skillButtonCallback(Ref* pSender, Widget::TouchEventType type);

	LabelTTF* _statusLabel;
	LabelTTF* _skillLabel;

	Button* _statusButton;
	Button* _skillButton;

	UnitInfo _unitInfo;

};


#endif // _UNITDETAILDIALOG_H_
