//
//  CCScrollViewX.cpp
//  scrollView
//
//  Created by mac on 13-10-23.
//  www.9miao.com
//

#include "CCScrollViewX.h"

#include "Strengthen.h"
#define dvalue(a) a*2



CCScrollViewX::CCScrollViewX(){
    
    m_array=new CCArray();
    m_Urlarray=new CCArray();
    _number=-1;
    childrencount=0;
    isActions=false;
}

CCScrollViewX::~CCScrollViewX(){
    m_array->release();
    m_array=NULL;
    m_Urlarray->release();
    m_Urlarray=NULL;
}

CCScrollViewX* CCScrollViewX::create(CCSize size, CCNode* container/* = NULL*/)
{
    CCScrollViewX* pRet = new CCScrollViewX();
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

CCScrollViewX* CCScrollViewX::create(CCSize contentsize,CCSize viewsize)
{
    CCLayer* layer=CCLayer::create();
    layer->setContentSize(contentsize);
    
    CCScrollViewX* pRet = CCScrollViewX::create(viewsize, layer);
    pRet->setBounceable(true);
    pRet->setDirection(kCCScrollViewDirectionHorizontal);
    pRet->setDelegate(pRet);
  //  pRet->initWithBg();

    return pRet;
}

void CCScrollViewX::initWithBg(){
    

    for (int i=0; i<10; i++) {
        
        CCLabelTTF * label=CCLabelTTF::create(CCString::createWithFormat("%d",i)->getCString(), "微软雅黑", 100);
        m_array->addObject(label);
        
        
    }

    for (int i=0; i<3; i++)
    {
        CCSprite* image=(CCLabelTTF *)m_array->objectAtIndex(i);
        this->addChild(image);
        image->setPosition(CCPoint(this->getViewSize().width*(i)+dvalue(5), dvalue(5)));
        m_nImage[i]=image;
    }
    this->getContainer()->setPosition(CCPoint(-this->getViewSize().width, 0));
    this->startPlay();
    this->initWithInstructsContainer();
    this->actionsFinish();
}
void CCScrollViewX::initWithInstructsContainer(){

    CCSize size=CCSize(64, 64);
    
    m_nInstructsContainer=CCSprite::create();
    m_nInstructsContainer->setAnchorPoint(CCPointZero);
    this->addChild(m_nInstructsContainer, 10);
    m_nInstruct=CCSprite::create("one.png");
    m_nInstructsContainer->addChild(m_nInstruct);
    m_nInstruct->setScale(1.3f);
    
    for (int i=0; i<m_array->count(); i++)
    {
        CCSprite* instructs=CCSprite::create("one.png");
        instructs->setColor(ccc3(127, 127, 127));
        m_nInstructsContainer->addChild(instructs);
        m_nInstructs.push_back(instructs);
    }
    
    m_nInstructsContainer->setContentSize(CCSize(m_nInstructs.size()*size.width, size.height));
    
    for (int i=0; i<m_nInstructs.size(); i++)
    {
        m_nInstructs.at(i)->setPosition(CCPoint((0.5+i)*size.width, size.height/2));
    }
    
    m_nInstruct->setPosition(m_nInstructs.at(0)->getPosition());
}

void CCScrollViewX::startPlay(){
    
    _isNext=1;
    this->schedule(schedule_selector(CCScrollViewX::scrollViewActions), 3.0f);
}

void CCScrollViewX::addChild(CCNode * child, int zOrder, int tag)
{
    child->ignoreAnchorPointForPosition(false);
    child->setAnchorPoint(ccp(0.0f, 0.0f));
    if (m_pContainer != child && m_nInstructsContainer != child) {
        m_pContainer->addChild(child, zOrder, tag);
        m_array->addObject(child);
        childrencount++;
    } else {
        CCLayer::addChild(child, zOrder, tag);
    }
}

void CCScrollViewX::addChild(CCNode * child, int zOrder)
{
    this->addChild(child, zOrder, child->getTag());
}

void CCScrollViewX::addChild(CCNode * child)
{
    this->addChild(child, child->getZOrder(), child->getTag());
}

bool CCScrollViewX::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    
    if (m_pTouches->count() > 2 ||
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


void CCScrollViewX::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){     
    CCScrollView::ccTouchMoved(pTouch, pEvent);
    
    CCPoint point=pTouch->getLocation();
    float lenght=ccpDistance(point, m_touchBeginPoint);
    if (lenght<6) return;
    
    _isMoved = true;
    
    if (pTouch->getPreviousLocation().x-pTouch->getLocation().x>388/2) {
        _isNext=1;
        return;
    }
    if (pTouch->getLocation().x-pTouch->getPreviousLocation().x>388/2) {
        _isNext=-1;
        return;
    }
    _isNext=0;
}

void CCScrollViewX::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
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
        if (this->getContainer()->getPositionX()>(-388*_number)+(-388/2))
        {
            _isNext=-1;
        }
        else if (this->getContainer()->getPositionX()<(-388*_number)+(-388/2*3))
        {
            _isNext=1;
        }
    }
    
    isActions=true;
    
    this->scrollViewActions();
    
  //  this->scheduleOnce(schedule_selector(CCScrollViewX::startPlay), 2.0f);
    
//    if (!_isMoved)
//    {
//        this->touchUpInSide();
//    }
    _isMoved=false;
}

void CCScrollViewX::scrollViewActions(){
    CCLog("%d",_isNext);
    if (_isNext>0)
    {
        _number++;
        if (_number>childrencount-2) {
            _number=childrencount-2;
        }
//        if (_number>=m_array->count())
//        {
//            _number=0;
//        }
    }

    if (_isNext<0)
    {
        _number--;
        
        
        if (_number<-1) {
            _number=-1;
        }
//        if (_number<0)
//        {
//            _number=m_array->count()-1;
//        }
    }
    CCLog("%f  %d",this->getViewSize().width,_number);
    CCMoveTo *moveto=CCMoveTo::create(0.3f, CCPoint(-388*(_number+1), 0));
    CCCallFunc* callFunc=CCCallFunc::create(this, callfunc_selector(CCScrollViewX::actionsFinish));
    CCSequence* sequence=CCSequence::create(moveto, callFunc, NULL);
    m_pContainer->runAction(sequence);
}

void CCScrollViewX::actionsFinish(){
     isActions=false;
    CCString *poststr=CCString::createWithFormat("%d",_number+2);
    CCNotificationCenter::sharedNotificationCenter()->postNotification("changePosition",poststr);
    return;
   
    Strengthen *stren=(Strengthen *)this->getParent();
    stren->numflag=_number+2;
    stren->initBaseProperty();
    
    return;
    for (int i=0; i<3; i++)
    {
        if (m_array->count()==0)
        {
            return;
        }
        CCLabelTTF * str= ((CCLabelTTF *)(m_array->objectAtIndex((_number+i-1+m_array->count())%m_array->count())));
        ((CCSprite*)m_nImage[i])->setTexture(str->getTexture());
    }
    m_nInstruct->setPosition(m_nInstructs.at(_number)->getPosition());
    
    m_pContainer->setPosition(CCPoint(-this->getViewSize().width, 0));

    isActions=false;
}

void CCScrollViewX::touchUpInSide(){
    
  CCLog("NUMBER=%d",_number);

}

void CCScrollViewX::scrollViewDidScroll(CCScrollView* view){
    

}

void CCScrollViewX::scrollViewDidZoom(CCScrollView* view){
    
}
