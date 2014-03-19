//
//  CheckPos.cpp
//  TestGame
//
//  Created by lh on 13-11-6.
//  www.9miao.com
//

#include "CheckPos.h"
static CheckPos *pos=NULL;
CheckPos::CheckPos(){
    
}
CheckPos::~CheckPos(){
    
}
CheckPos *CheckPos::shareCheckPos(){
    if (!pos) {
        pos=new CheckPos();
        
    }
    return pos;
}
CCPoint CheckPos::CallBackPos(int typeId){
    CCPoint pos;
    int a= arc4random()%4+1;
    int x,y;
    switch (a) {
        case 1:
            x=arc4random()%1920;
            y=1780;
            break;
        case 2:
            x=arc4random()%1920;
            y=-700;
            break;
        case 3:
            x=-500;
            y=arc4random()%1080;
            break;
        case 4:
            x=1920+500;
            y=arc4random()%1080;
            break;
        case 5:
            x=arc4random()%960;
            y=arc4random()%540+540;
            break;
        case 6:
            x=arc4random()%960+960;
            y=arc4random()%540+540;
            
            break;
        case 7:
            x=arc4random()%960;
            y=arc4random()%540;
            
            break;
        case 8:
            x=arc4random()%960+960;
            y=arc4random()%540;
            break;
        default:
            break;
    }
    return ccp(x, y);
}