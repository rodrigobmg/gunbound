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
	
	/* Init */
	virtual bool initWithId(int characterId);
	virtual void createImagePathByUnitId(int characterId);

	CC_SYNTHESIZE(string, _imagePath, ImagePath);
	CC_SYNTHESIZE(int, _currentMoveAnimation, CurrentMoveAnimation);
	CC_SYNTHESIZE(float, _characterMoveSpeech, CharacterMoveSpeed );
	CC_SYNTHESIZE(int, _moveMode, MoveMode);
	CC_SYNTHESIZE(float, _moveOneTapTime, MoveOneTapTime);

	Sprite* _characterSprite;


	/* Lay chi so direction tuong ung voi goc */
	int getDirectionWithAngle(float angle);
	/* Flag dung de xac dinh goc */
	bool getDetectAngleFlg(int offset, float angle);
	/* Thuc hien animation xoay character theo direction */
	void actionRotateWithDirection(int directionIndex);

	// Tao 1 doi tuong animation thuc hien hanh dong di chuyen duoc xay dung bang anh
	Animation* createMoveAnimationCharacterWithImage(int imageId , string imagePath);

	// Tao 1 doi tuong animation thuc hien hanh dong di chuyen duoc xay dung bang anh
	Animation* createAttackAnimationCharacterWithImage(int imageId);

	void actionMoveByVector(Vec2 moveVector);
	void actionStopMove();

};

#endif // _CHARACTER_H_