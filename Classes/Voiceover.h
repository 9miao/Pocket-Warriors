//
//  Voiceover.h
//  TestGame
//
//  Created by lh on 13-9-22.
//  www.9miao.com
//

#ifndef __TestGame__Voiceover__
#define __TestGame__Voiceover__

#include <iostream>
#include "HeaderBase.h"
class Voiceover:public CCLayerColor{
public:
    virtual bool init();
    void playDesc();
    void playDesc2();
    void fadeout();
    void removeSelf();
    CCLabelTTF * description;
    CCLabelTTF * description2;
    CREATE_FUNC(Voiceover);
    
};
#endif /* defined(__TestGame__Voiceover__) */
