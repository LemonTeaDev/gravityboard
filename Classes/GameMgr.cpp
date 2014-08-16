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
	std::random_device randomDev;

	switch (gameMode)
	{
		case ffa3:
		{
			auto dist = std::uniform_int_distribution<int>(1, 3);
			currentTurnStarter = dist(randomDev);
			numPlayers = 3;
		}
		break;

		case ffa4:
		{
			auto dist = std::uniform_int_distribution<int>(1, 4);
			currentTurnStarter = dist(randomDev);
			numPlayers = 4;
		}
		break;

		case pvp:
		{
			auto dist = std::uniform_int_distribution<int>(1, 2);
			currentTurnStarter = dist(randomDev);
			numPlayers = 2;
		}
		break;

		case teampvp:
		{
			auto dist = std::uniform_int_distribution<int>(1, 2);
			currentTurnStarter = dist(randomDev);
			if (currentTurnStarter == 2)
			{
				currentTurnStarter = 3;
			}
			numPlayers = 4;
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
	currentCastPlayer = currentTurnStarter;	
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
