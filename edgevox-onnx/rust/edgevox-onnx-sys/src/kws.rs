#![allow(non_camel_case_types)]
#![allow(non_snake_case)]
#![allow(non_upper_case_globals)]

use std::os::raw::{c_char, c_float};

#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct KeywordResult {
    pub keyword: *const c_char,
    pub tokens: *const c_char,
    pub tokens_arr: *const *const c_char,
    pub count: i32,
    pub timestamps: *mut c_float,
    pub start_time: c_float,
    pub json: *const c_char,
}

#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct KeywordSpotterConfig {
    pub feat_config: super::online_asr::FeatureConfig,
    pub model_config: super::online_asr::OnlineModelConfig,
    pub max_active_paths: i32,
    pub num_trailing_blanks: i32,
    pub keywords_score: c_float,
    pub keywords_threshold: c_float,
    pub keywords_file: *const c_char,
    pub keywords_buf: *const c_char,
    pub keywords_buf_size: i32,
}

#[repr(C)]
pub struct KeywordSpotter {
    _private: [u8; 0],
}

extern "C" {
    pub fn EdgevoxOnnxCreateKeywordSpotter(
        config: *const KeywordSpotterConfig,
    ) -> *const KeywordSpotter;

    pub fn EdgevoxOnnxDestroyKeywordSpotter(spotter: *const KeywordSpotter);

    pub fn EdgevoxOnnxCreateKeywordStream(
        spotter: *const KeywordSpotter,
    ) -> *const super::online_asr::OnlineStream;

    pub fn EdgevoxOnnxCreateKeywordStreamWithKeywords(
        spotter: *const KeywordSpotter,
        keywords: *const c_char,
    ) -> *const super::online_asr::OnlineStream;

    pub fn EdgevoxOnnxIsKeywordStreamReady(
        spotter: *const KeywordSpotter,
        stream: *const super::online_asr::OnlineStream,
    ) -> i32;

    pub fn EdgevoxOnnxDecodeKeywordStream(
        spotter: *const KeywordSpotter,
        stream: *const super::online_asr::OnlineStream,
    );

    pub fn EdgevoxOnnxResetKeywordStream(
        spotter: *const KeywordSpotter,
        stream: *const super::online_asr::OnlineStream,
    );

    pub fn EdgevoxOnnxDecodeMultipleKeywordStreams(
        spotter: *const KeywordSpotter,
        streams: *const *const super::online_asr::OnlineStream,
        n: i32,
    );

    pub fn EdgevoxOnnxGetKeywordResult(
        spotter: *const KeywordSpotter,
        stream: *const super::online_asr::OnlineStream,
    ) -> *const KeywordResult;

    pub fn EdgevoxOnnxDestroyKeywordResult(r: *const KeywordResult);

    pub fn EdgevoxOnnxGetKeywordResultAsJson(
        spotter: *const KeywordSpotter,
        stream: *const super::online_asr::OnlineStream,
    ) -> *const c_char;

    pub fn EdgevoxOnnxFreeKeywordResultJson(s: *const c_char);
}
