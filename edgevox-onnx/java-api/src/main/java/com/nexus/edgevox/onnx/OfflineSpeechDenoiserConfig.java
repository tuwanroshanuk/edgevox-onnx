// Copyright 2025 Xiaomi Corporation
package com.nexus.edgevox.onnx;

public class OfflineSpeechDenoiserConfig {
    private final OfflineSpeechDenoiserModelConfig model;

    private OfflineSpeechDenoiserConfig(OfflineSpeechDenoiserConfig.Builder builder) {
        this.model = builder.model;
    }

    public static Builder builder() {
        return new Builder();
    }

    public static class Builder {
        private OfflineSpeechDenoiserModelConfig model = OfflineSpeechDenoiserModelConfig.builder().build();

        public OfflineSpeechDenoiserConfig build() {
            return new OfflineSpeechDenoiserConfig(this);
        }

        public Builder setModel(OfflineSpeechDenoiserModelConfig model) {
            this.model = model;
            return this;
        }
    }
}
