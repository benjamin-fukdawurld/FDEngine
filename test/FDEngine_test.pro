TARGET = FDEngine_test

TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

DESTDIR = ../../build/bin
MAKEFILE = ../../build/makefiles/$${TARGET}
OBJECTS_DIR = ../../build/obj/$${TARGET}

LIBS += -L../../build/lib -lglad -lFDGL -lFD3D -lFDSerialize -lFDJson -lFD3DJson -lFDCore -lFDEngine
LIBS += -lglfw -lGL -lpthread -ldl -lX11 -lXrandr -lstdc++ -lpython3.7m
LIBS += -L../../thirdparty/assimp/bin -lassimp

INCLUDEPATH += \
    ../include \
    ../../FDCore/include \
    ../../FDSerialize/include \
    ../../FDJson/include \
    ../../FD3D/include \
    ../../FDGL/include \
    ../../thirdparty/rapidjson/include \
    ../../thirdparty/glad/include \
    ../../thirdparty/pybind11/include \
    ../../thirdparty/glm \
    ../../thirdparty/assimp/include \
    ../../thirdparty/ \
    /usr/include/python3.7m \
    /usr/include/x86_64-linux-gnu/python3.7m \

SOURCES += \
    main.cpp \
    Engine.cpp \
    GLFWImpl.cpp \
    GLUtils.cpp \
    Renderer.cpp \

HEADERS += \
    Engine.h \
    GLFWImpl.h \
    GLUtils.h \
    Renderer.h \

OTHER_FILES += \

DISTFILES += \
