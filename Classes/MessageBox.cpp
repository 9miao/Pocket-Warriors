//
//  MessageBox.cpp
//  TestGame
//
//  Created by lh on 14-2-11.
//  www.9miao.com
//

#include "MessageBox.h"
#include "ResourceName.h"


bool MessageBox::init(){
    CCSize winSize=CCDirector::sharedDirector()->getWinSize();
    if (!CCLayerColor::initWithColor(ccc4(0, 0, 0, 100), winSize.width, winSize.height)) {
        return false;
    }

    setTouchEnabled(true);
    m_pNodeBg=CCSprite::create(MESSAGEBOXBG);
    m_pNodeBg->setScale(0);
    m_pNodeBg->setPosition(ccp(winSize.width/2, winSize.height/2));
    m_pNodeTitleLable=CCLabelTTF::create("", SELF_TTF_ONE, 40);
    m_pNodeTitleLable->setFontFillColor(ccc3(0, 0, 0));
    m_pNodeTitleLable->setPosition(ccp(m_pNodeBg->getContentSize().width/2, m_pNodeBg->getContentSize().height-50));
    m_pNodeBg->addChild(m_pNodeTitleLable);
    m_pNodeMsgLable=CCLabelTTF::create("", SELF_TTF_ONE, 40, CCSizeMake(500, 400), kCCTextAlignmentLeft);
    m_pNodeMsgLable->setFontFillColor(ccc3(0, 0, 0));
    m_pNodeMsgLable->setAnchorPoint(ccp(0, 1));
    m_pNodeMsgLable->setPosition(ccp(40, m_pNodeBg->getContentSize().height-80));
    m_pNodeBg->addChild(m_pNodeMsgLable);
    CCMenuItemImage *pBtn0=CCMenuItemImage::create(SURE_UP, SURE_DOWN, this, NULL);
    m_pNodeSureBtn=CCMenu::create(pBtn0,NULL);
    m_pNodeSureBtn->setAnchorPoint(ccp(0, 0));
    m_pNodeSureBtn->setPosition(ccp(140, 100));
    m_pNodeBg->addChild(m_pNodeSureBtn);
    addChild(m_pNodeBg);
    CCDirector::sharedDirector()->getRunningScene()->addChild(this,2);
    return true;
}

MessageBox *MessageBox::create(){
    MessageBox* pRet = new MessageBox();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}
void MessageBox::CMessageBox(const char *title,const char *msgs){
    m_pNodeTitleLable->setString(title);
    m_pNodeMsgLable->setString(msgs);
    goseAction();
    
}
void MessageBox::goseAction(){
    CCScaleTo *actScale=CCScaleTo::create(0.1, 1);
    m_pNodeBg->runAction(actScale);
}
void MessageBox::registerWithTouchDispatcher(){
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,-99999, true);
    CCLayerColor::registerWithTouchDispatcher();
}
bool MessageBox::ccTouchBegan(CCTouch* touch, CCEvent* event){
    m_bSureBtnFlag=m_pNodeSureBtn->ccTouchBegan(touch, event);
    m_dGap= touch->getLocation().x-m_pNodeSureBtn->boundingBox().origin.x;
    return true;
}
void MessageBox::ccTouchMoved(CCTouch* touch, CCEvent* event){
    if (m_bSureBtnFlag) {
        m_pNodeSureBtn->ccTouchMoved(touch, event);
        if (touch->getLocation().x-m_dGap<140) {
            return;
        }else if(touch->getLocation().x-m_dGap>500){
            CCScaleTo *aScaleTo=CCScaleTo::create(0.1, 0);
            CCCallFunc *func=CCCallFunc::create(this, callfunc_selector(MessageBox::isOK));
            CCSequence *seq=CCSequence::create(aScaleTo,func,NULL);
            m_pNodeBg->runAction(seq);
            return;
        }
        m_pNodeSureBtn->setPosition(ccp(touch->getLocation().x-m_dGap, m_pNodeSureBtn->getPosition().y));
    }
}
void MessageBox::isOK(){
    this->removeFromParentAndCleanup(true);
}
void MessageBox::ccTouchEnded(CCTouch* touch, CCEvent* event){
    if (m_bSureBtnFlag) {
        m_pNodeSureBtn->ccTouchEnded(touch, event);
        CCActionInterval *moveold=CCMoveTo::create((m_pNodeSureBtn->getPosition().x-140)/1500, ccp(140, 100));
        CCEaseOut *easeOut=CCEaseOut::create(moveold, 1);
        m_pNodeSureBtn->runAction(easeOut);
    }
}