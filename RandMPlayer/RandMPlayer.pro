# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------

TEMPLATE = app
TARGET = RandMPlayer
DESTDIR = ../x64/Release
QT += core gui multimedia widgets winextras multimediawidgets
CONFIG += release
DEFINES += _UNICODE WIN64 QT_DLL QT_MULTIMEDIA_LIB QT_MULTIMEDIAWIDGETS_LIB QT_WIDGETS_LIB QT_WINEXTRAS_LIB
INCLUDEPATH += ./GeneratedFiles \
    . \
    ./GeneratedFiles/$(ConfigurationName)
PRECOMPILED_HEADER = stdafx.h
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/$(ConfigurationName)
OBJECTS_DIR += release
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(RandMPlayer.pri)
win32:RC_FILE = RandMPlayer.rc
