#include "QKeywordSpotting.h"
#include <QDataStream>
#include <QDebug>

/**
 * @brief QKeywordSpotting::QKeywordSpotting
 * @param parent
 *  w én s ēn t è k ǎ s uǒ  @文森特卡索
    zh ōu w àng j ūn @周望军
    zh ū l ì n án @朱丽楠
    j iǎng y ǒu b ó @蒋友伯
    n ǚ ér @女儿
    f ǎ g uó @法国
    j iàn m iàn h uì @见面会
    l uò sh í @落实
 */
QKeywordSpotting::QKeywordSpotting(QObject *parent)
    : QObject(parent)

{
    feature_dim=80;
    sample_rate=16000;

}

QAudioFormat QKeywordSpotting::defaultQAudioFormat(){
    QAudioFormat qAudioFormat;
    qAudioFormat.setSampleRate(16000);
    qAudioFormat.setChannelCount(1);
    qAudioFormat.setSampleFormat(QAudioFormat::Float);
    qAudioFormat.setChannelConfig(QAudioFormat::ChannelConfigMono);
    return qAudioFormat;
}
void QKeywordSpotting::init(const QAudioFormat &fmt){
    sample_rate=fmt.sampleRate();
}
void QKeywordSpotting::create()
{
    // const char *wav_filename =
    //     "/opt/models/sherpa-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01-mobile/test_wavs/"
    //     "6.wav";
    const char *encoder_filename =
        "/opt/models/sherpa-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01-mobile/"
        "encoder-epoch-12-avg-2-chunk-16-left-64.int8.onnx";
    const char *decoder_filename =
        "/opt/models/sherpa-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01-mobile/"
        "decoder-epoch-12-avg-2-chunk-16-left-64.onnx";
    const char *joiner_filename =
        "/opt/models/sherpa-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01-mobile/"
        "joiner-epoch-12-avg-2-chunk-16-left-64.int8.onnx";
    const char *provider = "cpu";
    const char *tokens_filename =
        "/opt/models/sherpa-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01-mobile/tokens.txt";
    const char *keywords_filename =
        "/opt/models/sherpa-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01-mobile/test_wavs/"
        "test_keywords.txt";

    // Zipformer config
    SherpaOnnxOnlineTransducerModelConfig zipformer_config;
    memset(&zipformer_config, 0x00, sizeof(zipformer_config));
    zipformer_config.encoder = encoder_filename;
    zipformer_config.decoder = decoder_filename;
    zipformer_config.joiner = joiner_filename;

    // Online model config
    SherpaOnnxOnlineModelConfig online_model_config;
    memset(&online_model_config, 0x00, sizeof(online_model_config));
    online_model_config.debug = 1;
    online_model_config.num_threads = 1;
    online_model_config.provider = provider;
    online_model_config.tokens=tokens_filename;
    online_model_config.transducer = zipformer_config;
    // Keywords-spotter config
    SherpaOnnxKeywordSpotterConfig keywords_spotter_config;
    memset(&keywords_spotter_config, 0x00, sizeof(keywords_spotter_config));
    keywords_spotter_config.max_active_paths = 4;
    keywords_spotter_config.keywords_threshold = 0.25;
    keywords_spotter_config.keywords_score = 1.0;
    keywords_spotter_config.model_config = online_model_config;
    keywords_spotter_config.keywords_file=keywords_filename;
    keywords_spotter_config.feat_config.sample_rate=sample_rate;
    keywords_spotter_config.feat_config.feature_dim=feature_dim;
    keywords_spotter= SherpaOnnxCreateKeywordSpotter(&keywords_spotter_config);
   // stream=SherpaOnnxCreateKeywordStream(keywords_spotter);

    stream=SherpaOnnxCreateKeywordStreamWithKeywords(keywords_spotter,"y ǎn y uán @演员");
}
void QKeywordSpotting::spotter(const QByteArray & bytes){
    std::vector<float> streamfloat= convertFloatVector(bytes);
    //qDebug() << "streamfloat.size:" <<streamfloat.size();
    SherpaOnnxOnlineStreamAcceptWaveform(stream,sample_rate, streamfloat.data(), streamfloat.size());
    while (SherpaOnnxIsKeywordStreamReady(keywords_spotter, stream)) {
        SherpaOnnxDecodeKeywordStream(keywords_spotter, stream);
    }
    const SherpaOnnxKeywordResult *r = SherpaOnnxGetKeywordResult(keywords_spotter, stream);
    if (strlen(r->keyword)) {
        qDebug() << "spotter:" << r->json;
        emit refresh(QString::fromStdString(r->keyword));
    }

    SherpaOnnxDestroyKeywordResult(r);
}

void QKeywordSpotting::spotter(const std::vector<float> & streamfloat){
    SherpaOnnxOnlineStreamAcceptWaveform(stream,sample_rate, streamfloat.data(), streamfloat.size());
    //SherpaOnnxOnlineStreamInputFinished(stream);

    while (SherpaOnnxIsKeywordStreamReady(keywords_spotter, stream)) {
        SherpaOnnxDecodeKeywordStream(keywords_spotter, stream);
    }
    const SherpaOnnxKeywordResult *r = SherpaOnnxGetKeywordResult(keywords_spotter, stream);
    if (strlen(r->keyword)) {
        qDebug() << "spotter:" << r->json;
        emit refresh(QString::fromStdString(r->keyword));
    }

    SherpaOnnxDestroyKeywordResult(r);
}

void QKeywordSpotting::destroy()
{
    SherpaOnnxDestroyOnlineStream(stream);
    SherpaOnnxDestroyKeywordSpotter(keywords_spotter);
}

std::vector<float> QKeywordSpotting::convertFloatVector(const QByteArray &byteArray) {
    std::vector<float> floatVector;
    if (byteArray.size() % sizeof(float) == 0) {
        const float *data = reinterpret_cast<const float*>(byteArray.constData());
        size_t dataSize = byteArray.size() / sizeof(float);
        floatVector.assign(data, data + dataSize);
    }
    return floatVector;
}

