#include "PCH.h"
#include "GameMgr.h"
#include "GameScene.h"

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

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Sprite* board = Sprite::create("board.png");
	Size boardSize = board->getContentSize();
	board->setAnchorPoint(Vec2(0.f, 0.f));
	board->setPosition(origin.x + visibleSize.width - boardSize.width, origin.y + visibleSize.height - boardSize.height);

	Sprite* p1Icon = Sprite::create("headSun.png");
	p1Icon->setTag(GameScene::PLAYER_1_SCORE_ICON_TAG);
	p1Icon->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
	gameScene->addChild(p1Icon, BOARD_CONTENT_Z);

	LabelTTF* p1Score = LabelTTF::create(GetPlayerScoreString(1), "Arial", 32);
	p1Score->setColor(Color3B(0, 0, 0));
	p1Score->setTag(GameScene::PLAYER_1_SCORE_LABEL_TAG);
	p1Score->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
	gameScene->addChild(p1Score, BOARD_CONTENT_Z);

	Sprite* p2Icon = Sprite::create("headMoon.png");
	p2Icon->setTag(GameScene::PLAYER_2_SCORE_ICON_TAG);
	p2Icon->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
	gameScene->addChild(p2Icon, BOARD_CONTENT_Z);

	LabelTTF* p2Score = LabelTTF::create(GetPlayerScoreString(2), "Arial", 32);
	p2Score->setColor(Color3B(0, 0, 0));
	p2Score->setTag(GameScene::PLAYER_2_SCORE_LABEL_TAG);
	p2Score->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
	gameScene->addChild(p2Score, BOARD_CONTENT_Z);

	Sprite* p3Icon = Sprite::create("headStar.png");
	p3Icon->setTag(GameScene::PLAYER_3_SCORE_ICON_TAG);
	p3Icon->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
	gameScene->addChild(p3Icon, BOARD_CONTENT_Z);

	LabelTTF* p3Score = LabelTTF::create(GetPlayerScoreString(3), "Arial", 32);
	p3Score->setColor(Color3B(0, 0, 0));
	p3Score->setTag(GameScene::PLAYER_3_SCORE_LABEL_TAG);
	p3Score->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
	gameScene->addChild(p3Score, BOARD_CONTENT_Z);

	Sprite* p4Icon = nullptr;
	LabelTTF* p4Score = nullptr;
	if (numPlayers > 3)
	{
		p4Icon = Sprite::create("headPlanet.png");
		p4Icon->setTag(GameScene::PLAYER_4_SCORE_ICON_TAG);
		p4Icon->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
		gameScene->addChild(p4Icon, BOARD_CONTENT_Z);

		p4Score = LabelTTF::create(GetPlayerScoreString(4), "Arial", 32);
		p4Score->setColor(Color3B(0, 0, 0));
		p4Score->setTag(GameScene::PLAYER_4_SCORE_LABEL_TAG);
		p4Score->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
		gameScene->addChild(p4Score, BOARD_CONTENT_Z);
	}

	{	// score line setting
		float endX = origin.x + visibleSize.width;
		float scoreY = origin.y + visibleSize.height - p3Score->getContentSize().height - 35;

		const int PADDING1 = 20;
		const int PADDING2 = 34;

		if (numPlayers == 3)
		{
			p3Score->setPosition(
				endX - p3Score->getContentSize().width - 30,
				scoreY);

			p3Icon->setPosition(
				p3Score->getPositionX() - p3Icon->getContentSize().width - PADDING1,
				scoreY);

			p2Score->setPosition(
				p3Icon->getPositionX() - p2Score->getContentSize().width - PADDING2,
				scoreY);

			p2Icon->setPosition(
				p2Score->getPositionX() - p2Icon->getContentSize().width - PADDING1,
				scoreY);

			p1Score->setPosition(
				p2Icon->getPositionX() - p1Score->getContentSize().width - PADDING2,
				scoreY);

			p1Icon->setPosition(
				p1Score->getPositionX() - p1Icon->getContentSize().width - PADDING1,
				scoreY);
		}
		else
		{
			p4Score->setPosition(
				endX - p4Score->getContentSize().width - 30,
				scoreY);
			p4Icon->setPosition(
				p4Score->getPositionX() - p4Icon->getContentSize().width - PADDING1,
				scoreY);

			p3Score->setPosition(
				p4Icon->getPositionX() - p3Score->getContentSize().width - PADDING2,
				scoreY);

			p3Icon->setPosition(
				p3Score->getPositionX() - p3Icon->getContentSize().width - PADDING1,
				scoreY);

			p2Score->setPosition(
				p3Icon->getPositionX() - p2Score->getContentSize().width - PADDING2,
				scoreY);

			p2Icon->setPosition(
				p2Score->getPositionX() - p2Icon->getContentSize().width - PADDING1,
				scoreY);

			p1Score->setPosition(
				p2Icon->getPositionX() - p1Score->getContentSize().width - PADDING2,
				scoreY);

			p1Icon->setPosition(
				p1Score->getPositionX() - p1Icon->getContentSize().width - PADDING1,
				scoreY);
		}
	}

	{	// other info line setting
		float endX = origin.x + visibleSize.width;
		float infoY = origin.y + visibleSize.height - p3Score->getContentSize().height - 120;
		LabelTTF* infoLabel = LabelTTF::create(GetCardInfoString(), "Arial", 26);
		infoLabel->setColor(Color3B(0, 0, 0));
		infoLabel->setTag(GameScene::INFO_LABEL_TAG);
		infoLabel->setPosition(endX - infoLabel->getContentSize().width - 35, infoY);
		infoLabel->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
		gameScene->addChild(infoLabel, BOARD_CONTENT_Z);
	}

	gameScene->addChild(board, BOARD_Z);
	
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
	turns = 0;

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
		turns += cardMap[i];
	}
	
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
	(gameScene->GetTileMgr()).GetTiles();

	currentTurnStarter = (currentTurnStarter + 1) % numPlayers;
	currentCastPlayer += currentTurnStarter;

	UpdateScoreBoard();
	UpdateCardInfo();
}

void GameMgr::OnPlayerCast()
{
	--currentCastPlayer;
	if (currentCastPlayer < 0)
	{
		currentCastPlayer += numPlayers;
	}

	UpdateCardInfo();
}

GameMgr::GameMode GameMgr::GetGameMode() const
{
	return gameMode;
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
	gameInfoStr += std::to_string(turns);

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
