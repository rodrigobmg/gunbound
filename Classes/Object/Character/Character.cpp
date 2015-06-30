#include "Object/Character/Character.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// KHOI TAO DOI TUONG CHARACTER
//////////////////////////////////////////////////////////////////////////////////////////////

Character* Character::createCharacterWithId(int characterId)
{
	Character* character = new (std::nothrow) Character();
	if (character && character->initWithId(characterId))
	{
		character->autorelease();
		return character;
	}
	CC_SAFE_DELETE(character);
	return nullptr;
}

bool Character::initWithId(int characterId)
{
	if (!Sprite::init())
	{
		return false;
	}

	createImagePathByUnitId(characterId);
	
	string imagePath =_imagePath;
	imagePath.append("unit_00_08_1.png");

	this->initWithFile(imagePath.c_str());

	return true;
}


void Character::createImagePathByUnitId(int characterId)
{
	switch (characterId)
	{
	case 1:
		_imagePath = "image/unit_new/move/red/";
		break;
	case 2:
		_imagePath = "image/unit_new/move/purple/";
		break;
	case 3:
		_imagePath = "image/unit_new/move/green/";
		break;
	case 4:
		_imagePath = "image/unit_new/move/blue/";
		break;
	case 5:
		_imagePath = "image/unit_new/move/black/";
		break;
	default:
		_imagePath = "image/unit_new/move/red/";
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Thuc hien moveAction method
//////////////////////////////////////////////////////////////////////////////////////////////

void Character::actionMoveByVector(Vec2 moveVector)
{
	int direction = getDirectionWithAngle(-(moveVector.getAngle() * RAD_DEG) + 90);
	
	switch (this->getMoveMode())
	{
		case 1: // MOVE_AUTO
			break;
		case 2: // MOVE_MANUAL
		case 3: // MOVE_CIRCLE_MANUAL
		case 4: // MOVE_CIRCLE_LONGTAP
			this->getPhysicsBody()->setVelocity(Vect(this->getCharacterMoveSpeed() * cos(moveVector.getAngle()), this->getCharacterMoveSpeed() * sin(moveVector.getAngle())));

			if (direction != 0)
			{
				actionRotateWithDirection(direction);
			}
			break;
		case 5: // MOVE_CIRCLE_ONETAP
		{
			this->stopActionByTag(101);

			auto actionOneTap = Sequence::create(DelayTime::create(this->getMoveOneTapTime()), CallFuncN::create([&](Ref* pSender){
				this->actionStopMove();
			}), nullptr);

			actionOneTap->setTag(101);

			this->runAction(actionOneTap);

			if (this->getPhysicsBody() != nullptr)
			{
				this->getPhysicsBody()->setVelocity(Vect(this->getCharacterMoveSpeed() * cos(moveVector.getAngle()), this->getCharacterMoveSpeed() * sin(moveVector.getAngle())));
			}
			if (direction != 0)
			{
				actionRotateWithDirection(direction);
			}

			break;
		}
		default:
			break;
	}



	
}


void Character::actionStopMove()
{
	if (this->getPhysicsBody() != nullptr)
	{
		this->getPhysicsBody()->setVelocity(Vect::ZERO);
	}

	this->stopActionByTag(this->getCurrentMoveAnimation());
}

//////////////////////////////////////////////////////////////////////////////////////////////
//XU LY LAY GOC
//////////////////////////////////////////////////////////////////////////////////////////////


/* Lay chi so direction tuong ung voi goc */
int Character::getDirectionWithAngle(float angle)
{
	if (getDetectAngleFlg(0, angle)) return 8; // if(angle < 22 && angle > -22) return 8
	if (getDetectAngleFlg(45, angle)) return 9; // if(angle < 45+22 && angle > 45-22) return 9
	if (getDetectAngleFlg(90, angle)) return 6;
	if (getDetectAngleFlg(135, angle)) return 3;
	if (getDetectAngleFlg(180, angle)) return 2;
	if (getDetectAngleFlg(-45, angle)) return 7;
	if (getDetectAngleFlg(-90, angle)) return 4;
	if (getDetectAngleFlg(255, angle)) return 1;

	return 0;
}

/* Flag dung de xac dinh goc */
bool Character::getDetectAngleFlg(int offset, float angle)
{
	if ((angle < (offset + 22)) && (angle >(offset - 22)))
	{
		return true;
	}

	return false;
}


//////////////////////////////////////////////////////////////////////////////////////////////
//XU LY MOVE & ATTACK ANIMATION
//////////////////////////////////////////////////////////////////////////////////////////////

void Character::actionRotateWithDirection(int directionIndex)
{
	log("Direct: %d ", directionIndex);
	/* Kiem tra neu van la action cu thi ko stop no lai*/
	if (this->getNumberOfRunningActions() > 0) {
		if (this->getActionByTag(directionIndex) != nullptr) {
			return;
		}
	}
	// Thuc hien stop action cu khi chuyen huong(direct thay doi)
	this->stopAllActionsByTag(this->getCurrentMoveAnimation());
	log("Stop action with tag: %d ", this->getCurrentMoveAnimation());
	// thuc hien animation repeat forever

	
	auto action = Animate::create(createMoveAnimationCharacterWithImage(directionIndex , _imagePath));
	auto repeat = RepeatForever::create(action);
	repeat->setTag(directionIndex);
	this->setCurrentMoveAnimation(directionIndex);

	this->runAction(repeat);

}


// Tao 1 doi tuong animation thuc hien hanh dong di chuyen duoc xay dung bang anh
/*
Neu khong truyen imagePath vao thi cu moi lan no se thuc hien getImagePath de lay _imagePath
nhu the se bi giat khi di chuyen do _imagePath phai duoc tim lien tuc
*/
Animation* Character::createMoveAnimationCharacterWithImage(int imageId , string imagePath)
{
	// Tao animation bang sprite sheet
	auto moveAnimation = Animation::create();

	for (int i = 1; i < 3; i++)
	{
		char szName[100] = { 0 };
		sprintf(szName, "unit_00_0%d_%d.png", imageId, i);
		string p = imagePath;
		p.append(szName);
		moveAnimation->addSpriteFrameWithFile(p.c_str());
	}

	// Thoi gian animation thuc hien
	moveAnimation->setDelayPerUnit(0.2f);
	// Restore lai diem dat cua cac frame moi khi animate thuc hien
	moveAnimation->setRestoreOriginalFrame(true);
	// Thuc hien lap
	moveAnimation->setLoops(true);

	return moveAnimation;

}

// create attack animation object using sprite sheet
Animation* Character::createAttackAnimationCharacterWithImage(int imageId)
{
	auto attackAnimation = Animation::create();
	for (int i = 0; i < 3; i++)
	{
		char szName[100] = { 0 };
		sprintf(szName, "unit_00_0%d_attack_%d.png", imageId, i);
		string p = getImagePath();
		p.append(szName);
		attackAnimation->addSpriteFrameWithFile(p.c_str());
	}

	attackAnimation->setDelayPerUnit(0.25f);
	attackAnimation->setRestoreOriginalFrame(true);
	attackAnimation->setLoops(true);

	return attackAnimation;
}


