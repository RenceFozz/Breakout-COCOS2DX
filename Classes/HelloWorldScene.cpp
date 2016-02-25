#include "HelloWorldScene.h"
#include "MainScreen.h"

#define LEFT    EventKeyboard::KeyCode::KEY_LEFT_ARROW
#define RIGHT   EventKeyboard::KeyCode::KEY_RIGHT_ARROW
#define ENTER   EventKeyboard::KeyCode::KEY_ENTER
#define ESC     EventKeyboard::KeyCode::KEY_ESCAPE

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    HelloWorld::initTitles();
    HelloWorld::initAudio();

    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
    //listener->onKeyReleased = CC_CALLBACK_2(KeyboardTest::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void HelloWorld::initAudio(){
    audio->preloadBackgroundMusic("Music/Theme_Song.wav");
    audio->playBackgroundMusic("Music/Theme_Song.wav", true);
    audio->setBackgroundMusicVolume(0.5);
}

void HelloWorld::initTitles(){
    title = Label::createWithTTF("BREAKOUT", "fonts/ARCADECLASSIC.TTF", 72);
    title->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - 75));
    this->addChild(title, 1);
    
    startLabel = Label::createWithTTF("START", "fonts/ARCADECLASSIC.TTF", 32);
    startLabel->setPosition(Vec2(origin.x + visibleSize.width/2,
                                 origin.y + 40));
    this->addChild(startLabel, 1);
    
    selectSprite = Sprite::create("Assets/ball_red_t.png");
    selectSprite->setPosition(Vec2(origin.x + visibleSize.width/2,
                                   startLabel->getPositionY()/2 ));
    this->addChild(selectSprite, 0);
}

void HelloWorld::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event){
    if(keyCode==ENTER){
        audio->stopBackgroundMusic();
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, MainScreen::createScene()));
    } else if (keyCode==ESC) {
        Director::getInstance()->end();
    }
}