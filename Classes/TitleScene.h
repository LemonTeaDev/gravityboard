#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class TitleScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cc::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cc::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(TitleScene);

	// Draw Buttons
	void DrawButtonffa3();
	void DrawButtonffa4();
	void DrawButtonffa3s();
	void DrawButtonffa4s();

private:
	void RegisterTouchHandler();
};

#endif // __HELLOWORLD_SCENE_H__
