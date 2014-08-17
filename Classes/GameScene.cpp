#include "PCH.h"
#include "GameScene.h"
#include "GameMgr.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// place background
	auto bgImage = Sprite::create("background.png");
	bgImage->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    
	// place ndmtan
	auto ndmTan = Sprite::create("ndm_big.png");
	ndmTan->setAnchorPoint(Vec2(0.f, 0.5f));
	ndmTan->setPosition(Vec2(origin.x + 25, origin.y + visibleSize.height * 0.45));

	// place ndmMouth
	auto ndmMouth = Sprite::create("ndm_mouth.png");
	ndmMouth->setAnchorPoint(Vec2(0.f, 0.5f));
	ndmMouth->setPosition(Vec2(origin.x + 195, origin.y + visibleSize.height * 0.43));

	// add sprites to the scene
	this->addChild(bgImage, 0);
	this->addChild(ndmTan, NDM_Z);
	this->addChild(ndmMouth, NDM_MOUTH_Z);

	// start game
	g_GameMgr.StartGame(this);

	// place tiles
	tileMgr.Init(
		this,
		ndmMouth->getPosition(),
		ndmMouth->getContentSize(),
		g_GameMgr.GetBoardWidth(),
		g_GameMgr.GetBoardLength() + 1);
	tileMgr.CreateTiles();

	// post start add sprites
	Sprite* board = Sprite::create("board.png");
	Size boardSize = board->getContentSize();
	board->setAnchorPoint(Vec2(0.f, 0.f));
	board->setPosition(origin.x + visibleSize.width - boardSize.width, origin.y + visibleSize.height - boardSize.height);

	Sprite* p1Icon = Sprite::create("headSun.png");
	p1Icon->setTag(GameScene::PLAYER_1_SCORE_ICON_TAG);
	p1Icon->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
	this->addChild(p1Icon, BOARD_CONTENT_Z);

	LabelTTF* p1Score = LabelTTF::create(g_GameMgr.GetPlayerScoreString(1), "Arial", 32);
	p1Score->setColor(Color3B(0, 0, 0));
	p1Score->setTag(GameScene::PLAYER_1_SCORE_LABEL_TAG);
	p1Score->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
	this->addChild(p1Score, BOARD_CONTENT_Z);

	Sprite* p2Icon = Sprite::create("headMoon.png");
	p2Icon->setTag(GameScene::PLAYER_2_SCORE_ICON_TAG);
	p2Icon->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
	this->addChild(p2Icon, BOARD_CONTENT_Z);

	LabelTTF* p2Score = LabelTTF::create(g_GameMgr.GetPlayerScoreString(2), "Arial", 32);
	p2Score->setColor(Color3B(0, 0, 0));
	p2Score->setTag(GameScene::PLAYER_2_SCORE_LABEL_TAG);
	p2Score->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
	this->addChild(p2Score, BOARD_CONTENT_Z);

	Sprite* p3Icon = Sprite::create("headStar.png");
	p3Icon->setTag(GameScene::PLAYER_3_SCORE_ICON_TAG);
	p3Icon->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
	this->addChild(p3Icon, BOARD_CONTENT_Z);

	LabelTTF* p3Score = LabelTTF::create(g_GameMgr.GetPlayerScoreString(3), "Arial", 32);
	p3Score->setColor(Color3B(0, 0, 0));
	p3Score->setTag(GameScene::PLAYER_3_SCORE_LABEL_TAG);
	p3Score->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
	this->addChild(p3Score, BOARD_CONTENT_Z);

	Sprite* p4Icon = nullptr;
	LabelTTF* p4Score = nullptr;
	if (g_GameMgr.GetNumPlayers() > 3)
	{
		p4Icon = Sprite::create("headPlanet.png");
		p4Icon->setTag(GameScene::PLAYER_4_SCORE_ICON_TAG);
		p4Icon->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
		this->addChild(p4Icon, BOARD_CONTENT_Z);

		p4Score = LabelTTF::create(g_GameMgr.GetPlayerScoreString(4), "Arial", 32);
		p4Score->setColor(Color3B(0, 0, 0));
		p4Score->setTag(GameScene::PLAYER_4_SCORE_LABEL_TAG);
		p4Score->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
		this->addChild(p4Score, BOARD_CONTENT_Z);
	}

	{	// score line setting
		float endX = origin.x + visibleSize.width;
		float scoreY = origin.y + visibleSize.height - p3Score->getContentSize().height - 35;

		const int PADDING1 = 20;
		const int PADDING2 = 34;

		if (g_GameMgr.GetNumPlayers() == 3)
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
		LabelTTF* infoLabel = LabelTTF::create(g_GameMgr.GetCardInfoString(), "Arial", 26);
		infoLabel->setColor(Color3B(0, 0, 0));
		infoLabel->setTag(INFO_LABEL_TAG);
		infoLabel->setPosition(endX - infoLabel->getContentSize().width - 35, infoY);
		infoLabel->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
		this->addChild(infoLabel, BOARD_CONTENT_Z);
	}
	this->addChild(board, BOARD_Z);

	// add menu
	MenuItemImage* reverseButton = MenuItemImage::create(
		"reverseHairbandOff.png",
		"reverseHairbandOn.png",
		[&](Ref *sender) {
			if (g_GameMgr.CanMakeMove(g_GameMgr.GetCurrentCastPlayer()))
			{
				g_GameMgr.reverseClicked = true;
				g_GameMgr.UseReverse(g_GameMgr.GetCurrentCastPlayer());
				UpdateReverseButton();
			}
			else
			{
				MessageBeep(MB_ICONINFORMATION);
				MessageBoxA(GetActiveWindow(), "You are stuck now. Please skip this turn.", "Skip", MB_ICONINFORMATION);
			}
		});

	reverseButton->setTag(REVERSE_BUTTON_TAG);
	reverseButton->setPosition(Vec2(origin.x + 250, origin.y + visibleSize.height * 0.8));

	// create menu, it's an autorelease object
	auto menu = Menu::create(reverseButton, NULL);
	menu->setPosition(Vec2::ZERO);
	menu->setTag(REVERSE_MENU_TAG);
	this->addChild(menu, SKIP_MENU_Z);


    return true;
}

void GameScene::UpdateScoreBoard()
{
	auto score1 = dynamic_cast<LabelTTF*>(this->getChildByTag(PLAYER_1_SCORE_LABEL_TAG));
	auto score2 = dynamic_cast<LabelTTF*>(this->getChildByTag(PLAYER_2_SCORE_LABEL_TAG));
	auto score3 = dynamic_cast<LabelTTF*>(this->getChildByTag(PLAYER_3_SCORE_LABEL_TAG));
	auto score4 = dynamic_cast<LabelTTF*>(this->getChildByTag(PLAYER_4_SCORE_LABEL_TAG));

	if (score1 != nullptr)
	{
		score1->setString(g_GameMgr.GetPlayerScoreString(1));
	}

	if (score2 != nullptr)
	{
		score2->setString(g_GameMgr.GetPlayerScoreString(2));
	}

	if (score3 != nullptr)
	{
		score3->setString(g_GameMgr.GetPlayerScoreString(3));
	}

	if (score4 != nullptr)
	{
		score4->setString(g_GameMgr.GetPlayerScoreString(4));
	}
}

void GameScene::UpdateCardInfo()
{
	auto infoLabel = dynamic_cast<LabelTTF*>(this->getChildByTag(INFO_LABEL_TAG));
	if (infoLabel != nullptr)
	{
		infoLabel->setString(g_GameMgr.GetCardInfoString());
	}
}

void GameScene::UpdateReverseButton()
{
	auto menu = getChildByTag(REVERSE_MENU_TAG);
	if (menu == nullptr) { return; }

	auto reverseBtn = menu->getChildByTag(REVERSE_BUTTON_TAG);
	if (reverseBtn == nullptr)
	{
		return;
	}

	int currentCastPlayer = g_GameMgr.GetCurrentCastPlayer();
	if (g_GameMgr.IsReverseUsed(currentCastPlayer))
	{
		reverseBtn->setVisible(false);
	}
	else
	{
		reverseBtn->setVisible(true);
	}
}
