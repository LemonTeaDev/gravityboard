#include "PCH.h"
#include "TileMgr.h"
#include "GameMgr.h"
#include "PlayStone.h"

USING_NS_CC;

TileMgr::TileMgr()
: bgLayer(nullptr)
, numRows(0)
, numCols(0)
, isInitialized(false)
, isTileCreated(false)
{
}

bool TileMgr::Init(
	cc::Layer* _bgLayer,
	Point _goalPosition,
	Size _goalSize,
	int _numRows /*= DEFAULT_NUM_ROWS */,
	int _numCols /*= DEFAULT_NUM_COLS */)
{
	isTileCreated = false;

	if (isInitialized)
	{
		CC_ASSERT(!isInitialized);
		return false;
	}

	bgLayer = _bgLayer;
	ndmMouthPosition = _goalPosition;
	goalSize = _goalSize;
	numRows = _numRows;
	numCols = _numCols;

	isInitialized = true;

	return true;
}

bool TileMgr::CreateTiles()
{
	CC_ASSERT(bgLayer != nullptr);
	if (bgLayer == nullptr)
	{
		return false;
	}

	CC_ASSERT(isTileCreated == false);
	if (isTileCreated)
	{
		return false;
	}

	CC_ASSERT(tiles.empty());
	if (!tiles.empty())
	{
		return false;
	}

	// assume goalBox anchorpoint is 0, 0.5f
	// coord align differs when #row is odd or even
	bool isEven = (numRows % 2 == 0);
	cc::Point tileOrigin = Vec2(
		ndmMouthPosition.x + goalSize.width - 50, 
		ndmMouthPosition.y);

	bool createResult = false;
	if (isEven)
	{
		createResult = CreateTilesEvenRows(tileOrigin);
	}
	else
	{
		createResult = CreateTilesOddRows(tileOrigin);
	}

	return createResult;
}

bool TileMgr::CreateTilesEvenRows(const cc::Point& tileOrigin)
{
	int numHalfRows = numRows / 2;

	// fill upper half
	for (int rowIdx = 0; rowIdx < numHalfRows; ++rowIdx)
	{
		SpriteVec spriteLine;
		for (int colIdx = 0; colIdx < numCols; ++colIdx)
		{
			Sprite* tile = Sprite::create("tile.png");
			Size tileSize = tile->getContentSize();
			float posX = tileOrigin.x + tileSize.width * colIdx;
			float posY = tileOrigin.y + tileSize.height * (numHalfRows - rowIdx - 1);
			PostTileCreate(tile, posX, posY, colIdx);
			spriteLine.push_back(tile);
		}
		tiles.push_back(spriteLine);
	}

	// fill lower half
	for (int rowIdx = numHalfRows; rowIdx < numRows; ++rowIdx)
	{
		SpriteVec spriteLine;
		for (int colIdx = 0; colIdx < numCols; ++colIdx)
		{
			Sprite* tile = Sprite::create("tile.png");
			Size tileSize = tile->getContentSize();
			float posX = tileOrigin.x + tileSize.width * colIdx;
			float posY = tileOrigin.y - tileSize.height * (numRows - rowIdx);
			PostTileCreate(tile, posX, posY, colIdx);
			spriteLine.push_back(tile);
		}
		tiles.push_back(spriteLine);
	}
	return true;
}

bool TileMgr::CreateTilesOddRows(const Point& tileOrigin)
{
	int numHalfRows = numRows / 2;

	// fill upper half
	for (int rowIdx = 0; rowIdx < numHalfRows; ++rowIdx)
	{
		SpriteVec spriteLine;
		for (int colIdx = 0; colIdx < numCols; ++colIdx)
		{
			Sprite* tile = Sprite::create("tile.png");
			Size tileSize = tile->getContentSize();
			float posX = tileOrigin.x + tileSize.width * colIdx;
			float posY = tileOrigin.y + tileSize.height * (numHalfRows - rowIdx - 1) + tileSize.height / 2;
			PostTileCreate(tile, posX, posY, colIdx);
			spriteLine.push_back(tile);
		}
		tiles.push_back(spriteLine);
	}

	// fill mid
	{
		SpriteVec spriteLine;
		for (int colIdx = 0; colIdx < numCols; ++colIdx)
		{
			Sprite* tile = Sprite::create("tile.png");
			Size tileSize = tile->getContentSize();
			float posX = tileOrigin.x + tileSize.width * colIdx;
			float posY = tileOrigin.y - tileSize.height / 2;
			PostTileCreate(tile, posX, posY, colIdx);
			spriteLine.push_back(tile);
		}
		tiles.push_back(spriteLine);
	}

	// fill lower half
	for (int rowIdx = numHalfRows + 1; rowIdx < numRows; ++rowIdx)
	{
		SpriteVec spriteLine;
		for (int colIdx = 0; colIdx < numCols; ++colIdx)
		{
			Sprite* tile = Sprite::create("tile.png");
			Size tileSize = tile->getContentSize();
			float posX = tileOrigin.x + tileSize.width * colIdx;
			float posY = tileOrigin.y - tileSize.height * (numRows - rowIdx) - tileSize.height / 2;
			PostTileCreate(tile, posX, posY, colIdx);
			spriteLine.push_back(tile);
		}
		tiles.push_back(spriteLine);
	}
	return true;
}

const TileMgr::SpriteVec2D& TileMgr::GetTiles() const
{
	return tiles;
}

int TileMgr::GetNumRows() const
{
	return numRows;
}

int TileMgr::GetNumCols() const
{
	return numCols;
}

void TileMgr::PostTileCreate(
	Sprite* tile, 
	float posX, 
	float posY, 
	int colIdx)
{
	if (tile != nullptr)
	{
		tile->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
		tile->setPosition(posX, posY);
		bgLayer->addChild(tile, TILE_Z);
		tile->setTag(colIdx);
		if (colIdx < 1)
		{
			return;
		}
		
		auto touchListener = EventListenerTouchOneByOne::create();
		touchListener->onTouchBegan = [=](Touch* touch, Event* event)
		{
			if (tile->getChildrenCount() > 0)
			{
				return false;
			}

			auto touchLocation = touch->getLocation();
			auto tilePos = tile->getPosition();
			auto tileSize = tile->getContentSize();
			if (!Rect(tilePos.x, tilePos.y, tileSize.width, tileSize.height).containsPoint(touchLocation))
			{
				return false;
			}

			return true;
		};

		touchListener->onTouchEnded = [=](Touch* touch, Event* event)
		{
			auto player = g_GameMgr.GetCurrentCastPlayer();
			auto& reverseClicked = g_GameMgr.reverseClicked;
			PlayStone* playStone = PlayStone::create(player, colIdx, reverseClicked);

			auto tilePos = tile->getPosition();
			if (reverseClicked)
			{
				reverseClicked = false;
			}
			playStone->setAnchorPoint(Point::ANCHOR_MIDDLE);
			tile->addChild(playStone);

			g_GameMgr.OnPlayerCast();
		};

		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, tile);
	}
}
