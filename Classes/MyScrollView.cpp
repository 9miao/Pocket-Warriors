//
//  MyScrollView.cpp
//  TestGame
//
//  Created by lh on 13-11-11.
//  www.9miao.com
//

#include "MyScrollView.h"

MyScrollView::MyScrollView(){
    
}
MyScrollView::~MyScrollView(){
    
}
MyScrollView* MyScrollView::create(CCSize size,CCNode  * container/*=NULL*/){
    MyScrollView* pRet = new MyScrollView();
    if (pRet && pRet->initWithViewSize(size, container))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}
void MyScrollView::scrollViewDidScroll(CCScrollView* view){
    
}

void MyScrollView::scrollViewDidZoom(CCScrollView* view){
    
}

bool MyScrollView::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if (m_pTouches->count() > 1 ||
        m_bTouchMoved           ||
        !this->getViewRect().containsPoint(m_pContainer->convertToWorldSpace(m_pContainer->convertTouchToNodeSpace(pTouch))))
    {
        return false;
    }
    
    if (isActions)
    {
        return false;
    }
    
    m_touchBeginPoint=pTouch->getLocation();

    _isNext=0;
    
    return CCScrollView::ccTouchBegan(pTouch, pEvent);
}
void MyScrollView::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    CCScrollView::ccTouchMoved(pTouch, pEvent);
    CCPoint point=pTouch->getLocation();
    float lenght=ccpDistance(point, m_touchBeginPoint);
    if (lenght<10) return;
    _isMoved = true;
    if (pTouch->getPreviousLocation().x-pTouch->getLocation().x>330) {
        _isNext=1;
        return;
    }
    if (pTouch->getLocation().x-pTouch->getPreviousLocation().x>330) {
        _isNext=-1;
        return;
    }
}
void MyScrollView::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if (m_pTouches->containsObject(pTouch))
    {
        m_pTouches->removeObject(pTouch);
    }
    
    if (m_pTouches->count() == 0)
    {
        m_bDragging = false;
        m_bTouchMoved = false;
    }
    
    if (_isNext==0)
    {
        if (this->getContainer()->getPositionX()>-this->getViewSize().width/2)
        {
            _isNext=-1;
        }
        else if (this->getContainer()->getPositionX()<-this->getViewSize().width/2*3)
        {
            _isNext=1;
        }
    }
    
    isActions=true;
    
    this->scrollViewActions();

    _isMoved=false;
}

void MyScrollView::scrollViewActions(){
    
}

