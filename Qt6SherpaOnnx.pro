QT += gui core multimedia

TEMPLATE = lib
DEFINES += QTSHERPAONNX_LIBRARY

CONFIG += c++17

include(version.pri)

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += debug_and_release
CONFIG(debug, debug|release){

    INCLUDEPATH +=$$PWD/include
    unix{
        contains(QT_ARCH, arm64){
        LIBS += -L$$PWD/lib/linux/aarch64 \
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
        }
    }
}


SOURCES += \
    QKeywordSpotting.cpp

HEADERS += \
    QKeywordSpotting.h \
    Qt6SherpaOnnx_global.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    README.md
