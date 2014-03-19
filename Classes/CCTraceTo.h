//
//  CCTraceTo.h
//  TestGame
//
//  Created by lh on 14-1-2.
//  www.9miao.com
//

#ifndef __TestGame__CCTraceTo__
#define __TestGame__CCTraceTo__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
class CCTraceTo:public CCFollow{
public:
    bool initWithTarget(CCNode *pFollowedNode,int speed, const CCRect& rect = CCRectZero);
    virtual void step(float dt);
    CCPoint m_startPosition;
    CCPoint m_endPosition;
    
    float m_duration;
    float m_moveSpeed;
    static CCTraceTo* create(CCNode *pFollowedNode,CCNode *selfs,int speed, const CCRect& rect = CCRectZero);
protected:
    int isspeed;
    float m_elapsed;
    bool   m_bFirstTick;
};

#endif /* defined(__TestGame__CCTraceTo__) */
