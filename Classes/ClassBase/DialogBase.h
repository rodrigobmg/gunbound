#ifndef _DIALOGBASE_H_
#define _DIALOGBASE_H_

//////////////////////////////////////////////////////
// Library Include
//////////////////////////////////////////////////////

#include "cocos2d.h"
#include "ui\CocosGUI.h"
#include "extensions\cocos-ext.h"

//////////////////////////////////////////////////////
// Source Include
//////////////////////////////////////////////////////
#include "ClassBase\LayerBase.h"
#include "ClassBase\Define.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;
using namespace ui;

typedef std::function<void(Ref* pSender , Widget::TouchEventType type)> MyTouchEvent;

class DialogBase : public LayerBase
{
public:
	static DialogBase* create(MyTouchEvent decideCallback, MyTouchEvent cancelCallback);
	bool init(MyTouchEvent decideCallback, MyTouchEvent cancelCallback);

private:

	// Khoi tao cac bien private co thuoc tinh get,set
	CC_SYNTHESIZE(Sprite* , _dialogBG , DialogBG);
	CC_SYNTHESIZE(Button* , _dialogCancelButton , DialogCancelButton);
	CC_SYNTHESIZE(Button* , _dialogDecideButton , DialogDecideButton);

	// Khoi tao 2 ham callback global
	MyTouchEvent _decideCallback;
	MyTouchEvent _cancelCallback;

	virtual void cancelButtonCallback(Ref* pSender, Widget::TouchEventType type);
	virtual void decideButtonCallback(Ref* pSender, Widget::TouchEventType type);
	virtual void closeDialog();




};


#endif // _DIALOGBASE_H_
