//
//  LoadingLayer.cpp
//  TestGame
//
//  Created by lh on 13-9-18.
//  www.9miao.com
//

#include "LoadingLayer.h"
#include "ResourceName.h"
bool LoadingLayer::init(){
    if(!CCLayerColor::initWithColor(ccc4(0, 0, 0, 0))){
        return false;
    }
    this->threadStart();
    m_nCurCount=0;
    CCSize size=CCDirector::sharedDirector()->getWinSize();
    m_pNodeLoadingBg=CCSprite::create(M_LOADING_BG);
    m_pNodeLoadingBg->setAnchorPoint(CCPointZero);
    m_pNodeLoadingBg->setPosition(CCPointZero);
    this->addChild(m_pNodeLoadingBg);
    m_pNodeProgressBg=CCSprite::create(M_PROGRESS_BG);
    m_pProgress=CCProgressTimer::create(CCSprite::create(M_PROGRESS));
    m_pProgress->setType(kCCProgressTimerTypeBar);
    //progress->setPercentage(100);
   // progress->setVisible(false);
    m_pProgress->setMidpoint(ccp(0, 0));
    m_pProgress->setBarChangeRate(ccp(1, 0));
    m_pNodeProgressBg->setAnchorPoint(CCPointZero);
    m_pProgress->setAnchorPoint(CCPointZero);
    m_pNodeProgressBg->setPosition(ccp(size.width/2-400, size.height/2-50));
    m_pProgress->setPosition(ccp(size.width/2-400, size.height/2-50));
    m_pNodeLoadingBg->addChild(m_pProgress,2);
    m_pNodeLoadingBg->addChild(m_pNodeProgressBg,1);
    return true;
}
void LoadingLayer::setProgress(int percents,int max){
    CCLog("%d  %ld",m_nCurCount,m_vResourceArray.size());
    if (m_nCurCount==m_vResourceArray.size()||m_vResourceArray.size()==0) {
        
        return;
    }else{
        nowcount++;
        m_nCurCount++;
    }
    CCTextureCache::sharedTextureCache()->addImageAsync((string(*nowcount)).c_str(), this, callfuncO_selector(LoadingLayer::setProgress));
    m_pProgress->stopAllActions();
    CCProgressTo * to2= CCProgressTo::create(1,m_nCurCount/m_vResourceArray.size()*100);
    to2->setTag(20);
    CCCallFunc *func=CCCallFunc::create(this, callfunc_selector(LoadingLayer::loadFinished));
    CCSequence *seq=CCSequence::create(to2,func,NULL);
    m_pProgress->runAction(seq);
}
void LoadingLayer::loadFinished(){
    if (m_nCurCount==m_vResourceArray.size()) {
        CCNotificationCenter::sharedNotificationCenter()->postNotification("loadingremove", m_pObj);
        this->removeFromParentAndCleanup(true);
//        CCFadeOut *fadeout=CCFadeOut::create(1);
//        CCCallFunc *func=CCCallFunc::create(this, callfunc_selector(LoadingLayer::RemoveSelf));
//        CCSequence *seq=CCSequence::create(fadeout,func,NULL);
//        loadingbg->runAction(seq);
    }
}
void LoadingLayer::RemoveSelf(){
    
    
}
void LoadingLayer::addImage(vector<string> arr,CCObject *obj){
    m_nCurCount=1;
    m_vResourceArray=arr;
    nowcount=m_vResourceArray.begin();
    m_pObj=obj;
    setProgress(0, 0);
   
}
int LoadingLayer::threadStart(){
    int errCode=0;
    do {
        pthread_attr_t tAttr;
        errCode=pthread_attr_init(&tAttr);
        CC_BREAK_IF(errCode!=0);
        errCode=pthread_attr_setdetachstate(&tAttr, PTHREAD_CREATE_DETACHED);
        if(errCode!=0)
        {
            pthread_attr_destroy(&tAttr);
            break;
        }
        errCode=pthread_create(&th_load, &tAttr, thread_funcation, this);
        
    } while (0);
    return errCode;
}
void* LoadingLayer::thread_funcation(void *arg){
    return NULL;
}