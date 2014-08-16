#include "PCH.h"
#include "GameMgr.h"
#include "GameScene.h"

Singleton<GameMgr> _g_GameMgr;

USING_NS_CC;

GameMgr::GameMgr()
: gameMode(none)
, reservedGameMode(ffa3)
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

	for (int_fast8_t i = 1; i <= numPlayers; ++i)
	{
		playerReverseUsedMap[i] = false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Sprite* board = Sprite::create("board.png");
	Size boardSize = board->getContentSize();
	board->setAnchorPoint(Vec2(0.f, 0.f));
	board->setPosition(origin.x + visibleSize.width - boardSize.width, origin.y + visibleSize.height - boardSize.height);

	Sprite* p1Icon = Sprite::create("player1.png");
	p1Icon->setTag(GameScene::PLAYER_1_SCORE_ICON_TAG);
	p1Icon->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
	gameScene->addChild(p1Icon, BOARD_CONTENT_Z);

	LabelTTF* p1Score = LabelTTF::create("00", "Arial", 32);
	p1Score->setColor(Color3B(0, 0, 0));
	p1Score->setTag(GameScene::PLAYER_1_SCORE_LABEL_TAG);
	p1Score->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
	gameScene->addChild(p1Score, BOARD_CONTENT_Z);

	Sprite* p2Icon = Sprite::create("player2.png");
	p2Icon->setTag(GameScene::PLAYER_2_SCORE_ICON_TAG);
	p2Icon->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
	gameScene->addChild(p2Icon, BOARD_CONTENT_Z);

	LabelTTF* p2Score = LabelTTF::create("00", "Arial", 32);
	p2Score->setColor(Color3B(0, 0, 0));
	p2Score->setTag(GameScene::PLAYER_2_SCORE_LABEL_TAG);
	p2Score->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
	gameScene->addChild(p2Score, BOARD_CONTENT_Z);

	Sprite* p3Icon = Sprite::create("player3.png");
	p3Icon->setTag(GameScene::PLAYER_3_SCORE_ICON_TAG);
	p3Icon->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
	gameScene->addChild(p3Icon, BOARD_CONTENT_Z);

	LabelTTF* p3Score = LabelTTF::create("00", "Arial", 32);
	p3Score->setColor(Color3B(0, 0, 0));
	p3Score->setTag(GameScene::PLAYER_3_SCORE_LABEL_TAG);
	p3Score->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
	gameScene->addChild(p3Score, BOARD_CONTENT_Z);

	Sprite* p4Icon = nullptr;
	LabelTTF* p4Score = nullptr;
	if (numPlayers > 3)
	{
		p4Icon = Sprite::create("player4.png");
		p4Icon->setTag(GameScene::PLAYER_4_SCORE_ICON_TAG);
		p4Icon->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
		gameScene->addChild(p4Icon, BOARD_CONTENT_Z);

		p4Score = LabelTTF::create("00", "Arial", 32);
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

	{

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

	int_fast8_t buffer;
	if (gameMode == ffa3s || gameMode == ffa4s) {
		buffer = GetPrivateProfileInt(modeName, L"CardPoints", 123456, L"gravity.ini");
	} else {
		buffer = GetPrivateProfileInt(modeName, L"CardPoints", 111111, L"gravity.ini");
	}
	for (int i = boardLength; i >= 1; i--) {
		cardPoints[i] = buffer % 10;
		buffer /= 10;
	}
	GetPrivateProfileInt(L"Custom", L"CardMap", buffer, L"gravity.ini");
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
	currentCastPlayer = currentTurnStarter;
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
	(gameScene->GetTileMgr()).GetTiles();

	currentTurnStarter = (currentTurnStarter + 1) % numPlayers;
	currentCastPlayer += currentTurnStarter;
}

void GameMgr::OnPlayerCast()
{
	--currentCastPlayer;
	if (currentCastPlayer < 0)
	{
		currentCastPlayer += numPlayers;
	}
}

GameMgr::GameMode GameMgr::GetGameMode() const
{
	return gameMode;
}
