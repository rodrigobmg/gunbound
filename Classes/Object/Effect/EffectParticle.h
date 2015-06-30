#ifndef _EFFECTPARTICLE_H_
#define _EFFECTPARTICLE_H_

#include "cocos2d.h"
#include "ClassBase/Define.h"

USING_NS_CC;
using namespace std;

class EffectParticle
{
public:
	static EffectParticle* createEffect();
	virtual bool init();

	static EffectParticle* createWithParticle(string plistEffectPath, int effectColor, int skillType);
	virtual bool initWithParticle(string plistEffectPath, int effectColor, int skillType);

private:

};


#endif /* _EFFECTPARTICLE_H_ */