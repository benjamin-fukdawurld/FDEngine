TARGET = FDEngine_test

TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += link_pkgconfig
PKGCONFIG += python3 glfw3 gl xrandr x11

DESTDIR = ../../build/bin
MAKEFILE = ../../build/makefiles/$${TARGET}
OBJECTS_DIR = ../../build/obj/$${TARGET}

LIBS += -L../../build/lib
LIBS += -L../../thirdparty/assimp/bin

LIBS += -lFDCore -lFDGL -lFD3D
LIBS += -lFDSerialize -lFDJson -lFD3DJson
LIBS += -lFDScript -lFDPython -lFDCorePython -lFD3DPython
LIBS += -lFDEngine
LIBS += -lglad -lpthread -ldl -lstdc++ -lassimp

copyscripts.commands = $(COPY) $$PWD/Scripts/* $$DESTDIR
first.depends = $(first) copyscripts

export(first.depends)
export(copyscripts)

QMAKE_EXTRA_TARGETS += first copyscripts

INCLUDEPATH += \
    ../include \
    ../../FDCore/include \
    ../../FDSerialize/include \
    ../../FDJson/include \
    ../../FD3D/include \
    ../../FDGL/include \
    ../../FDScript/include \
    ../../FDPython/include \
    ../../thirdparty/rapidjson/include \
    ../../thirdparty/glad/include \
    ../../thirdparty/pybind11/include \
    ../../thirdparty/glm \
    ../../thirdparty/assimp/include \
    ../../thirdparty/ \

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
    Scripts/PythonBehavior.py
