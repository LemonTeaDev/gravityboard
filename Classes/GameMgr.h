#pragma once
#include "Singleton.hpp"
#include "GameScene.h"

class GameMgr
{
private:
	const int DEFAULT_NUM_PLAYERS = 4;
	typedef std::map<int, int> CardMap;
	typedef std::map<int, CardMap> PlayerCardMap;
	typedef std::map<int, int> CardScoreMap;
	typedef std::map<int, int> PlayerScoreMap;
	typedef std::map<int, bool> PlayerReverseUsedMap;

public:
	enum GameMode
	{
		none,
		ffa3,
		ffa4,
		ffa3s,
		ffa4s,
	};

public:
	GameMgr();

	void StartGame(GameScene* gameScene);
	void EndGame();

	int GetCurrentCastPlayer();
	int GetCurrentTurnStarter();
	int GetNumPlayers() const;

	void OnPlayerCast();
	void OnTurnEnd();

	GameMode GetGameMode() const;
	GameMode reservedGameMode;	// can change in title screen

	int GetPlayerScore(int playerIdx);
	std::string GetPlayerScoreString(int playerIdx);
	std::string GetCardInfoString();
	void UpdatePlayerCard(int playerIdx, int colIdx);

	void UseReverse(int playerIdx);
	bool IsReverseUsed(int playerIdx);
	
	int GetCardScore(int cardIdx);

	bool CanPlaceInColumn(int playerIdx, int colIdx);
	bool CanMakeMove(int playerIdx);

	int GetBoardWidth()
	{
		return boardWidth;
	}

	int GetBoardLength()
	{
		return boardLength;
	}

	void DrawNext();
	void DrawSkip();

	bool GetPressNext();
public:
	bool reverseClicked;

private:
	int numPlayers;
	int boardWidth, boardLength;
	int maxTurns;
	int turnsLeft;

	int currentTurnStarter;
	int currentCastPlayer;
	PlayerCardMap playerCardMap;
	CardScoreMap cardPoints;
	PlayerScoreMap playerScoreMap;
	PlayerReverseUsedMap playerReverseUsedMap;
	GameMode gameMode;
	GameScene* gameScene;

	bool pressNext;

	void LoadSettings(LPCWSTR modeName);
};

extern Singleton<GameMgr> _g_GameMgr;
#define g_GameMgr _g_GameMgr.Instance()