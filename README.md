# QtSherpaOnnx
Based on Qt 6.7.2 (GCC 10.3.1 20210422 (Red Hat 10.3.1-1), x86_64)
### https://github.com/k2-fsa/sherpa-onnx/releases/tag/kws-models
```c++
qKeywordSpotting= new Qt6SherpaOnnx::QKeywordSpotting(this);
Qt6SherpaOnnx::QKeywordSpottingConfig config;
memset(&config,0x00,sizeof(config));
config.encoder= "/opt/models/sherpa-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01-mobile/encoder-epoch-12-avg-2-chunk-16-left-64.int8.onnx";
config.decoder="/opt/models/sherpa-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01-mobile/decoder-epoch-12-avg-2-chunk-16-left-64.onnx";
config.joiner="/opt/models/sherpa-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01-mobile/joiner-epoch-12-avg-2-chunk-16-left-64.int8.onnx";
config.tokens="/opt/models/sherpa-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01-mobile/tokens.txt";
config.keywords="/opt/models/sherpa-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01-mobile/test_wavs/test_keywords.txt";
qKeywordSpotting->create(config);

void ChatWidget::refreshSoundRecorder(QByteArray audio_bytes)
{
    if(qKeywordSpotting){
        qKeywordSpotting->spotter(audio_bytes);
    }
}

qKeywordSpotting->destroy();
```
