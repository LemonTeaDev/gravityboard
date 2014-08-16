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
    
	// place ndmtan
	auto ndmTan = Sprite::create("ndm_big.png");
	ndmTan->setAnchorPoint(Vec2(0.f, 0.5f));
	ndmTan->setPosition(Vec2(origin.x + 25, origin.y + visibleSize.height * 0.45));

	// place ndmMouth
	auto ndmMouth = Sprite::create("ndm_mouth.png");
	ndmMouth->setAnchorPoint(Vec2(0.f, 0.5f));
	ndmMouth->setPosition(Vec2(origin.x + 195, origin.y + visibleSize.height * 0.43));

	// place tiles
	tileMgr.Init(
		this,
		ndmMouth->getPosition(),
		ndmMouth->getContentSize());
	tileMgr.CreateTiles();

	// add sprites to the scene
	this->addChild(ndmTan, NDM_Z);
	this->addChild(ndmMouth, NDM_MOUTH_Z);

	// start game
	g_GameMgr.StartGame();

    return true;
}

void GameScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void GameScene::RegisterTouchHandler()
{

}
