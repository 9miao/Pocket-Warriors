//
//  GameOver.cpp
//  TestGame
//
//  Created by lh on 13-10-28.
//  www.9miao.com
//

#include "GameOver.h"
#include "MainView.h"
#include "message.h"
#include "PersonTemplate.h"
#include <time.h>
#include "MessageBox.h"
bool GameOver::init(){
    if (!CCLayer::init()) {
        return false;
    }
    time_t difference=time(NULL)-GameData::shareGameData()->oldtime;
    CCLog("%ld",difference);
    char costtime[40]="";
    sprintf(costtime, "%ld s",difference);
    socket=GameData::shareGameData()->socket;
    layer=UILayer::create();
    this->addChild(layer,0);
    layer->addWidget(CCUIHELPER->createWidgetFromJsonFile("gameover_1.json"));
    CCLog("%d",layer->getTag());
    setTouchEnabled(true);
    UIImageView *bg=(UIImageView *)layer->getWidgetByTag(2);
    UILabel *timelabel=(UILabel *)bg->getChildByTag(26);
    timelabel->setText(costtime);
    showRoleExp();
    return true;
}
void GameOver::showRoleExp(){
    for (int i=0; i<GameData::shareGameData()->roleArr->count(); i++) {
        //FriendlyRole *role=(FriendlyRole *)GameData::shareGameData()->roleArr->objectAtIndex(i);
        UIImageView *roleIcon=(UIImageView *)layer->getWidgetByTag(4+i);
        roleIcon->setVisible(true);
        UIImageView *roleExp=(UIImageView *)layer->getWidgetByTag(16+i);
        roleExp->setVisible(true);
        UILabel *expLabel=(UILabel *)layer->getWidgetByTag(28+i);
        expLabel->setVisible(true);
       // UIImageView *exp=(UIImageView *)roleExp->getChildByTag(20+i);

    }
}

void  GameOver::registerWithTouchDispatcher(){
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,-99902, true);
    CCLayer::registerWithTouchDispatcher();
}
bool GameOver::ccTouchBegan(CCTouch* touch, CCEvent* event){
    
    return true;
}
void GameOver::ccTouchMoved(CCTouch* touch, CCEvent* event){
    
}
void GameOver::ccTouchEnded(CCTouch* touch, CCEvent* event){
    setTouchEnabled(false);
    sendGameOverMsg();
}
void GameOver::sendGameOverMsg(){
    CSJson::FastWriter writer;
    CSJson::Value root;
    root["pid"]=GameData::shareGameData()->chaId;
    std::string str= writer.write(root);
    
    Message * msg= socket->constructMessage(str, CMID_EXITBATTLE);
    socket->sendMessage_(msg, false);
    this->schedule(schedule_selector(GameOver::receiveGameOverMsg));
    
}
void GameOver::receiveGameOverMsg(){
 
    Message * msg= (Message *)GameData::shareGameData()->msg_dic->objectForKey(CMID_EXITBATTLE);
    
    if (msg) {
        this->unschedule(schedule_selector(GameOver::receiveGameOverMsg));
        GameData::shareGameData()->msg_dic->removeObjectForKey(CMID_EXITBATTLE);
        CCLog("%s",msg->data);
        CSJson::Reader reader;
        CSJson::Value root;
        if (reader.parse(msg->data, root)) {
            if (root["result"].asBool()) {
                CCScene *scene= MainView::scene();
                MainView *mv= (MainView *)scene->getChildByTag(101);
                mv->color->setOpacity(GLubyte(0));
                mv->setRole();
                mv->setCoin();
                mv->intensifLayer->setVisible(true);
                CCDirector::sharedDirector()->replaceScene(scene);
            }else{
                
                MessageBox *msgbox=MessageBox::create();
                msgbox->CMessageBox("错 误", "发生错误!");
              //  CCDirector::sharedDirector()->getRunningScene()->addChild(msgbox,2);
            }
        }
        
    }
}