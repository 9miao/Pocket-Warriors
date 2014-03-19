//
//  LoadingLayer.h
//  TestGame
//
//  Created by lh on 13-9-18.
//  www.9miao.com
//

#ifndef __TestGame__LoadingLayer__
#define __TestGame__LoadingLayer__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;
using namespace std;
class LoadingLayer:public CCLayerColor{
public:
    virtual bool init();
    CCObject *m_pObj;
    CCSprite *m_pNodeProgressBg;
    CCSprite *m_pNodeLoadingBg;
    CCProgressTimer *m_pProgress;
    int m_nCurCount;
    void loadFinished();
    void setProgress(int percents,int max);
    vector<string> m_vResourceArray;
    void addImage(vector<string> arr,CCObject *obj);
    vector<string>::const_iterator nowcount;
    pthread_t th_load;
    int threadStart();
    void RemoveSelf();
    static void* thread_funcation(void *arg);
    CREATE_FUNC(LoadingLayer);
};
#endif /* defined(__TestGame__LoadingLayer__) */
