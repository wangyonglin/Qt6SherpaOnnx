QT += gui core multimedia

TEMPLATE = lib
DEFINES += LIBQT6SHERPAONNX_LIBRARY

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += $$PWD/../libQt6SherpaOnnx/include
CONFIG += debug_and_release
linux-g++* {
    message(Compling with linux-g++)
    CONFIG(debug, debug|release){
        message(Debug build)
        TARGET = Qt6SherpaOnnx
        LIBS += -L$$PWD/../libQt6SherpaOnnx/lib/linux/aarch64 \
        -lsherpa-onnx-c-api \
        -lsherpa-onnx-core \
        -lkaldi-native-fbank-core \
        -lkaldi-decoder-core \
        -lsherpa-onnx-fst \
        -lsherpa-onnx-fstfar \
        -lsherpa-onnx-kaldifst-core \
        -lssentencepiece_core \
        -lonnxruntime \
        -lespeak-ng \
        -lucd \
        -lpiper_phonemize \
        -lcargs
        release

    }

    CONFIG(release, debug|release){
        message(Release build)
        TARGET = Qt6SherpaOnnx

    }

    #DEFINES += CSM_TARGET_LINUX_GL
}




SOURCES += \
    QKeywordSpotting.cpp \
    QSoundRecorder.cpp

HEADERS += \
    QKeywordSpotting.h \
    QSoundRecorder.h \
    libQt6SherpaOnnx_global.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    README.md
