#include "PCH.h"
#include "TileMgr.h"

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
	int_fast8_t _numRows /*= DEFAULT_NUM_ROWS */,
	int_fast8_t _numCols /*= DEFAULT_NUM_COLS */)
{
	isTileCreated = false;

	if (isInitialized)
	{
		CC_ASSERT(!isInitialized);
		return false;
	}

	bgLayer = _bgLayer;
	goalPosition = _goalPosition;
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
	cc::Point tileOrigin = Vec2(goalPosition.x + goalSize.width, goalPosition.y);

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
	int_fast8_t numHalfRows = numRows / 2;

	// fill upper half
	for (int_fast8_t rowIdx = 0; rowIdx < numHalfRows; ++rowIdx)
	{
		SpriteVec spriteLine;
		for (int_fast8_t colIdx = 0; colIdx < numCols; ++colIdx)
		{
			Sprite* tile = Sprite::create("tile.png");
			Size tileSize = tile->getContentSize();
			float posX = tileOrigin.x + tileSize.width * colIdx;
			float posY = tileOrigin.y + tileSize.height * (numHalfRows - rowIdx - 1);
			tile->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
			tile->setPosition(posX, posY);
			bgLayer->addChild(tile, TILE_Z);
			spriteLine.push_back(tile);
		}
		tiles.push_back(spriteLine);
	}

	// fill lower half
	for (int_fast8_t rowIdx = numHalfRows; rowIdx < numRows; ++rowIdx)
	{
		SpriteVec spriteLine;
		for (int_fast8_t colIdx = 0; colIdx < numCols; ++colIdx)
		{
			Sprite* tile = Sprite::create("tile.png");
			Size tileSize = tile->getContentSize();
			float posX = tileOrigin.x + tileSize.width * colIdx;
			float posY = tileOrigin.y - tileSize.height * (numRows - rowIdx);
			tile->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
			tile->setPosition(posX, posY);
			bgLayer->addChild(tile, TILE_Z);
			spriteLine.push_back(tile);
		}
		tiles.push_back(spriteLine);
	}
	return true;
}

bool TileMgr::CreateTilesOddRows(const Point& tileOrigin)
{
	int_fast8_t numHalfRows = numRows / 2;

	// fill upper half
	for (int_fast8_t rowIdx = 0; rowIdx < numHalfRows; ++rowIdx)
	{
		SpriteVec spriteLine;
		for (int_fast8_t colIdx = 0; colIdx < numCols; ++colIdx)
		{
			Sprite* tile = Sprite::create("tile.png");
			Size tileSize = tile->getContentSize();
			float posX = tileOrigin.x + tileSize.width * colIdx;
			float posY = tileOrigin.y + tileSize.height * (numHalfRows - rowIdx - 1) + tileSize.height / 2;
			tile->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
			tile->setPosition(posX, posY);
			bgLayer->addChild(tile, TILE_Z);
			spriteLine.push_back(tile);
		}
		tiles.push_back(spriteLine);
	}

	// fill mid
	{
		SpriteVec spriteLine;
		for (int_fast8_t colIdx = 0; colIdx < numCols; ++colIdx)
		{
			Sprite* tile = Sprite::create("tile.png");
			Size tileSize = tile->getContentSize();
			float posX = tileOrigin.x + tileSize.width * colIdx;
			float posY = tileOrigin.y - tileSize.height / 2;
			tile->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
			tile->setPosition(posX, posY);
			bgLayer->addChild(tile, TILE_Z);
			spriteLine.push_back(tile);
		}
		tiles.push_back(spriteLine);
	}

	// fill lower half
	for (int_fast8_t rowIdx = numHalfRows + 1; rowIdx < numRows; ++rowIdx)
	{
		SpriteVec spriteLine;
		for (int_fast8_t colIdx = 0; colIdx < numCols; ++colIdx)
		{
			Sprite* tile = Sprite::create("tile.png");
			Size tileSize = tile->getContentSize();
			float posX = tileOrigin.x + tileSize.width * colIdx;
			float posY = tileOrigin.y - tileSize.height * (numRows - rowIdx) - tileSize.height / 2;
			tile->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
			tile->setPosition(posX, posY);
			bgLayer->addChild(tile, TILE_Z);
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

int_fast8_t TileMgr::GetNumRows() const
{
	return numRows;
}

int_fast8_t TileMgr::GetNumCols() const
{
	return numCols;
}