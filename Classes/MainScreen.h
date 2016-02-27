#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <string.h>
#include <math.h>
#include "cocos2d.h"
#include <vector>
#include "AudioEngine.h"
#include "Box2D/Box2D.h"

using namespace cocos2d;
using namespace std;

class MainScreen : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void menuCloseCallback(cocos2d::Ref* pSender);
    CREATE_FUNC(MainScreen);
    
    void update(float) override;
    
private:
    PhysicsWorld *sceneWorld;
    void SetPhysicsWorld( PhysicsWorld *world ) { sceneWorld = world; };
    bool onContactBegin(PhysicsContact &contact);
    
    Size visibleSize;
    Vec2 origin;
    
    Label* scoreLabel;
    Label* gameOverLabel;
    Label* winLabel;
    Sprite* ball;
    Sprite* paddle;
    Sprite* border;
    Sprite* floor;
    Sprite* brick;
    experimental::AudioEngine* music;
    vector<string> tracks;
    vector<string> effects;
    vector<string> incidentals;
    
    bool play;
    bool hasWon;
    bool isPlaying;
    int score;
    int enter;
    int brickCount;
    int musicCounter;
    int currTrack;
    float velocX;
    float velocY;
    float paddleX;
    vector<string> brickSprites;
    
    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
    void initVars();
    void initAudio();
    void initBorder();
    void initBricks();
    void initScore();
    void initPaddle();
    void initBall();
    void initGameOver();
    void initWin();
    void updateScore();
    void updateVelocity();
    void playAudio();
    void stopMusic();
    void playIncidental();
    void gameOver();
};