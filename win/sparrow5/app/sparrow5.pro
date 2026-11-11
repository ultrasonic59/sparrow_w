QT += widgets network

TEMPLATE = app
TARGET = sparrow5
CONFIG += c++17

SOURCES += \
    ../src/main.cpp \
    ../src/MainWindow.cpp \
    ../src/SignalModel.cpp \
    ../src/ThkLink.cpp \
    ../src/WaveformPlot.cpp

HEADERS += \
    ../src/MainWindow.h \
    ../src/SignalModel.h \
    ../src/ThkLink.h \
    ../src/WaveformPlot.h \
    ../../protocol/thk_proto.h

INCLUDEPATH += ../src ../../protocol

msvc: QMAKE_CXXFLAGS += /utf-8
