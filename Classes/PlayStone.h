#pragma once

class PlayStone : public cc::Node
{
public:
	PlayStone(
		int_fast8_t _ownerPlayer, 
		int_fast8_t _col, 
		bool _reverse);
	bool IsReverse() const;
	int_fast32_t GetScore() const;
	int_fast8_t GetOwnerPlayer() const;

private:
	bool isReverse;
	int_fast8_t ownerPlayer;
	int_fast8_t colPos;
	cc::Sprite* sprite;
};