 
TEMPLATE = lib
CONFIG += console c++14 staticlib
CONFIG -= qt

QMAKE_CXXFLAGS += -mavx -fopenmp
QMAKE_LFLAGS += -fopenmp

HEADERS += \
    snSIMD.h

SOURCES += \
    src/convolutionBWD.cpp \
    src/convolutionFWD.cpp

INCLUDEPATH += \
    src \
    ..

DEPENDPATH += \
    src \
    ..

unix {
    QMAKE_POST_LINK += mkdir -p $$PWD/../../builds/_nix;
    QMAKE_POST_LINK += cp -rf *.a $$PWD/../../builds/_nix/
}