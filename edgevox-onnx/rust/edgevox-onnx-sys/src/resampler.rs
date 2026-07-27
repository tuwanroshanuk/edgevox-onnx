use std::os::raw::c_float;

#[repr(C)]
pub struct EdgevoxOnnxLinearResampler {
    _private: [u8; 0],
}

#[repr(C)]
pub struct EdgevoxOnnxResampleOut {
    pub samples: *const f32,
    pub n: i32,
}

extern "C" {
    pub fn EdgevoxOnnxCreateLinearResampler(
        samp_rate_in_hz: i32,
        samp_rate_out_hz: i32,
        filter_cutoff_hz: c_float,
        num_zeros: i32,
    ) -> *const EdgevoxOnnxLinearResampler;

    pub fn EdgevoxOnnxDestroyLinearResampler(p: *const EdgevoxOnnxLinearResampler);

    pub fn EdgevoxOnnxLinearResamplerReset(p: *const EdgevoxOnnxLinearResampler);

    pub fn EdgevoxOnnxLinearResamplerResample(
        p: *const EdgevoxOnnxLinearResampler,
        input: *const f32,
        input_dim: i32,
        flush: i32,
    ) -> *const EdgevoxOnnxResampleOut;

    pub fn EdgevoxOnnxLinearResamplerResampleFree(p: *const EdgevoxOnnxResampleOut);

    pub fn EdgevoxOnnxLinearResamplerResampleGetInputSampleRate(
        p: *const EdgevoxOnnxLinearResampler,
    ) -> i32;

    pub fn EdgevoxOnnxLinearResamplerResampleGetOutputSampleRate(
        p: *const EdgevoxOnnxLinearResampler,
    ) -> i32;
}
