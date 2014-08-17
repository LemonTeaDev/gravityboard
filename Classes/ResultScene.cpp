#include "PCH.h"
#include "ResultScene.h"
#include "GameScene.h"
#include "GameMgr.h"
#include <string>
#include "TitleScene.h"

USING_NS_CC;

Scene* ResultScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = ResultScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool ResultScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	// winner sort
	std::vector<int> playerVector;
	for (int i = 1; i <= g_GameMgr.GetNumPlayers(); ++i)
	{
		playerVector.push_back(i);
	}
	std::sort(playerVector.begin(), playerVector.end(), [=](int p1, int p2){
		if (g_GameMgr.GetPlayerScore(p1) > g_GameMgr.GetPlayerScore(p2))
		{
			return true;
		}
		return false;
	});

	auto GetRankString = [](int rank) -> std::string
	{
		if (rank == 1)
		{
			return "[1st]";
		}
		else if (rank == 2)
		{
			return "[2nd]";
		}
		else if (rank == 3)
		{
			return "[3rd]";
		}
		else
		{
			return "[" + std::to_string(rank) + "th]";
		}
	};

	int currentRank = 0;
	int prevPlayerScore = -1;
	std::string resultString = "GAME RESULT\n\n";
	for (auto numPlayer : playerVector)
	{
		int currentPlayerScore = g_GameMgr.GetPlayerScore(numPlayer);
		if (prevPlayerScore == -1 || currentPlayerScore < prevPlayerScore)
		{
			++currentRank;
		}
		prevPlayerScore = currentPlayerScore;

		resultString += GetRankString(currentRank);
		resultString += "  ";
		resultString += std::to_string(numPlayer);
		resultString += "P";
		resultString += "      ";
		resultString += std::to_string(currentPlayerScore);
		resultString += "\n";
	}

	resultString += "\n\nTouch Screen to Continue";

	LabelTTF* resultLabel = LabelTTF::create(resultString, "Consolas", 32);
	resultLabel->setColor(Color3B(39, 223, 19));
	resultLabel->setHorizontalAlignment(TextHAlignment::CENTER);
	resultLabel->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	this->addChild(resultLabel);

	RegisterTouchHandler();
    
    return true;
}

void ResultScene::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void ResultScene::RegisterTouchHandler()
{
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [=](Touch* touch, Event* event)
	{
		return true;
	};
	
	touchListener->onTouchEnded = [=](Touch* touch, Event* event)
	{
		g_GameMgr.EndGame();
		auto titleScene = TitleScene::createScene();
		auto transition = TransitionFade::create(1.0f, titleScene);
		Director::getInstance()->replaceScene(transition);
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
			"FlipPage.wav");
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}
