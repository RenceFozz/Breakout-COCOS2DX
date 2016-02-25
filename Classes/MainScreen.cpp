#include "MainScreen.h"

#define LEFT        EventKeyboard::KeyCode::KEY_LEFT_ARROW
#define RIGHT       EventKeyboard::KeyCode::KEY_RIGHT_ARROW
#define KEY_P       EventKeyboard::KeyCode::KEY_P
#define KEY_M       EventKeyboard::KeyCode::KEY_M
#define KEY_R       EventKeyboard::KeyCode::KEY_R
#define KEY_ESC     EventKeyboard::KeyCode::KEY_ESCAPE
#define KEY_ENTER   EventKeyboard::KeyCode::KEY_ENTER
#define KEY_SPACE   EventKeyboard::KeyCode::KEY_SPACE

USING_NS_CC;

enum Category   {BALL, PADDLE, BRICK, WALL};

Scene* MainScreen::createScene()
{
    auto scene = Scene::createWithPhysics();
    //scene->getPhysicsWorld()->setDebugDrawMask( PhysicsWorld::DEBUGDRAW_ALL );
    scene->getPhysicsWorld()->setGravity(Vec2(0,0));
    
    auto layer = MainScreen::create();
    layer->SetPhysicsWorld( scene->getPhysicsWorld() );
    
    scene->addChild(layer);
    return scene;
}

bool MainScreen::init()
{
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    MainScreen::initVars();
    MainScreen::initBorder();
    MainScreen::initScore();
    MainScreen::initPaddle();
    MainScreen::initBall();
    MainScreen::initBricks();
    MainScreen::initAudio();
    MainScreen::initGameOver();
    MainScreen::initWin();
    
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(MainScreen::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(MainScreen::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(MainScreen::onContactBegin, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
    
    return true;
}

void MainScreen::update(float delta){
    if(play){
        auto paddleP = paddle->getPosition();
        if(paddle->getPositionX() > 63 && paddle->getPositionX() < 417){
            paddleP.x += paddleX * delta;
        } else {
            if (paddle->getPositionX() >= visibleSize.width / 2){
                paddleP.x = 416;
            }
            else if (paddle->getPositionX() <= visibleSize.width / 2){
                paddleP.x = 64;
            }
        }
        paddle->setPosition(paddleP);
        
        if(brickCount==0){
            hasWon = true;
            MainScreen::gameOver();
        }
    }
}

void MainScreen::initVars(){
    play = true;
    hasWon = false;
    score = 0;
    enter = 0;
    brickCount = 0;
    velocX = 165;
    velocY = 165;
    paddleX = 0;
    musicCounter = 0;
    currTrack = -1;
}

void MainScreen::initAudio(){
    music = new experimental::AudioEngine;
    
    //effects
    effects = {"Effects/Boing.wav", "Effects/Wall.wav", "Effects/Wowpulse.wav"};
    
    //incidentals
    incidentals = {"Music/Determination.wav", "Music/victoryFanfare.wav"};
    
    //music
    tracks = {"Music/Ghost_Fight.wav", "Music/Dating_Start.wav", "Music/Enemy_Approaching.wav",
        "Music/Song_A.wav"};
    
    isPlaying = true;
    MainScreen::playAudio();
}

void MainScreen::initBricks(){
    brickSprites = {"Assets/red_brick.png", "Assets/yellow_brick.png", "Assets/blue_brick.png"};
    for(int i=0;i<brickSprites.size();i++){
        int y = 50*(i+1);
        for (int j=1;j<=10;j++){
            brick = Sprite::create(brickSprites[i]);
            brick->setPosition(origin.x + j*39, origin.y + visibleSize.height - y);
        
            auto brickBody = PhysicsBody::createBox(brick->getContentSize(), PhysicsMaterial(10,1,0) );
            brickBody->setTag(Category::BRICK);
            brickBody->setContactTestBitmask(true);
            brickBody->setDynamic(false);
            brick->setPhysicsBody(brickBody);
        
            this->addChild(brick);
            brickCount++;
        }
        //brickCount+=2;
    }
}

void MainScreen::initPaddle(){
    paddle = Sprite::create("Assets/paddle_t.png");
    paddle->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + 8));
    auto spriteBody = PhysicsBody::createBox(paddle->getContentSize(), PhysicsMaterial(15,1,0) );
    paddle->setPhysicsBody(spriteBody);
    
    paddle->getPhysicsBody()->setTag(Category::PADDLE);
    paddle->getPhysicsBody()->setContactTestBitmask(true);
    paddle->getPhysicsBody()->setDynamic(false);
    
    this->addChild(paddle);
}

void MainScreen::initBall(){
    ball = Sprite::create("Assets/ball_white_t.png");
    ball->setPosition(Vec2(origin.x + visibleSize.width/2,
                           origin.y + 10 + paddle->getContentSize().height));
    
    auto spriteBody = PhysicsBody::createCircle(ball->getContentSize().width / 2,
                                                PhysicsMaterial(1.0f,1.0f,0.0f));
    ball->setPhysicsBody(spriteBody);
    
    ball->getPhysicsBody()->setTag(Category::BALL);
    ball->getPhysicsBody()->setContactTestBitmask(true);
    ball->getPhysicsBody()->setVelocity(Vec2(0,0));
    ball->getPhysicsBody()->setGravityEnable(false);
    
    this->addChild(ball);
}

void MainScreen::initBorder(){
    border = Sprite::create();
    border->setPosition(Point(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 ));
    
    auto borderBody = PhysicsBody::createEdgeBox(visibleSize, PhysicsMaterial(1000.0f,1.0f,0.0));
    border->setPhysicsBody(borderBody);
    
    border->getPhysicsBody()->setTag(Category::WALL);
    border->getPhysicsBody()->setContactTestBitmask(true);
    border->getPhysicsBody()->setDynamic(false);
    
    this->addChild(border);
}

void MainScreen::initScore(){
    scoreLabel = Label::createWithTTF("0000", "fonts/INVASION2000.TTF", 24);
    scoreLabel->setPosition(Vec2(origin.x + visibleSize.width - 40 , visibleSize.height - 5));
    
    this->addChild(scoreLabel);
}

void MainScreen::initGameOver(){
    gameOverLabel = Label::createWithTTF("GAME\nOVER", "fonts/ARCADECLASSIC.TTF", 52);
    gameOverLabel->setPosition(Vec2(origin.x + visibleSize.width / 2 , origin.y + visibleSize.height / 2));
    gameOverLabel->setOpacity(0);
    
    this->addChild(gameOverLabel,2);
}

void MainScreen::initWin(){
    winLabel = Label::createWithTTF("A WINNER\n   IS YOU!", "fonts/ARCADECLASSIC.TTF", 52);
    winLabel->setPosition(Vec2(origin.x + visibleSize.width / 2 , origin.y + visibleSize.height / 2));
    winLabel->setOpacity(0);
    
    this->addChild(winLabel,2);
}

void MainScreen::gameOver(){
    play = false;
    MainScreen::playIncidental();
    
    updateVelocity();
    if(!hasWon){
        auto fadeIn = FadeIn::create(3.0f);
        gameOverLabel->runAction(fadeIn);
    } else {
        auto fadeIn = FadeIn::create(2.0f);
        winLabel->runAction(fadeIn);
    }
}

void MainScreen::updateScore(){
    score+=25;
    if(score<100){
        scoreLabel->setString("00"+to_string(score));
    }
    else if(score>= 100 && score<1000){
        scoreLabel->setString("0"+to_string(score));
    }
    else {
        scoreLabel->setString(to_string(score));
    }
}

void MainScreen::updateVelocity(){
    if( ball->getPhysicsBody()->getVelocity().x == 0 ){
        ball->getPhysicsBody()->setVelocity(Vec2(velocX,velocY));
    }
    else{
        velocX = ball->getPhysicsBody()->getVelocity().x;
        velocY = ball->getPhysicsBody()->getVelocity().y;
        ball->getPhysicsBody()->setVelocity(Vec2(0,0));
    }
}

void MainScreen::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event){
    if (keyCode==KEY_ESC) { Director::getInstance()->end(); }
    if(play){
        switch (keyCode){
            case KEY_P:
            case KEY_SPACE:
                if(enter>=1){
                    if(!Director::getInstance()->isPaused()){
                        Director::getInstance()->pause();
                    } else {
                        Director::getInstance()->resume();
                    }
                    updateVelocity();
                }
                break;
            case KEY_ENTER:
                enter++;
                if(enter==1){
                    this->scheduleUpdate();
                    updateVelocity();
                }
                break;
            case LEFT: paddleX = -500; break;
            case RIGHT: paddleX = 500; break;
            case KEY_M:
                if(isPlaying){
                    musicCounter++;
                    music->pause(currTrack);
                    currTrack = -1;
                    isPlaying = false;
                } else {
                    MainScreen::playAudio();
                    isPlaying = true;
                }
                break;
            default: ;//cout << "Not among the ff. options." << endl;
        }
    }
    if (keyCode==KEY_R){
        music->stopAll();
        Director::getInstance()->replaceScene(
                TransitionFade::create(0.5, MainScreen::createScene()));
    }
}

void MainScreen::playAudio(){
    if(currTrack==-1){
        switch(musicCounter % 4){
            case 0: currTrack = music->play2d(tracks[musicCounter % 4], true, 0.5f); break;
            case 1: currTrack = music->play2d(tracks[musicCounter % 4], true, 0.5f); break;
            case 2: currTrack = music->play2d(tracks[musicCounter % 4], true, 0.5f); break;
            case 3: currTrack = music->play2d(tracks[musicCounter % 4], true, 0.5f); break;
        }
    } else {
        music->resume(currTrack);
    }
}

void MainScreen::playIncidental(){
    music->stopAll();
    if(hasWon){ music->play2d(incidentals[1]); }
    else if (!hasWon && brickCount!=0) { music->play2d(incidentals[0]); }
}

void MainScreen::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event){
    if(keyCode==LEFT || keyCode==RIGHT){
        paddleX = 0;
    }
}

bool MainScreen::onContactBegin(PhysicsContact &contact){
    int a = contact.getShapeA()->getBody()->getTag();
    int b = contact.getShapeB()->getBody()->getTag();
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();
    
    if( (a==Category::BALL && b==Category::WALL) ||
        (b==Category::BALL && a==Category::WALL)){
        music->play2d(effects[1], false, 1.0f);
        if(ball->getPositionY() < 10){
            MainScreen::gameOver();
        }
    }
    
    else if( (a==Category::BALL && b==Category::PADDLE) ||
             (b==Category::BALL && a==Category::PADDLE)){
        music->play2d(effects[0], false, 1.0f);
    }
    
    else if(a==Category::BALL && b==Category::BRICK){
        music->play2d(effects[2], false, 1.0f);
        updateScore();
        nodeB->removeFromParent();
        brickCount--;
    }
    
    else if(b==Category::BALL && a==Category::BRICK){
        music->play2d(effects[2], false, 1.0f);
        updateScore();
        nodeA->removeFromParent();
        brickCount--;
    }
    return true;
}