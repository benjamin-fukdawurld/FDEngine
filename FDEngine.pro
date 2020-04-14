#-------------------------------------------------
#
# Project created by QtCreator 2019-07-11T10:57:00
#
#-------------------------------------------------
TARGET = FDEngine
TEMPLATE = lib
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

DESTDIR = ../build/lib
MAKEFILE = ../build/makefiles/$${TARGET}
OBJECTS_DIR = ../build/.obj/$${TARGET}

LIBS += -L../build/lib -L../thirdparty/assimp/bin
LIBS += -lglad -lFDGL -lFD3D -lFDCore -lassimp
LIBS += -lglfw -lGL -lpthread -ldl -lX11 -lXrandr -lstdc++

INCLUDEPATH += include \
    ../FDCore/include \
    ../FD3D/include \
    ../FDGL/include \
    ../thirdparty/glad/include \
    ../thirdparty/pybind11/include \
    ../thirdparty/glm \
    ../thirdparty/assimp/include \
    ../thirdparty/ \

SOURCES += \
    src/BaseEngine.cpp

HEADERS += \
    include/FDEngine/BaseEngine.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
