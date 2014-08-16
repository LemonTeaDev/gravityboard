#include "PCH.h"
#include "GameMgr.h"

Singleton<GameMgr> _g_GameMgr;

GameMgr::GameMgr()
: gameMode(none)
, reservedGameMode(ffa4)
, numPlayers(4)
, currentCastPlayer(1)
, currentTurnStarter(1)
, reverseClicked(false)
{
}

void GameMgr::StartGame()
{
	gameMode = reservedGameMode;	
	switch (gameMode)
	{
		case ffa3:
		{
			LoadSettings(L"ffa3");
		}
		break;

		case ffa4:
		{
			LoadSettings(L"ffa4");
		}
		break;

		case ffa3s:
		{
			LoadSettings(L"ffa3s");
		}
		break;

		case ffa4s:
		{
			LoadSettings(L"ffa4");
		}
		break;

		case custom:
		{
			LoadSettings(L"custom");
		}
		break;
	}

	for (int_fast8_t i = 1; i <= numPlayers; ++i)
	{
		playerReverseUsedMap[i] = false;
	}
}

void GameMgr::EndGame()
{
	// TODO
}

void GameMgr::LoadSettings(LPCWSTR modeName) {
	numPlayers = GetPrivateProfileInt(modeName, L"Players", 4, L"settings.ini");

	boardLength = GetPrivateProfileInt(modeName, L"Length", 6, L"settings.ini");;
	boardWidth = GetPrivateProfileInt(modeName, L"Width", 3, L"settings.ini");
	turns = 0;

	int_fast8_t buffer;
	buffer = GetPrivateProfileInt(modeName, L"CardPoints", 111111, L"settings.ini");
	for (int i = boardLength; i >= 1; i--) {
		cardPoints[i] = buffer % 10;
		buffer /= 10;
	}
	GetPrivateProfileInt(L"Custom", L"CardMap", buffer, L"settings.ini");
	CardMap cardMap;
	for (int i = boardLength; i >= 1; i--) {
		cardMap[i] = buffer % 10;
		buffer /= 10;
		turns += cardMap[i];
	}
	
	for (int i = 1; i <= numPlayers; i++) {
		playerCardMap[i] = cardMap;
		playerScoreMap[i] = 0;
	}

	std::random_device randomDev;
	auto dist = std::uniform_int_distribution<int>(1, numPlayers);
	currentTurnStarter = dist(randomDev);
}

int_fast8_t GameMgr::GetCurrentCastPlayer()
{
	return currentCastPlayer;
}

int_fast8_t GameMgr::GetCurrentTurnStarter()
{
	return currentTurnStarter;
}

void GameMgr::OnTurnEnd()
{
	// Á¤»ê here

	currentTurnStarter = (currentTurnStarter + 1) % numPlayers;
	currentCastPlayer += currentTurnStarter;
}

void GameMgr::OnPlayerCast()
{
	--currentCastPlayer;
	if (currentCastPlayer < 0)
	{
		currentCastPlayer = numPlayers - 1;
	}
}

GameMgr::GameMode GameMgr::GetGameMode() const
{
	return gameMode;
}
