//
//  GameData.cpp
//  TestGame
//
//  Created by lh on 13-9-12.
//  www.9miao.com
//

#include "GameData.h"
static GameData *gamedata=NULL;
GameData::GameData(){
    eneMoveFlag=false;
    SkillBtnIsTouch= false;
    fire_Flag=false;
    roleDic= CCDictionary::create();
    roleDic->retain();
    eneDic=CCDictionary::create();
    eneDic->retain();
    roleArr=CCArray::create();
    roleArr->retain();
    enemyArr=CCArray::create();
    enemyArr->retain();
    posArr=CCArray::create();
    posArr->retain();
    msg_dic= CCDictionary::create();
    msg_dic->retain();
}
GameData::~GameData(){
    
}
int GameData::getRandom(){
    int random=arc4random()%1000+1;
//    int send=1;
//    srand(unsigned(time(NULL)));
//    switch (random) {
//        case 0:
//            send=rand()%1000+1;
//            break;
//        case 1:
//            send=rand()%1000+1;
//            break;
//        case 2:
//            send=rand()%1000+1;
//            break;
//        case 3:
//            send=rand()%1000+1;
//            break;
//        case 4:
//            send=rand()%1000+1;
//            break;
//        case 5:
//            send=rand()%1000+1;
//            break;
//        default:
//            break;
//    }
    return random;
}
GameData * GameData::shareGameData(){
    if(!gamedata)
        gamedata = new GameData();
    
    return gamedata;
}
void GameData::setTouchPoint(cocos2d::CCPoint pos){
    point=pos;
}
CCPoint GameData::getTouchPoint(){
    return point;
}

//inline const ::allPetInfo1101::PetFight& GameData::pfinfo(int index) const {
//    return pfinfo_.Get(index);
//}
//inline const ::google::protobuf::RepeatedPtrField< ::allPetInfo1101::PetFight >&
//GameData::pfinfo() const {
//    return pfinfo_;
//}
//void GameData::setPetFightArr(::google::protobuf::RepeatedPtrField< ::allPetInfo1101::PetFight > pfinfo){
//    pfinfo_=pfinfo;
//}
//
//void GameData::setGroupList(::google::protobuf::RepeatedPtrField< ::instanceInfo1201::Group > grouplist){
//    grouplist_=grouplist;
//}
//inline const ::google::protobuf::RepeatedPtrField< ::instanceInfo1201::Group >&
//GameData::grouplist() const{
//    return grouplist_;
//}
//inline const ::instanceInfo1201::Group& GameData::grouplist(int index) const{
//    return grouplist_.Get(index);
//}
//
//
//inline const ::MonsterInfo& GameData::mlist(int index) const {
//    return mlist_.Get(index);
//}
//inline const ::google::protobuf::RepeatedPtrField< ::MonsterInfo >&
//GameData::mlist() const {
//    return mlist_;
//}
//void GameData::setMonsterGroup(::google::protobuf::RepeatedPtrField< ::MonsterInfo> mlist){
//    mlist_=mlist;
//}


attackInfo::attackInfo(){
    charid=0;
    attack_id=0;
    countofAttack=0;
    skillId=0;
    attack_time="";
    attackeds=CCArray::create();
    attackeds->retain();
}
attackInfo::~attackInfo(){
    
}



attackedInfo::attackedInfo(){
    
    
}
attackedInfo::~attackedInfo(){
    
}




