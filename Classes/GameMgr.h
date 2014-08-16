#pragma once
#include "Singleton.hpp"
#include "GameScene.h"

class GameMgr
{
private:
	const int BOARD_Z = 2;
	const int BOARD_CONTENT_Z = 3;

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

	void OnPlayerCast();
	void OnTurnEnd();

	GameMode GetGameMode() const;
	
	bool IsTeamMember(int playerIdx1, int playerIdx2);

	GameMode reservedGameMode;	// can change in title screen

	std::string GetPlayerScoreString(int playerIdx);
	std::string GetCardInfoString();
	void UpdateScoreBoard();
	void UpdateCardInfo();
	
	int GetCardScore(int cardIdx);

	bool CanMakeMove(int playerIdx);

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

	void LoadSettings(LPCWSTR modeName);
};

extern Singleton<GameMgr> _g_GameMgr;
#define g_GameMgr _g_GameMgr.Instance()