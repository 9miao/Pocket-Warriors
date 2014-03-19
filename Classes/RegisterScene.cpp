//
//  RegisterScene.cpp
//  TestGame
//
//  Created by lh on 14-1-16.
//  www.9miao.com
//

#include "RegisterScene.h"
#include "message.h"
#include "MessageManager.h"
#include "MessageBox.h"
bool RegisterScene::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(RegisterScene::reciveRegisterMsg), "registerMessage", NULL);
    
    m_winSize=CCDirector::sharedDirector()->getWinSize();
//    socket=new SocketClient(ip,port,1,1,NULL);
//    socket->start();
    CCScale9Sprite *sp0=CCScale9Sprite::create();
    m_pEditBox0=CCEditBox::create(CCSizeMake(500, 80), sp0);
    m_pEditBox0->setFontSize(30);
    m_pEditBox0->setReturnType(kKeyboardReturnTypeDone);
    m_pEditBox0->setInputFlag(kEditBoxInputFlagPassword);
    m_pEditBox0->setPosition(ccp(m_winSize.width/2-130, m_winSize.height/3-60));
    m_pEditBox0->setFontColor(ccBLACK);
    m_pEditBox0->setText("");
    this->addChild(m_pEditBox0,1);
    
    CCScale9Sprite *sp1=CCScale9Sprite::create();
    m_pEditBox1=CCEditBox::create(CCSizeMake(500, 80), sp1);
    m_pEditBox1->setFontSize(30);
    m_pEditBox1->setReturnType(kKeyboardReturnTypeDone);
    m_pEditBox1->setInputFlag(kEditBoxInputFlagPassword);
    m_pEditBox1->setPosition(ccp(m_winSize.width/2-130, m_winSize.height/2-130));
    m_pEditBox1->setFontColor(ccBLACK);
    m_pEditBox1->setText("");
    this->addChild(m_pEditBox1,1);
    
    CCScale9Sprite *sp2=CCScale9Sprite::create();
    m_pEditBox2=CCEditBox::create(CCSizeMake(500, 80), sp2);
    m_pEditBox2->setFontSize(30);
    m_pEditBox2->setReturnType(kKeyboardReturnTypeDone);
    m_pEditBox2->setPosition(ccp(m_winSize.width/2-130, m_winSize.height/2));
    m_pEditBox2->setFontColor(ccBLACK);
    m_pEditBox2->setText("");
    this->addChild(m_pEditBox2,1);
    
    
    btnIsRuning=false;
    CCSprite *m_bg=CCSprite::create(M_REGISTER_BG);
    m_bg->setAnchorPoint(CCPointZero);
    m_bg->setPosition(CCPointZero);
    this->addChild(m_bg);
    CCSprite *m_bg_bound=CCSprite::create(M_REGISTER_BOUNDER);
    m_bg_bound->setAnchorPoint(CCPointZero);
    m_bg_bound->setPosition(ccp(80, 50));
    m_bg->addChild(m_bg_bound);
    setTouchEnabled(true);
    CCMenuItemImage *m_btn_register=CCMenuItemImage::create(M_REGISTER_BTN_UP, M_REGISTER_BTN_DOWN, this, NULL);
    m_pBtnMenu=CCMenu::create(m_btn_register,NULL);
    oldPos=ccp(320, 170);
    m_pBtnMenu->setAnchorPoint(CCPointZero);
    m_pBtnMenu->setPosition(ccp(320, 170));
    this->addChild(m_pBtnMenu);
    return true;
}
void RegisterScene::registerWithTouchDispatcher(){
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,-99901, true);
    CCLayer::registerWithTouchDispatcher();
}
bool RegisterScene::ccTouchBegan(CCTouch* touch, CCEvent* event){
    edit0=m_pEditBox0->ccTouchBegan(touch, event);
    edit1=m_pEditBox1->ccTouchBegan(touch, event);
    edit2=m_pEditBox2->ccTouchBegan(touch, event);
    noticeFlag=true;
    if (!btnIsRuning) {
        btn_menuflag=m_pBtnMenu->ccTouchBegan(touch, event);
        m_pBtnMenu->stopAllActions();
        n_btnDistance= touch->getLocation().x-m_pBtnMenu->boundingBox().origin.x;
    }
    return true;
}
void RegisterScene::ccTouchMoved(CCTouch* touch, CCEvent* event){
    if (btn_menuflag) {
        if (touch->getLocation().x-n_btnDistance<320) {
            return;
        }else if(touch->getLocation().x-n_btnDistance>900){
            if (noticeFlag) {
                judgePassWord();
                noticeFlag=false;
            }
            
            return;
        }
        m_pBtnMenu->ccTouchMoved(touch, event);
        m_pBtnMenu->setPosition(ccp(touch->getLocation().x-n_btnDistance, m_pBtnMenu->getPosition().y));
    }
    if (edit0) {
        m_pEditBox0->ccTouchMoved(touch, event);
    }
    if (edit2) {
        m_pEditBox2->ccTouchMoved(touch, event);
    }
    if (edit1) {
        m_pEditBox1->ccTouchMoved(touch, event);
    }
}
void RegisterScene::sendRegisterMsg(){
    MessageManager::shareMessageManager()->sendRegisterMessage(m_pEditBox2->getText(), m_pEditBox1->getText());
}
void RegisterScene::reciveRegisterMsg(CCObject *obj){
    Message *msg=(Message *)obj;
    if (msg) {
        CSJson::Reader reader;
        CSJson::Value root;
        if (reader.parse(msg->data, root)) {
            GameData::shareGameData()->chaId=root["pid"].asInt();
            GameData::shareGameData()->gold=root["gold"].asInt();
            GameData::shareGameData()->coin=root["silverMoney"].asInt();
        }
        CCUserDefault::sharedUserDefault()->setStringForKey("user", m_pEditBox2->getText());
        CCUserDefault::sharedUserDefault()->flush();
        CCUserDefault::sharedUserDefault()->setStringForKey("password", m_pEditBox1->getText());
        CCUserDefault::sharedUserDefault()->flush();
        CCMoveTo *move=CCMoveTo::create(1, ccp(0, 1100));
        CCEaseOut *ease=CCEaseOut::create(move, 1);
        CCCallFunc *func=CCCallFunc::create(this, callfunc_selector(RegisterScene::exitFunc));
        CCSequence *seq=CCSequence::create(ease,func,NULL);
        this->runAction(seq);
    }
}
void RegisterScene::judgePassWord(){
    if (!strcmp(m_pEditBox2->getText(), "")) {

        MessageBox *msgbox=MessageBox::create();
        msgbox->CMessageBox("提  示", "账户名不能为空");
       // CCDirector::sharedDirector()->getRunningScene()->addChild(msgbox,2);
    }else if(!strcmp(m_pEditBox1->getText(), "")||!strcmp(m_pEditBox0->getText(), "")){

        MessageBox *msgbox=MessageBox::create();
        msgbox->CMessageBox("提  示", "密码不能为空");
       // CCDirector::sharedDirector()->getRunningScene()->addChild(msgbox,2);
    }else if(strcmp(m_pEditBox0->getText(), m_pEditBox1->getText())){

        MessageBox *msgbox=MessageBox::create();
        msgbox->CMessageBox("提  示", "两次输入的密码不同");
       // CCDirector::sharedDirector()->getRunningScene()->addChild(msgbox,2);
    }else{
        sendRegisterMsg();
        
    }
}
void RegisterScene::exitFunc(){
    MessageManager::shareMessageManager()->sendPetMessage();
    setTouchEnabled(false);
    m_pBtnMenu->stopAllActions();
    this->removeFromParentAndCleanup(true);
}

void RegisterScene::ccTouchEnded(CCTouch* touch, CCEvent* event){
    if (btn_menuflag) {
        m_pBtnMenu->ccTouchEnded(touch, event);
        btn_menuflag=false;
        CCActionInterval *moveold=CCMoveTo::create((m_pBtnMenu->getPosition().x-oldPos.x)/1500, ccp(320, 170));
        CCEaseOut *easeOut=CCEaseOut::create(moveold, 1);
        m_pBtnMenu->runAction(easeOut);
    }
    if (edit1) {
        edit1=false;
        m_pEditBox1->ccTouchEnded(touch, event);
    }
    if (edit0) {
        edit0=false;
        m_pEditBox0->ccTouchEnded(touch, event);
    }
    if (edit2) {
        edit2=false;
        m_pEditBox2->ccTouchEnded(touch, event);
    }
}
RegisterScene::~RegisterScene(){
    this->removeAllChildren();
    CCTextureCache::sharedTextureCache()->removeTextureForKey("register_bg.png");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("reg_cj_up.png");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("reg_cj_down.png");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("register_bounder.png");
}