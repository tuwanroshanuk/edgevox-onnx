// scripts/node-addon-api/src/non-streaming-speaker-diarization.cc
//
// Copyright (c)  2024  Xiaomi Corporation

#include <algorithm>
#include <memory>
#include <sstream>
#include <utility>
#include <vector>

#include "macros.h"  // NOLINT
#include "napi.h"    // NOLINT
#include "edgevox-onnx/c-api/c-api.h"

static EdgevoxOnnxOfflineSpeakerSegmentationPyannoteModelConfig
GetOfflineSpeakerSegmentationPyannoteModelConfig(Napi::Object obj) {
  EdgevoxOnnxOfflineSpeakerSegmentationPyannoteModelConfig c;
  memset(&c, 0, sizeof(c));

  if (!obj.Has("pyannote") || !obj.Get("pyannote").IsObject()) {
    return c;
  }

  Napi::Object o = obj.Get("pyannote").As<Napi::Object>();
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(model, model);

  return c;
}

static EdgevoxOnnxOfflineSpeakerSegmentationModelConfig
GetOfflineSpeakerSegmentationModelConfig(Napi::Object obj) {
  EdgevoxOnnxOfflineSpeakerSegmentationModelConfig c;
  memset(&c, 0, sizeof(c));

  if (!obj.Has("segmentation") || !obj.Get("segmentation").IsObject()) {
    return c;
  }

  Napi::Object o = obj.Get("segmentation").As<Napi::Object>();

  c.pyannote = GetOfflineSpeakerSegmentationPyannoteModelConfig(o);

  EDGEVOX_ONNX_ASSIGN_ATTR_INT32(num_threads, numThreads);

  if (o.Has("debug") &&
      (o.Get("debug").IsNumber() || o.Get("debug").IsBoolean())) {
    if (o.Get("debug").IsBoolean()) {
      c.debug = o.Get("debug").As<Napi::Boolean>().Value();
    } else {
      c.debug = o.Get("debug").As<Napi::Number>().Int32Value();
    }
  }

  EDGEVOX_ONNX_ASSIGN_ATTR_STR(provider, provider);

  return c;
}

static EdgevoxOnnxSpeakerEmbeddingExtractorConfig
GetSpeakerEmbeddingExtractorConfig(Napi::Object obj) {
  EdgevoxOnnxSpeakerEmbeddingExtractorConfig c;
  memset(&c, 0, sizeof(c));

  if (!obj.Has("embedding") || !obj.Get("embedding").IsObject()) {
    return c;
  }

  Napi::Object o = obj.Get("embedding").As<Napi::Object>();

  EDGEVOX_ONNX_ASSIGN_ATTR_STR(model, model);
  EDGEVOX_ONNX_ASSIGN_ATTR_INT32(num_threads, numThreads);

  if (o.Has("debug") &&
      (o.Get("debug").IsNumber() || o.Get("debug").IsBoolean())) {
    if (o.Get("debug").IsBoolean()) {
      c.debug = o.Get("debug").As<Napi::Boolean>().Value();
    } else {
      c.debug = o.Get("debug").As<Napi::Number>().Int32Value();
    }
  }

  EDGEVOX_ONNX_ASSIGN_ATTR_STR(provider, provider);

  return c;
}

static EdgevoxOnnxFastClusteringConfig GetFastClusteringConfig(
    Napi::Object obj) {
  EdgevoxOnnxFastClusteringConfig c;
  memset(&c, 0, sizeof(c));

  if (!obj.Has("clustering") || !obj.Get("clustering").IsObject()) {
    return c;
  }

  Napi::Object o = obj.Get("clustering").As<Napi::Object>();

  EDGEVOX_ONNX_ASSIGN_ATTR_INT32(num_clusters, numClusters);
  EDGEVOX_ONNX_ASSIGN_ATTR_FLOAT(threshold, threshold);

  return c;
}

static Napi::External<EdgevoxOnnxOfflineSpeakerDiarization>
CreateOfflineSpeakerDiarizationWrapper(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

#if __OHOS__
  if (info.Length() != 1 && info.Length() != 2) {
    std::ostringstream os;
    os << "Expect 1 or 2 arguments. Given: " << info.Length();

    Napi::TypeError::New(env, os.str()).ThrowAsJavaScriptException();

    return {};
  }
#else
  if (info.Length() != 1) {
    std::ostringstream os;
    os << "Expect only 1 argument. Given: " << info.Length();

    Napi::TypeError::New(env, os.str()).ThrowAsJavaScriptException();

    return {};
  }
#endif

  if (!info[0].IsObject()) {
    Napi::TypeError::New(env, "Expect an object as the argument")
        .ThrowAsJavaScriptException();

    return {};
  }

#if __OHOS__
  bool use_resource_manager =
      info.Length() == 2 && !info[1].IsUndefined() && !info[1].IsNull();
  if (use_resource_manager && !info[1].IsObject()) {
    Napi::TypeError::New(
        env, "You should pass a resource manager as the second argument.")
        .ThrowAsJavaScriptException();

    return {};
  }
#endif

  Napi::Object o = info[0].As<Napi::Object>();

  EdgevoxOnnxOfflineSpeakerDiarizationConfig c;
  memset(&c, 0, sizeof(c));

  c.segmentation = GetOfflineSpeakerSegmentationModelConfig(o);
  c.embedding = GetSpeakerEmbeddingExtractorConfig(o);
  c.clustering = GetFastClusteringConfig(o);

  EDGEVOX_ONNX_ASSIGN_ATTR_FLOAT(min_duration_on, minDurationOn);
  EDGEVOX_ONNX_ASSIGN_ATTR_FLOAT(min_duration_off, minDurationOff);

#if __OHOS__
  const EdgevoxOnnxOfflineSpeakerDiarization *sd = nullptr;

  if (use_resource_manager) {
    std::unique_ptr<NativeResourceManager,
                    decltype(&OH_ResourceManager_ReleaseNativeResourceManager)>
        mgr(OH_ResourceManager_InitNativeResourceManager(env, info[1]),
            &OH_ResourceManager_ReleaseNativeResourceManager);

    sd = EdgevoxOnnxCreateOfflineSpeakerDiarizationOHOS(&c, mgr.get());
  } else {
    sd = EdgevoxOnnxCreateOfflineSpeakerDiarization(&c);
  }
#else
  const EdgevoxOnnxOfflineSpeakerDiarization *sd =
      EdgevoxOnnxCreateOfflineSpeakerDiarization(&c);
#endif

  EDGEVOX_ONNX_DELETE_C_STR(c.segmentation.pyannote.model);
  EDGEVOX_ONNX_DELETE_C_STR(c.segmentation.provider);
  EDGEVOX_ONNX_DELETE_C_STR(c.embedding.model);
  EDGEVOX_ONNX_DELETE_C_STR(c.embedding.provider);

  if (!sd) {
    Napi::TypeError::New(env, "Please check your config!")
        .ThrowAsJavaScriptException();

    return {};
  }

  return Napi::External<EdgevoxOnnxOfflineSpeakerDiarization>::New(
      env, const_cast<EdgevoxOnnxOfflineSpeakerDiarization *>(sd),
      [](Napi::Env env, EdgevoxOnnxOfflineSpeakerDiarization *sd) {
        EdgevoxOnnxDestroyOfflineSpeakerDiarization(sd);
      });
}

static Napi::Number OfflineSpeakerDiarizationGetSampleRateWrapper(
    const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() != 1) {
    std::ostringstream os;
    os << "Expect only 1 argument. Given: " << info.Length();

    Napi::TypeError::New(env, os.str()).ThrowAsJavaScriptException();

    return {};
  }

  if (!info[0].IsExternal()) {
    Napi::TypeError::New(
        env, "Argument 0 should be an offline speaker diarization pointer.")
        .ThrowAsJavaScriptException();

    return {};
  }

  const EdgevoxOnnxOfflineSpeakerDiarization *sd =
      info[0].As<Napi::External<EdgevoxOnnxOfflineSpeakerDiarization>>().Data();

  int32_t sample_rate = EdgevoxOnnxOfflineSpeakerDiarizationGetSampleRate(sd);

  return Napi::Number::New(env, sample_rate);
}

static Napi::Array OfflineSpeakerDiarizationProcessWrapper(
    const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() != 2) {
    std::ostringstream os;
    os << "Expect only 2 arguments. Given: " << info.Length();

    Napi::TypeError::New(env, os.str()).ThrowAsJavaScriptException();

    return {};
  }

  if (!info[0].IsExternal()) {
    Napi::TypeError::New(
        env, "Argument 0 should be an offline speaker diarization pointer.")
        .ThrowAsJavaScriptException();

    return {};
  }

  const EdgevoxOnnxOfflineSpeakerDiarization *sd =
      info[0].As<Napi::External<EdgevoxOnnxOfflineSpeakerDiarization>>().Data();

  if (!info[1].IsTypedArray()) {
    Napi::TypeError::New(env, "Argument 1 should be a typed array")
        .ThrowAsJavaScriptException();

    return {};
  }

  Napi::Float32Array samples = info[1].As<Napi::Float32Array>();

#if __OHOS__
  // Note(fangjun): For unknown reasons on HarmonyOS, we need to divide it by
  // sizeof(float) here
  const EdgevoxOnnxOfflineSpeakerDiarizationResult *r =
      EdgevoxOnnxOfflineSpeakerDiarizationProcess(
          sd, samples.Data(), samples.ElementLength() / sizeof(float));
#else
  const EdgevoxOnnxOfflineSpeakerDiarizationResult *r =
      EdgevoxOnnxOfflineSpeakerDiarizationProcess(sd, samples.Data(),
                                                 samples.ElementLength());
#endif

  int32_t num_segments =
      EdgevoxOnnxOfflineSpeakerDiarizationResultGetNumSegments(r);

  const EdgevoxOnnxOfflineSpeakerDiarizationSegment *segments =
      EdgevoxOnnxOfflineSpeakerDiarizationResultSortByStartTime(r);

  Napi::Array ans = Napi::Array::New(env, num_segments);

  for (int32_t i = 0; i != num_segments; ++i) {
    Napi::Object obj = Napi::Object::New(env);

    obj.Set(Napi::String::New(env, "start"), segments[i].start);
    obj.Set(Napi::String::New(env, "end"), segments[i].end);
    obj.Set(Napi::String::New(env, "speaker"), segments[i].speaker);

    ans.Set(i, obj);
  }

  EdgevoxOnnxOfflineSpeakerDiarizationDestroySegment(segments);
  EdgevoxOnnxOfflineSpeakerDiarizationDestroyResult(r);

  return ans;
}

struct SpeakerDiarizationCallbackData {
  int32_t num_processed_chunks;
  int32_t num_total_chunks;
};

// see
// https://github.com/nodejs/node-addon-examples/blob/main/src/6-threadsafe-function/typed_threadsafe_function/node-addon-api/clock.cc
static void InvokeJsCallback(Napi::Env env, Napi::Function callback,
                             Napi::Reference<Napi::Value> *context,
                             SpeakerDiarizationCallbackData *data) {
  if (env != nullptr) {
    if (callback != nullptr) {
      Napi::Number num_processed_chunks =
          Napi::Number::New(env, data->num_processed_chunks);
      Napi::Number num_total_chunks =
          Napi::Number::New(env, data->num_total_chunks);

      callback.Call(context->Value(), {num_processed_chunks, num_total_chunks});
    }
  }
  delete data;
}

using TSFN = Napi::TypedThreadSafeFunction<Napi::Reference<Napi::Value>,
                                           SpeakerDiarizationCallbackData,
                                           InvokeJsCallback>;

class SpeakerDiarizationProcessWorker : public Napi::AsyncWorker {
 public:
  SpeakerDiarizationProcessWorker(const Napi::Env &env, TSFN tsfn,
                                  const EdgevoxOnnxOfflineSpeakerDiarization *sd,
                                  std::vector<float> samples)
      : tsfn_(tsfn),
        Napi::AsyncWorker{env, "SpeakerDiarizationProcessAsyncWorker"},
        deferred_(env),
        sd_(sd),
        samples_(std::move(samples)) {}

  Napi::Promise Promise() { return deferred_.Promise(); }

 protected:
  void Execute() override {
    auto callback = [](int32_t num_processed_chunks, int32_t num_total_chunks,
                       void *arg) -> int32_t {
      auto _this = reinterpret_cast<SpeakerDiarizationProcessWorker *>(arg);

      auto data = new SpeakerDiarizationCallbackData;
      data->num_processed_chunks = num_processed_chunks;
      data->num_total_chunks = num_total_chunks;

      _this->tsfn_.NonBlockingCall(data);

      return 0;
    };

    r_ = EdgevoxOnnxOfflineSpeakerDiarizationProcessWithCallback(
        sd_, samples_.data(), samples_.size(), callback, this);

    tsfn_.Release();
  }

  void OnOK() override {
    Napi::Env env = deferred_.Env();

    int32_t num_segments =
        EdgevoxOnnxOfflineSpeakerDiarizationResultGetNumSegments(r_);

    const EdgevoxOnnxOfflineSpeakerDiarizationSegment *segments =
        EdgevoxOnnxOfflineSpeakerDiarizationResultSortByStartTime(r_);

    Napi::Array ans = Napi::Array::New(env, num_segments);

    for (int32_t i = 0; i != num_segments; ++i) {
      Napi::Object obj = Napi::Object::New(env);

      obj.Set(Napi::String::New(env, "start"), segments[i].start);
      obj.Set(Napi::String::New(env, "end"), segments[i].end);
      obj.Set(Napi::String::New(env, "speaker"), segments[i].speaker);

      ans.Set(i, obj);
    }

    EdgevoxOnnxOfflineSpeakerDiarizationDestroySegment(segments);
    EdgevoxOnnxOfflineSpeakerDiarizationDestroyResult(r_);

    deferred_.Resolve(ans);
  }

 private:
  TSFN tsfn_;
  Napi::Promise::Deferred deferred_;
  const EdgevoxOnnxOfflineSpeakerDiarization *sd_;
  std::vector<float> samples_;
  const EdgevoxOnnxOfflineSpeakerDiarizationResult *r_;
};

static Napi::Object OfflineSpeakerDiarizationProcessAsyncWrapper(
    const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() != 3) {
    std::ostringstream os;
    os << "Expect only 3 arguments. Given: " << info.Length();

    Napi::TypeError::New(env, os.str()).ThrowAsJavaScriptException();

    return {};
  }

  if (!info[0].IsExternal()) {
    Napi::TypeError::New(
        env, "Argument 0 should be an offline speaker diarization pointer.")
        .ThrowAsJavaScriptException();

    return {};
  }

  const EdgevoxOnnxOfflineSpeakerDiarization *sd =
      info[0].As<Napi::External<EdgevoxOnnxOfflineSpeakerDiarization>>().Data();

  if (!info[1].IsTypedArray()) {
    Napi::TypeError::New(env, "Argument 1 should be a typed array")
        .ThrowAsJavaScriptException();

    return {};
  }

  if (!info[2].IsFunction()) {
    Napi::TypeError::New(env, "Argument 2 should be a function")
        .ThrowAsJavaScriptException();

    return {};
  }

  Napi::Function cb = info[2].As<Napi::Function>();

  auto context =
      new Napi::Reference<Napi::Value>(Napi::Persistent(info.This()));

  TSFN tsfn = TSFN::New(
      env,
      cb,  // JavaScript function called asynchronously
      "SpeakerDiarizationProcessAsyncFunc",  // Name
      0,                                     // Unlimited queue
      1,  // Only one thread will use this initially
      context,
      [](Napi::Env, void *, Napi::Reference<Napi::Value> *ctx) { delete ctx; });

  Napi::Float32Array samples = info[1].As<Napi::Float32Array>();

#if __OHOS__
  int32_t num_samples = samples.ElementLength() / sizeof(float);
#else
  int32_t num_samples = samples.ElementLength();
#endif
  std::vector<float> v(num_samples);
  std::copy(samples.Data(), samples.Data() + num_samples, v.begin());

  SpeakerDiarizationProcessWorker *worker =
      new SpeakerDiarizationProcessWorker(env, tsfn, sd, v);
  worker->Queue();
  return worker->Promise();
}

static void OfflineSpeakerDiarizationSetConfigWrapper(
    const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() != 2) {
    std::ostringstream os;
    os << "Expect only 2 arguments. Given: " << info.Length();

    Napi::TypeError::New(env, os.str()).ThrowAsJavaScriptException();

    return;
  }

  if (!info[0].IsExternal()) {
    Napi::TypeError::New(
        env, "Argument 0 should be an offline speaker diarization pointer.")
        .ThrowAsJavaScriptException();

    return;
  }

  const EdgevoxOnnxOfflineSpeakerDiarization *sd =
      info[0].As<Napi::External<EdgevoxOnnxOfflineSpeakerDiarization>>().Data();

  if (!info[1].IsObject()) {
    Napi::TypeError::New(env, "Expect an object as the argument")
        .ThrowAsJavaScriptException();

    return;
  }

  Napi::Object o = info[1].As<Napi::Object>();

  EdgevoxOnnxOfflineSpeakerDiarizationConfig c;
  memset(&c, 0, sizeof(c));

  c.clustering = GetFastClusteringConfig(o);
  EdgevoxOnnxOfflineSpeakerDiarizationSetConfig(sd, &c);
}

void InitNonStreamingSpeakerDiarization(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "createOfflineSpeakerDiarization"),
              Napi::Function::New(env, CreateOfflineSpeakerDiarizationWrapper));

  exports.Set(
      Napi::String::New(env, "getOfflineSpeakerDiarizationSampleRate"),
      Napi::Function::New(env, OfflineSpeakerDiarizationGetSampleRateWrapper));

  exports.Set(
      Napi::String::New(env, "offlineSpeakerDiarizationProcess"),
      Napi::Function::New(env, OfflineSpeakerDiarizationProcessWrapper));

  exports.Set(
      Napi::String::New(env, "offlineSpeakerDiarizationProcessAsync"),
      Napi::Function::New(env, OfflineSpeakerDiarizationProcessAsyncWrapper));

  exports.Set(
      Napi::String::New(env, "offlineSpeakerDiarizationSetConfig"),
      Napi::Function::New(env, OfflineSpeakerDiarizationSetConfigWrapper));
}
