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

	Sprite* p1Icon = Sprite::create("player1.png");
	p1Icon->setTag(GameScene::PLAYER_1_SCORE_ICON_TAG);

	LabelTTF* p1Score = LabelTTF::create("00", "Arial", 12);
	p1Score->setTag(GameScene::PLAYER_1_SCORE_LABEL_TAG);

	Sprite* p2Icon = Sprite::create("player2.png");
	p2Icon->setTag(GameScene::PLAYER_2_SCORE_ICON_TAG);

	LabelTTF* p2Score = LabelTTF::create("00", "Arial", 12);
	p2Score->setTag(GameScene::PLAYER_2_SCORE_LABEL_TAG);

	Sprite* p3Icon = Sprite::create("player3.png");
	p3Icon->setTag(GameScene::PLAYER_3_SCORE_ICON_TAG);

	LabelTTF* p3Score = LabelTTF::create("00", "Arial", 12);
	p3Score->setTag(GameScene::PLAYER_3_SCORE_LABEL_TAG);

	if (numPlayers > 3)
	{
		Sprite* p4Icon = Sprite::create("player4.png");
		p4Icon->setTag(GameScene::PLAYER_4_SCORE_ICON_TAG);

		LabelTTF* p4Score = LabelTTF::create("00", "Arial", 12);
		p4Score->setTag(GameScene::PLAYER_4_SCORE_LABEL_TAG);
	}

	gameScene->addChild(board);
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
	for (int i = 0; i < boardWidth; ++i) {
		std::map<int, int> gravity; // magnitudes of gravity at each position
		gravity[1] = 1; // gravity at first column is always 1
		for (int j = 2; j <= boardLength; ++j) { // record gravity magnitudes
			gravity[j] = gravity[j - 1] + (gameScene->GetTileMgr()).GetTiles()[i][j]->getChildrenCount() * (reverseClicked ? -1 : 1);
		}

		for (int j = 1; j <= boardLength; ++j) { // move pieces
			if ((gameScene->GetTileMgr()).GetTiles()[i][j]->getChildrenCount()) {
				int destination = j - gravity[j];

				

				((gameScene->GetTileMgr()).GetTiles()[i][j]->getChildren())[0]
				(gameScene->GetTileMgr()).GetTiles()[i][destination]->addChild((gameScene->GetTileMgr()).GetTiles()[i][j]->getChildByTag());
				(gameScene->GetTileMgr()).GetTiles()[i][j]->removeAllChildren();
			}
		}

		for (int j = 0; j <= boardLength; ++j) { // delete destroyed pieces and calculate points
			if (j == 0) {

			}
		}
	}

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
