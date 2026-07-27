use std::os::raw::c_char;

#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct OfflineSpeechDenoiserGtcrnModelConfig {
    pub model: *const c_char,
}

#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct OfflineSpeechDenoiserDpdfNetModelConfig {
    pub model: *const c_char,
}

#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct OfflineSpeechDenoiserModelConfig {
    pub gtcrn: OfflineSpeechDenoiserGtcrnModelConfig,
    pub num_threads: i32,
    pub debug: i32,
    pub provider: *const c_char,
    pub dpdfnet: OfflineSpeechDenoiserDpdfNetModelConfig,
}

#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct OfflineSpeechDenoiserConfig {
    pub model: OfflineSpeechDenoiserModelConfig,
}

#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct OnlineSpeechDenoiserConfig {
    pub model: OfflineSpeechDenoiserModelConfig,
}

#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct DenoisedAudio {
    pub samples: *const f32,
    pub n: i32,
    pub sample_rate: i32,
}

#[repr(C)]
pub struct OfflineSpeechDenoiser {
    _private: [u8; 0],
}

#[repr(C)]
pub struct OnlineSpeechDenoiser {
    _private: [u8; 0],
}

extern "C" {
    pub fn EdgevoxOnnxCreateOfflineSpeechDenoiser(
        config: *const OfflineSpeechDenoiserConfig,
    ) -> *const OfflineSpeechDenoiser;
    pub fn EdgevoxOnnxDestroyOfflineSpeechDenoiser(p: *const OfflineSpeechDenoiser);
    pub fn EdgevoxOnnxOfflineSpeechDenoiserGetSampleRate(p: *const OfflineSpeechDenoiser) -> i32;
    pub fn EdgevoxOnnxOfflineSpeechDenoiserRun(
        p: *const OfflineSpeechDenoiser,
        samples: *const f32,
        n: i32,
        sample_rate: i32,
    ) -> *const DenoisedAudio;

    pub fn EdgevoxOnnxCreateOnlineSpeechDenoiser(
        config: *const OnlineSpeechDenoiserConfig,
    ) -> *const OnlineSpeechDenoiser;
    pub fn EdgevoxOnnxDestroyOnlineSpeechDenoiser(p: *const OnlineSpeechDenoiser);
    pub fn EdgevoxOnnxOnlineSpeechDenoiserGetSampleRate(p: *const OnlineSpeechDenoiser) -> i32;
    pub fn EdgevoxOnnxOnlineSpeechDenoiserGetFrameShiftInSamples(
        p: *const OnlineSpeechDenoiser,
    ) -> i32;
    pub fn EdgevoxOnnxOnlineSpeechDenoiserRun(
        p: *const OnlineSpeechDenoiser,
        samples: *const f32,
        n: i32,
        sample_rate: i32,
    ) -> *const DenoisedAudio;
    pub fn EdgevoxOnnxOnlineSpeechDenoiserFlush(
        p: *const OnlineSpeechDenoiser,
    ) -> *const DenoisedAudio;
    pub fn EdgevoxOnnxOnlineSpeechDenoiserReset(p: *const OnlineSpeechDenoiser);

    pub fn EdgevoxOnnxDestroyDenoisedAudio(audio: *const DenoisedAudio);
}
