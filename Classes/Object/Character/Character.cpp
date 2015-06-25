#include "Object/Character/Character.h"

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

Character* Character::createCharacterWithImage(std::string imagePath)
{
	Character* character = new (std::nothrow) Character();
	if (character && character->initWithImage(imagePath))
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

	return true;
}

bool Character::initWithImage(std::string imagePath)
{
	if (!Sprite::init())
	{
		return false;
	}

	return true;
}