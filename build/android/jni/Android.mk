LOCAL_PATH := $(call my-dir)

CPGF_PATH := /projects/cpgf
SFML_PATH := /source/SFML-2.4.1


include $(CLEAR_VARS)
LOCAL_MODULE := cpgf
LOCAL_SRC_FILES := libcpgf.so
include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE := lua
LOCAL_SRC_FILES := liblua.so
include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE := gincu

ROOT_PATH := E:\projects\gincu
#ROOT_PATH := $(LOCAL_PATH)\..\..\..\..
SRC_PATH := $(ROOT_PATH)\src
TESTS_PATH := $(ROOT_PATH)\tests
METADATA_PATH := $(ROOT_PATH)\metagen

LOCAL_CFLAGS += -O3 -frtti -fexceptions -std=c++11 -DENABLE_LUA=1

#LIB_PATH := e:/projects/cpgf/build/android/libs/armeabi-v7a/
#LOCAL_LDLIBS += -llibcpgf.so

LOCAL_C_INCLUDES += \
	$(ROOT_PATH)/include \
	$(TESTS_PATH)/application \
	$(CPGF_PATH)/include \
	$(SFML_PATH)/include \
	$(ROOT_PATH)/thirdparty \

LOCAL_SRC_FILES := \
	$(ROOT_PATH)/thirdparty/zf_log.cpp \
	$(TESTS_PATH)/application/main.cpp \
	$(TESTS_PATH)/application/uiutil.cpp \
	$(TESTS_PATH)/application/mainapplication.cpp \
	$(TESTS_PATH)/application/scenelogo.cpp \
	$(TESTS_PATH)/application/scenemenu.cpp \
	$(TESTS_PATH)/application/matchthree/componentchess.cpp \
	$(TESTS_PATH)/application/matchthree/matchthreeboard.cpp \
	$(TESTS_PATH)/application/matchthree/matchthreechess.cpp \
	$(TESTS_PATH)/application/matchthree/matchthreeinfoview.cpp \
	$(TESTS_PATH)/application/matchthree/scenematchthree.cpp \
	$(TESTS_PATH)/application/matchthree/statecheckdeadboard.cpp \
	$(TESTS_PATH)/application/matchthree/stateerasematches.cpp \
	$(TESTS_PATH)/application/matchthree/stateinitializenewround.cpp \
	$(TESTS_PATH)/application/matchthree/statemainloop.cpp \
	$(TESTS_PATH)/application/matchthree/stateresult.cpp \
	$(TESTS_PATH)/application/matchthree/stateswapchesses.cpp \
	$(TESTS_PATH)/application/testbed/scenetestbed.cpp \
	$(TESTS_PATH)/application/testbed/scenetestcase.cpp \
	$(TESTS_PATH)/application/testbed/testbed.cpp \
	$(TESTS_PATH)/application/testbed/testcase.cpp \
	$(TESTS_PATH)/application/testbed/testcases/testcase_animation.cpp \
	$(TESTS_PATH)/application/testbed/testcases/testcase_benchmark.cpp \
	$(TESTS_PATH)/application/testbed/testcases/testcase_camera.cpp \
	$(TESTS_PATH)/application/testbed/testcases/testcase_flip.cpp \
	$(TESTS_PATH)/application/testbed/testcases/testcase_scenegraph.cpp \
	$(SRC_PATH)/gerrorhandler.cpp \
	$(SRC_PATH)/gcolor.cpp \
	$(SRC_PATH)/gframeanimation.cpp \
	$(SRC_PATH)/gatlas.cpp \
	$(SRC_PATH)/gatlasrender.cpp \
	$(SRC_PATH)/grenderanchor.cpp \
	$(SRC_PATH)/sfml/gsfmldevicecontext.cpp \
	$(SRC_PATH)/sfml/gsfmlrendercontext.cpp \
	$(SRC_PATH)/gimage.cpp \
	$(SRC_PATH)/gtransform.cpp \
	$(SRC_PATH)/ecs/gcomponent.cpp \
	$(SRC_PATH)/ecs/gcomponentanchor.cpp \
	$(SRC_PATH)/ecs/gcomponentanimation.cpp \
	$(SRC_PATH)/ecs/gcomponentcamera.cpp \
	$(SRC_PATH)/ecs/gcomponentframeanimation.cpp \
	$(SRC_PATH)/ecs/gcomponentlocaltransform.cpp \
	$(SRC_PATH)/ecs/gcomponentmanager.cpp \
	$(SRC_PATH)/ecs/gcomponenttouchhandler.cpp \
	$(SRC_PATH)/ecs/gcomponenttransform.cpp \
	$(SRC_PATH)/ecs/gcomponentrender.cpp \
	$(SRC_PATH)/ecs/gentity.cpp \
	$(SRC_PATH)/ecs/gentitypolicy.cpp \
	$(SRC_PATH)/ecs/gentityutil.cpp \
	$(SRC_PATH)/transition/gtransition.cpp \
	$(SRC_PATH)/transition/gtransitionmovein.cpp \
	$(SRC_PATH)/transition/gtransitionfadeinout.cpp \
	$(SRC_PATH)/gapplication.cpp \
	$(SRC_PATH)/gcamera.cpp \
	$(SRC_PATH)/gdevicecontext.cpp \
	$(SRC_PATH)/gevent.cpp \
	$(SRC_PATH)/geventqueue.cpp \
	$(SRC_PATH)/gfont.cpp \
	$(SRC_PATH)/gfileinputstream.cpp \
	$(SRC_PATH)/gmatrix.cpp \
	$(SRC_PATH)/gobjectfactory.cpp \
	$(SRC_PATH)/gresourcemanager.cpp \
	$(SRC_PATH)/gscene.cpp \
	$(SRC_PATH)/gscenemanager.cpp \
	$(SRC_PATH)/gstate.cpp \
	$(SRC_PATH)/gstatemachine.cpp \
	$(SRC_PATH)/gtextrender.cpp \
	$(SRC_PATH)/gtexture.cpp \
	$(SRC_PATH)/gutil.cpp \
	$(SRC_PATH)/gvertexarray.cpp \
	$(SRC_PATH)/gvertexarrayrender.cpp \
	$(SRC_PATH)/gworker.cpp \
	$(SRC_PATH)/scripting/gscriptingmain.cpp \
	$(SRC_PATH)/scripting/gscriptingmetadata.cpp \
	$(SRC_PATH)/scripting/gscriptingutil.cpp \

LOCAL_SHARED_LIBRARIES := cpgf
LOCAL_SHARED_LIBRARIES += lua
LOCAL_SHARED_LIBRARIES += sfml-system
LOCAL_SHARED_LIBRARIES += sfml-window
LOCAL_SHARED_LIBRARIES += sfml-graphics
LOCAL_SHARED_LIBRARIES += sfml-audio
LOCAL_SHARED_LIBRARIES += sfml-network
LOCAL_WHOLE_STATIC_LIBRARIES := sfml-main

include $(BUILD_SHARED_LIBRARY)

$(call import-module,sfml)

