// edgevox-onnx/csrc/log.h
//
// Copyright (c)  2023  Xiaomi Corporation

#ifndef EDGEVOX_ONNX_CSRC_LOG_H_
#define EDGEVOX_ONNX_CSRC_LOG_H_

#include <stdio.h>

#include <mutex>  // NOLINT
#include <sstream>
#include <string>

namespace edgevox_onnx {

#if EDGEVOX_ONNX_ENABLE_CHECK

#if defined(NDEBUG)
constexpr bool kDisableDebug = true;
#else
constexpr bool kDisableDebug = false;
#endif

enum class LogLevel {
  kTrace = 0,
  kDebug = 1,
  kInfo = 2,
  kWarning = 3,
  kError = 4,
  kFatal = 5,  // print message and abort the program
};

// They are used in EDGEVOX_ONNX_LOG(xxx), so their names
// do not follow the google c++ code style
//
// You can use them in the following way:
//
//  EDGEVOX_ONNX_LOG(TRACE) << "some message";
//  EDGEVOX_ONNX_LOG(DEBUG) << "some message";
#ifndef _MSC_VER
constexpr LogLevel TRACE = LogLevel::kTrace;
constexpr LogLevel DEBUG = LogLevel::kDebug;
constexpr LogLevel INFO = LogLevel::kInfo;
constexpr LogLevel WARNING = LogLevel::kWarning;
constexpr LogLevel ERROR = LogLevel::kError;
constexpr LogLevel FATAL = LogLevel::kFatal;
#else
#define TRACE LogLevel::kTrace
#define DEBUG LogLevel::kDebug
#define INFO LogLevel::kInfo
#define WARNING LogLevel::kWarning
#define ERROR LogLevel::kError
#define FATAL LogLevel::kFatal
#endif

std::string GetStackTrace();

/* Return the current log level.


   If the current log level is TRACE, then all logged messages are printed out.

   If the current log level is DEBUG, log messages with "TRACE" level are not
   shown and all other levels are printed out.

   Similarly, if the current log level is INFO, log message with "TRACE" and
   "DEBUG" are not shown and all other levels are printed out.

   If it is FATAL, then only FATAL messages are shown.
 */
inline LogLevel GetCurrentLogLevel() {
  static LogLevel log_level = INFO;
  static std::once_flag init_flag;
  std::call_once(init_flag, []() {
    const char *env_log_level = std::getenv("EDGEVOX_ONNX_LOG_LEVEL");
    if (env_log_level == nullptr) return;

    std::string s = env_log_level;
    if (s == "TRACE")
      log_level = TRACE;
    else if (s == "DEBUG")
      log_level = DEBUG;
    else if (s == "INFO")
      log_level = INFO;
    else if (s == "WARNING")
      log_level = WARNING;
    else if (s == "ERROR")
      log_level = ERROR;
    else if (s == "FATAL")
      log_level = FATAL;
    else
      fprintf(stderr,
              "Unknown EDGEVOX_ONNX_LOG_LEVEL: %s"
              "\nSupported values are: "
              "TRACE, DEBUG, INFO, WARNING, ERROR, FATAL",
              s.c_str());
  });
  return log_level;
}

inline bool EnableAbort() {
  static std::once_flag init_flag;
  static bool enable_abort = false;
  std::call_once(init_flag, []() {
    enable_abort = (std::getenv("EDGEVOX_ONNX_ABORT") != nullptr);
  });
  return enable_abort;
}

class Logger {
 public:
  Logger(const char *filename, const char *func_name, uint32_t line_num,
         LogLevel level)
      : filename_(filename),
        func_name_(func_name),
        line_num_(line_num),
        level_(level) {
    cur_level_ = GetCurrentLogLevel();
    switch (level) {
      case TRACE:
        if (cur_level_ <= TRACE) fprintf(stderr, "[T] ");
        break;
      case DEBUG:
        if (cur_level_ <= DEBUG) fprintf(stderr, "[D] ");
        break;
      case INFO:
        if (cur_level_ <= INFO) fprintf(stderr, "[I] ");
        break;
      case WARNING:
        if (cur_level_ <= WARNING) fprintf(stderr, "[W] ");
        break;
      case ERROR:
        if (cur_level_ <= ERROR) fprintf(stderr, "[E] ");
        break;
      case FATAL:
        if (cur_level_ <= FATAL) fprintf(stderr, "[F] ");
        break;
    }

    if (cur_level_ <= level_) {
      fprintf(stderr, "%s:%u:%s ", filename, line_num, func_name);
    }
  }

  ~Logger() noexcept(false) {
    static constexpr const char *kErrMsg = R"(
    Some bad things happened. Please read the above error messages and stack
    trace. If you are using Python, the following command may be helpful:

      gdb --args python /path/to/your/code.py

    (You can use `gdb` to debug the code. Please consider compiling
    a debug version of edgevox_onnx.).

    If you are unable to fix it, please open an issue at:

      https://github.com/csukuangfj/kaldi-native-fbank/issues/new
    )";
    if (level_ == FATAL) {
      fprintf(stderr, "\n");
      std::string stack_trace = GetStackTrace();
      if (!stack_trace.empty()) {
        fprintf(stderr, "\n\n%s\n", stack_trace.c_str());
      }

      fflush(nullptr);

#ifndef __ANDROID_API__
      if (EnableAbort()) {
        // NOTE: abort() will terminate the program immediately without
        // printing the Python stack backtrace.
        abort();
      }

      throw std::runtime_error(kErrMsg);
#else
      abort();
#endif
    }
  }

  const Logger &operator<<(bool b) const {
    if (cur_level_ <= level_) {
      fprintf(stderr, b ? "true" : "false");
    }
    return *this;
  }

  const Logger &operator<<(int8_t i) const {
    if (cur_level_ <= level_) fprintf(stderr, "%d", i);
    return *this;
  }

  const Logger &operator<<(const char *s) const {
    if (cur_level_ <= level_) fprintf(stderr, "%s", s);
    return *this;
  }

  const Logger &operator<<(int32_t i) const {
    if (cur_level_ <= level_) fprintf(stderr, "%d", i);
    return *this;
  }

  const Logger &operator<<(uint32_t i) const {
    if (cur_level_ <= level_) fprintf(stderr, "%u", i);
    return *this;
  }

  const Logger &operator<<(uint64_t i) const {
    if (cur_level_ <= level_)
      fprintf(stderr, "%llu", (long long unsigned int)i);  // NOLINT
    return *this;
  }

  const Logger &operator<<(int64_t i) const {
    if (cur_level_ <= level_)
      fprintf(stderr, "%lli", (long long int)i);  // NOLINT
    return *this;
  }

  const Logger &operator<<(float f) const {
    if (cur_level_ <= level_) fprintf(stderr, "%f", f);
    return *this;
  }

  const Logger &operator<<(double d) const {
    if (cur_level_ <= level_) fprintf(stderr, "%f", d);
    return *this;
  }

  template <typename T>
  const Logger &operator<<(const T &t) const {
    // require T overloads operator<<
    std::ostringstream os;
    os << t;
    return *this << os.str().c_str();
  }

  // specialization to fix compile error: `stringstream << nullptr` is ambiguous
  const Logger &operator<<(const std::nullptr_t &null) const {
    if (cur_level_ <= level_) *this << "(null)";
    return *this;
  }

 private:
  const char *filename_;
  const char *func_name_;
  uint32_t line_num_;
  LogLevel level_;
  LogLevel cur_level_;
};
#endif  // EDGEVOX_ONNX_ENABLE_CHECK

class Voidifier {
 public:
#if EDGEVOX_ONNX_ENABLE_CHECK
  void operator&(const Logger &) const {}
#endif
};
#if !defined(EDGEVOX_ONNX_ENABLE_CHECK)
template <typename T>
const Voidifier &operator<<(const Voidifier &v, T &&) {
  return v;
}
#endif

}  // namespace edgevox_onnx

#define EDGEVOX_ONNX_STATIC_ASSERT(x) static_assert(x, "")

#ifdef EDGEVOX_ONNX_ENABLE_CHECK

#if defined(__clang__) || defined(__GNUC__) || defined(__GNUG__) || \
    defined(__PRETTY_FUNCTION__)
// for clang and GCC
#define EDGEVOX_ONNX_FUNC __PRETTY_FUNCTION__
#else
// for other compilers
#define EDGEVOX_ONNX_FUNC __func__
#endif

#define EDGEVOX_ONNX_CHECK(x)                                            \
  (x) ? (void)0                                                         \
      : ::edgevox_onnx::Voidifier() &                                    \
            ::edgevox_onnx::Logger(__FILE__, EDGEVOX_ONNX_FUNC, __LINE__, \
                                  ::edgevox_onnx::FATAL)                 \
                << "Check failed: " << #x << " "

// WARNING: x and y may be evaluated multiple times, but this happens only
// when the check fails. Since the program aborts if it fails, we don't think
// the extra evaluation of x and y matters.
//
// CAUTION: we recommend the following use case:
//
//      auto x = Foo();
//      auto y = Bar();
//      EDGEVOX_ONNX_CHECK_EQ(x, y) << "Some message";
//
//  And please avoid
//
//      EDGEVOX_ONNX_CHECK_EQ(Foo(), Bar());
//
//  if `Foo()` or `Bar()` causes some side effects, e.g., changing some
//  local static variables or global variables.
#define _EDGEVOX_ONNX_CHECK_OP(x, y, op)                                        \
  ((x)op(y)) ? (void)0                                                         \
             : ::edgevox_onnx::Voidifier() &                                    \
                   ::edgevox_onnx::Logger(__FILE__, EDGEVOX_ONNX_FUNC, __LINE__, \
                                         ::edgevox_onnx::FATAL)                 \
                       << "Check failed: " << #x << " " << #op << " " << #y    \
                       << " (" << (x) << " vs. " << (y) << ") "

#define EDGEVOX_ONNX_CHECK_EQ(x, y) _EDGEVOX_ONNX_CHECK_OP(x, y, ==)
#define EDGEVOX_ONNX_CHECK_NE(x, y) _EDGEVOX_ONNX_CHECK_OP(x, y, !=)
#define EDGEVOX_ONNX_CHECK_LT(x, y) _EDGEVOX_ONNX_CHECK_OP(x, y, <)
#define EDGEVOX_ONNX_CHECK_LE(x, y) _EDGEVOX_ONNX_CHECK_OP(x, y, <=)
#define EDGEVOX_ONNX_CHECK_GT(x, y) _EDGEVOX_ONNX_CHECK_OP(x, y, >)
#define EDGEVOX_ONNX_CHECK_GE(x, y) _EDGEVOX_ONNX_CHECK_OP(x, y, >=)

#define EDGEVOX_ONNX_LOG(x) \
  ::edgevox_onnx::Logger(__FILE__, EDGEVOX_ONNX_FUNC, __LINE__, ::edgevox_onnx::x)

// ------------------------------------------------------------
//       For debug check
// ------------------------------------------------------------
// If you define the macro "-D NDEBUG" while compiling kaldi-native-fbank,
// the following macros are in fact empty and does nothing.

#define EDGEVOX_ONNX_DCHECK(x) \
  ::edgevox_onnx::kDisableDebug ? (void)0 : EDGEVOX_ONNX_CHECK(x)

#define EDGEVOX_ONNX_DCHECK_EQ(x, y) \
  ::edgevox_onnx::kDisableDebug ? (void)0 : EDGEVOX_ONNX_CHECK_EQ(x, y)

#define EDGEVOX_ONNX_DCHECK_NE(x, y) \
  ::edgevox_onnx::kDisableDebug ? (void)0 : EDGEVOX_ONNX_CHECK_NE(x, y)

#define EDGEVOX_ONNX_DCHECK_LT(x, y) \
  ::edgevox_onnx::kDisableDebug ? (void)0 : EDGEVOX_ONNX_CHECK_LT(x, y)

#define EDGEVOX_ONNX_DCHECK_LE(x, y) \
  ::edgevox_onnx::kDisableDebug ? (void)0 : EDGEVOX_ONNX_CHECK_LE(x, y)

#define EDGEVOX_ONNX_DCHECK_GT(x, y) \
  ::edgevox_onnx::kDisableDebug ? (void)0 : EDGEVOX_ONNX_CHECK_GT(x, y)

#define EDGEVOX_ONNX_DCHECK_GE(x, y) \
  ::edgevox_onnx::kDisableDebug ? (void)0 : EDGEVOX_ONNX_CHECK_GE(x, y)

#define EDGEVOX_ONNX_DLOG(x)    \
  ::edgevox_onnx::kDisableDebug \
      ? (void)0                \
      : ::edgevox_onnx::Voidifier() & EDGEVOX_ONNX_LOG(x)

#else

#define EDGEVOX_ONNX_CHECK(x) ::edgevox_onnx::Voidifier()
#define EDGEVOX_ONNX_LOG(x) ::edgevox_onnx::Voidifier()

#define EDGEVOX_ONNX_CHECK_EQ(x, y) ::edgevox_onnx::Voidifier()
#define EDGEVOX_ONNX_CHECK_NE(x, y) ::edgevox_onnx::Voidifier()
#define EDGEVOX_ONNX_CHECK_LT(x, y) ::edgevox_onnx::Voidifier()
#define EDGEVOX_ONNX_CHECK_LE(x, y) ::edgevox_onnx::Voidifier()
#define EDGEVOX_ONNX_CHECK_GT(x, y) ::edgevox_onnx::Voidifier()
#define EDGEVOX_ONNX_CHECK_GE(x, y) ::edgevox_onnx::Voidifier()

#define EDGEVOX_ONNX_DCHECK(x) ::edgevox_onnx::Voidifier()
#define EDGEVOX_ONNX_DLOG(x) ::edgevox_onnx::Voidifier()
#define EDGEVOX_ONNX_DCHECK_EQ(x, y) ::edgevox_onnx::Voidifier()
#define EDGEVOX_ONNX_DCHECK_NE(x, y) ::edgevox_onnx::Voidifier()
#define EDGEVOX_ONNX_DCHECK_LT(x, y) ::edgevox_onnx::Voidifier()
#define EDGEVOX_ONNX_DCHECK_LE(x, y) ::edgevox_onnx::Voidifier()
#define EDGEVOX_ONNX_DCHECK_GT(x, y) ::edgevox_onnx::Voidifier()
#define EDGEVOX_ONNX_DCHECK_GE(x, y) ::edgevox_onnx::Voidifier()

#endif  // EDGEVOX_ONNX_CHECK_NE

#endif  // EDGEVOX_ONNX_CSRC_LOG_H_
