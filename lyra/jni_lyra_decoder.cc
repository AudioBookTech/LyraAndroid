#include <jni.h>
#include <vector>
#include <memory>
#include <cstdint>
#include "lyra/lyra_decoder.h"
#include "lyra/_models.h" 

extern "C" {


JNIEXPORT jlong JNICALL
Java_com_KonstantinShramko_Audiobook_LyraDecoder_init(JNIEnv* env, jobject thiz, jint sample_rate_hz, jint num_channels, jint bitrate) {
    

    const chromemedia::codec::LyraModels models = GetEmbeddedLyraModels();
    

    auto decoder = chromemedia::codec::LyraDecoder::Create(sample_rate_hz, num_channels, models);
    if (!decoder) return 0;
    
    return reinterpret_cast<jlong>(decoder.release());
}


JNIEXPORT jshortArray JNICALL
Java_com_KonstantinShramko_Audiobook_LyraDecoder_decode(JNIEnv* env, jobject thiz, jlong decoder_ptr, jbyteArray encoded_data) {
    
    auto* decoder = reinterpret_cast<chromemedia::codec::LyraDecoder*>(decoder_ptr);
    if (!decoder) return nullptr;
    
    jsize encoded_len = env->GetArrayLength(encoded_data);
    std::vector<uint8_t> encoded_vector(encoded_len);
    env->GetByteArrayRegion(encoded_data, 0, encoded_len, reinterpret_cast<jbyte*>(encoded_vector.data()));
    

    bool valid = decoder->SetEncodedPacket(encoded_vector);
    if (!valid) return nullptr;
    

    int num_samples_per_packet = decoder->sample_rate_hz() / 50; 
    

    auto decoded_samples = decoder->DecodeSamples(num_samples_per_packet);
    if (!decoded_samples.has_value()) return nullptr;
    
    jshortArray result = env->NewShortArray(decoded_samples->size());
    if (result == nullptr) return nullptr;
    
    env->SetShortArrayRegion(result, 0, decoded_samples->size(), reinterpret_cast<const jshort*>(decoded_samples->data()));
    return result;
}


JNIEXPORT void JNICALL
Java_com_KonstantinShramko_Audiobook_LyraDecoder_release(JNIEnv* env, jobject thiz, jlong decoder_ptr) {
    auto* decoder = reinterpret_cast<chromemedia::codec::LyraDecoder*>(decoder_ptr);
    if (decoder) {
        delete decoder;
    }
}

} // extern "C"

