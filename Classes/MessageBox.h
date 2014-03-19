//
//  MessageBox.h
//  TestGame
//
//  Created by lh on 14-2-11.
//  //www.9miao.com
//

#ifndef __TestGame__MessageBox__
#define __TestGame__MessageBox__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
class MessageBox:public CCLayerColor{
public:
    static MessageBox *create();
    void CMessageBox(const char *title,const char *msgs);
private:
    CCSprite *m_pNodeBg;
    CCMenu *m_pNodeSureBtn;
    CCLabelTTF *m_pNodeTitleLable;
    CCLabelTTF *m_pNodeMsgLable;
    bool m_bSureBtnFlag;
    double m_dGap;
    virtual bool init();
    void goseAction();
    void isOK();
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
};
#endif /* defined(__TestGame__MessageBox__) */
