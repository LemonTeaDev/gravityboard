#include "PCH.h"
#include "PlayStone.h"
#include "GameMgr.h"

USING_NS_CC;

PlayStone* PlayStone::create(
	int _ownerPlayer, 
	int _col, 
	bool _reverse)
{
	PlayStone* ret = new PlayStone();
	if (ret && ret->Init(_ownerPlayer, _col, _reverse))
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

bool PlayStone::Init(
	int _ownerPlayer,
	int _col,
	bool _reverse)
{
	if (!Node::init())
	{
		return false;
	}

	ownerPlayer = _ownerPlayer;
	isReverse = _reverse;
	colPos = _col;

	CC_ASSERT(g_GameMgr.GetGameMode() != GameMgr::none);

	std::string playerPrefix = "";
	if (_reverse)
	{
		playerPrefix += "r";
	}

	if (ownerPlayer == 1)
	{
		playerPrefix += "sun";
	}
	else if (ownerPlayer == 2)
	{
		playerPrefix += "moon";
	}
	else if (ownerPlayer == 3)
	{
		playerPrefix += "star";
	}
	else if (ownerPlayer == 4)
	{
		playerPrefix += "planet";
	}

	std::string spriteName = playerPrefix + std::to_string(colPos) + ".png";
	sprite = Sprite::create(spriteName);
	sprite->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
	this->addChild(sprite);
	return true;
}

int PlayStone::GetScore() const
{
	CC_ASSERT(g_GameMgr.GetGameMode() != GameMgr::none);
	if (g_GameMgr.GetGameMode() == GameMgr::none)
	{
		return -1;
	}

	return g_GameMgr.GetCardScore(colPos);
}

int PlayStone::GetOwnerPlayer() const
{
	return ownerPlayer;
}