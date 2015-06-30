#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_

#include "cocos2d.h"
#include "ClassBase/Define.h"

USING_NS_CC;

class Background : public Node
{
public:
	static Background* createBackground();
	virtual bool init();

	Size _visibleSize;
	Node* _background;

private:

	virtual Sprite* createBackgroundPart(Vec2 pos);
	
	
	/* Physicsbody */
	virtual void createBackgroundBody();
	virtual Node* createBodyPart(Vec2 pos, Size bodySize);
};

#endif //_BACKGROUND_H_