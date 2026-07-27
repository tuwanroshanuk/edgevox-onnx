# File descriptions

- [./edgevox-onnx-alsa.cc](./edgevox-onnx-alsa.cc) For Linux only, especially for
  embedded Linux, e.g., Raspberry Pi; it uses a streaming model for real-time
  speech recognition with a microphone.

- [./edgevox-onnx-microphone.cc](./edgevox-onnx-microphone.cc)
  For Linux/Windows/macOS; it uses a streaming model for real-time speech
  recognition with a microphone.

- [./edgevox-onnx-microphone-offline.cc](./edgevox-onnx-microphone-offline.cc)
  For Linux/Windows/macOS; it uses a non-streaming model for speech
  recognition with a microphone.

- [./edgevox-onnx.cc](./edgevox-onnx.cc)
  It uses a streaming model to decode wave files

- [./edgevox-onnx-offline.cc](./edgevox-onnx-offline.cc)
  It uses a non-streaming model to decode wave files

- [./online-websocket-server.cc](./online-websocket-server.cc)
  WebSocket server for streaming models.

- [./offline-websocket-server.cc](./offline-websocket-server.cc)
  WebSocket server for non-streaming models.

- [./edgevox-onnx-vad-microphone.cc](./edgevox-onnx-vad-microphone.cc)
  Use silero VAD to detect speeches with a microphone.

