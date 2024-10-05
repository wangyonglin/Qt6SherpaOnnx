#include "QSoundRecorder.h"

#include <QAudioFormat>
#include <QAudioDevice>
#include <QMediaDevices>
#include <QAudioSource>
#include <QDebug>

Qt6SherpaOnnx::QSoundRecorder::QSoundRecorder(QObject *parent)
    :QObject(parent)
{
    defaultDevice= QMediaDevices::defaultAudioInput();
    defaultFormat.setSampleRate(16000);
    defaultFormat.setChannelCount(1);
    defaultFormat.setSampleFormat(QAudioFormat::Int16);
    defaultFormat.setChannelConfig(QAudioFormat::ChannelConfigStereo);
    qMediaDevices=new QMediaDevices(this);
    listDevices= qMediaDevices->audioInputs();
    for (auto &currentDevice : listDevices){
        qInfo() << "可用麦克风:" <<     currentDevice.description();
    }

}

QList<QAudioDevice> Qt6SherpaOnnx::QSoundRecorder::getAudioDevices(){
    return listDevices;
}

void Qt6SherpaOnnx::QSoundRecorder::init(const QAudioFormat &format, const QString &description)
{
    if(format.isValid()){
        defaultFormat=format;
    }
    for (auto &currentDevice : listDevices){
        if(currentDevice.description() == description){
            defaultDevice=currentDevice;
            break;
        }
    }

}




void Qt6SherpaOnnx::QSoundRecorder::create()
{

    if (!defaultDevice.isFormatSupported(defaultFormat)) {
        qWarning() << "Default format not supported, trying to use the nearest.";
    }
    qAudioSource = new QAudioSource(defaultDevice,defaultFormat, this);
    if(qAudioSource){
        connect(qAudioSource, &QAudioSource::stateChanged, this, &QSoundRecorder::handleStateChanged);
        qInfo() << tr("Microphone - %1").arg(defaultDevice.description());

        qIODevice=qAudioSource->start();
        connect(qIODevice,&QIODevice::readyRead,this,&Qt6SherpaOnnx::QSoundRecorder::readyRead);
    }
}

void Qt6SherpaOnnx::QSoundRecorder::readyRead()
{
    if(qIODevice){
        emit refresh(qIODevice->readAll());
    }
}



void Qt6SherpaOnnx::QSoundRecorder::handleStateChanged(QAudio::State newState)
{
    switch (newState) {
    case QAudio::StoppedState:
        if (qAudioSource->error() != QAudio::NoError) {
            qDebug()<<"录音出现错误.\n";
        } else {
            qInfo()<<"完成录音\n";
        }
        break;

    case QAudio::ActiveState:
        qInfo()<<"开始从IO设备读取PCM声音数据.\n";
        break;

    default:
        // ... other cases as appropriate
        break;
    }
}

void Qt6SherpaOnnx::QSoundRecorder::release(){
    if(qAudioSource){
        qAudioSource->stop();
        delete qAudioSource;
    }
}

QAudioFormat Qt6SherpaOnnx::QSoundRecorder::format()
{
    return defaultFormat;
}

