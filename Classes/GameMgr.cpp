#include "PCH.h"
#include "GameMgr.h"
#include "GameScene.h"
#include "PlayStone.h"
#include "TitleScene.h"
#include "ResultScene.h"

Singleton<GameMgr> _g_GameMgr;

USING_NS_CC;

GameMgr::GameMgr()
: gameMode(none)
, reservedGameMode(ffa4)
, numPlayers(4)
, currentCastPlayer(1)
, currentTurnStarter(1)
, reverseClicked(false)
{
}

void GameMgr::StartGame(GameScene* _gameScene)
{
	gameScene = _gameScene;
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
	}

	for (int i = 1; i <= numPlayers; ++i)
	{
		playerReverseUsedMap[i] = false;
	}
}

void GameMgr::EndGame()
{
	gameScene = nullptr;
	playerCardMap.clear();
	playerReverseUsedMap.clear();
	reservedGameMode = none;
}

void GameMgr::LoadSettings(LPCWSTR modeName) {
	if (gameMode == ffa3 || gameMode == ffa3s) {
		numPlayers = GetPrivateProfileInt(modeName, L"Players", 3, L"gravity.ini");
	} else {
		numPlayers = GetPrivateProfileInt(modeName, L"Players", 4, L"gravity.ini");
	}

	if (gameMode == ffa3 || gameMode == ffa3s) {
		boardWidth = GetPrivateProfileInt(modeName, L"Width", 2, L"gravity.ini");
	} else {
		boardWidth = GetPrivateProfileInt(modeName, L"Width", 3, L"gravity.ini");
	}
	boardLength = GetPrivateProfileInt(modeName, L"Length", 6, L"gravity.ini");;
	maxTurns = 0;

	int buffer;
	if (gameMode == ffa3s || gameMode == ffa4s) {
		buffer = GetPrivateProfileInt(modeName, L"CardPoints", 123456, L"gravity.ini");
	} else {
		buffer = GetPrivateProfileInt(modeName, L"CardPoints", 111111, L"gravity.ini");
	}
	for (int i = boardLength; i >= 1; i--) {
		cardPoints[i] = buffer % 10;
		buffer /= 10;
	}
	buffer = GetPrivateProfileInt(L"Custom", L"CardMap", 111222, L"gravity.ini");
	CardMap cardMap;
	for (int i = boardLength; i >= 1; i--) {
		cardMap[i] = buffer % 10;
		buffer /= 10;
		maxTurns += cardMap[i];
	}
	turnsLeft = maxTurns;
	
	for (int i = 1; i <= numPlayers; i++) {
		for (int j = 1; j <= boardLength; j++) {
			playerCardMap[i][j] = cardMap[j];
		}
		playerScoreMap[i] = 0;
	}

	std::random_device randomDev;
	auto dist = std::uniform_int_distribution<int>(1, numPlayers);
	currentTurnStarter = dist(randomDev);
	currentCastPlayer = currentTurnStarter;
}

int GameMgr::GetCurrentCastPlayer()
{
	return currentCastPlayer;
}

int GameMgr::GetCurrentTurnStarter()
{
	return currentTurnStarter;
}

void GameMgr::OnTurnEnd()
{
	for (int i = 0; i < boardWidth; ++i) {
		std::map<int, int> gravity; // magnitudes of gravity at each position
		gravity[1] = 1; // gravity at first column is always 1
		for (int j = 2; j <= boardLength; ++j) { // record gravity magnitudes
			gravity[j] = gravity[j - 1] + (gameScene->GetTileMgr()).GetTiles()[i][j - 1]->getChildrenCount() 
				* (reverseClicked ? -1 : 1);
		}

		for (int j = 1; j <= boardLength; ++j) { // move pieces
			if ((gameScene->GetTileMgr()).GetTiles()[i][j]->getChildrenCount()) {
				int destination = std::max(j - gravity[j], 0);

				auto childrenVec = gameScene->GetTileMgr().GetTiles()[i][j]->getChildren();
				Node* spriteToMove = childrenVec.at(0);
				spriteToMove->removeFromParentAndCleanup(false);

				(gameScene->GetTileMgr()).GetTiles()[i][destination]->addChild(spriteToMove);
			}
		}

		for (int j = 0; j <= boardLength; ++j) { // delete destroyed pieces and calculate points
			if (j == 0)
			{
				auto children = (gameScene->GetTileMgr()).GetTiles()[i][j]->getChildren();
				for (auto child : children)
				{
					auto playStone = dynamic_cast<PlayStone*>(child);
					if (playStone != nullptr)
					{
						// eat nom nom
						// todo some fancy effects
						auto stoneOwner = playStone->GetOwnerPlayer();
						auto stoneScore = playStone->GetScore();
						playerScoreMap[stoneOwner] += stoneScore;
					}

					if (child != nullptr)
					{
						child->removeFromParent();
					}
				}
			}
			else
			{
				// destroy crashed players
				// todo some fancy effects
				if ((gameScene->GetTileMgr()).GetTiles()[i][j]->getChildrenCount() >= 2)
				{
					(gameScene->GetTileMgr()).GetTiles()[i][j]->removeAllChildren();
				}
			}
		}
	}

	currentTurnStarter = (currentTurnStarter + 1) % numPlayers;
	if (currentTurnStarter == 0)
	{
		currentTurnStarter += numPlayers;
	}

	currentCastPlayer = currentTurnStarter;

	--turnsLeft;
	if (turnsLeft <= 0)
	{
		auto resultScene = ResultScene::createScene();
		auto transition = TransitionFade::create(1.0f, resultScene);
		Director::getInstance()->replaceScene(transition);
	}

	UpdateScoreBoard();
	UpdateCardInfo();
}

void GameMgr::OnPlayerCast()
{
	--currentCastPlayer;
	if (currentCastPlayer <= 0)
	{
		currentCastPlayer += numPlayers;
	}

	// update card usage info
	UpdateCardInfo();

	// update 




	if (((currentCastPlayer + numPlayers) - currentTurnStarter) % numPlayers == 0)
	{
		OnTurnEnd();
	}
}

GameMgr::GameMode GameMgr::GetGameMode() const
{
	return gameMode;
}

bool GameMgr::IsReverseUsed(int playerIdx)
{
	return playerReverseUsedMap[playerIdx];
}

int GameMgr::GetPlayerScore(int playerIdx)
{
	return playerScoreMap[playerIdx];
}

std::string GameMgr::GetPlayerScoreString(int playerIdx)
{
	return std::to_string(playerScoreMap[playerIdx]);
}

std::string GameMgr::GetCardInfoString()
{
	std::string gameInfoStr = "[" + std::to_string(currentCastPlayer) + "P Turn]  ";
	for (int i = 1; i <= boardLength; ++i)
	{
		gameInfoStr += std::to_string(i);
		gameInfoStr += ": ";
		gameInfoStr += std::to_string(playerCardMap[currentCastPlayer][i]);
		gameInfoStr += " / "; 
	}

	gameInfoStr += "Rotations Left: ";
	gameInfoStr += std::to_string(turnsLeft);

	return gameInfoStr;
}

void GameMgr::UpdateCardInfo()
{
	auto infoLabel = dynamic_cast<LabelTTF*>(gameScene->getChildByTag(GameScene::INFO_LABEL_TAG));
	if (infoLabel != nullptr)
	{
		infoLabel->setString(GetCardInfoString());
	}
}

void GameMgr::UpdatePlayerCard(int playerIdx, int colIdx)
{
	playerCardMap[playerIdx][colIdx]--;

//	CC_ASSERT(playerCardMap[playerIdx][colIdx] >= 0, "One of the remaining cards just became negative!");
}

void GameMgr::UpdateScoreBoard()
{
	if (gameScene == nullptr) { return; }
	auto score1 = dynamic_cast<LabelTTF*>(gameScene->getChildByTag(GameScene::PLAYER_1_SCORE_LABEL_TAG));
	auto score2 = dynamic_cast<LabelTTF*>(gameScene->getChildByTag(GameScene::PLAYER_2_SCORE_LABEL_TAG));
	auto score3 = dynamic_cast<LabelTTF*>(gameScene->getChildByTag(GameScene::PLAYER_3_SCORE_LABEL_TAG));
	auto score4 = dynamic_cast<LabelTTF*>(gameScene->getChildByTag(GameScene::PLAYER_4_SCORE_LABEL_TAG));

	if (score1 != nullptr)
	{
		score1->setString(GetPlayerScoreString(1));
	}

	if (score2 != nullptr)
	{
		score2->setString(GetPlayerScoreString(2));
	}

	if (score3 != nullptr)
	{
		score3->setString(GetPlayerScoreString(3));
	}

	if (score4 != nullptr)
	{
		score4->setString(GetPlayerScoreString(4));
	}
}

int GameMgr::GetCardScore(int cardIdx)
{
	return cardPoints[cardIdx];
}

int GameMgr::GetNumPlayers() const
{
	return numPlayers;
}

bool GameMgr::CanPlaceInColumn(int playerIdx, int colIdx)
{
	if (playerCardMap[playerIdx][colIdx]) {
		return true;
	} else {
		return false;
	}
}

bool GameMgr::CanMakeMove(int playerIdx)
{
	for (int i = 1; i <= boardLength; ++i)
	{
		if (playerCardMap[playerIdx][i])
		{
			for (int j = 0; j < boardWidth; j++)
			{
				if (!((gameScene->GetTileMgr()).GetTiles()[i][j]->getChildrenCount()))
				{
					return true;
				}
			}
		}
	}
	
	return false;
}