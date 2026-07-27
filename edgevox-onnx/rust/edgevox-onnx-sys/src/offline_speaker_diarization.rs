#![allow(non_camel_case_types)]
#![allow(non_snake_case)]
#![allow(non_upper_case_globals)]

use std::os::raw::{c_char, c_float};

#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct OfflineSpeakerSegmentationPyannoteModelConfig {
    pub model: *const c_char,
}

#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct OfflineSpeakerSegmentationModelConfig {
    pub pyannote: OfflineSpeakerSegmentationPyannoteModelConfig,
    pub num_threads: i32,
    pub debug: i32,
    pub provider: *const c_char,
}

#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct FastClusteringConfig {
    pub num_clusters: i32,
    pub threshold: c_float,
}

#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct OfflineSpeakerDiarizationConfig {
    pub segmentation: OfflineSpeakerSegmentationModelConfig,
    pub embedding: crate::speaker_embedding::SpeakerEmbeddingExtractorConfig,
    pub clustering: FastClusteringConfig,
    pub min_duration_on: c_float,
    pub min_duration_off: c_float,
}

#[repr(C)]
pub struct OfflineSpeakerDiarization {
    _private: [u8; 0],
}

#[repr(C)]
pub struct OfflineSpeakerDiarizationResult {
    _private: [u8; 0],
}

#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct OfflineSpeakerDiarizationSegment {
    pub start: c_float,
    pub end: c_float,
    pub speaker: i32,
}

extern "C" {
    pub fn EdgevoxOnnxCreateOfflineSpeakerDiarization(
        config: *const OfflineSpeakerDiarizationConfig,
    ) -> *const OfflineSpeakerDiarization;

    pub fn EdgevoxOnnxDestroyOfflineSpeakerDiarization(sd: *const OfflineSpeakerDiarization);

    pub fn EdgevoxOnnxOfflineSpeakerDiarizationGetSampleRate(
        sd: *const OfflineSpeakerDiarization,
    ) -> i32;

    pub fn EdgevoxOnnxOfflineSpeakerDiarizationSetConfig(
        sd: *const OfflineSpeakerDiarization,
        config: *const OfflineSpeakerDiarizationConfig,
    );

    pub fn EdgevoxOnnxOfflineSpeakerDiarizationResultGetNumSpeakers(
        r: *const OfflineSpeakerDiarizationResult,
    ) -> i32;

    pub fn EdgevoxOnnxOfflineSpeakerDiarizationResultGetNumSegments(
        r: *const OfflineSpeakerDiarizationResult,
    ) -> i32;

    pub fn EdgevoxOnnxOfflineSpeakerDiarizationResultSortByStartTime(
        r: *const OfflineSpeakerDiarizationResult,
    ) -> *const OfflineSpeakerDiarizationSegment;

    pub fn EdgevoxOnnxOfflineSpeakerDiarizationDestroySegment(
        s: *const OfflineSpeakerDiarizationSegment,
    );

    pub fn EdgevoxOnnxOfflineSpeakerDiarizationProcess(
        sd: *const OfflineSpeakerDiarization,
        samples: *const c_float,
        n: i32,
    ) -> *const OfflineSpeakerDiarizationResult;

    pub fn EdgevoxOnnxOfflineSpeakerDiarizationDestroyResult(
        r: *const OfflineSpeakerDiarizationResult,
    );
}
