//
//  MyScrollView.h
//  TestGame
//
//  Created by lh on 13-11-11.
//  www.9miao.com
//

#ifndef __TestGame__MyScrollView__
#define __TestGame__MyScrollView__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
using namespace cocos2d;
using namespace extension;

class MyScrollView :public CCScrollView,public CCScrollViewDelegate{
public:
    MyScrollView();
    ~MyScrollView();
    virtual void scrollViewDidScroll(CCScrollView* view);
    
    virtual void scrollViewDidZoom(CCScrollView* view);
    static MyScrollView * create(CCSize size,CCNode  *container=NULL);
    
    int _number ;
    
    int _isNext ;
    
    bool isActions ;
    
    CCPoint m_touchBeginPoint ;
    
    bool _isMoved ;
    
    void scrollViewActions();
public:
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

};
#endif /* defined(__TestGame__MyScrollView__) */
