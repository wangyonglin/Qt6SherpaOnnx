#ifndef QSOUNDRECORDER_H
#define QSOUNDRECORDER_H


#include "libQt6SherpaOnnx_global.h"
#include <QMediaDevices>
#include <QAudioSource>

namespace Qt6SherpaOnnx {
class LIBQT6SHERPAONNX_EXPORT QSoundRecorder : public QObject
{
    Q_OBJECT
public:
    explicit QSoundRecorder(QObject *parent = nullptr);
    QList<QAudioDevice> getAudioDevices();
    void init(const QAudioFormat &format,const QString &description=nullptr);
    void create();
    void release();
    QAudioFormat format();

public slots:
    void readyRead();
private:
    QAudioDevice defaultDevice;
    QAudioFormat defaultFormat;
    QMediaDevices *qMediaDevices;
    QAudioSource *qAudioSource;
    QIODevice *qIODevice;
    QList<QAudioDevice> listDevices;
signals:
 void refresh(const QByteArray & bytes);
private slots:
    void handleStateChanged(QAudio::State newState);
};
}


#endif // QSOUNDRECORDER_H
