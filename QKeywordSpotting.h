#ifndef QKEYWORDSPOTTING_H
#define QKEYWORDSPOTTING_H
#include <QAudioFormat>
#include <QObject>

#include "sherpa-onnx/c-api/c-api.h"

class QKeywordSpotting : public QObject
{
    Q_OBJECT
public:
    explicit QKeywordSpotting(QObject *parent = nullptr);

    void create();
    void destroy();
    void spotter(const QByteArray & bytes);
    void init(const QAudioFormat &fmt);
    void spotter(const std::vector<float> &streamfloat);
    static QAudioFormat defaultQAudioFormat();
private:
    int32_t sample_rate;
    int32_t feature_dim=80;
    SherpaOnnxKeywordSpotter *keywords_spotter;
    SherpaOnnxOnlineStream *stream;
    std::vector<float> convertFloatVector(const QByteArray &byteArray);
signals:
    void refresh(const QString & keywrod);
};

#endif // QKEYWORDSPOTTING_H
