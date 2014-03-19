//
//  LoadRes.cpp
//  TestGame
//
//  Created by lh on 13-11-13.
//  www.9miao.com
//

#include "LoadRes.h"
#include "AnimatePacker.h"
static LoadRes * load=NULL;
LoadRes::LoadRes(){
    
}
LoadRes::~LoadRes(){
    
}
LoadRes * LoadRes::shareLoadRes(){
    if (!load) {
        load= new LoadRes();
    }
    return load;
}
void LoadRes::loadRolePersonTexture02(){
    AnimatePacker::getInstance()->loadAnimations("fsoldier01.xml");

    return;
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("fuzinv0.png", "fuzinv0.plist", "fuzinv.ExportJson");
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("fuzinv1.png", "fuzinv1.plist", "fuzinv.ExportJson");
    
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("fuzinvrun0.png", "fuzinvrun0.plist", "fuzinvrun.ExportJson");
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("fuzinvrun1.png", "fuzinvrun1.plist", "fuzinvrun.ExportJson");

}
void LoadRes::freeRolePersonTexture02(){

    
}


void LoadRes::loadRolePersonTexture(){
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("person-back/char_back0.png", "person-back/char_back0.plist", "person-back/char_back.ExportJson");
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("person-back/char_back1.png", "person-back/char_back1.plist", "person-back/char_back.ExportJson");
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("person-back/char_back2.png", "person-back/char_back2.plist", "person-back/char_back.ExportJson");
    
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("person-front/char_front0.png", "person-front/char_front0.plist", "person-front/char_front.ExportJson");
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("person-front/char_front1.png", "person-front/char_front1.plist", "person-front/char_front.ExportJson");
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("person-front/char_front2.png", "person-front/char_front2.plist", "person-front/char_front.ExportJson");
    
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("person-right/char_right0.png", "person-right/char_right0.plist", "person-right/char_right.ExportJson");
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("person-right/char_right1.png", "person-right/char_right1.plist", "person-right/char_right.ExportJson");
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("person-right/char_right2.png", "person-right/char_right2.plist", "person-right/char_right.ExportJson");
    
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("person-rightb/char_rightb0.png", "person-rightb/char_rightb0.plist", "person-rightb/char_rightb.ExportJson");
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("person-rightb/char_rightb1.png", "person-rightb/char_rightb1.plist", "person-rightb/char_rightb.ExportJson");
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("person-rightb/char_rightb2.png", "person-rightb/char_rightb2.plist", "person-rightb/char_rightb.ExportJson");
    
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("person-rightf/char_rightf0.png", "person-rightf/char_rightf0.plist", "person-rightf/char_rightf.ExportJson");
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("person-rightf/char_rightf1.png", "person-rightf/char_rightf1.plist", "person-rightf/char_rightf.ExportJson");
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("person-rightf/char_rightf2.png", "person-rightf/char_rightf2.plist", "person-rightf/char_rightf.ExportJson");

}
void LoadRes::freeRolePersonTexture(){
    CCArmatureDataManager::sharedArmatureDataManager()->removeTextureData("char_back2.png");
    CCArmatureDataManager::sharedArmatureDataManager()->removeTextureData("char_back1.png");
    CCArmatureDataManager::sharedArmatureDataManager()->removeTextureData("char_back0.png");
}
void LoadRes::loadBossTexture(){
    AnimatePacker::getInstance()->loadAnimations("cattlem02.xml");
    return;
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("boss/daozhangrun0.png", "boss/daozhangrun0.plist", "boss/daozhangrun.ExportJson");
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("boss/daozhangrun1.png", "boss/daozhangrun1.plist", "boss/daozhangrun.ExportJson");
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("boss/daozhang1.png", "boss/daozhang1.plist", "boss/daozhang.ExportJson");
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("boss/daozhang0.png", "boss/daozhang0.plist", "boss/daozhang.ExportJson");
}
void LoadRes::freeBossTexture(){
    
}

void LoadRes::loadEnemyPersonTexture(){
    AnimatePacker::getInstance()->loadAnimations("fechief01.xml");
    return;
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("man0/shanzinan0.png", "man0/shanzinan0.plist", "man0/shanzinan.ExportJson");
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("man0/shanzinan1.png", "man0/shanzinan1.plist", "man0/shanzinan.ExportJson");
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("man0/shanzinanrun0.png", "man0/shanzinanrun0.plist", "man0/shanzinanrun.ExportJson");
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("man0/shanzinanrun1.png", "man0/shanzinanrun1.plist", "man0/shanzinanrun.ExportJson");
}
void LoadRes::freeEnemyPersonTexture(){
    
}