QT += widgets testlib network

TEMPLATE = app
TARGET = signal_tests
CONFIG += console c++17

QTPLUGIN += qoffscreen

SOURCES += \
    test_signal.cpp \
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
