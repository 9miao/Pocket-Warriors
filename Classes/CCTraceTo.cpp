//
//  CCTraceTo.cpp
//  TestGame
//
//  Created by lh on 14-1-2.
//  www.9miao.com
//

#include "CCTraceTo.h"
static CCNode *_selfs=NULL;
static int m_speed=0;
CCTraceTo* CCTraceTo::create(CCNode *pFollowedNode, CCNode *selfs,int speed,const CCRect& rect/* = CCRectZero*/)
{
    CCTraceTo *pRet = new CCTraceTo();
    _selfs=NULL;
    _selfs=selfs;
    m_speed=speed;
    if (pRet && pRet->initWithTarget(pFollowedNode,speed, rect))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}
bool CCTraceTo::initWithTarget(CCNode *pFollowedNode, int speed,const CCRect& rect/* = CCRectZero*/)
{

    CCAssert(pFollowedNode != NULL, "");
    pFollowedNode->retain();
    isspeed=speed;
    m_pobFollowedNode = pFollowedNode;
    m_endPosition=m_pobFollowedNode->getPosition();
    m_obHalfScreenSize = _selfs->getPosition();
    m_elapsed = 0;
    m_bFirstTick = true;
    m_duration = ccpDistance(m_endPosition, m_obHalfScreenSize)/isspeed;
    m_moveSpeed=(CCDirector::sharedDirector()->getSecondsPerFrame()/60.0f)*((float)isspeed/(float)200.0f)*7.0f ;
    return true;
}

void CCTraceTo::step(float dt){
    
    CC_UNUSED_PARAM(dt);
    
    if ((1/(CCDirector::sharedDirector()->getSecondsPerFrame()))/60.0f>1) {
        m_moveSpeed=3.0f*((float)isspeed/(float)200.0f);
    }else{
        m_moveSpeed=((float)isspeed/(float)200.0f)*3.0f/((1/(CCDirector::sharedDirector()->getSecondsPerFrame()))/60.0f) ;
    }
    m_endPosition=m_pobFollowedNode->getPosition();
    if (ccpDistance(m_obHalfScreenSize, m_endPosition)<=120) {
        CCNotificationCenter::sharedNotificationCenter()->postNotification("enterAttackRegion");
        return;
    }
    if (m_obHalfScreenSize.x>m_endPosition.x) {
        m_endPosition=ccp(m_endPosition.x+100, m_endPosition.y);
    }else if(m_obHalfScreenSize.x<m_endPosition.y){
        m_endPosition=ccp(m_endPosition.x-100, m_endPosition.y);
    }
    CCPoint newPos =ccpAdd( m_obHalfScreenSize,ccpMult(ccpSub(m_endPosition, m_obHalfScreenSize), m_moveSpeed/ccpDistance(m_endPosition, m_obHalfScreenSize)));
    m_pTarget->setPosition(newPos);
    m_obHalfScreenSize = newPos;
}