
#include <QApplication>
#include <Qt6SherpaOnnx/QKeywordSpotting>
#include <Qt6SherpaOnnx/QSoundRecorder>
#include <QDebug>
#include <QTimer>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Qt6SherpaOnnx::QSoundRecorder recorder;
    Qt6SherpaOnnx::QKeywordSpotting spotting;
    QAudioFormat defaultFormat =Qt6SherpaOnnx::QKeywordSpotting::defaultQAudioFormat();
    Qt6SherpaOnnx::QKeywordSpottingConfig  config;
    memset(&config,0x00,sizeof( Qt6SherpaOnnx::QKeywordSpottingConfig));
    config.encoder= "/opt/models/sherpa-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01-mobile/encoder-epoch-12-avg-2-chunk-16-left-64.int8.onnx";
    config.decoder="/opt/models/sherpa-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01-mobile/decoder-epoch-12-avg-2-chunk-16-left-64.onnx";
    config.joiner="/opt/models/sherpa-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01-mobile/joiner-epoch-12-avg-2-chunk-16-left-64.int8.onnx";
    config.tokens="/opt/models/sherpa-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01-mobile/tokens.txt";
    config.keywords="/opt/models/sherpa-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01-mobile/test_wavs/test_keywords.txt";
    spotting.init(defaultFormat,config);
    recorder.init(defaultFormat);
    recorder.create();
    spotting.create();

    a.connect(&spotting,&Qt6SherpaOnnx::QKeywordSpotting::refresh,[&](const QString & word){
        qDebug() << word;
        if(word == "法国"){
            spotting.destroy();
            recorder.destroy();
            a.exit();
        }
    });
    a.connect(&recorder,&Qt6SherpaOnnx::QSoundRecorder::refresh,[&](const QByteArray & bytes){
        spotting.spotter(bytes);
    });




    return a.exec();
}

