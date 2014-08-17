#pragma once

#include "cocos2d.h"
#include "TileMgr.h"

class GameScene : public cocos2d::Layer
{
public:
	static const int PLAY_STONE_Z = 4;

private:
	static const int NDM_Z = 1;
	static const int NDM_MOUTH_Z = 1;

	static const int BOARD_Z = 4;
	static const int BOARD_CONTENT_Z = 5;

	static const int SKIP_MENU_Z = 2;

	static const int PLAYER_1_SCORE_ICON_TAG = 101;
	static const int PLAYER_2_SCORE_ICON_TAG = 102;
	static const int PLAYER_3_SCORE_ICON_TAG = 103;
	static const int PLAYER_4_SCORE_ICON_TAG = 104;
	static const int PLAYER_1_SCORE_LABEL_TAG = 151;
	static const int PLAYER_2_SCORE_LABEL_TAG = 152;
	static const int PLAYER_3_SCORE_LABEL_TAG = 153;
	static const int PLAYER_4_SCORE_LABEL_TAG = 154;
	static const int INFO_LABEL_TAG = 201;
	static const int REVERSE_MENU_TAG = 300;
	static const int REVERSE_BUTTON_TAG = 301;

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
	TileMgr& GetTileMgr() 
	{
		return tileMgr;
	}

	void UpdateScoreBoard();
	void UpdateCardInfo();
	void UpdateReverseButton();

private:
	void RegisterTouchHandler();
	void CreateReverseButton();

private:
	TileMgr tileMgr;
};

