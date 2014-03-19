//
//  WelfareLayer.h
//  TestGame
//
//  Created by lh on 14-2-17.
//  www.9miao.com
//

#ifndef __TestGame__WelfareLayer__
#define __TestGame__WelfareLayer__

#include <iostream>
#include "HeaderBase.h"
#include "message.h"
class WelfareLayer:public CCLayer,public CCTableViewDataSource,CCTableViewDelegate{
public:
    ~WelfareLayer();
    virtual bool init();
    static CCScene *scene();
    CCTableView *tableview;
    CCArray *m_welfareArr;
    bool receiveFlag;
    bool isMoving;
    float distance;
    void layerEaseOut(CCNode *node,bool dircetion);
    void callBackLayerGoOut();
    void callBackLayerEaseOut();
    void easeOut(CCNode *sender);
    void easeOutCallBack(CCObject *obj);
    void setData();
    void lqcgCallBack();
    void reloadInfo(CCObject *obj);

    CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);
    CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    unsigned int numberOfCellsInTableView(CCTableView *table);
    unsigned int touchNum;
    virtual void  registerWithTouchDispatcher(void);
    virtual void ccTouchesBegan(CCSet* set, CCEvent* event);
    virtual void ccTouchesMoved(CCSet* set, CCEvent* event);
    virtual void ccTouchesEnded(CCSet* set, CCEvent* event);
    virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);
    CREATE_FUNC(WelfareLayer);
public:
    CCPoint m_beginPoint;
    CCPoint m_endPoint;
    CCTableViewCell *m_pTouchCell;
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual void tableCellHighlight(CCTableView* table, CCTableViewCell* cell);
    virtual void tableCellUnhighlight(CCTableView* table, CCTableViewCell* cell);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view);
    
};
class Welfare:public CCObject{
public:
    int typeId;
    int state;
    int icontype;
    int number;
    std::string descriptions;
    
    Welfare(){
        typeId=0;
        state=0;
        icontype=0;
        number=0;
        descriptions="";
    }
    ~Welfare(){
        
    }
};
#endif /* defined(__TestGame__WelfareLayer__) */
