# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

TEMPLATE = app
TARGET = DHAPP
DESTDIR = ../Release
QT += core gui xml xmlpatterns
CONFIG += debug console
DEFINES += QT_LARGEFILE_SUPPORT QT_XML_LIB QT_XMLPATTERNS_LIB QT_DLL SAVE_MINI_IMAGE
INCLUDEPATH += ./ElementQuery \
    ./GeneratedFiles \
    ./GeneratedFiles/$(Configuration) \
    ../../../../../../../SDK/opencv/build/include \
    ../../../../../../../SDK/opencv/build/include/opencv \
    ../../../../../../../SDK/boost_1_55_0 \
    ../../../../../../../SDK/opencv/build/include/opencv2 \
    $(SolutionDir)DHAPP \
    $(SolutionDir)thirdparty/Database/Driver
LIBS += -L"./../Release" \
    -L"../../../../../../../SDK/opencv/build/x86/vc10/lib" \
    -lopencv_highgui249 \
    -lopencv_core249 \
    -lopencv_ml249 \
    -lopencv_imgproc249 \
    -lopencv_features2d249 \
    -lopencv_flann249 \
    -lopencv_gpu249 \
    -lopencv_nonfree249 \
    -lopencv_calib3d249
PRECOMPILED_HEADER = StdAfx.h
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/release
OBJECTS_DIR += release
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(DHAPP.pri)
