//
//  TeamScene.cpp
//  KouDaiXianXia
//
//  Created by lh on 14-3-11.
//  www.9miao.com
//

#include "TeamScene.h"
#include "PersonTemplate.h"
#include "MessageManager.h"
#include "message.h"
bool TeamScene::init(){
    if (!CCLayer::init()) {
        return false;
    }
    MessageManager::shareMessageManager()->sendPersonal(GameData::shareGameData()->chaId);
    touchIcon=NULL;
    this->setTouchEnabled(true);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TeamScene::receivePetMessage), "personMessage", NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TeamScene::receiveNoticFunc), "changePosition", NULL);
    currentCount=1;
    totalCount=0;
    dragX=0.f;
    dragY=0.f;
    moveflag=false;
    winSize=CCDirector::sharedDirector()->getWinSize();
    m_pUILayer=UILayer::create();
    this->addChild(m_pUILayer,0);
    m_pUILayer->addWidget(CCUIHELPER->createWidgetFromJsonFile("team_layer_1/team_layer_1.json"));
    addScrollView();
    
    return true;
}
void TeamScene::receivePetMessage(cocos2d::CCObject *sender){
    Message *msg=(Message *)sender;
    petData=msg->data;
    this->scheduleOnce(schedule_selector(TeamScene::analyzePetMessage), 0);
}
void TeamScene::analyzePetMessage(){
    CSJson::Reader reader;
    CSJson::Value root;
    if (reader.parse(petData, root)) {
        CSJson::Value psinfo;
        psinfo=root["psinfo"];
        for (int i=0; i<psinfo.size(); i++) {
            int ppid=psinfo[i]["ppid"].asInt();
            HeadIcon *head=HeadIcon::create();
            head->petId=ppid;
            head->m_nLevel=psinfo[i]["level"].asInt();
            head->m_nHp=psinfo[i]["hp"].asInt();
            head->m_nAtt=psinfo[i]["att"].asInt();
            head->lockFlag=psinfo[i]["flg"].asBool();
            for (int j=0; j<GameData::shareGameData()->roleArr->count(); j++) {
                Person *person=(Person *)GameData::shareGameData()->roleArr->objectAtIndex(j);
                if (person->lockFlag) {
                    continue;
                }
                if (ppid==person->m_id) {
                    person->lockFlag=true;
                }else{
                    person->lockFlag=false;
                }
            }
            
        }
        initHeadIcon();
    }
}
void TeamScene::receiveNoticFunc(cocos2d::CCObject *sender){
    CCString *str=(CCString *)sender;
    currentCount=str->intValue();
    this->scheduleOnce(schedule_selector(TeamScene::initProperties), 0);
}
void TeamScene::initHeadIcon(){
    for (int i=0; i<4; i++) {
        HeadIcon *iconBound=HeadIcon::create();
        iconBound->initWithFile("icon-bound.png");
        iconBound->setTag(9000+i);
        iconBound->setPosition(ccp(125+i*220, winSize.height-iconBound->boundingBox().size.height/2+28));
        this->addChild(iconBound,2);
        for (int i=0; i<pRoleArray->count(); i++) {
            
        }
    }
    int addNum=0;
    for (int i=0; i<pRoleArray->count(); i++) {
        Person *pPer=(Person *)pRoleArray->objectAtIndex(i);
        if (pPer->lockFlag&&addNum<4) {
            HeadIcon *head=(HeadIcon *)this->getChildByTag(9000+addNum);
            char iconRes[20]="";
            sprintf(iconRes, "icon-%03d.png",addNum+1);
            head->icon->initWithFile(iconRes);
            addNum++;
        }
    }
}
CCScene *TeamScene::scene(){
    CCScene *scene=CCScene::create();
    CCLayer *layer=TeamScene::create();
    scene->addChild(layer);
    return scene;
}
void TeamScene::addScrollView(){
    CCSprite *pCoverSprite=CCSprite::create("zhegai.png");
    pCoverSprite->setAnchorPoint(ccp(0, 0));
    this->addChild(pCoverSprite,1);
    pRoleArray=GameData::shareGameData()->roleArr;
    scrollview=CCScrollViewX::create(CCSizeMake((388*pRoleArray->count()+4), 488),CCSizeMake(winSize.width, 488));
    scrollview->setPosition(ccp(30, 250));
    scrollview->unscheduleAllSelectors();
    scrollview->setBounceable(true);
    this->addChild(scrollview);
    totalCount=pRoleArray->count();
    for (int i=0; i<pRoleArray->count(); i++) {
        
        HeadIcon *person=HeadIcon::create();
        person->initWithFile("person.png");
        person->setPosition(ccp(388*2+388*(i), 0));
        scrollview->addChild(person, 1, i+100);
    }
    initProperties(0.f);
}
void TeamScene::initProperties(float dt){
    if (currentCount>pRoleArray->count()) {
        return;
    }
    UILabel *pAttackLabel= dynamic_cast<UILabel * >(m_pUILayer->getWidgetByTag(6));
    UILabel *pHpLabel= dynamic_cast<UILabel *>(m_pUILayer->getWidgetByTag(5));
    UILabel *pDefenceLabel= dynamic_cast<UILabel *>(m_pUILayer->getWidgetByTag(7));
    UILabel *pLevelLabel=dynamic_cast<UILabel *>(m_pUILayer->getWidgetByTag(10));
    UILabel *pNameLabel= dynamic_cast<UILabel *>(m_pUILayer->getWidgetByTag(8));
    Person *person=(Person *)pRoleArray->objectAtIndex(currentCount-1);
    char attackData[10]="";
    char hpData[10]="";
    char defenceData[10]="";
    char levelData[10]="";
    char nameData[10]="";
    sprintf(attackData, "%d",person->m_attack);
    sprintf(hpData, "%d",person->m_hp);
    sprintf(defenceData, "%d",person->m_defense);
    sprintf(levelData, "%d",person->m_level);
    sprintf(nameData, "%s",person->m_name.c_str());
    pAttackLabel->setText(attackData);
    pHpLabel->setText(hpData);
    pDefenceLabel->setText(defenceData);
    pLevelLabel->setText(levelData);
    pNameLabel->setText(nameData);
}

void  TeamScene::registerWithTouchDispatcher(){
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,1, true);
    CCLayer::registerWithTouchDispatcher();
}
bool TeamScene::ccTouchBegan(CCTouch* touch, CCEvent* event){
    if (touchIcon!=NULL) {
        return false;
    }
    for (int i=0; i<pRoleArray->count(); i++) {
        HeadIcon *icon=(HeadIcon *)scrollview->getContainer()->getChildByTag(100+i);
        if (icon->touchEvent(touch)) {
            touchIcon=icon;
            CCSprite *person=CCSprite::create("person.png");
            person->setOpacity(GLubyte(90));
            person->setTag(1024);
            person->setPosition(touch->getLocation());
            this->addChild(person,5);
            dragX=touch->getLocation().x-touchIcon->boundingBox().origin.x;
            dragY=touch->getLocation().y-touchIcon->boundingBox().origin.y;
            oldPoint=touchIcon->getPosition();
            
            return true;
        }
        
    }
    return true;
}
void TeamScene::ccTouchMoved(CCTouch* touch, CCEvent* event){
    if (touchIcon!=NULL) {
        if (moveflag) {
            CCSprite *moveIcon=(CCSprite *)this->getChildByTag(1024);
            moveIcon->setPosition(touch->getLocation());
            scrollview->setTouchEnabled(false);
            for (int i=0; i<4; i++) {
                HeadIcon *person=(HeadIcon *)this->getChildByTag(9000+i);
                CCRect rHeadRect=CCRectMake(person->boundingBox().origin.x, person->boundingBox().origin.y, 110, 110);
                CCRect rPersonRect=CCRectMake(moveIcon->boundingBox().origin.x, moveIcon->boundingBox().origin.y, 110, 110);
                if (rHeadRect.intersectsRect(rPersonRect)) {
                    person->setScale(1.3);
                    touchHead=person;
                  //  return;
                }else{
                    person->setScale(1);
                    
                }
            }
            return;
        }
        if (fabs(touchIcon->getPosition().x-touch->getLocation().x+dragX)<10&&fabs(touch->getLocation().y-touchIcon->getPosition().y+dragY)>10) {
//            CCPoint nodePos= scrollview->convertToNodeSpace(ccp(touch->getLocation().x, touch->getLocation().y));
            
            moveflag=true;
            CCSprite *moveIcon=(CCSprite *)this->getChildByTag(1024);
            moveIcon->setPosition(touch->getLocation());
            scrollview->setTouchEnabled(false);
            
            
            
        }else if(!moveflag){
            
            return;
        }
        
    }
    
}
void TeamScene::ccTouchEnded(CCTouch* touch, CCEvent* event){
    if (touchIcon!=NULL) {
        scrollview->setTouchEnabled(true);
        this->removeChildByTag(1024,true);
        touchIcon->setPosition(oldPoint);
        touchIcon=NULL;
        for (int i=0; i<4; i++) {
            HeadIcon *iconBound=(HeadIcon *)this->getChildByTag(9000+i);
            iconBound->setScale(1);
            
            
        }
    }
    
}
TeamScene::~TeamScene(){
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}


bool HeadIcon::init(){
    if (!CCSprite::init()) {
        return false;
    }
    icon=CCSprite::create();
    icon->setPosition(ccp(138, 132));
    this->addChild(icon);
    m_nLevel=0;
    m_nHp=0;
    m_nAtt=0;
    petId=0;
    lockFlag=false;
    return true;
}
bool HeadIcon::touchEvent(cocos2d::CCTouch *pTouch){
    CCRect touchRect=CCRectMake(pTouch->getLocation().x, pTouch->getLocation().y, 50, 50);
    CCPoint pos= this->convertToWorldSpace(CCPointZero);
    CCRect thisBound=CCRectMake(pos.x, pos.y, this->getContentSize().width, this->getContentSize().height);
    
    
    if (thisBound.intersectsRect(touchRect)) {
        CCLog("touch here");
        return true;
    }
    return false;
}
