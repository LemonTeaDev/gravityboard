#pragma once
#include "Singleton.hpp"

class GameMgr
{
private:
	const int_fast8_t DEFAULT_NUM_PLAYERS = 2;
	typedef std::map<int_fast8_t, int_fast8_t> CardMap;
	typedef std::map<int_fast8_t, CardMap> PlayerCardMap;
	typedef std::map<int_fast8_t, int_fast8_t> CardScoreMap;
	typedef std::map<int_fast8_t, int_fast32_t> PlayerScoreMap;

public:
	enum GameMode
	{
		none,
		ffa3,
		ffa4,
		ffa3s,
		ffa4s,
		custom
	};

public:
	GameMgr();

	void Init(GameMode mode);

	int_fast8_t GetCurrentCastPlayer();
	int_fast8_t GetCurrentTurnStarter();

	void OnPlayerCast();
	void OnTurnEnd();

	GameMode GetGameMode() const;
	
	bool IsTeamMember(int_fast8_t playerIdx1, int_fast8_t playerIdx2);

private:
	int_fast8_t numPlayers;
	int_fast8_t boardWidth, boardLength;
	int_fast8_t turns;
	CardScoreMap cardPoints;

	int_fast8_t currentTurnStarter;
	int_fast8_t currentCastPlayer;
	PlayerCardMap playerCardMap;
	PlayerScoreMap playerScoreMap;
	GameMode gameMode;

	void LoadSettings(LPCWSTR modeName);
};

extern Singleton<GameMgr> _g_GameMgr;
#define g_GameMgr _g_GameMgr.Instance()