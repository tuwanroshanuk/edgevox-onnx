// Copyright (c) 2026 Nexus Studio
#ifndef EDGEVOX_ONNX_CSRC_MEMORY_RESOURCE_MANAGER_H_
#define EDGEVOX_ONNX_CSRC_MEMORY_RESOURCE_MANAGER_H_

#include <algorithm>
#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace edgevox_onnx {

// Owns decrypted package resources for the complete TTS instance lifetime.
// Paths are logical package paths and never refer to plaintext disk files.
class MemoryResourceManager {
 public:
  MemoryResourceManager() = default;
  MemoryResourceManager(const MemoryResourceManager &) = delete;
  MemoryResourceManager &operator=(const MemoryResourceManager &) = delete;

  ~MemoryResourceManager() {
    for (auto &item : resources_) {
      std::fill(item.second.begin(), item.second.end(), 0);
    }
  }

  void Put(std::string name, std::vector<char> bytes) {
    resources_.insert_or_assign(Normalize(std::move(name)), std::move(bytes));
  }

  std::vector<char> Read(const std::string &name) const {
    auto normalized = Normalize(name);
    auto iter = resources_.find(normalized);
    if (iter == resources_.end()) {
      throw std::runtime_error("Memory resource not found: " + normalized);
    }
    return iter->second;
  }

  bool Contains(const std::string &name) const {
    return resources_.find(Normalize(name)) != resources_.end();
  }

 private:
  static std::string Normalize(std::string name) {
    std::replace(name.begin(), name.end(), '\\', '/');
    while (name.rfind("./", 0) == 0) name.erase(0, 2);
    while (!name.empty() && name.back() == '/') name.pop_back();
    if (name.empty() || name.front() == '/') {
      throw std::invalid_argument("Unsafe memory resource path");
    }
    std::istringstream input(name);
    std::string segment;
    while (std::getline(input, segment, '/')) {
      if (segment.empty() || segment == "..") {
        throw std::invalid_argument("Unsafe memory resource path");
      }
    }
    return name;
  }

  std::unordered_map<std::string, std::vector<char>> resources_;
};

}  // namespace edgevox_onnx
#endif  // EDGEVOX_ONNX_CSRC_MEMORY_RESOURCE_MANAGER_H_
