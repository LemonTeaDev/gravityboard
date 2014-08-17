#pragma once

class PlayStone : public cc::Node
{
public:
	static PlayStone* create(
		int _ownerPlayer,
		int _col,
		bool _reverse);

	bool Init(
		int _ownerPlayer, 
		int _col, 
		bool _reverse);

	bool IsReverse() const
	{
		return isReverse;
	}

	int GetScore() const;
	int GetOwnerPlayer() const;

private:
	bool isReverse;
	int ownerPlayer;
	int colPos;
	cc::Sprite* sprite;
};