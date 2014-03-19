//
//  LoginScene.cpp
//  TestGame
//
//  Created by lh on 13-9-18.
//  www.9miao.com
//

#include "LoginScene.h"
#include "Singleton.h"
#include "CCScrollViewX.h"
#include "message.h"
#include "MainView.h"
#include <typeinfo>
#include "MessageManager.h"
#include "MessageBox.h"
using namespace std;

CCScene * LoginScene::scene(){
    CCScene *scene = CCScene::create();

    LoginScene *layer = LoginScene::create();

    MainView *mainLayer= MainView::create();
    scene->addChild(mainLayer);
    mainLayer->setTag(10001);
    scene->addChild(layer,1);
    return scene;
}

bool LoginScene::init(){
    if(!CCLayer::init()){
        return false;
    }
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LoginScene::loginMenuCallBack), "loginMessage", NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LoginScene::loadUI), "loadingremove", NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LoginScene::receiveMainScene), "petsInfo", NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LoginScene::personalBack), "personMessage", NULL);
    m_pLoading=LoadingLayer::create();
    this->addChild(m_pLoading,999);
    m_vResourceArray.push_back(M_LOGIN_BG);
    m_vResourceArray.push_back(M_LOGIN_BTN_UP);
    m_vResourceArray.push_back(M_LOGIN_BTN_DOWN);
    m_vResourceArray.push_back(M_CHUANGJIAN_UP);
    m_vResourceArray.push_back(M_CHUANGJIAN_DOWN);
    m_vResourceArray.push_back(M_REGISTER_BTN_UP);
    m_vResourceArray.push_back(M_REGISTER_BTN_DOWN);
    m_vResourceArray.push_back(M_REGISTER_BOUNDER);
    m_vResourceArray.push_back(M_MAINVIEW_BG);
    m_vResourceArray.push_back(M_MVDRAG_BG);
    m_vResourceArray.push_back(M_KZBTN_UP);
    m_vResourceArray.push_back(M_KZBTN_DOWN);
    m_vResourceArray.push_back(M_TDBTN_UP);
    m_vResourceArray.push_back(M_TDBTN_DOWN);
    m_vResourceArray.push_back(M_QHBTN_UP);
    m_vResourceArray.push_back(M_QHBTN_DOWN);
    m_vResourceArray.push_back(M_FLBTN_UP);
    m_vResourceArray.push_back(M_FLBTN_DOWN);
    m_pLoading->addImage(m_vResourceArray,this);
    return true;
}
void LoginScene::loadUI(CCObject *obj){
    if (obj==this) {
        CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "loadingremove");
    }else{
        return;
    }
    g_cSize=CCDirector::sharedDirector()->getWinSize();
    setTouchEnabled(true);
    m_bRegisterBtn=false;
    g_pSocket= new SocketClient(ip,ports,1,1,NULL);//146
    g_pSocket->start();
    GameData::shareGameData()->socket=g_pSocket;
    if(!strcmp(CCUserDefault::sharedUserDefault()->getStringForKey("password").c_str(), "")||!strcmp(CCUserDefault::sharedUserDefault()->getStringForKey("user").c_str(), "")){
        addRegisterLayer();
    }else
    {
        MessageManager::shareMessageManager()->sendLoginMessage();
    }
    CCSprite *pNodeBgSprite=  CCSprite::create(M_LOGIN_BG);
    pNodeBgSprite->setPosition(ccp(g_cSize.width/2, g_cSize.height/2));
    CCMenuItemImage *pNodeLoginBtn=CCMenuItemImage::create(M_LOGIN_BTN_UP, M_LOGIN_BTN_DOWN, this, NULL);
    CCMenuItemImage *pNodeRegisterBtn= CCMenuItemImage ::create(M_CHUANGJIAN_UP, M_CHUANGJIAN_DOWN, this,menu_selector(LoginScene::registerMenuCallBack));
    pNodeLoginBtn->setAnchorPoint(ccp(1, 0));
    pNodeRegisterBtn->setAnchorPoint(CCPointZero);
    m_pLoginMenu=CCMenu::create(pNodeLoginBtn,NULL);
    m_pRegisterMenu=CCMenu::create(pNodeRegisterBtn,NULL);
    m_pLoginMenu->setAnchorPoint(CCPointZero);
     m_pRegisterMenu->setAnchorPoint(CCPointZero);
    m_pLoginMenu->setPosition(ccp(g_cSize.width,g_cSize.height/2-100));
    m_pRegisterMenu->setPosition(ccp(0, g_cSize.height/2+140));
    this->addChild(pNodeBgSprite);
    this->addChild(m_pLoginMenu);
    this->addChild(m_pRegisterMenu);
}

void  LoginScene::registerWithTouchDispatcher(){
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,-99900, true);
    CCLayer::registerWithTouchDispatcher();
}
void LoginScene::loginMenuCallBack(CCObject * obj){
    Message *msg=(Message *)obj;
    if(msg){
        CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "loginMessage");
        CSJson::Reader reader;
        CSJson::Value root;
        if (reader.parse(msg->data, root)) {
            if (root["result"].asBool()) {
                m_nPid = root["pid"].asInt();
                GameData::shareGameData()->coin=root["silverMoney"].asInt();
                GameData::shareGameData()->gold=root["gold"].asInt();
                GameData::shareGameData()->chaId=m_nPid;
                this->scheduleOnce(schedule_selector(LoginScene::registerMenuCallBack), 0.01);
            }else{
                MessageBox *msgbox=MessageBox::create();
                msgbox->CMessageBox("错  误", "出错了！");
            }
        }
    }
}
void LoginScene::registerMenuCallBack(CCObject * obj){
    MessageManager::shareMessageManager()->sendPetMessage();
}
void LoginScene::personalBack(CCObject * obj){
    Message * msgs=(Message *)obj;
    if (msgs) {
        
        CCLog("%s",msgs->data);
    }
    return;

}
void LoginScene::addLoadingLayer(){

}
bool LoginScene::ccTouchBegan(CCTouch* touch, CCEvent* event){
    if (m_bLoginFlag||m_bRegisterBtn) {
        return false;
    }
    g_fGap= touch->getLocation().x-m_pLoginMenu->boundingBox().origin.x;
    m_bLoginFlag= m_pLoginMenu->ccTouchBegan(touch, event);
    if (m_bLoginFlag) {
        m_pLoginMenu->setScale(1.1);
        m_pLoadLayer= (MainView *)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(10001);
        ((MainView *)m_pLoadLayer)->setCoin();
        m_pLoadLayer->setTouchEnabled(false);
    }
    m_beginPoint=touch->getLocation();
    g_fDis=m_oldPoint.x-m_beginPoint.x;
    m_bRegisterBtn=m_pRegisterMenu->ccTouchBegan(touch, event);
    if (m_bRegisterBtn) {
        m_pRegisterMenu->setScale(1.1);
    }
    return true;
}
void LoginScene::ccTouchMoved(CCTouch* touch, CCEvent* event){
    if(m_bLoginFlag){
        m_pLoginMenu->ccTouchMoved(touch, event);
        this->setPosition(ccp(touch->getLocation().x-g_fGap-g_cSize.width, 0));
        ((MainView *)m_pLoadLayer)->color->setOpacity(GLubyte(150-((abs(touch->getLocation().x-g_cSize.width)/g_cSize.width)*150)));
        m_pLoadLayer->setScaleY(0.95+(abs(touch->getLocation().x-g_cSize.width)/g_cSize.width)*0.05);
    }
    if(m_bRegisterBtn){
        m_pRegisterMenu->ccTouchMoved(touch, event);
    }
}
void LoginScene::ccTouchEnded(CCTouch* touch, CCEvent* event){
    if (m_bLoginFlag) {
        m_pLoginMenu->ccTouchEnded(touch, event);
        m_bLoginFlag=false;
        LayerEaseIn(this);
        m_oldPoint= touch->getLocation();
        m_pLoginMenu->setScale(1);
    }
    if(m_bRegisterBtn){
        m_bRegisterBtn=false;
        m_pRegisterMenu->setScale(1);
        m_pRegisterMenu->ccTouchEnded(touch, event);
    }
}
void LoginScene::LayerEaseIn(CCNode *Layersender){
    if (Layersender->getPosition().x<(-g_cSize.width/2)) {
        CCMoveTo *move=CCMoveTo::create(0.3, ccp(-g_cSize.width, 0));
        CCEaseIn *easein=CCEaseIn::create(move, 1);
        CCCallFunc *func=CCCallFunc::create(this, callfunc_selector(LoginScene::mainScene));
        CCSequence *actions=CCSequence::create(easein,NULL);
        Layersender->runAction(actions);
        CCMoveTo *menu_Move=CCMoveTo::create(0.3, ccp(-g_cSize.width,g_cSize.height/2-100));
        CCEaseIn *menu_EaseIn=CCEaseIn::create(menu_Move, 1);
        CCDelayTime *delay=CCDelayTime::create(0.5);
        CCSequence *menu_Seq=CCSequence::create(menu_EaseIn,delay,func,NULL);
        CCScaleTo *scale = CCScaleTo::create(0.3, 1,1);
        CCFadeTo *fade = CCFadeTo::create(0.3, GLubyte(0));
        ((MainView *)m_pLoadLayer)->color->runAction(fade);
        m_pLoadLayer->runAction(scale);
        m_pLoginMenu->runAction(menu_Seq);
        m_pLoadLayer->setTouchEnabled(true);
    }
    else if(Layersender->getPosition().x>(-g_cSize.width/2)){
        CCMoveTo *move=CCMoveTo::create(0.3, CCPointZero);
        CCEaseIn *easein=CCEaseIn::create(move, 1);
        Layersender->runAction(easein);
            CCScaleTo *scale= CCScaleTo::create(0.3, 1, 0.95);
        CCFadeTo *fade = CCFadeTo::create(0.3, GLubyte(150));
        ((MainView *)m_pLoadLayer)->color->runAction(fade);
        m_pLoadLayer->runAction(scale);
        MoveAndEase(ccp(g_cSize.width,g_cSize.height/2-100), m_pLoginMenu);
    }
}
void LoginScene::MoveAndEase(cocos2d::CCPoint pos,CCNode * sender){
    CCMoveTo *menu_Move=CCMoveTo::create(0.3, pos);
    CCEaseIn *menu_EaseIn=CCEaseIn::create(menu_Move, 1);
    
    CCSequence *menu_Seq=CCSequence::create(menu_EaseIn,NULL);
    sender->runAction(menu_Seq);
}
void LoginScene::mainScene(){
    ((MainView *)m_pLoadLayer)->setRole();
    ((MainView *)m_pLoadLayer)->intensifLayer->setVisible(true);
    m_pLoadLayer->setTouchEnabled(true);
    this->removeFromParentAndCleanup(true);

}
void LoginScene::receiveMainScene(CCObject *pObj){
    CCAssert(pObj, "MAINSCENE is null");
    Message *msg= (Message *)pObj;
    GameData::shareGameData()->str=msg->data;
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "petsInfo");
}
void LoginScene::onExit(){
    CCLayer::onExit();
}
LoginScene::~LoginScene(){
    CCTextureCache::sharedTextureCache()->removeTextureForKey(M_LOGIN_BG);
    CCTextureCache::sharedTextureCache()->removeTextureForKey(M_LOGIN_BTN_DOWN);
    CCTextureCache::sharedTextureCache()->removeTextureForKey(M_LOGIN_BTN_UP);
    CCTextureCache::sharedTextureCache()->removeTextureForKey(M_CHUANGJIAN_UP);
    CCTextureCache::sharedTextureCache()->removeTextureForKey(M_CHUANGJIAN_DOWN);
    ccDrawFree();
}
void LoginScene::addRegisterLayer(){
    RegisterScene *pNodeRegister=RegisterScene::create();
    pNodeRegister->setPosition(CCPointZero);
    this->addChild(pNodeRegister,50);
}