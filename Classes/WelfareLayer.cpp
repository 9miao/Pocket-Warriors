//
//  WelfareLayer.cpp
//  TestGame
//
//  Created by lh on 14-2-17.
//  www.9miao.com
//

#include "WelfareLayer.h"
#include "AnimatePacker.h"
#include "MessageManager.h"
#include "MainView.h"
CCScene *WelfareLayer::scene(){
    CCScene *scene=CCScene::create();
    CCLayer *layer=WelfareLayer::create();
    layer->setTag(100000);
    scene->addChild(layer);
    return scene;
}
bool WelfareLayer::init(){
    if (!CCLayer::init()) {
        return false;
    }
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(WelfareLayer::reloadInfo), "welfare", NULL);
    m_pTouchCell=NULL;
    receiveFlag=false;
    isMoving=false;
    AnimatePacker::getInstance()->loadAnimations("lqcg.xml");
    CCSize winSize=CCDirector::sharedDirector()->getWinSize();
    CCLabelTTF *nameLabel=CCLabelTTF::create("这里是名字", SELF_TTF_ONE, 60, CCSizeMake(400, 60), kCCTextAlignmentLeft);
    nameLabel->setAnchorPoint(ccp(0, 0));
    nameLabel->setColor(fontColor);
    nameLabel->setPosition(ccp(240, 870));
    this->addChild(nameLabel,1);
    char coin[8]="";
    char gold[8]="";
    sprintf(gold, "%d",GameData::shareGameData()->gold);
    sprintf(coin,"%d",GameData::shareGameData()->coin);
    CCLabelTTF *coinLabel=CCLabelTTF::create(coin, SELF_TTF_ONE, 60, CCSizeMake(200, 60), kCCTextAlignmentLeft);
    coinLabel->setAnchorPoint(ccp(0, 0));
    coinLabel->setPosition(ccp(winSize.width/2-30, 870));
    coinLabel->setColor(fontColor);
    this->addChild(coinLabel,1);
    CCLabelTTF *goldLabel=CCLabelTTF::create(gold, SELF_TTF_ONE, 60, CCSizeMake(200, 60), kCCTextAlignmentLeft);
    goldLabel->setAnchorPoint(ccp(0, 0));
    goldLabel->setPosition(ccp(winSize.width*3/4+60, 870));
    goldLabel->setColor(fontColor);
    this->addChild(goldLabel,1);
    
    m_welfareArr=CCArray::create();
    m_welfareArr->retain();
    CCSprite *layerBg=CCSprite::create(WELFLAYER_BG);
    layerBg->setPosition(ccp(winSize.width/2, winSize.height/2));
    this->addChild(layerBg,-1);
    
    tableview=CCTableView::create(this, CCSizeMake(1920, 800));
    tableview->setVerticalFillOrder(kCCTableViewFillTopDown);
    tableview->setAnchorPoint(ccp(0, 0));
    tableview->setPosition(ccp(0, 0));
    tableview->setDelegate(this);
    tableview->setDataSource(this);
    this->addChild(tableview);
    setData();
    setTouchEnabled(true);
    return true;
}
CCSize WelfareLayer::tableCellSizeForIndex(CCTableView *table, unsigned int idx){
    return CCSizeMake(1429, 192);
}
CCTableViewCell* WelfareLayer::tableCellAtIndex(CCTableView *table, unsigned int idx){
//    CCTableViewCell *cell=table->dequeueCell();
//    if (!cell) {
Welfare *wf=(Welfare *)m_welfareArr->objectAtIndex(idx);
    CCTableViewCell * cell=new CCTableViewCell();
    CCSprite *cellBg=CCSprite::create(WELFCELL_BG);
    cellBg->setAnchorPoint(ccp(0, 0));
    cellBg->setPosition(ccp(0, 0));
    cell->addChild(cellBg);
    CCSprite *money;
    char coin[10]="";
    
    if (wf->icontype==1) {
        sprintf(coin, "%d金币",wf->number);
        money=CCSprite::create("gold.png");
        
    }else if(wf->icontype==2){
        sprintf(coin, "%d银币",wf->number);
        money=CCSprite::create("coin.png");
        
    }
    money->setAnchorPoint(ccp(0, 0));
    money->setPosition(ccp(20, 30));
    cell->addChild(money,1);
    
    
    
    CCSprite *receiveBtn;
    
    if (wf->state==0) {
        receiveBtn=CCSprite::create(RECEIVE_BTN_DIS);
        receiveFlag=false;
    }else if(wf->state==1){
        receiveBtn=CCSprite::create(RECEIVE_BTN_UP);
        receiveFlag=true;
    }
    receiveBtn->setAnchorPoint(ccp(0, 0));
    receiveBtn->setPosition(ccp(1429, 0));
    cell->addChild(receiveBtn);
    
    
    CCLabelTTF *coinCount=CCLabelTTF::create(coin, SELF_TTF_ONE, 60, CCSizeMake(300, 60), kCCTextAlignmentLeft);
    coinCount->setAnchorPoint(ccp(0, 0));
    coinCount->setPosition(ccp(250, 70));
    coinCount->setColor(ccc3(57, 37, 31));
    cell->addChild(coinCount,1);
    //CCMenuItemImage::create(RECEIVE_BTN_UP, RECEIVE_BTN_DOWN, this, NULL);
    
    CCLabelTTF *descriptionslabel=CCLabelTTF::create(wf->descriptions.c_str(), SELF_TTF_ONE, 60, CCSizeMake(800, 60), kCCTextAlignmentLeft);
    descriptionslabel->setAnchorPoint(ccp(0, 0));
    descriptionslabel->setColor(ccc3(57, 37, 31));
    descriptionslabel->setPosition(ccp(630, 70));
    cell->addChild(descriptionslabel,1);
    // 250  70     300 60
    
    return cell;
}
unsigned int WelfareLayer::numberOfCellsInTableView(CCTableView *table){
    return m_welfareArr->count();
}



void  WelfareLayer::registerWithTouchDispatcher(){
    CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,0);
}
void WelfareLayer::ccTouchesBegan(CCSet* set, CCEvent* event){
    CCSetIterator  sel= set->begin();
    touchNum++;
    for (; sel!=set->end(); sel++) {
        CCTouch *pTouch=(CCTouch *)(* sel);
        if (touchNum==2) {
          
            distance=pTouch->getLocation().x-this->boundingBox().origin.x;
        }else if(m_pTouchCell){
            distance=pTouch->getLocation().x-this->boundingBox().origin.x;
            m_beginPoint=pTouch->getLocation();
            return;
        }
        
        return;
    }
}
void WelfareLayer::ccTouchesMoved(CCSet* set, CCEvent* event){
    CCSetIterator  sel= set->begin();
    for (; sel!=set->end(); sel++) {
        
        CCTouch *pTouch=(CCTouch *)(* sel);
        
        if (touchNum>1) {
            this->setPosition(ccp(pTouch->getLocation().x-distance, 0));
            
        }else{
            if (fabs(m_beginPoint.y-pTouch->getLocation().y)<20&&m_pTouchCell) {
                m_pTouchCell->setPosition(ccp(pTouch->getLocation().x-distance, m_pTouchCell->getPosition().y));
            }else{
                distance=0;
                
                layerEaseOut(this,true);
            }
        }
    }
}
void WelfareLayer::ccTouchesEnded(CCSet* set, CCEvent* event){
    CCSetIterator  sel= set->begin();
    touchNum--;
    
    for (; sel!=set->end(); sel++) {
        CCTouch *pTouch=(CCTouch *)(* sel);
        if (m_pTouchCell) {
            easeOut(m_pTouchCell);
            return;
        }
        if (this->getPosition().x>500) {
           layerEaseOut(this,false);
        }
       
    }
}
void WelfareLayer::setData(){
    m_welfareArr->removeAllObjects();
    
    CSJson::Reader reader;
    CSJson::Value root;
    if (reader.parse(GameData::shareGameData()->welfare_Str, root)) {
        CSJson::Value dlist;
        dlist=root["dlist"];
        for (int i=0; i<dlist.size(); i++) {
            Welfare *wfr=new Welfare();
            wfr->typeId=dlist[i]["typeid"].asInt();
            wfr->state=dlist[i]["state"].asInt();
            wfr->icontype=dlist[i]["icontype"].asInt();
            wfr->number=dlist[i]["number"].asInt();
            wfr->descriptions=dlist[i]["descriptions"].asString();
            m_welfareArr->addObject(wfr);
            
        }
    }
    tableview->reloadData();
}
void WelfareLayer::layerEaseOut(cocos2d::CCNode *node,bool dircetion){
    if (!isMoving&&dircetion) {
        node->stopActionByTag(0);
        CCMoveTo *moveto=CCMoveTo::create(0.3, ccp(0, 0));
        isMoving=true;
        CCEaseOut *easeout=CCEaseOut::create(moveto, 1);
        CCCallFunc *func=CCCallFunc::create(this, callfunc_selector(WelfareLayer::callBackLayerEaseOut));
        CCSequence *seq=CCSequence::create(easeout,func,NULL);
        easeout->setTag(0);
        node->runAction(seq);
    }else if(!isMoving&&!dircetion){
        node->stopActionByTag(1);
        CCSize winSize=CCDirector::sharedDirector()->getWinSize();
        CCMoveTo *moveReplace=CCMoveTo::create(0.3, ccp(winSize.width, 0));
        isMoving=true;
        CCEaseOut *easeout=CCEaseOut::create(moveReplace, 1);
        CCCallFunc *func=CCCallFunc::create(this, callfunc_selector(WelfareLayer::callBackLayerGoOut));
        CCSequence *seq=CCSequence::create(easeout,func,NULL);
        seq->setTag(1);
        node->runAction(seq);
    }
    
}
void WelfareLayer::callBackLayerGoOut(){
    isMoving=false;
    CCScene *scene=MainView::scene();
    CCDirector::sharedDirector()->replaceScene(scene);
}
void WelfareLayer::callBackLayerEaseOut(){
    isMoving=false;
}
void WelfareLayer::easeOut(CCNode *sender){
    CCNode *cell=(CCNode *)sender;
    CCSize winSize=CCDirector::sharedDirector()->getWinSize();
    CCPoint endedPos;
    CCSequence *seq;
    CCCallFuncN *func;
    if (cell->getPosition().x>600) {
        endedPos=CCPointMake(winSize.width, cell->getPosition().y);
        func=CCCallFuncN::create(this, callfuncN_selector(WelfareLayer::easeOutCallBack));
        //(this, callfunc_selector(WelfareLayer::easeOutCallBack));
        CCSprite *lqcgAni=CCSprite::createWithSpriteFrameName("lqcg_00.png");
        lqcgAni->setTag(10000);
        lqcgAni->setScale(2);
        CCCallFunc *func=CCCallFunc::create(this, callfunc_selector(WelfareLayer::lqcgCallBack));
        CCSequence *seq=CCSequence::create(AnimatePacker::getInstance()->getAnimate("lqcg"),func,NULL);
        lqcgAni->runAction(seq);
        lqcgAni->setPosition(ccp(winSize.width/2, winSize.height/2));
        this->addChild(lqcgAni,10);
    }else{
        endedPos=CCPointMake(0, cell->getPosition().y);
        func=CCCallFuncN::create(this,NULL);
    }
    CCMoveTo *moveto=CCMoveTo::create(0.3, endedPos);
    CCEaseIn *easein=CCEaseIn::create(moveto, 1);
    seq=CCSequence::create(easein,func,NULL);
    //func=CCCallFunc::create(this, callfunc_selector(WelfareLayer::easeOutCallBack));
    
    
    cell->runAction(seq);
}
void WelfareLayer::lqcgCallBack(){
    this->removeChildByTag(10000);
}
void WelfareLayer::easeOutCallBack(CCObject *obj){
    CCTableViewCell *cell=(CCTableViewCell *)obj;
    Welfare *wfr=(Welfare *)m_welfareArr->objectAtIndex(cell->getIdx());
    MessageManager::shareMessageManager()->sendeGetWelfareMessage(wfr->typeId);
    
    CCSize winSize=CCDirector::sharedDirector()->getWinSize();
    cell->setPosition(ccp(-winSize.width, cell->getPosition().y));
    CCMoveTo *moveto=CCMoveTo::create(0.2, ccp(0, cell->getPosition().y));
    CCEaseOut *easeout=CCEaseOut::create(moveto, 1);
    cell->runAction(easeout);
}
void WelfareLayer::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent){
    touchNum--;
}
void WelfareLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell){

    
}
void WelfareLayer::tableCellHighlight(CCTableView* table, CCTableViewCell* cell){
    if (((Welfare *)(m_welfareArr->objectAtIndex(cell->getIdx())))->state==1) {
        m_pTouchCell=cell;
    }else if (((Welfare *)(m_welfareArr->objectAtIndex(cell->getIdx())))->state==0)
    {
        return;
    }

}
void WelfareLayer::tableCellUnhighlight(CCTableView* table, CCTableViewCell* cell){
    
}
void WelfareLayer::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell){
    
}
void WelfareLayer::scrollViewDidScroll(CCScrollView* view){
    
}
void WelfareLayer::scrollViewDidZoom(CCScrollView* view){
    
}

void WelfareLayer::reloadInfo(CCObject *obj){
    Message *msg=(Message *)obj;
    GameData::shareGameData()->welfare_Str=msg->data;
    this->scheduleOnce(schedule_selector(WelfareLayer::setData), 0.01);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "welfare");
    
}
WelfareLayer::~WelfareLayer(){
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}
