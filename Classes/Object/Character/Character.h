#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "cocos2d.h"
#include "ClassBase/Define.h"

USING_NS_CC;
using namespace std;

class Character : public Sprite
{
public:
	/* Khoi tao character theo id cua unit */
	static Character* createCharacterWithId(int characterId);
	
	/* Khoi tao character theo imagePath */
	static Character* createCharacterWithImage(std::string imagePath);
	
	/* Init */
	virtual bool initWithId(int characterId);
	virtual bool initWithImage(std::string imagePath);


private:
	Sprite* _characterSprite;


	CC_SYNTHESIZE(string, _imagePath, ImagePath);

};

#endif // _CHARACTER_H_