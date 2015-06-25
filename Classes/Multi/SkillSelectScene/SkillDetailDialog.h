#ifndef _SKILLDETAILDIALOG_H_
#define _SKILLDETAILDIALOG_H_

#include "ClassBase/Define.h"
#include "ClassBase/DialogBase.h"

class SkillDetailDialog : public DialogBase
{
public:
	static SkillDetailDialog* create(SkillInfo skillInfo, MyTouchEvent decideCallback, MyTouchEvent cancelCallback);
	bool init(SkillInfo skillInfo, MyTouchEvent decideCallback, MyTouchEvent cancelCallback);

private:

	SkillInfo _skillInfo;
	LabelTTF* _statusLabel;

	virtual void createDisplaySkillInfo(Sprite* parent);

};

#endif // _SKILLDETAILDIALOG_H_
