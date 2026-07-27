use std::os::raw::{c_char, c_float};

#[repr(C)]
pub struct SileroVadModelConfig {
    pub model: *const c_char,
    pub threshold: c_float,
    pub min_silence_duration: c_float,
    pub min_speech_duration: c_float,
    pub window_size: i32,
    pub max_speech_duration: c_float,
}

#[repr(C)]
pub struct TenVadModelConfig {
    pub model: *const c_char,
    pub threshold: c_float,
    pub min_silence_duration: c_float,
    pub min_speech_duration: c_float,
    pub window_size: i32,
    pub max_speech_duration: c_float,
}

#[repr(C)]
pub struct VadModelConfig {
    pub silero_vad: SileroVadModelConfig,
    pub sample_rate: i32,
    pub num_threads: i32,
    pub provider: *const c_char,
    pub debug: i32,
    pub ten_vad: TenVadModelConfig,
}

#[repr(C)]
pub struct CircularBuffer {
    _private: [u8; 0],
}

#[repr(C)]
pub struct SpeechSegment {
    pub start: i32,
    pub samples: *mut f32,
    pub n: i32,
}

#[repr(C)]
pub struct VoiceActivityDetector {
    _private: [u8; 0],
}

extern "C" {
    pub fn EdgevoxOnnxCreateCircularBuffer(capacity: i32) -> *const CircularBuffer;
    pub fn EdgevoxOnnxDestroyCircularBuffer(buffer: *const CircularBuffer);
    pub fn EdgevoxOnnxCircularBufferPush(buffer: *const CircularBuffer, p: *const f32, n: i32);
    pub fn EdgevoxOnnxCircularBufferGet(
        buffer: *const CircularBuffer,
        start_index: i32,
        n: i32,
    ) -> *const f32;
    pub fn EdgevoxOnnxCircularBufferFree(p: *const f32);
    pub fn EdgevoxOnnxCircularBufferPop(buffer: *const CircularBuffer, n: i32);
    pub fn EdgevoxOnnxCircularBufferSize(buffer: *const CircularBuffer) -> i32;
    pub fn EdgevoxOnnxCircularBufferHead(buffer: *const CircularBuffer) -> i32;
    pub fn EdgevoxOnnxCircularBufferReset(buffer: *const CircularBuffer);

    pub fn EdgevoxOnnxCreateVoiceActivityDetector(
        config: *const VadModelConfig,
        buffer_size_in_seconds: c_float,
    ) -> *const VoiceActivityDetector;
    pub fn EdgevoxOnnxDestroyVoiceActivityDetector(p: *const VoiceActivityDetector);
    pub fn EdgevoxOnnxVoiceActivityDetectorAcceptWaveform(
        p: *const VoiceActivityDetector,
        samples: *const f32,
        n: i32,
    );
    pub fn EdgevoxOnnxVoiceActivityDetectorEmpty(p: *const VoiceActivityDetector) -> i32;
    pub fn EdgevoxOnnxVoiceActivityDetectorDetected(p: *const VoiceActivityDetector) -> i32;
    pub fn EdgevoxOnnxVoiceActivityDetectorPop(p: *const VoiceActivityDetector);
    pub fn EdgevoxOnnxVoiceActivityDetectorClear(p: *const VoiceActivityDetector);
    pub fn EdgevoxOnnxVoiceActivityDetectorFront(
        p: *const VoiceActivityDetector,
    ) -> *const SpeechSegment;
    pub fn EdgevoxOnnxDestroySpeechSegment(p: *const SpeechSegment);
    pub fn EdgevoxOnnxVoiceActivityDetectorReset(p: *const VoiceActivityDetector);
    pub fn EdgevoxOnnxVoiceActivityDetectorFlush(p: *const VoiceActivityDetector);
}
