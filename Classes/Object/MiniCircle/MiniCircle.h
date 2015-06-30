#ifndef _MINICIRCLE_H_
#define _MINICIRCLE_H_

#include "cocos2d.h"
#include "ClassBase/Define.h"

USING_NS_CC;

class MiniCircle : public Sprite
{
public:
	static MiniCircle* createMiniCircle(int circleLocation);
	virtual bool initWithLocation(int circleLocation);

private:
	Sprite* _miniCircle;

	CC_SYNTHESIZE(float, _circleScaleXRate, MiniCircleScaleXRate);
	CC_SYNTHESIZE(float, _circleScaleYRate, MiniCircleScaleYRate);
	CC_SYNTHESIZE(float, _circleX, MiniCircleAfterScaleX);
	CC_SYNTHESIZE(float, _circleY, MiniCircleAfterScaleY);

	Rect getRect();
	bool isTapped(Vec2 point);


};

#endif // _MINICIRCLE_H_