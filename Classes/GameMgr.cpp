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
// , reservedGameMode(ffa4)
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

			auto tile = gameScene->GetTileMgr().GetTiles()[i][j - 1];

			auto children = tile->getChildren();
			gravity[j] = gravity[j - 1];
			for (auto child : children)
			{
				if (child->getActionManager()->getNumberOfRunningActionsInTarget(child) > 0)
				{
					continue;
				}

				bool isReverseStone = false;
				PlayStone* playStone = dynamic_cast<PlayStone*>(child);
				if (playStone != nullptr && playStone->IsReverse())
				{
					isReverseStone = true;
				}
				gravity[j] += (isReverseStone ? -1 : 1);
			}
		}

		for (int j = 1; j <= boardLength; ++j) { // move pieces while marking moved pieces
			if ((gameScene->GetTileMgr()).GetTiles()[i][j]->getChildrenCount()) {
				int destination = std::max(j - gravity[j], 0);

				auto childrenVec = gameScene->GetTileMgr().GetTiles()[i][j]->getChildren();
				Node* spriteToMove = childrenVec.at(0);
				if (spriteToMove->getTag() != -10
					&& spriteToMove->getActionManager()->getNumberOfRunningActionsInTarget(spriteToMove) < 1)
				{
					if (destination > boardLength)
					{
						CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
							"blowaway.wav");
						auto jumpAction = JumpBy::create(3.0f, Vec2(1500, 500), 20.f, 2);
						auto callback = CallFuncN::create([](Node *pSender){
							
							Sprite *spr = (Sprite *)pSender;
							if (spr != nullptr)
							{
								spr->removeFromParent();
							}
						});
						auto seq = Sequence::create(jumpAction, callback, NULL);
						spriteToMove->runAction(seq);
					}
					else
					{
						spriteToMove->removeFromParentAndCleanup(false);
						(gameScene->GetTileMgr()).GetTiles()[i][destination]->addChild(spriteToMove);
						spriteToMove->setTag(-10); // marks as already moved
					}
				}
			}
		}

		for (int j = 0; j <= boardLength; ++j) { // delete destroyed pieces, unmark marked pieces, and calculate points
			if (j == 0)
			{
				auto children = (gameScene->GetTileMgr()).GetTiles()[i][j]->getChildren();
				for (auto child : children)
				{
					if (child->getActionManager()->getNumberOfRunningActionsInTarget(child) > 0)
					{
						continue;
					}

					auto playStone = dynamic_cast<PlayStone*>(child);
					if (playStone != nullptr)
					{
						auto stoneOwner = playStone->GetOwnerPlayer();
						auto stoneScore = playStone->GetScore();
						playerScoreMap[stoneOwner] += stoneScore;
						CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
							"eat.wav");
					}

					if (child != nullptr)
					{
						auto jumpAction = JumpBy::create(1.3f, Vec2(-200, -150), 30.f, 2);
						auto callback = CallFuncN::create([](Node *pSender){

							Sprite *spr = (Sprite *)pSender;
							if (spr != nullptr)
							{
								spr->removeFromParent();
							}
						});
						auto seq = Sequence::create(jumpAction, callback, NULL);
						child->runAction(seq);
					}
				}
			}
			else
			{
				// destroy crashed players
				// todo some fancy effects
				if ((gameScene->GetTileMgr()).GetTiles()[i][j]->getChildrenCount() >= 2)
				{
					auto children = (gameScene->GetTileMgr()).GetTiles()[i][j]->getChildren();
					for (auto child : children)
					{
						if (child->getActionManager()->getNumberOfRunningActionsInTarget(child) > 0)
						{
							continue;
						}

						if (child != nullptr)
						{
							auto blinkAction = Blink::create(0.2f, 8);
							auto callback = CallFuncN::create([](Node *pSender){

								Sprite *spr = (Sprite *)pSender;
								if (spr != nullptr)
								{
									spr->removeFromParent();
								}
							});
							auto seq = Sequence::create(blinkAction, callback, NULL);
							child->runAction(seq);
						}

					}
					//(gameScene->GetTileMgr()).GetTiles()[i][j]->removeAllChildren();
					CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
						"boom.wav");
				}
				else if ((gameScene->GetTileMgr()).GetTiles()[i][j]->getChildrenCount() == 1)
				{
					((gameScene->GetTileMgr()).GetTiles()[i][j]->getChildren()).at(0)->setTag(0);
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
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
			"FlipPage.wav");
	}
	else
	{
		if (!CanMakeMove(GetCurrentCastPlayer()))
		{
			g_GameMgr.DrawSkip();
		}
	}

	if (gameScene != nullptr)
	{
		gameScene->UpdateScoreBoard();
		gameScene->UpdateCardInfo();
		gameScene->UpdateReverseButton();
	}
}

void GameMgr::OnPlayerCast()
{
	--currentCastPlayer;
	if (currentCastPlayer <= 0)
	{
		currentCastPlayer += numPlayers;
	}



	// update card usage info
	// update reverse btn
	if (gameScene != nullptr)
	{
		gameScene->UpdateCardInfo();
		gameScene->UpdateReverseButton();
	}

	if (((currentCastPlayer + numPlayers) - currentTurnStarter) % numPlayers == 0)
	{
		OnTurnEnd();
	}
	else
	{
		if (!CanMakeMove(GetCurrentCastPlayer()))
		{
			g_GameMgr.DrawSkip();
		}
	}
}

GameMgr::GameMode GameMgr::GetGameMode() const
{
	return gameMode;
}

void GameMgr::UseReverse(int playerIdx)
{
	playerReverseUsedMap[playerIdx] = true;
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


void GameMgr::UpdatePlayerCard(int playerIdx, int colIdx)
{
	playerCardMap[playerIdx][colIdx]--;

//	CC_ASSERT(playerCardMap[playerIdx][colIdx] >= 0, "One of the remaining cards just became negative!");
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
				const TileMgr::SpriteVec2D& tiles = gameScene->GetTileMgr().GetTiles();
				if (!((tiles[j][i]->getChildrenCount())))
				{
					return true;
				}
			}
		}
	}
	
	return false;
}

void GameMgr::DrawNext()
{
	if (g_GameMgr.GetGameMode() == GameMgr::none) { return; }

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto nextItem = MenuItemImage::create(
		"skip.png", // TODO: change to next.png
		"skipPushed.png", // TODO: change to nextPushed.png
		[&](Ref* sender) {
		g_GameMgr.OnPlayerCast();
	}
	);

	nextItem->setPosition(Vec2(origin.x - 290 + visibleSize.width - nextItem->getContentSize().width / 2,
		origin.y + nextItem->getContentSize().height / 2));

	auto menu = Menu::create(nextItem, NULL);
	menu->setPosition(Vec2::ZERO);
	gameScene->addChild(menu, 1);
}

void GameMgr::DrawSkip()
{
	if (g_GameMgr.GetGameMode() == GameMgr::none) { return; }

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto skipItem = MenuItemImage::create(
		"skip.png",
		"skipPushed.png",
		[&](Ref* sender) {
		g_GameMgr.OnPlayerCast();
	}
	);

	skipItem->setPosition(Vec2(origin.x + visibleSize.width - skipItem->getContentSize().width / 2,
		origin.y + skipItem->getContentSize().height / 2));

	auto menu = Menu::create(skipItem, NULL);
	menu->setPosition(Vec2::ZERO);
	gameScene->addChild(menu, 1);
}