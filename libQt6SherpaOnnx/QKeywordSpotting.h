#ifndef QKEYWORDSPOTTING_H
#define QKEYWORDSPOTTING_H
#include "libQt6SherpaOnnx_global.h"
#include <QAudioFormat>
#include <QObject>
#include "sherpa-onnx/c-api/c-api.h"
namespace Qt6SherpaOnnx {

typedef struct{
    const char * encoder=nullptr;
    const char * decoder=nullptr;
    const char * joiner=nullptr;
    const char *provider = "cpu";
    const char * tokens=nullptr;
    const char * keywords=nullptr;
}QKeywordSpottingConfig;

class LIBQT6SHERPAONNX_EXPORT QKeywordSpotting : public QObject
{
    Q_OBJECT
public:
    explicit QKeywordSpotting(QObject *parent = nullptr);
    void create();
    void destroy();
    void spotter(const QByteArray & bytes);
    void init(const QAudioFormat &fmt,const QKeywordSpottingConfig & config);
    void spotter(const std::vector<float> &streamfloat);
    static QAudioFormat defaultQAudioFormat();
private:
    int32_t sample_rate;
    int32_t feature_dim=80;
    QKeywordSpottingConfig config;
    SherpaOnnxKeywordSpotter *keywords_spotter;
    SherpaOnnxOnlineStream *stream;
    std::vector<float> convertFloatVector(const QByteArray &byteArray);
signals:
    void refresh(const QString & keywrod);
};
}


#endif // QKEYWORDSPOTTING_H
