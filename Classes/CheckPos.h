//
//  CheckPos.h
//  TestGame
//
//  Created by lh on 13-11-6.
//  www.9miao.com
//

#ifndef __TestGame__CheckPos__
#define __TestGame__CheckPos__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;
class CheckPos{
public:
    CheckPos();
    ~CheckPos();
    static CheckPos *shareCheckPos();
    CCPoint CallBackPos(int typeId);
};
#endif /* defined(__TestGame__CheckPos__) */
