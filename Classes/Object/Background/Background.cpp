#include "Background.h"

Background* Background::createBackground()
{
	auto layer = new Background();
	if (layer && layer->init())
	{
		layer->autorelease();
		return layer;
	}
	CC_SAFE_DELETE(layer);
	return nullptr;
}

bool Background::init()
{
	if (!Node::init())
	{
		return false;
	}

	_visibleSize = Director::getInstance()->getVisibleSize();


	/////////////////////////////////////////////////////////////////////////////
	// Version 1: Thiet lap background bang 4 background con
	/////////////////////////////////////////////////////////////////////////////

	// create background chinh
	_background = Node::create();
	_background->setPosition(Vec2::ZERO);
	_background->setPhysicsBody(PhysicsBody::createEdgeBox(Size(1, 1), PhysicsMaterial(1, 1, 1)));

	this->addChild(_background);

	// Xay dung background bang 4 thanh phan chinh
	auto part1BG = createBackgroundPart(Vec2(_visibleSize.width, _visibleSize.height));
	part1BG->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_background->addChild(part1BG);
	auto part2BG = createBackgroundPart(Vec2(_visibleSize.width, _visibleSize.height));
	part2BG->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
	_background->addChild(part2BG);
	auto part3BG = createBackgroundPart(Vec2(_visibleSize.width, _visibleSize.height));
	part3BG->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	_background->addChild(part3BG);
	auto part4BG = createBackgroundPart(Vec2(_visibleSize.width, _visibleSize.height));
	part4BG->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	_background->addChild(part4BG);



	// Create physicsbody for background
	createBackgroundBody();


	/////////////////////////////////////////////////////////////////////////////
	// Version 2: Xay dung background bang tilde map
	/////////////////////////////////////////////////////////////////////////////

	return true;
}

/* Khoi tao 1 doi tuong Sprite chinh la 1 phan cua background */
Sprite* Background::createBackgroundPart(Vec2 pos)
{
	auto part = Sprite::create("image/screen/battle/bg.png");
	part->setPosition(pos);

	return part;
}

/* Khoi tao phusics body cho background */
void Background::createBackgroundBody()
{
	Size hSize = Size(_visibleSize.width * 2, 100);
	Size vSize = Size(100, _visibleSize.height * 2);


	auto bottomBody = createBodyPart(Vec2(_visibleSize.width, 0), hSize);
	_background->addChild(bottomBody);
	auto topBody = createBodyPart(Vec2(_visibleSize.width, _visibleSize.height * 2), hSize);
	_background->addChild(topBody);
	auto ringhtBody = createBodyPart(Vec2(_visibleSize.width * 2, _visibleSize.height), vSize);
	_background->addChild(ringhtBody);
	auto leftBody = createBodyPart(Vec2(0, _visibleSize.height), vSize);
	_background->addChild(leftBody);

}

/* Khoi tao tung phan cua physics body */
Node* Background::createBodyPart(Vec2 pos, Size bodySize)
{
	auto node = Node::create();

	PhysicsBody* nodeBody = PhysicsBody::createBox(bodySize, PhysicsMaterial(1, 0, 0));
	nodeBody->setGravityEnable(false); // Khong chiu tac dung cua trong luc
	nodeBody->setDynamic(false); // Khong chiu tac dung cua dong luc(ko co ma sat, ko co van toc)
	nodeBody->setContactTestBitmask(0x1);

	node->setPhysicsBody(nodeBody);
	node->setPosition(pos);

	return node;
}