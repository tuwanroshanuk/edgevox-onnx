
CUR_DIR :=$(shell pwd)

CFLAGS := -I ../ -I ../build/_deps/cargs-src/include/
LDFLAGS := -L ../build/lib
LDFLAGS += -L ../build/_deps/onnxruntime-src/lib
LDFLAGS += -ledgevox-onnx-c-api -ledgevox-onnx-core -lkaldi-decoder-core -ledgevox-onnx-kaldifst-core -ledgevox-onnx-fstfar -ledgevox-onnx-fst -lkaldi-native-fbank-core -lkissfft-float -lpiper_phonemize -lespeak-ng -lucd -lcargs -lonnxruntime
LDFLAGS += -framework Foundation
LDFLAGS += -lc++
LDFLAGS += -Wl,-rpath,${CUR_DIR}/../build/lib
LDFLAGS += -Wl,-rpath,${CUR_DIR}/../build/_deps/onnxruntime-src/lib

.PHONY: all clean

all: decode-file-c-api offline-tts-c-api

decode-file-c-api: decode-file-c-api.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

offline-tts-c-api: offline-tts-c-api.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	$(RM) ./decode-file-c-api ./offline-tts-c-api
