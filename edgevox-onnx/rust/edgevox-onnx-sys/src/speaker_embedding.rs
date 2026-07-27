#![allow(non_camel_case_types)]
#![allow(non_snake_case)]
#![allow(non_upper_case_globals)]

use std::os::raw::{c_char, c_float};

#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct SpeakerEmbeddingExtractorConfig {
    pub model: *const c_char,
    pub num_threads: i32,
    pub debug: i32,
    pub provider: *const c_char,
}

#[repr(C)]
pub struct SpeakerEmbeddingExtractor {
    _private: [u8; 0],
}

#[repr(C)]
pub struct SpeakerEmbeddingManager {
    _private: [u8; 0],
}

#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct SpeakerEmbeddingManagerSpeakerMatch {
    pub score: c_float,
    pub name: *const c_char,
}

#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct SpeakerEmbeddingManagerBestMatchesResult {
    pub matches: *const SpeakerEmbeddingManagerSpeakerMatch,
    pub count: i32,
}

extern "C" {
    pub fn EdgevoxOnnxCreateSpeakerEmbeddingExtractor(
        config: *const SpeakerEmbeddingExtractorConfig,
    ) -> *const SpeakerEmbeddingExtractor;

    pub fn EdgevoxOnnxDestroySpeakerEmbeddingExtractor(p: *const SpeakerEmbeddingExtractor);

    pub fn EdgevoxOnnxSpeakerEmbeddingExtractorDim(p: *const SpeakerEmbeddingExtractor) -> i32;

    pub fn EdgevoxOnnxSpeakerEmbeddingExtractorCreateStream(
        p: *const SpeakerEmbeddingExtractor,
    ) -> *const crate::online_asr::OnlineStream;

    pub fn EdgevoxOnnxSpeakerEmbeddingExtractorIsReady(
        p: *const SpeakerEmbeddingExtractor,
        s: *const crate::online_asr::OnlineStream,
    ) -> i32;

    pub fn EdgevoxOnnxSpeakerEmbeddingExtractorComputeEmbedding(
        p: *const SpeakerEmbeddingExtractor,
        s: *const crate::online_asr::OnlineStream,
    ) -> *const c_float;

    pub fn EdgevoxOnnxSpeakerEmbeddingExtractorDestroyEmbedding(v: *const c_float);

    pub fn EdgevoxOnnxCreateSpeakerEmbeddingManager(dim: i32) -> *const SpeakerEmbeddingManager;

    pub fn EdgevoxOnnxDestroySpeakerEmbeddingManager(p: *const SpeakerEmbeddingManager);

    pub fn EdgevoxOnnxSpeakerEmbeddingManagerAdd(
        p: *const SpeakerEmbeddingManager,
        name: *const c_char,
        v: *const c_float,
    ) -> i32;

    pub fn EdgevoxOnnxSpeakerEmbeddingManagerAddList(
        p: *const SpeakerEmbeddingManager,
        name: *const c_char,
        v: *const *const c_float,
    ) -> i32;

    pub fn EdgevoxOnnxSpeakerEmbeddingManagerAddListFlattened(
        p: *const SpeakerEmbeddingManager,
        name: *const c_char,
        v: *const c_float,
        n: i32,
    ) -> i32;

    pub fn EdgevoxOnnxSpeakerEmbeddingManagerRemove(
        p: *const SpeakerEmbeddingManager,
        name: *const c_char,
    ) -> i32;

    pub fn EdgevoxOnnxSpeakerEmbeddingManagerSearch(
        p: *const SpeakerEmbeddingManager,
        v: *const c_float,
        threshold: c_float,
    ) -> *const c_char;

    pub fn EdgevoxOnnxSpeakerEmbeddingManagerFreeSearch(name: *const c_char);

    pub fn EdgevoxOnnxSpeakerEmbeddingManagerGetBestMatches(
        p: *const SpeakerEmbeddingManager,
        v: *const c_float,
        threshold: c_float,
        n: i32,
    ) -> *const SpeakerEmbeddingManagerBestMatchesResult;

    pub fn EdgevoxOnnxSpeakerEmbeddingManagerFreeBestMatches(
        r: *const SpeakerEmbeddingManagerBestMatchesResult,
    );

    pub fn EdgevoxOnnxSpeakerEmbeddingManagerVerify(
        p: *const SpeakerEmbeddingManager,
        name: *const c_char,
        v: *const c_float,
        threshold: c_float,
    ) -> i32;

    pub fn EdgevoxOnnxSpeakerEmbeddingManagerContains(
        p: *const SpeakerEmbeddingManager,
        name: *const c_char,
    ) -> i32;

    pub fn EdgevoxOnnxSpeakerEmbeddingManagerNumSpeakers(
        p: *const SpeakerEmbeddingManager,
    ) -> i32;

    pub fn EdgevoxOnnxSpeakerEmbeddingManagerGetAllSpeakers(
        p: *const SpeakerEmbeddingManager,
    ) -> *const *const c_char;

    pub fn EdgevoxOnnxSpeakerEmbeddingManagerFreeAllSpeakers(names: *const *const c_char);
}
