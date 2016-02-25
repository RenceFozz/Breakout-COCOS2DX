#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <string.h>
#include "cocos2d.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace std;

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    CREATE_FUNC(HelloWorld);

private:
    int currOp = 1;
    Size visibleSize;
    Vec2 origin;
    
    Label* title;
    Label* startLabel;
    Sprite* selectSprite;
    CocosDenshion::SimpleAudioEngine* audio = CocosDenshion::SimpleAudioEngine::getInstance();
    
    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    void initAudio();
    void initTitles();
};

