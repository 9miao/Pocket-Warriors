LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_MODULE_CLASS := STATIC_LIBRARIES

LOCAL_CPP_EXTENSION=.cc .c .cpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
../../Classes/BattleLayer.cpp \
../../Classes/WelfareLayer.cpp \
../../Classes/MessageBox.cpp \
../../Classes/GameOver.cpp \
../../Classes/CCScrollViewX.cpp \
../../Classes/PersonTemplate.cpp \
../../Classes/AnimatePacker.cpp \
../../Classes/AppDelegate.cpp \
../../Classes/BattleScene.cpp \
../../Classes/RegisterScene.cpp \
../../Classes/SkillInfo.cpp \
../../Classes/GameData.cpp \
../../Classes/CCTraceTo.cpp \
../../Classes/GeometricRecognizer.cpp \
../../Classes/LoadingLayer.cpp \
../../Classes/LoginScene.cpp \
../../Classes/MainView.cpp \
../../Classes/CheckPos.cpp \
../../Classes/Strengthen.cpp \
../../Classes/MessageManager.cpp \
../../Classes/SkillButton.cpp \
../../Classes/SelectFB.cpp \
../../Classes/LoadRes.cpp \
../../Classes/MyScrollView.cpp \
../../Classes/lib/network/ByteBuffer.cpp \
../../Classes/lib/network/MessageQueue.cpp \
../../Classes/lib/network/SocketClient.cpp \
../../Classes/lib/network/Util.cpp \
../../Classes/lib/network/message.cpp \
../../Classes/Voiceover.cpp 

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
$(LOCAL_PATH)/../../Classes/lib \
$(LOCAL_PATH)/../../Classes/lib/network \
$(LOCAL_PATH)/../../Classes/protofile \
$(LOCAL_PATH)/../../../../external/Box2D \
$(LOCAL_PATH)/src
LOCAL_CFLAGS +=-DCOCOS2D_DEBUG=1
LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
$(call import-module,external/Box2D)
$(call import-module,external/chipmunk)
