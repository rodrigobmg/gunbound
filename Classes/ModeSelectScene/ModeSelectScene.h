#ifndef _MODESELECTSCENE_H_
#define _MODESELECTSCENE_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <iostream>
#include <fstream>
/* Thu vien can khai bao de su dung lop HttpRequest */
#include "network/HttpRequest.h"
#include "network/HttpClient.h"


#include "ClassBase/LayerBase.h"
#include "Multi/UnitSelectScene/UnitSelectScene.h"
#include "Multi/BattleScene/BattleScene.h"
#include "Database/SqlUtils.h"

#include "Model/DataModel.h"


USING_NS_CC;
using namespace ui;
using namespace network;

class ModeSelectScene : public LayerBase
{
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(ModeSelectScene);

private :

	void multiButtonCallback(Ref* pSender, Widget::TouchEventType type );
	void soloButtonCallback(Ref* pSender, Widget::TouchEventType type );

	/*Test version*/
	void testBattleSceneCallback(Ref* pSender, Widget::TouchEventType type);
};

#endif // _MODESELECTSCENE_H_