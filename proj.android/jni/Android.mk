LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/ClassBase/DialogBase.cpp \
                   ../../Classes/ClassBase/LayerBase.cpp \
                   ../../Classes/ClassBase/MyBodyParser.cpp \
                   ../../Classes/Database/DataUtils.cpp \
                   ../../Classes/Database/sqlite3.c \
                   ../../Classes/Database/SqlUtils.cpp \
                   ../../Classes/ModeSelectScene/ModeSelectScene.cpp \
                   ../../Classes/Multi/BattleScene/BattleScene.cpp \
                   ../../Classes/Multi/BattleScene/MoveTypeSelectDialog.cpp \
                   ../../Classes/Multi/SkillSelectScene/SkillSelectScene.cpp \
                   ../../Classes/Multi/SkillSelectScene/SkillDetailDialog.cpp \
                   ../../Classes/Multi/UnitSelectScene/UnitSelectScene.cpp \
                   ../../Classes/Multi/UnitSelectScene/UnitDetailDialog.cpp \
                   ../../Classes/Object/Character/Character.cpp \
                   ../../Classes/Object/Background/Background.cpp \
                   ../../Classes/Object/Effect/EffectParticle.cpp \
                   ../../Classes/Object/Effect/EffectGAF.cpp \
                   ../../Classes/Object/Effect/EffectLWF.cpp \
                   ../../Classes/Object/MiniCircle/MiniCircle.cpp \
                   ../../Classes/Model/DataModel.cpp \
                   ../../Classes/Model/UnitDataModel.cpp \
                   ../../Classes/Model/SkillDataModel.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
