#pragma once

#include "cocos2d.h"
#include "TileMgr.h"

class GameScene : public cocos2d::Layer
{
private:
	static const int NDM_Z = 2;
	static const int NDM_MOUTH_Z = 3;

public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cc::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cc::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);

private:
	void RegisterTouchHandler();

private:
	TileMgr tileMgr;
};

