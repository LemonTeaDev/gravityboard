#pragma once

class TileMgr
{
private:
	typedef std::vector<cc::Sprite*> SpriteVec;
	typedef std::vector<SpriteVec> SpriteVec2D;
	static const int_fast8_t DEFAULT_NUM_ROWS = 3;
	static const int_fast8_t DEFAULT_NUM_COLS = 7;
	static const int TILE_Z = 1;

public:
	TileMgr();

	bool Init(
		cc::Layer* _bgLayer,
		cc::Point _goalPosition,
		cc::Size _goalSize,
		int_fast8_t _numRows = DEFAULT_NUM_ROWS,
		int_fast8_t _numCols = DEFAULT_NUM_COLS);

	bool CreateTiles();
	bool CreateTilesEvenRows(const cc::Point& origin);
	bool CreateTilesOddRows(const cc::Point& origin);
	void PostTileCreate(cc::Sprite* tile,
						float posX,
						float posY,
						int_fast8_t colIdx,
						int_fast8_t rowIdx);

	const SpriteVec2D& GetTiles() const;

	int_fast8_t GetNumRows() const;
	int_fast8_t GetNumCols() const;

private:
	cc::Layer* bgLayer;
	cc::Point ndmMouthPosition;
	cc::Size goalSize;
	int_fast8_t numRows;
	int_fast8_t numCols;
	SpriteVec2D tiles;

	bool isInitialized;
	bool isTileCreated;
};
