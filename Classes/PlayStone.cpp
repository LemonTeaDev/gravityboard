#include "PCH.h"
#include "PlayStone.h"
#include "GameMgr.h"

USING_NS_CC;

PlayStone::PlayStone(
	int_fast8_t _ownerPlayer, 
	int_fast8_t _col,
	bool _reverse)
: ownerPlayer(_ownerPlayer)
, isReverse(_reverse)
, colPos(_col)
{
	CC_ASSERT(g_GameMgr.GetGameMode() != GameMgr::none);

	if (ownerPlayer == 1)
	{
		sprite = Sprite::create("player1.png");
	}
	else if (ownerPlayer == 2)
	{
		sprite = Sprite::create("player2.png");
	}
	else if (ownerPlayer == 3)
	{
		sprite = Sprite::create("player3.png");
	}
	else if (ownerPlayer == 4)
	{
		sprite = Sprite::create("player4.png");
	}
	
	sprite->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
}
