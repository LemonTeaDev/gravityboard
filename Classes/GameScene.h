#pragma once

#include "cocos2d.h"
#include "TileMgr.h"

class GameScene : public cocos2d::Layer
{
private:
	static const int NDM_Z = 2;
	static const int NDM_MOUTH_Z = 3;

public:
	static const int PLAYER_1_SCORE_ICON_TAG = 101;
	static const int PLAYER_2_SCORE_ICON_TAG = 102;
	static const int PLAYER_3_SCORE_ICON_TAG = 103;
	static const int PLAYER_4_SCORE_ICON_TAG = 104;
	static const int PLAYER_1_SCORE_LABEL_TAG = 151;
	static const int PLAYER_2_SCORE_LABEL_TAG = 152;
	static const int PLAYER_3_SCORE_LABEL_TAG = 153;
	static const int PLAYER_4_SCORE_LABEL_TAG = 154;

	static const int INFO_LABEL_TAG = 201;

public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cc::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cc::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);

public:
	TileMgr GetTileMgr() {
		return tileMgr;
	}

private:
	void RegisterTouchHandler();

private:
	TileMgr tileMgr;
};

