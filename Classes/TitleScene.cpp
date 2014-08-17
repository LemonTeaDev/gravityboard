#include "PCH.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "GameMgr.h"

USING_NS_CC;

Scene* TitleScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = TitleScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool TitleScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(TitleScene::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add "titleBackground" splash screen"
    auto sprite = Sprite::create("titleBackground.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);

	RegisterTouchHandler();
	
	// ffa3
	auto ffa3Item = MenuItemImage::create(
		"titleArrowUL.png",
		"titleArrowULPushed.png",
		[&](Ref* sender) {
			g_GameMgr.reservedGameMode = GameMgr::ffa3;
		}
	);

	ffa3Item->setPosition(Vec2(origin.x + visibleSize.width - ffa3Item->getContentSize().width / 2,
		origin.y + ffa3Item->getContentSize().height / 2));

	auto menu = Menu::create(ffa3Item, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	// ffa4
	auto ffa4Item = MenuItemImage::create(
		"titleArrowUR.png",
		"titleArrowURPushed.png",
		[&](Ref* sender) {
			g_GameMgr.reservedGameMode = GameMgr::ffa4;
		}
	);

	ffa4Item->setPosition(Vec2(origin.x + visibleSize.width - ffa4Item->getContentSize().width / 2,
		origin.y + ffa4Item->getContentSize().height / 2));

	auto menu = Menu::create(ffa4Item, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	// ffa3s
	auto ffa3sItem = MenuItemImage::create(
		"titleArrowDL.png",
		"titleArrowDLPushed.png",
		[&](Ref* sender) {
			g_GameMgr.reservedGameMode = GameMgr::ffa3s;
		}
	);

	ffa3sItem->setPosition(Vec2(origin.x + visibleSize.width - ffa3sItem->getContentSize().width / 2,
		origin.y + ffa3sItem->getContentSize().height / 2));

	auto menu = Menu::create(ffa3sItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	// ffa4
	auto ffa4sItem = MenuItemImage::create(
		"titleArrowDR.png",
		"titleArrowDRPushed.png",
		[&](Ref* sender) {
			g_GameMgr.reservedGameMode = GameMgr::ffa4s;
		}
	);

	ffa4sItem->setPosition(Vec2(origin.x + visibleSize.width - ffa4sItem->getContentSize().width / 2,
		origin.y + ffa4sItem->getContentSize().height / 2));

	auto menu = Menu::create(ffa4sItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);





    return true;
}

void TitleScene::menuCloseCallback(Ref* pSender)
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

void TitleScene::RegisterTouchHandler()
{
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [=](Touch* touch, Event* event)
	{
		return true;
	};
	
	touchListener->onTouchEnded = [=](Touch* touch, Event* event)
	{
		auto gameScene = GameScene::createScene();
		auto transition = TransitionFade::create(1.0f, gameScene);
		Director::getInstance()->replaceScene(transition);
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}
