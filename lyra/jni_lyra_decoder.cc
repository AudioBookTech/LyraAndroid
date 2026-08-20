#include <jni.h>
#include <vector>
#include <memory>
#include <cstdint>
#include <algorithm>
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

/**
 * Optimized version of decoding using Direct ByteBuffer.
 * Enables buffer overflow protection and eliminates unnecessary heap allocations.
 */
JNIEXPORT jint JNICALL
Java_com_KonstantinShramko_Audiobook_LyraDecoder_decodeToBuffer(JNIEnv* env, jobject thiz, jlong decoder_ptr, jbyteArray encoded_data, jobject out_direct_buffer) {

    auto* decoder = reinterpret_cast<chromemedia::codec::LyraDecoder*>(decoder_ptr);
    if (!decoder) return -1;

    // 1. We obtain the address and capacity of the Direct buffer.
    void* buffer_address = env->GetDirectBufferAddress(out_direct_buffer);
    jlong buffer_capacity = env->GetDirectBufferCapacity(out_direct_buffer);
    if (!buffer_address) return -2;

    // 2. Reading input data (Lyra packet)
    jsize encoded_len = env->GetArrayLength(encoded_data);

    // Optimization: use an array on the stack instead of std::vector,
    // because Lyra packets are very small (typically 8, 15 or 23 bytes).
    uint8_t stack_encoded_data[128];
    if (encoded_len > 128) return -3; // На всякий случай, если формат изменится

    env->GetByteArrayRegion(encoded_data, 0, encoded_len, reinterpret_cast<jbyte*>(stack_encoded_data));

    // We pass data to the decoder
    // Note: Depending on the Lyra API version, std::vector or absl::Span may be required.
    // If a std::vector is required, use: std::vector<uint8_t> encoded_vec(stack_encoded_data, stack_encoded_data + encoded_len);
    if (!decoder->SetEncodedPacket({stack_encoded_data, static_cast<size_t>(encoded_len)})) {
        return -4;
    }

    // 3. Decoding
    int num_samples_per_packet = decoder->sample_rate_hz() / 50; // 20ms кадр
    auto decoded_samples = decoder->DecodeSamples(num_samples_per_packet);
    if (!decoded_samples.has_value()) return -5;

    // 4. Security Check: Is there enough space in DirectBuffer?
    // Size in bytes: number of samples * sizeof(int16_t)
    size_t required_size_bytes = decoded_samples->size() * sizeof(int16_t);
    if (static_cast<size_t>(buffer_capacity) < required_size_bytes) {
        return -6; // Error: Buffer too small (Buffer Overflow Protection)
    }

    // 5. Zero-copy copying directly into DirectBuffer memory
    jshort* out_ptr = reinterpret_cast<jshort*>(buffer_address);
    std::copy(decoded_samples->begin(), decoded_samples->end(), out_ptr);

    // Return the number of samples written (not bytes)
    return static_cast<jint>(decoded_samples->size());
}

JNIEXPORT void JNICALL
Java_com_KonstantinShramko_Audiobook_LyraDecoder_release(JNIEnv* env, jobject thiz, jlong decoder_ptr) {
    auto* decoder = reinterpret_cast<chromemedia::codec::LyraDecoder*>(decoder_ptr);
    if (decoder) {
        delete decoder;
    }
}

} // extern "C"
