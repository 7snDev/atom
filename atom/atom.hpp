/*
 * MIT License
 *
 * Copyright (c) 2017 Serge Zaitsev
 * Copyright (c) 2022 Steffen André Langnes
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
// file begin: include/webview/errors.h
#ifndef WEBVIEW_ERRORS_H
#define WEBVIEW_ERRORS_H

/// @name Errors
/// @{

/**
 * @brief Error codes returned to callers of the API.
 *
 * The following codes are commonly used in the library:
 * - @c WEBVIEW_ERROR_OK
 * - @c WEBVIEW_ERROR_UNSPECIFIED
 * - @c WEBVIEW_ERROR_INVALID_ARGUMENT
 * - @c WEBVIEW_ERROR_INVALID_STATE
 *
 * With the exception of @c WEBVIEW_ERROR_OK which is normally expected,
 * the other common codes do not normally need to be handled specifically.
 * Refer to specific functions regarding handling of other codes.
 */
typedef enum {
  /// Missing dependency.
  WEBVIEW_ERROR_MISSING_DEPENDENCY = -5,
  /// Operation canceled.
  WEBVIEW_ERROR_CANCELED = -4,
  /// Invalid state detected.
  WEBVIEW_ERROR_INVALID_STATE = -3,
  /// One or more invalid arguments have been specified e.g. in a function call.
  WEBVIEW_ERROR_INVALID_ARGUMENT = -2,
  /// An unspecified error occurred. A more specific error code may be needed.
  WEBVIEW_ERROR_UNSPECIFIED = -1,
  /// OK/Success. Functions that return error codes will typically return this
  /// to signify successful operations.
  WEBVIEW_ERROR_OK = 0,
  /// Signifies that something already exists.
  WEBVIEW_ERROR_DUPLICATE = 1,
  /// Signifies that something does not exist.
  WEBVIEW_ERROR_NOT_FOUND = 2
} webview_error_t;

/// @}

#endif // WEBVIEW_ERRORS_H
// file end: include/webview/errors.h
// file begin: include/webview/macros.h
#ifndef WEBVIEW_MACROS_H
#define WEBVIEW_MACROS_H

/**
 * Used to specify function linkage such as extern, inline, etc.
 *
 * When @c WEBVIEW_API is not already defined, the defaults are as follows:
 *
 * - @c inline when compiling C++ code.
 * - @c extern when compiling C code.
 *
 * The following macros can be used to automatically set an appropriate
 * value for @c WEBVIEW_API:
 *
 * - Define @c WEBVIEW_BUILD_SHARED when building a shared library.
 * - Define @c WEBVIEW_SHARED when using a shared library.
 * - Define @c WEBVIEW_STATIC when building or using a static library.
 */
#ifndef WEBVIEW_API
#if defined(WEBVIEW_SHARED) || defined(WEBVIEW_BUILD_SHARED)
#if defined(_WIN32) || defined(__CYGWIN__)
#if defined(WEBVIEW_BUILD_SHARED)
#define WEBVIEW_API __declspec(dllexport)
#else
#define WEBVIEW_API __declspec(dllimport)
#endif
#else
#define WEBVIEW_API __attribute__((visibility("default")))
#endif
#elif !defined(WEBVIEW_STATIC) && defined(__cplusplus)
#define WEBVIEW_API inline
#else
#define WEBVIEW_API extern
#endif
#endif

/// @name Used internally
/// @{

/// Utility macro for stringifying a macro argument.
#define WEBVIEW_STRINGIFY(x) #x

/// Utility macro for stringifying the result of a macro argument expansion.
#define WEBVIEW_EXPAND_AND_STRINGIFY(x) WEBVIEW_STRINGIFY(x)

/// @}

/// @brief Evaluates to @c TRUE for error codes indicating success or
///        additional information.
#define WEBVIEW_SUCCEEDED(error) ((int)(error) >= 0)

/// Evaluates to @c TRUE if the given error code indicates failure.
#define WEBVIEW_FAILED(error) ((int)(error) < 0)

#ifdef __cplusplus
#ifndef WEBVIEW_HEADER

#if defined(__APPLE__)
#define WEBVIEW_PLATFORM_DARWIN
#elif defined(__unix__)
#define WEBVIEW_PLATFORM_LINUX
#elif defined(_WIN32)
#define WEBVIEW_PLATFORM_WINDOWS
#else
#error "Unable to detect current platform"
#endif

#if !defined(WEBVIEW_GTK) && !defined(WEBVIEW_COCOA) && !defined(WEBVIEW_EDGE)
#if defined(WEBVIEW_PLATFORM_DARWIN)
#define WEBVIEW_COCOA
#elif defined(WEBVIEW_PLATFORM_LINUX)
#define WEBVIEW_GTK
#elif defined(WEBVIEW_PLATFORM_WINDOWS)
#define WEBVIEW_EDGE
#else
#error "please, specify webview backend"
#endif
#endif

#ifndef WEBVIEW_DEPRECATED
#if __cplusplus >= 201402L
#define WEBVIEW_DEPRECATED(reason) [[deprecated(reason)]]
#elif defined(_MSC_VER)
#define WEBVIEW_DEPRECATED(reason) __declspec(deprecated(reason))
#else
#define WEBVIEW_DEPRECATED(reason) __attribute__((deprecated(reason)))
#endif
#endif

#ifndef WEBVIEW_DEPRECATED_PRIVATE
#define WEBVIEW_DEPRECATED_PRIVATE                                             \
  WEBVIEW_DEPRECATED("Private API should not be used")
#endif

#endif // WEBVIEW_HEADER
#endif // __cplusplus

#endif // WEBVIEW_MACROS_H
// file end: include/webview/macros.h
// file begin: include/webview/types.h
#ifndef WEBVIEW_TYPES_H
#define WEBVIEW_TYPES_H

/// Holds the elements of a MAJOR.MINOR.PATCH version number.
typedef struct {
  /// Major version.
  unsigned int major;
  /// Minor version.
  unsigned int minor;
  /// Patch version.
  unsigned int patch;
} webview_version_t;

/// Holds the library's version information.
typedef struct {
  /// The elements of the version number.
  webview_version_t version;
  /// SemVer 2.0.0 version number in MAJOR.MINOR.PATCH format.
  char version_number[32];
  /// SemVer 2.0.0 pre-release labels prefixed with "-" if specified, otherwise
  /// an empty string.
  char pre_release[48];
  /// SemVer 2.0.0 build metadata prefixed with "+", otherwise an empty string.
  char build_metadata[48];
} webview_version_info_t;

/// Pointer to a webview instance.
typedef void *webview_t;

/// Native handle kind. The actual type depends on the backend.
typedef enum {
  /// Top-level window. @c GtkWindow pointer (GTK), @c NSWindow pointer (Cocoa)
  /// or @c HWND (Win32).
  WEBVIEW_NATIVE_HANDLE_KIND_UI_WINDOW,
  /// Browser widget. @c GtkWidget pointer (GTK), @c NSView pointer (Cocoa) or
  /// @c HWND (Win32).
  WEBVIEW_NATIVE_HANDLE_KIND_UI_WIDGET,
  /// Browser controller. @c WebKitWebView pointer (WebKitGTK), @c WKWebView
  /// pointer (Cocoa/WebKit) or @c ICoreWebView2Controller pointer
  /// (Win32/WebView2).
  WEBVIEW_NATIVE_HANDLE_KIND_BROWSER_CONTROLLER
} webview_native_handle_kind_t;

/// Window size hints
typedef enum {
  /// Width and height are default size.
  WEBVIEW_HINT_NONE,
  /// Width and height are minimum bounds.
  WEBVIEW_HINT_MIN,
  /// Width and height are maximum bounds.
  WEBVIEW_HINT_MAX,
  /// Window size can not be changed by a user.
  WEBVIEW_HINT_FIXED
} webview_hint_t;

#endif // WEBVIEW_TYPES_H
// file end: include/webview/types.h
// file begin: include/webview/detail/user_script.hh
#ifndef WEBVIEW_DETAIL_USER_SCRIPT_HH
#define WEBVIEW_DETAIL_USER_SCRIPT_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)

#include <functional>
#include <memory>
#include <string>
#include <utility>

namespace webview {
namespace detail {

class user_script {
public:
  class impl;
  using impl_deleter = std::function<void(impl *)>;
  using impl_ptr = std::unique_ptr<impl, impl_deleter>;

  user_script(const std::string &code, impl_ptr &&impl_)
      : m_code{code}, m_impl{std::move(impl_)} {}

  user_script(const user_script &other) = delete;
  user_script &operator=(const user_script &other) = delete;
  user_script(user_script &&other) noexcept { *this = std::move(other); }

  user_script &operator=(user_script &&other) noexcept {
    if (this == &other) {
      return *this;
    }
    m_code = std::move(other.m_code);
    m_impl = std::move(other.m_impl);
    return *this;
  }

  const std::string &get_code() const { return m_code; }

  impl &get_impl() { return *m_impl; }

  const impl &get_impl() const { return *m_impl; }

private:
  std::string m_code;
  impl_ptr m_impl;
};

} // namespace detail
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_DETAIL_USER_SCRIPT_HH
// file end: include/webview/detail/user_script.hh
// file begin: include/webview/detail/utility/string.hh
#ifndef WEBVIEW_DETAIL_UTILITY_STRING_HH
#define WEBVIEW_DETAIL_UTILITY_STRING_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)

#include <string>

#if defined(_WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

namespace webview {
namespace detail {

#if defined(_WIN32)
// Converts a narrow (UTF-8-encoded) string into a wide (UTF-16-encoded) string.
inline std::wstring widen_string(const std::string &input) {
  if (input.empty()) {
    return std::wstring();
  }
  UINT cp = CP_UTF8;
  DWORD flags = MB_ERR_INVALID_CHARS;
  auto input_c = input.c_str();
  auto input_length = static_cast<int>(input.size());
  auto required_length =
      MultiByteToWideChar(cp, flags, input_c, input_length, nullptr, 0);
  if (required_length > 0) {
    std::wstring output(static_cast<std::size_t>(required_length), L'\0');
    if (MultiByteToWideChar(cp, flags, input_c, input_length, &output[0],
                            required_length) > 0) {
      return output;
    }
  }
  // Failed to convert string from UTF-8 to UTF-16
  return std::wstring();
}

// Converts a wide (UTF-16-encoded) string into a narrow (UTF-8-encoded) string.
inline std::string narrow_string(const std::wstring &input) {
  struct wc_flags {
    enum TYPE : unsigned int {
      // WC_ERR_INVALID_CHARS
      err_invalid_chars = 0x00000080U
    };
  };
  if (input.empty()) {
    return std::string();
  }
  UINT cp = CP_UTF8;
  DWORD flags = wc_flags::err_invalid_chars;
  auto input_c = input.c_str();
  auto input_length = static_cast<int>(input.size());
  auto required_length = WideCharToMultiByte(cp, flags, input_c, input_length,
                                             nullptr, 0, nullptr, nullptr);
  if (required_length > 0) {
    std::string output(static_cast<std::size_t>(required_length), '\0');
    if (WideCharToMultiByte(cp, flags, input_c, input_length, &output[0],
                            required_length, nullptr, nullptr) > 0) {
      return output;
    }
  }
  // Failed to convert string from UTF-16 to UTF-8
  return std::string();
}
#endif

} // namespace detail
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_DETAIL_UTILITY_STRING_HH
// file end: include/webview/detail/utility/string.hh
// file begin: include/webview/detail/exceptions.hh
#ifndef WEBVIEW_DETAIL_EXCEPTIONS_HH
#define WEBVIEW_DETAIL_EXCEPTIONS_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)

#include <exception>

namespace webview {
namespace detail {

class bad_access : public std::exception {};

} // namespace detail
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_DETAIL_EXCEPTIONS_HH
// file end: include/webview/detail/exceptions.hh
// file begin: include/webview/detail/json.hh
#ifndef WEBVIEW_DETAIL_JSON_HH
#define WEBVIEW_DETAIL_JSON_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)

#include <cassert>
#include <cstring>
#include <string>

namespace webview {
namespace detail {

inline int json_parse_c(const char *s, size_t sz, const char *key, size_t keysz,
                        const char **value, size_t *valuesz) {
  enum {
    JSON_STATE_VALUE,
    JSON_STATE_LITERAL,
    JSON_STATE_STRING,
    JSON_STATE_ESCAPE,
    JSON_STATE_UTF8
  } state = JSON_STATE_VALUE;
  const char *k = nullptr;
  int index = 1;
  int depth = 0;
  int utf8_bytes = 0;

  *value = nullptr;
  *valuesz = 0;

  if (key == nullptr) {
    index = static_cast<decltype(index)>(keysz);
    if (index < 0) {
      return -1;
    }
    keysz = 0;
  }

  for (; sz > 0; s++, sz--) {
    enum {
      JSON_ACTION_NONE,
      JSON_ACTION_START,
      JSON_ACTION_END,
      JSON_ACTION_START_STRUCT,
      JSON_ACTION_END_STRUCT
    } action = JSON_ACTION_NONE;
    auto c = static_cast<unsigned char>(*s);
    switch (state) {
    case JSON_STATE_VALUE:
      if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == ',' ||
          c == ':') {
        continue;
      } else if (c == '"') {
        action = JSON_ACTION_START;
        state = JSON_STATE_STRING;
      } else if (c == '{' || c == '[') {
        action = JSON_ACTION_START_STRUCT;
      } else if (c == '}' || c == ']') {
        action = JSON_ACTION_END_STRUCT;
      } else if (c == 't' || c == 'f' || c == 'n' || c == '-' ||
                 (c >= '0' && c <= '9')) {
        action = JSON_ACTION_START;
        state = JSON_STATE_LITERAL;
      } else {
        return -1;
      }
      break;
    case JSON_STATE_LITERAL:
      if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == ',' ||
          c == ']' || c == '}' || c == ':') {
        state = JSON_STATE_VALUE;
        s--;
        sz++;
        action = JSON_ACTION_END;
      } else if (c < 32 || c > 126) {
        return -1;
      } // fallthrough
    case JSON_STATE_STRING:
      if (c < 32 || (c > 126 && c < 192)) {
        return -1;
      } else if (c == '"') {
        action = JSON_ACTION_END;
        state = JSON_STATE_VALUE;
      } else if (c == '\\') {
        state = JSON_STATE_ESCAPE;
      } else if (c >= 192 && c < 224) {
        utf8_bytes = 1;
        state = JSON_STATE_UTF8;
      } else if (c >= 224 && c < 240) {
        utf8_bytes = 2;
        state = JSON_STATE_UTF8;
      } else if (c >= 240 && c < 247) {
        utf8_bytes = 3;
        state = JSON_STATE_UTF8;
      } else if (c >= 128 && c < 192) {
        return -1;
      }
      break;
    case JSON_STATE_ESCAPE:
      if (c == '"' || c == '\\' || c == '/' || c == 'b' || c == 'f' ||
          c == 'n' || c == 'r' || c == 't' || c == 'u') {
        state = JSON_STATE_STRING;
      } else {
        return -1;
      }
      break;
    case JSON_STATE_UTF8:
      if (c < 128 || c > 191) {
        return -1;
      }
      utf8_bytes--;
      if (utf8_bytes == 0) {
        state = JSON_STATE_STRING;
      }
      break;
    default:
      return -1;
    }

    if (action == JSON_ACTION_END_STRUCT) {
      depth--;
    }

    if (depth == 1) {
      if (action == JSON_ACTION_START || action == JSON_ACTION_START_STRUCT) {
        if (index == 0) {
          *value = s;
        } else if (keysz > 0 && index == 1) {
          k = s;
        } else {
          index--;
        }
      } else if (action == JSON_ACTION_END ||
                 action == JSON_ACTION_END_STRUCT) {
        if (*value != nullptr && index == 0) {
          *valuesz = static_cast<size_t>(s + 1 - *value);
          return 0;
        } else if (keysz > 0 && k != nullptr) {
          if (keysz == static_cast<size_t>(s - k - 1) &&
              memcmp(key, k + 1, keysz) == 0) {
            index = 0;
          } else {
            index = 2;
          }
          k = nullptr;
        }
      }
    }

    if (action == JSON_ACTION_START_STRUCT) {
      depth++;
    }
  }
  return -1;
}

constexpr bool is_json_special_char(char c) {
  return c == '"' || c == '\\' || c == '\b' || c == '\f' || c == '\n' ||
         c == '\r' || c == '\t';
}

constexpr bool is_ascii_control_char(char c) { return c >= 0 && c <= 0x1f; }

inline std::string json_escape(const std::string &s, bool add_quotes = true) {
  // Calculate the size of the resulting string.
  // Add space for the double quotes.
  size_t required_length = add_quotes ? 2 : 0;
  for (auto c : s) {
    if (is_json_special_char(c)) {
      // '\' and a single following character
      required_length += 2;
      continue;
    }
    if (is_ascii_control_char(c)) {
      // '\', 'u', 4 digits
      required_length += 6;
      continue;
    }
    ++required_length;
  }
  // Allocate memory for resulting string only once.
  std::string result;
  result.reserve(required_length);
  if (add_quotes) {
    result += '"';
  }
  // Copy string while escaping characters.
  for (auto c : s) {
    if (is_json_special_char(c)) {
      static constexpr char special_escape_table[256] =
          "\0\0\0\0\0\0\0\0btn\0fr\0\0"
          "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
          "\0\0\"\0\0\0\0\0\0\0\0\0\0\0\0\0"
          "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
          "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
          "\0\0\0\0\0\0\0\0\0\0\0\0\\";
      result += '\\';
      // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
      result += special_escape_table[static_cast<unsigned char>(c)];
      continue;
    }
    if (is_ascii_control_char(c)) {
      // Escape as \u00xx
      static constexpr char hex_alphabet[]{"0123456789abcdef"};
      auto uc = static_cast<unsigned char>(c);
      auto h = (uc >> 4) & 0x0f;
      auto l = uc & 0x0f;
      result += "\\u00";
      // NOLINTBEGIN(cppcoreguidelines-pro-bounds-constant-array-index)
      result += hex_alphabet[h];
      result += hex_alphabet[l];
      // NOLINTEND(cppcoreguidelines-pro-bounds-constant-array-index)
      continue;
    }
    result += c;
  }
  if (add_quotes) {
    result += '"';
  }
  // Should have calculated the exact amount of memory needed
  assert(required_length == result.size());
  return result;
}

inline int json_unescape(const char *s, size_t n, char *out) {
  int r = 0;
  if (*s++ != '"') {
    return -1;
  }
  while (n > 2) {
    char c = *s;
    if (c == '\\') {
      s++;
      n--;
      switch (*s) {
      case 'b':
        c = '\b';
        break;
      case 'f':
        c = '\f';
        break;
      case 'n':
        c = '\n';
        break;
      case 'r':
        c = '\r';
        break;
      case 't':
        c = '\t';
        break;
      case '\\':
        c = '\\';
        break;
      case '/':
        c = '/';
        break;
      case '\"':
        c = '\"';
        break;
      default: // TODO: support unicode decoding
        return -1;
      }
    }
    if (out != nullptr) {
      *out++ = c;
    }
    s++;
    n--;
    r++;
  }
  if (*s != '"') {
    return -1;
  }
  if (out != nullptr) {
    *out = '\0';
  }
  return r;
}

inline std::string json_parse(const std::string &s, const std::string &key,
                              const int index) {
  const char *value;
  size_t value_sz;
  if (key.empty()) {
    json_parse_c(s.c_str(), s.length(), nullptr, index, &value, &value_sz);
  } else {
    json_parse_c(s.c_str(), s.length(), key.c_str(), key.length(), &value,
                 &value_sz);
  }
  if (value != nullptr) {
    if (value[0] != '"') {
      return {value, value_sz};
    }
    int n = json_unescape(value, value_sz, nullptr);
    if (n > 0) {
      char *decoded = new char[n + 1];
      json_unescape(value, value_sz, decoded);
      std::string result(decoded, n);
      delete[] decoded;
      return result;
    }
  }
  return "";
}

} // namespace detail
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_DETAIL_JSON_HH
// file end: include/webview/detail/json.hh
// file begin: include/webview/errors.hh
#ifndef WEBVIEW_ERRORS_HH
#define WEBVIEW_ERRORS_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#include <exception>
#include <string>

namespace webview {

class error_info {
public:
  error_info(webview_error_t code, const std::string &message = {}) noexcept
      : m_code{code}, m_message{message} {}
  error_info() = default;

  webview_error_t code() const { return m_code; }
  const std::string &message() const { return m_message; }

private:
  webview_error_t m_code{WEBVIEW_ERROR_UNSPECIFIED};
  std::string m_message;
};

class exception : public std::exception {
public:
  exception(webview_error_t code, const std::string &message,
            std::exception_ptr cause) noexcept
      : exception{error_info{code, message}, cause} {}

  exception(webview_error_t code, const std::string &message) noexcept
      : exception{error_info{code, message}} {}

  exception(const error_info &error, std::exception_ptr cause) noexcept
      : m_error{error},
        // NOLINTNEXTLINE(bugprone-throw-keyword-missing)
        m_cause{cause} {}

  exception(const error_info &error) noexcept : m_error{error} {}

  exception() = default;

  const error_info &error() const { return m_error; }
  std::exception_ptr cause() const { return m_cause; }

  const char *what() const noexcept override {
    return m_error.message().c_str();
  }

private:
  error_info m_error{WEBVIEW_ERROR_UNSPECIFIED};
  std::exception_ptr m_cause;
};

} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_ERRORS_HH
// file end: include/webview/errors.hh
// file begin: include/webview/detail/platform/darwin/cocoa/NSPoint.hh
#ifndef WEBVIEW_PLATFORM_DARWIN_COCOA_NSPOINT_HH
#define WEBVIEW_PLATFORM_DARWIN_COCOA_NSPOINT_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)

#include <CoreGraphics/CoreGraphics.h>

namespace webview {
namespace detail {
namespace cocoa {

using NSPoint = CGPoint;

constexpr inline NSPoint NSPointMake(CGFloat x, CGFloat y) {
  return CGPointMake(x, y);
}

} // namespace cocoa
} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_DARWIN_COCOA_NSPOINT_HH
// file end: include/webview/detail/platform/darwin/cocoa/NSPoint.hh
// file begin: include/webview/detail/platform/darwin/cocoa/NSRect.hh
#ifndef WEBVIEW_PLATFORM_DARWIN_COCOA_NSRECT_HH
#define WEBVIEW_PLATFORM_DARWIN_COCOA_NSRECT_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)

#include <CoreGraphics/CoreGraphics.h>

namespace webview {
namespace detail {
namespace cocoa {

using NSRect = CGRect;

constexpr inline NSRect NSRectMake(CGFloat x, CGFloat y, CGFloat w, CGFloat h) {
  return CGRectMake(x, y, w, h);
}

} // namespace cocoa
} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_DARWIN_COCOA_NSRECT_HH
// file end: include/webview/detail/platform/darwin/cocoa/NSRect.hh
// file begin: include/webview/detail/platform/darwin/cocoa/NSSize.hh
#ifndef WEBVIEW_PLATFORM_DARWIN_COCOA_NSSIZE_HH
#define WEBVIEW_PLATFORM_DARWIN_COCOA_NSSIZE_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)

#include <CoreGraphics/CoreGraphics.h>

namespace webview {
namespace detail {
namespace cocoa {

using NSSize = CGSize;

constexpr inline NSSize NSSizeMake(CGFloat w, CGFloat h) {
  return CGSizeMake(w, h);
}

} // namespace cocoa
} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_DARWIN_COCOA_NSSIZE_HH
// file end: include/webview/detail/platform/darwin/cocoa/NSSize.hh
// file begin: include/webview/detail/platform/darwin/cocoa/types.hh
#ifndef WEBVIEW_PLATFORM_DARWIN_COCOA_TYPES_HH
#define WEBVIEW_PLATFORM_DARWIN_COCOA_TYPES_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)

#include <objc/NSObjCRuntime.h>

namespace webview {
namespace detail {
namespace cocoa {

using NSTimeInterval = double;

} // namespace cocoa
} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_DARWIN_COCOA_TYPES_HH
// file end: include/webview/detail/platform/darwin/cocoa/types.hh
// file begin: include/webview/detail/platform/darwin/objc/invoke.hh
#ifndef WEBVIEW_PLATFORM_DARWIN_OBJC_INVOKE_HH
#define WEBVIEW_PLATFORM_DARWIN_OBJC_INVOKE_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_DARWIN)

#include <objc/objc-runtime.h>

namespace webview {
namespace detail {
namespace objc {

// A convenient template function for unconditionally casting the specified
// C-like function into a function that can be called with the given return
// type and arguments. Caller takes full responsibility for ensuring that
// the function call is valid. It is assumed that the function will not
// throw exceptions.
template <typename Result, typename Callable, typename... Args>
Result invoke(Callable callable, Args... args) noexcept {
  return reinterpret_cast<Result (*)(Args...)>(callable)(args...);
}

// Calls objc_msgSend.
template <typename Result, typename... Args>
Result msg_send(Args... args) noexcept {
  return invoke<Result>(objc_msgSend, args...);
}

// Calls objc_msgSend_stret or objc_msgSend depending on architecture.
template <typename Result, typename... Args>
Result msg_send_stret(Args... args) noexcept {
#if defined(__arm64__)
  return invoke<Result>(objc_msgSend, args...);
#else
  return invoke<Result>(objc_msgSend_stret, args...);
#endif
}

inline SEL selector(const char *name) { return sel_registerName(name); }

} // namespace objc
} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_DARWIN)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_DARWIN_OBJC_INVOKE_HH
// file end: include/webview/detail/platform/darwin/objc/invoke.hh
// file begin: include/webview/detail/platform/linux/gtk/compat.hh
#ifndef WEBVIEW_PLATFORM_LINUX_GTK_COMPAT_HH
#define WEBVIEW_PLATFORM_LINUX_GTK_COMPAT_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_LINUX) && defined(WEBVIEW_GTK)

#include <gtk/gtk.h>

#if GTK_MAJOR_VERSION >= 4

#ifdef GDK_WINDOWING_X11
#include <gdk/x11/gdkx.h>
#endif

#elif GTK_MAJOR_VERSION >= 3

#ifdef GDK_WINDOWING_X11
#include <gdk/gdkx.h>
#endif

#endif

namespace webview {
namespace detail {

/**
 * GTK compatibility helper class.
 */
class gtk_compat {
public:
  static gboolean init_check() {
#if GTK_MAJOR_VERSION >= 4
    return gtk_init_check();
#else
    return gtk_init_check(nullptr, nullptr);
#endif
  }

  static GtkWidget *window_new() {
#if GTK_MAJOR_VERSION >= 4
    return gtk_window_new();
#else
    return gtk_window_new(GTK_WINDOW_TOPLEVEL);
#endif
  }

  static void window_set_child(GtkWindow *window, GtkWidget *widget) {
#if GTK_MAJOR_VERSION >= 4
    gtk_window_set_child(window, widget);
#else
    gtk_container_add(GTK_CONTAINER(window), widget);
#endif
  }

  static void window_remove_child(GtkWindow *window, GtkWidget *widget) {
#if GTK_MAJOR_VERSION >= 4
    if (gtk_window_get_child(window) == widget) {
      gtk_window_set_child(window, nullptr);
    }
#else
    gtk_container_remove(GTK_CONTAINER(window), widget);
#endif
  }

  static void widget_set_visible(GtkWidget *widget, bool visible) {
#if GTK_MAJOR_VERSION >= 4
    gtk_widget_set_visible(widget, visible ? TRUE : FALSE);
#else
    if (visible) {
      gtk_widget_show(widget);
    } else {
      gtk_widget_hide(widget);
    }
#endif
  }

  static void window_set_size(GtkWindow *window, int width, int height) {
    // GTK 4 can set a default window size, but unlike GTK 3 it can't resize
    // the window after it has been set up.
#if GTK_MAJOR_VERSION >= 4
    gtk_window_set_default_size(window, width, height);
#else
    gtk_window_resize(window, width, height);
#endif
  }

  static void window_set_max_size(GtkWindow *window, int width, int height) {
// X11-specific features are available in GTK 3 but not GTK 4
#if GTK_MAJOR_VERSION < 4
    GdkGeometry g{};
    g.max_width = width;
    g.max_height = height;
    GdkWindowHints h = GDK_HINT_MAX_SIZE;
    gtk_window_set_geometry_hints(GTK_WINDOW(window), nullptr, &g, h);
#else
    // Avoid "unused parameter" warnings
    (void)window;
    (void)width;
    (void)height;
#endif
  }
};

} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_LINUX) && defined(WEBVIEW_GTK)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_LINUX_GTK_COMPAT_HH
// file end: include/webview/detail/platform/linux/gtk/compat.hh
// file begin: include/webview/detail/platform/linux/webkitgtk/compat.hh
#ifndef WEBVIEW_PLATFORM_LINUX_WEBKITGTK_COMPAT_HH
#define WEBVIEW_PLATFORM_LINUX_WEBKITGTK_COMPAT_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_LINUX) && defined(WEBVIEW_GTK)

#include <functional>
#include <string>

#include <gtk/gtk.h>

#if GTK_MAJOR_VERSION >= 4

#include <jsc/jsc.h>
#include <webkit/webkit.h>

#ifdef GDK_WINDOWING_X11
#include <gdk/x11/gdkx.h>
#endif

#elif GTK_MAJOR_VERSION >= 3

#include <JavaScriptCore/JavaScript.h>
#include <webkit2/webkit2.h>

#ifdef GDK_WINDOWING_X11
#include <gdk/gdkx.h>
#endif

#endif

namespace webview {
namespace detail {

/**
 * WebKitGTK compatibility helper class.
 */
class webkitgtk_compat {
public:
#if GTK_MAJOR_VERSION >= 4
  using wk_handler_js_value_t = JSCValue;
#else
  using wk_handler_js_value_t = WebKitJavascriptResult;
#endif

  using on_script_message_received_t =
      std::function<void(WebKitUserContentManager *, const std::string &)>;
  static void
  connect_script_message_received(WebKitUserContentManager *manager,
                                  const std::string &handler_name,
                                  on_script_message_received_t handler) {
    std::string signal_name = "script-message-received::";
    signal_name += handler_name;

    auto callback = +[](WebKitUserContentManager *manager,
                        wk_handler_js_value_t *r, gpointer arg) {
      auto *handler = static_cast<on_script_message_received_t *>(arg);
      (*handler)(manager, get_string_from_js_result(r));
    };

    auto deleter = +[](gpointer data, GClosure *) {
      delete static_cast<on_script_message_received_t *>(data);
    };

    g_signal_connect_data(manager, signal_name.c_str(), G_CALLBACK(callback),
                          new on_script_message_received_t{handler}, deleter,
                          static_cast<GConnectFlags>(0) /*G_CONNECT_DEFAULT*/);
  }

  static std::string get_string_from_js_result(JSCValue *r) {
    char *cs = jsc_value_to_string(r);
    std::string s{cs};
    g_free(cs);
    return s;
  }

#if GTK_MAJOR_VERSION < 4
  static std::string get_string_from_js_result(WebKitJavascriptResult *r) {
#if (WEBKIT_MAJOR_VERSION == 2 && WEBKIT_MINOR_VERSION >= 22) ||               \
    WEBKIT_MAJOR_VERSION > 2
    JSCValue *value = webkit_javascript_result_get_js_value(r);
    return get_string_from_js_result(value);
#else
    JSGlobalContextRef ctx = webkit_javascript_result_get_global_context(r);
    JSValueRef value = webkit_javascript_result_get_value(r);
    JSStringRef js = JSValueToStringCopy(ctx, value, nullptr);
    size_t n = JSStringGetMaximumUTF8CStringSize(js);
    char *cs = g_new(char, n);
    JSStringGetUTF8CString(js, cs, n);
    JSStringRelease(js);
    std::string s{cs};
    g_free(cs);
    return s;
#endif
  }
#endif

  static void user_content_manager_register_script_message_handler(
      WebKitUserContentManager *manager, const gchar *name) {
#if GTK_MAJOR_VERSION >= 4
    webkit_user_content_manager_register_script_message_handler(manager, name,
                                                                nullptr);
#else
    webkit_user_content_manager_register_script_message_handler(manager, name);
#endif
  }
};

} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_LINUX) && defined(WEBVIEW_GTK)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_LINUX_WEBKITGTK_COMPAT_HH
// file end: include/webview/detail/platform/linux/webkitgtk/compat.hh
// file begin: include/webview/detail/platform/linux/webkitgtk/dmabuf.hh
#ifndef WEBVIEW_BACKENDS_GTK_WEBKITGTK_DMABUF_HH
#define WEBVIEW_BACKENDS_GTK_WEBKITGTK_DMABUF_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_LINUX) && defined(WEBVIEW_GTK)

#include <cstdlib>
#include <string>

#include <gtk/gtk.h>

#if GTK_MAJOR_VERSION >= 4

#include <jsc/jsc.h>
#include <webkit/webkit.h>

#ifdef GDK_WINDOWING_X11
#include <gdk/x11/gdkx.h>
#endif

#elif GTK_MAJOR_VERSION >= 3

#include <JavaScriptCore/JavaScript.h>
#include <webkit2/webkit2.h>

#ifdef GDK_WINDOWING_X11
#include <gdk/gdkx.h>
#endif

#endif

#include <fcntl.h>
#include <sys/stat.h>

namespace webview {
namespace detail {

// Namespace containing workaround for WebKit 2.42 when using NVIDIA GPU
// driver.
// See WebKit bug: https://bugs.webkit.org/show_bug.cgi?id=261874
// Please remove all of the code in this namespace when it's no longer needed.
namespace webkit_dmabuf {

// Get environment variable. Not thread-safe.
static inline std::string get_env(const std::string &name) {
  auto *value = std::getenv(name.c_str());
  if (value) {
    return {value};
  }
  return {};
}

// Set environment variable. Not thread-safe.
static inline void set_env(const std::string &name, const std::string &value) {
  ::setenv(name.c_str(), value.c_str(), 1);
}

// Checks whether the NVIDIA GPU driver is used based on whether the kernel
// module is loaded.
static inline bool is_using_nvidia_driver() {
  struct ::stat buffer {};
  if (::stat("/sys/module/nvidia", &buffer) != 0) {
    return false;
  }
  return S_ISDIR(buffer.st_mode);
}

// Checks whether the windowing system is Wayland.
static inline bool is_wayland_display() {
  if (!get_env("WAYLAND_DISPLAY").empty()) {
    return true;
  }
  if (get_env("XDG_SESSION_TYPE") == "wayland") {
    return true;
  }
  if (get_env("DESKTOP_SESSION").find("wayland") != std::string::npos) {
    return true;
  }
  return false;
}

// Checks whether the GDK X11 backend is used.
// See: https://docs.gtk.org/gdk3/class.DisplayManager.html
static inline bool is_gdk_x11_backend() {
#ifdef GDK_WINDOWING_X11
  auto *gdk_display = gdk_display_get_default();
  return GDK_IS_X11_DISPLAY(gdk_display); // NOLINT(misc-const-correctness)
#else
  return false;
#endif
}

// Checks whether WebKit is affected by bug when using DMA-BUF renderer.
// Returns true if all of the following conditions are met:
//  - WebKit version is >= 2.42 (please narrow this down when there's a fix).
//  - Environment variables are empty or not set:
//    - WEBKIT_DISABLE_DMABUF_RENDERER
//  - Windowing system is not Wayland.
//  - GDK backend is X11.
//  - NVIDIA GPU driver is used.
static inline bool is_webkit_dmabuf_bugged() {
  auto wk_major = webkit_get_major_version();
  auto wk_minor = webkit_get_minor_version();
  // TODO: Narrow down affected WebKit version when there's a fixed version
  auto is_affected_wk_version = wk_major == 2 && wk_minor >= 42;
  if (!is_affected_wk_version) {
    return false;
  }
  if (!get_env("WEBKIT_DISABLE_DMABUF_RENDERER").empty()) {
    return false;
  }
  if (is_wayland_display()) {
    return false;
  }
  if (!is_gdk_x11_backend()) {
    return false;
  }
  if (!is_using_nvidia_driver()) {
    return false;
  }
  return true;
}

// Applies workaround for WebKit DMA-BUF bug if needed.
// See WebKit bug: https://bugs.webkit.org/show_bug.cgi?id=261874
static inline void apply_webkit_dmabuf_workaround() {
  if (!is_webkit_dmabuf_bugged()) {
    return;
  }
  set_env("WEBKIT_DISABLE_DMABUF_RENDERER", "1");
}

} // namespace webkit_dmabuf
} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_LINUX) && defined(WEBVIEW_GTK)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_BACKENDS_GTK_WEBKITGTK_DMABUF_HH
// file end: include/webview/detail/platform/linux/webkitgtk/dmabuf.hh
// file begin: include/webview/detail/platform/windows/iid.hh
#ifndef WEBVIEW_PLATFORM_WINDOWS_IID_HH
#define WEBVIEW_PLATFORM_WINDOWS_IID_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_WINDOWS)

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>

#include <objbase.h>

#ifdef _MSC_VER
#pragma comment(lib, "ole32.lib")
#endif

namespace webview {
namespace detail {

template <typename T> struct cast_info_t {
  using type = T;
  IID iid;
};

// Checks whether the specified IID equals the IID of the specified type and
// if so casts the "this" pointer to T and returns it. Returns nullptr on
// mismatching IIDs.
// If ppv is specified then the pointer will also be assigned to *ppv.
template <typename From, typename To>
To *cast_if_equal_iid(From *from, REFIID riid, const cast_info_t<To> &info,
                      LPVOID *ppv = nullptr) noexcept {
  To *ptr = nullptr;
  if (IsEqualIID(riid, info.iid)) {
    ptr = static_cast<To *>(from);
    ptr->AddRef();
  }
  if (ppv) {
    *ppv = ptr;
  }
  return ptr;
}

} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_WINDOWS)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_WINDOWS_IID_HH
// file end: include/webview/detail/platform/windows/iid.hh
// file begin: include/webview/detail/platform/windows/reg_key.hh
#ifndef WEBVIEW_PLATFORM_WINDOWS_REG_KEY_HH
#define WEBVIEW_PLATFORM_WINDOWS_REG_KEY_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_WINDOWS)

#include <string>
#include <vector>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>

#ifdef _MSC_VER
#pragma comment(lib, "advapi32.lib")
#endif

namespace webview {
namespace detail {

class reg_key {
public:
  explicit reg_key(HKEY root_key, const wchar_t *sub_key, DWORD options,
                   REGSAM sam_desired) {
    HKEY handle;
    auto status =
        RegOpenKeyExW(root_key, sub_key, options, sam_desired, &handle);
    if (status == ERROR_SUCCESS) {
      m_handle = handle;
    }
  }

  explicit reg_key(HKEY root_key, const std::wstring &sub_key, DWORD options,
                   REGSAM sam_desired)
      : reg_key(root_key, sub_key.c_str(), options, sam_desired) {}

  virtual ~reg_key() {
    if (m_handle) {
      RegCloseKey(m_handle);
      m_handle = nullptr;
    }
  }

  reg_key(const reg_key &other) = delete;
  reg_key &operator=(const reg_key &other) = delete;
  reg_key(reg_key &&other) = delete;
  reg_key &operator=(reg_key &&other) = delete;

  bool is_open() const { return !!m_handle; }
  bool get_handle() const { return m_handle; }

  template <typename Container>
  void query_bytes(const wchar_t *name, Container &result) const {
    DWORD buf_length = 0;
    // Get the size of the data in bytes.
    auto status = RegQueryValueExW(m_handle, name, nullptr, nullptr, nullptr,
                                   &buf_length);
    if (status != ERROR_SUCCESS && status != ERROR_MORE_DATA) {
      result.resize(0);
      return;
    }
    // Read the data.
    result.resize(buf_length / sizeof(typename Container::value_type));
    auto *buf = reinterpret_cast<LPBYTE>(&result[0]);
    status =
        RegQueryValueExW(m_handle, name, nullptr, nullptr, buf, &buf_length);
    if (status != ERROR_SUCCESS) {
      result.resize(0);
      return;
    }
  }

  std::wstring query_string(const wchar_t *name) const {
    std::wstring result;
    query_bytes(name, result);
    // Remove trailing null-characters.
    for (std::size_t length = result.size(); length > 0; --length) {
      if (result[length - 1] != 0) {
        result.resize(length);
        break;
      }
    }
    return result;
  }

  unsigned int query_uint(const wchar_t *name,
                          unsigned int default_value) const {
    std::vector<char> data;
    query_bytes(name, data);
    if (data.size() < sizeof(DWORD)) {
      return default_value;
    }
    return static_cast<unsigned int>(*reinterpret_cast<DWORD *>(data.data()));
  }

private:
  HKEY m_handle = nullptr;
};

} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_WINDOWS)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_WINDOWS_REG_KEY_HH
// file end: include/webview/detail/platform/windows/reg_key.hh
// file begin: include/webview/version.h
#ifndef WEBVIEW_VERSION_H
#define WEBVIEW_VERSION_H

/// @{

#ifndef WEBVIEW_VERSION_MAJOR
/// The current library major version.
#define WEBVIEW_VERSION_MAJOR 0
#endif

#ifndef WEBVIEW_VERSION_MINOR
/// The current library minor version.
#define WEBVIEW_VERSION_MINOR 12
#endif

#ifndef WEBVIEW_VERSION_PATCH
/// The current library patch version.
#define WEBVIEW_VERSION_PATCH 0
#endif

#ifndef WEBVIEW_VERSION_PRE_RELEASE
/// SemVer 2.0.0 pre-release labels prefixed with "-".
#define WEBVIEW_VERSION_PRE_RELEASE ""
#endif

#ifndef WEBVIEW_VERSION_BUILD_METADATA
/// SemVer 2.0.0 build metadata prefixed with "+".
#define WEBVIEW_VERSION_BUILD_METADATA ""
#endif

/// SemVer 2.0.0 version number in MAJOR.MINOR.PATCH format.
#define WEBVIEW_VERSION_NUMBER                                                 \
  WEBVIEW_EXPAND_AND_STRINGIFY(WEBVIEW_VERSION_MAJOR)                          \
  "." WEBVIEW_EXPAND_AND_STRINGIFY(                                            \
      WEBVIEW_VERSION_MINOR) "." WEBVIEW_EXPAND_AND_STRINGIFY(WEBVIEW_VERSION_PATCH)

/// @}

#endif // WEBVIEW_VERSION_H
// file end: include/webview/version.h
// file begin: include/webview/api.h
#ifndef WEBVIEW_API_H
#define WEBVIEW_API_H
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a new webview instance.
 *
 * @param debug Enable developer tools if supported by the backend.
 * @param window Optional native window handle, i.e. @c GtkWindow pointer
 *        @c NSWindow pointer (Cocoa) or @c HWND (Win32). If non-null,
 *        the webview widget is embedded into the given window, and the
 *        caller is expected to assume responsibility for the window as
 *        well as application lifecycle. If the window handle is null,
 *        a new window is created and both the window and application
 *        lifecycle are managed by the webview instance.
 * @remark Win32: The function also accepts a pointer to @c HWND (Win32) in the
 *         window parameter for backward compatibility.
 * @remark Win32/WebView2: @c CoInitializeEx should be called with
 *         @c COINIT_APARTMENTTHREADED before attempting to call this function
 *         with an existing window. Omitting this step may cause WebView2
 *         initialization to fail.
 * @return @c NULL on failure. Creation can fail for various reasons such
 *         as when required runtime dependencies are missing or when window
 *         creation fails.
 * @retval WEBVIEW_ERROR_MISSING_DEPENDENCY
 *         May be returned if WebView2 is unavailable on Windows.
 */
WEBVIEW_API webview_t webview_create(int debug, void *window);

/**
 * Destroys a webview instance and closes the native window.
 *
 * @param w The webview instance.
 */
WEBVIEW_API webview_error_t webview_destroy(webview_t w);

/**
 * Runs the main loop until it's terminated.
 *
 * @param w The webview instance.
 */
WEBVIEW_API webview_error_t webview_run(webview_t w);

/**
 * Stops the main loop. It is safe to call this function from another
 * background thread.
 *
 * @param w The webview instance.
 */
WEBVIEW_API webview_error_t webview_terminate(webview_t w);

/**
 * Schedules a function to be invoked on the thread with the run/event loop.
 *
 * Since library functions generally do not have thread safety guarantees,
 * this function can be used to schedule code to execute on the main/GUI
 * thread and thereby make that execution safe in multi-threaded applications.
 *
 * @param w The webview instance.
 * @param fn The function to be invoked.
 * @param arg An optional argument passed along to the callback function.
 */
WEBVIEW_API webview_error_t webview_dispatch(webview_t w,
                                             void (*fn)(webview_t w, void *arg),
                                             void *arg);

/**
 * Returns the native handle of the window associated with the webview instance.
 * The handle can be a @c GtkWindow pointer (GTK), @c NSWindow pointer (Cocoa)
 * or @c HWND (Win32).
 *
 * @param w The webview instance.
 * @return The handle of the native window.
 */
WEBVIEW_API void *webview_get_window(webview_t w);

/**
 * Get a native handle of choice.
 *
 * @param w The webview instance.
 * @param kind The kind of handle to retrieve.
 * @return The native handle or @c NULL.
 * @since 0.11
 */
WEBVIEW_API void *webview_get_native_handle(webview_t w,
                                            webview_native_handle_kind_t kind);

/**
 * Updates the title of the native window.
 *
 * @param w The webview instance.
 * @param title The new title.
 */
WEBVIEW_API webview_error_t webview_set_title(webview_t w, const char *title);

/**
 * Updates the size of the native window.
 *
 * Remarks:
 * - Subsequent calls to this function may behave inconsistently across
 *   different versions of GTK and windowing systems (X11/Wayland).
 * - Using WEBVIEW_HINT_MAX for setting the maximum window size is not
 *   supported with GTK 4 because X11-specific functions such as
 *   gtk_window_set_geometry_hints were removed. This option has no effect
 *   when using GTK 4.
 *
 * @param w The webview instance.
 * @param width New width.
 * @param height New height.
 * @param hints Size hints.
 */
WEBVIEW_API webview_error_t webview_set_size(webview_t w, int width, int height,
                                             webview_hint_t hints);

/**
 * Navigates webview to the given URL. URL may be a properly encoded data URI.
 *
 * Example:
 * @code{.c}
 * webview_navigate(w, "https://github.com/webview/webview");
 * webview_navigate(w, "data:text/html,%3Ch1%3EHello%3C%2Fh1%3E");
 * webview_navigate(w, "data:text/html;base64,PGgxPkhlbGxvPC9oMT4=");
 * @endcode
 *
 * @param w The webview instance.
 * @param url URL.
 */
WEBVIEW_API webview_error_t webview_navigate(webview_t w, const char *url);

/**
 * Load HTML content into the webview.
 *
 * Example:
 * @code{.c}
 * webview_set_html(w, "<h1>Hello</h1>");
 * @endcode
 *
 * @param w The webview instance.
 * @param html HTML content.
 */
WEBVIEW_API webview_error_t webview_set_html(webview_t w, const char *html);

/**
 * Injects JavaScript code to be executed immediately upon loading a page.
 * The code will be executed before @c window.onload.
 *
 * @param w The webview instance.
 * @param js JS content.
 */
WEBVIEW_API webview_error_t webview_init(webview_t w, const char *js);

/**
 * Evaluates arbitrary JavaScript code.
 *
 * Use bindings if you need to communicate the result of the evaluation.
 *
 * @param w The webview instance.
 * @param js JS content.
 */
WEBVIEW_API webview_error_t webview_eval(webview_t w, const char *js);

/**
 * Binds a function pointer to a new global JavaScript function.
 *
 * Internally, JS glue code is injected to create the JS function by the
 * given name. The callback function is passed a request identifier,
 * a request string and a user-provided argument. The request string is
 * a JSON array of the arguments passed to the JS function.
 *
 * @param w The webview instance.
 * @param name Name of the JS function.
 * @param fn Callback function.
 * @param arg User argument.
 * @retval WEBVIEW_ERROR_DUPLICATE
 *         A binding already exists with the specified name.
 */
WEBVIEW_API webview_error_t webview_bind(webview_t w, const char *name,
                                         void (*fn)(const char *id,
                                                    const char *req, void *arg),
                                         void *arg);

/**
 * Removes a binding created with webview_bind().
 *
 * @param w The webview instance.
 * @param name Name of the binding.
 * @retval WEBVIEW_ERROR_NOT_FOUND No binding exists with the specified name.
 */
WEBVIEW_API webview_error_t webview_unbind(webview_t w, const char *name);

/**
 * Responds to a binding call from the JS side.
 *
 * This function is safe to call from another thread.
 *
 * @param w The webview instance.
 * @param id The identifier of the binding call. Pass along the value received
 *           in the binding handler (see webview_bind()).
 * @param status A status of zero tells the JS side that the binding call was
 *               successful; any other value indicates an error.
 * @param result The result of the binding call to be returned to the JS side.
 *               This must either be a valid JSON value or an empty string for
 *               the primitive JS value @c undefined.
 */
WEBVIEW_API webview_error_t webview_return(webview_t w, const char *id,
                                           int status, const char *result);

/**
 * Get the library's version information.
 *
 * @since 0.10
 */
WEBVIEW_API const webview_version_info_t *webview_version(void);

#ifdef __cplusplus
}
#endif

#endif // WEBVIEW_API_H
// file end: include/webview/api.h
// file begin: include/webview/detail/native_library.hh
#ifndef WEBVIEW_DETAIL_NATIVE_LIBRARY_HH
#define WEBVIEW_DETAIL_NATIVE_LIBRARY_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#include <string>

#if defined(_WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace webview {
namespace detail {

// Holds a symbol name and associated type for code clarity.
template <typename T> class library_symbol {
public:
  using type = T;

  constexpr explicit library_symbol(const char *name) : m_name(name) {}
  constexpr const char *get_name() const { return m_name; }

private:
  const char *m_name;
};

// Loads a native shared library and allows one to get addresses for those
// symbols.
class native_library {
public:
  native_library() = default;

  explicit native_library(const std::string &name)
      : m_handle{load_library(name)} {}

#ifdef _WIN32
  explicit native_library(const std::wstring &name)
      : m_handle{load_library(name)} {}
#endif

  ~native_library() {
    if (m_handle) {
#ifdef _WIN32
      FreeLibrary(m_handle);
#else
      dlclose(m_handle);
#endif
      m_handle = nullptr;
    }
  }

  native_library(const native_library &other) = delete;
  native_library &operator=(const native_library &other) = delete;
  native_library(native_library &&other) noexcept { *this = std::move(other); }

  native_library &operator=(native_library &&other) noexcept {
    if (this == &other) {
      return *this;
    }
    m_handle = other.m_handle;
    other.m_handle = nullptr;
    return *this;
  }

  // Returns true if the library is currently loaded; otherwise false.
  operator bool() const { return is_loaded(); }

  // Get the address for the specified symbol or nullptr if not found.
  template <typename Symbol>
  typename Symbol::type get(const Symbol &symbol) const {
    if (is_loaded()) {
      // NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
#ifdef _WIN32
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-function-type"
#endif
      return reinterpret_cast<typename Symbol::type>(
          GetProcAddress(m_handle, symbol.get_name()));
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
#else
      return reinterpret_cast<typename Symbol::type>(
          dlsym(m_handle, symbol.get_name()));
#endif
      // NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)
    }
    return nullptr;
  }

  // Returns true if the library is currently loaded; otherwise false.
  bool is_loaded() const { return !!m_handle; }

  void detach() { m_handle = nullptr; }

  // Returns true if the library by the given name is currently loaded;
  // otherwise false.
  static inline bool is_loaded(const std::string &name) {
#ifdef _WIN32
    auto handle = GetModuleHandleW(widen_string(name).c_str());
#else
    auto handle = dlopen(name.c_str(), RTLD_NOW | RTLD_NOLOAD);
    if (handle) {
      dlclose(handle);
    }
#endif
    return !!handle;
  }

private:
#ifdef _WIN32
  using mod_handle_t = HMODULE;
#else
  using mod_handle_t = void *;
#endif

  static inline mod_handle_t load_library(const std::string &name) {
#ifdef _WIN32
    return load_library(widen_string(name));
#else
    return dlopen(name.c_str(), RTLD_NOW);
#endif
  }

#ifdef _WIN32
  static inline mod_handle_t load_library(const std::wstring &name) {
    return LoadLibraryW(name.c_str());
  }
#endif

  mod_handle_t m_handle{};
};

} // namespace detail
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_DETAIL_NATIVE_LIBRARY_HH
// file end: include/webview/detail/native_library.hh
// file begin: include/webview/detail/optional.hh
#ifndef WEBVIEW_DETAIL_OPTIONAL_HH
#define WEBVIEW_DETAIL_OPTIONAL_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#include <new>
#include <type_traits>
#include <utility>

namespace webview {
namespace detail {

template <typename T> class optional {
public:
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init, hicpp-member-init)
  optional() = default;

  optional(const T &other) noexcept : m_has_data{true} {
    new (&m_data) T{other};
  }

  optional(T &&other) noexcept : m_has_data{true} {
    new (&m_data) T{std::move(other)};
  }

  optional(const optional<T> &other) noexcept { *this = other; }

  optional &operator=(const optional<T> &other) noexcept {
    if (this == &other) {
      return *this;
    }
    m_has_data = other.has_value();
    if (m_has_data) {
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      new (&m_data) T{*reinterpret_cast<const T *>(&other.m_data)};
    }
    return *this;
  }

  optional(optional<T> &&other) noexcept { *this = std::move(other); }

  optional &operator=(optional<T> &&other) noexcept {
    if (this == &other) {
      return *this;
    }
    m_has_data = other.has_value();
    if (m_has_data) {
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      new (&m_data) T{std::move(*reinterpret_cast<T *>(&other.m_data))};
    }
    return *this;
  }

  ~optional() {
    if (m_has_data) {
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      reinterpret_cast<T *>(&m_data)->~T();
    }
  }

  const T &get() const {
    if (!m_has_data) {
      throw bad_access{};
    }
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    return *reinterpret_cast<const T *>(&m_data);
  }

  T &get() {
    if (!m_has_data) {
      throw bad_access{};
    }
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    return *reinterpret_cast<T *>(&m_data);
  }

  bool has_value() const { return m_has_data; }

private:
  // NOLINTNEXTLINE(bugprone-sizeof-expression): pointer to aggregate is OK
  typename std::aligned_storage<sizeof(T), alignof(T)>::type m_data;
  bool m_has_data{};
};

template <> class optional<void> {};

} // namespace detail
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_DETAIL_OPTIONAL_HH
// file end: include/webview/detail/optional.hh
// file begin: include/webview/json_deprecated.hh
#ifndef WEBVIEW_JSON_HH
#define WEBVIEW_JSON_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
namespace webview {

WEBVIEW_DEPRECATED_PRIVATE
inline int json_parse_c(const char *s, size_t sz, const char *key, size_t keysz,
                        const char **value, size_t *valuesz) {
  return detail::json_parse_c(s, sz, key, keysz, value, valuesz);
}

WEBVIEW_DEPRECATED_PRIVATE
inline std::string json_escape(const std::string &s) {
  return detail::json_escape(s);
}

WEBVIEW_DEPRECATED_PRIVATE
inline int json_unescape(const char *s, size_t n, char *out) {
  return detail::json_unescape(s, n, out);
}

WEBVIEW_DEPRECATED_PRIVATE
inline std::string json_parse(const std::string &s, const std::string &key,
                              const int index) {
  return detail::json_parse(s, key, index);
}

} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_JSON_HH
// file end: include/webview/json_deprecated.hh
// file begin: include/webview/detail/platform/windows/com_init_wrapper.hh
#ifndef WEBVIEW_PLATFORM_WINDOWS_COM_INIT_WRAPPER_HH
#define WEBVIEW_PLATFORM_WINDOWS_COM_INIT_WRAPPER_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_WINDOWS)

//
// ====================================================================
//
// This implementation uses Win32 API to create a native window. It
// uses Edge/Chromium webview2 backend as a browser engine.
//
// ====================================================================
//
#include <utility>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>

#include <objbase.h>

#ifdef _MSC_VER
#pragma comment(lib, "ole32.lib")
#endif

namespace webview {
namespace detail {

/**
 * A wrapper around COM library initialization. Calls CoInitializeEx in the
 * constructor and CoUninitialize in the destructor.
 *
 * @exception exception Thrown if CoInitializeEx has already been called with a
 * different concurrency model.
 */
class com_init_wrapper {
public:
  com_init_wrapper() = default;

  com_init_wrapper(DWORD dwCoInit) {
    // We can safely continue as long as COM was either successfully
    // initialized or already initialized.
    // RPC_E_CHANGED_MODE means that CoInitializeEx was already called with
    // a different concurrency model.
    switch (CoInitializeEx(nullptr, dwCoInit)) {
    case S_OK:
    case S_FALSE:
      m_initialized = true;
      break;
    case RPC_E_CHANGED_MODE:
      throw exception{
          WEBVIEW_ERROR_INVALID_STATE,
          "CoInitializeEx already called with a different concurrency model"};
    default:
      throw exception{WEBVIEW_ERROR_UNSPECIFIED,
                      "Unexpected result from CoInitializeEx"};
    }
  }

  ~com_init_wrapper() {
    if (m_initialized) {
      CoUninitialize();
      m_initialized = false;
    }
  }

  com_init_wrapper(const com_init_wrapper &other) = delete;
  com_init_wrapper &operator=(const com_init_wrapper &other) = delete;

  com_init_wrapper(com_init_wrapper &&other) noexcept {
    *this = std::move(other);
  }

  com_init_wrapper &operator=(com_init_wrapper &&other) noexcept {
    if (this == &other) {
      return *this;
    }
    m_initialized = other.m_initialized;
    other.m_initialized = false;
    return *this;
  }

private:
  bool m_initialized = false;
};

} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_WINDOWS)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_WINDOWS_COM_INIT_WRAPPER_HH
// file end: include/webview/detail/platform/windows/com_init_wrapper.hh
// file begin: include/webview/detail/platform/darwin/objc/Class.hh
#ifndef WEBVIEW_PLATFORM_DARWIN_OBJC_CLASS_HH
#define WEBVIEW_PLATFORM_DARWIN_OBJC_CLASS_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_DARWIN)
#include <objc/objc-runtime.h>

namespace webview {
namespace detail {
namespace objc {

inline id Class_new(Class class_) {
  return msg_send<id>(class_, selector("new"));
}

inline Class get_class(const char *name) { return objc_getClass(name); }

} // namespace objc
} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_DARWIN_OBJC_CLASS_HH
// file end: include/webview/detail/platform/darwin/objc/Class.hh
// file begin: include/webview/detail/platform/darwin/objc/autoreleasepool.hh
#ifndef WEBVIEW_PLATFORM_DARWIN_OBJC_AUTORELEASEPOOL_HH
#define WEBVIEW_PLATFORM_DARWIN_OBJC_AUTORELEASEPOOL_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_DARWIN)
#include <objc/objc-runtime.h>

namespace webview {
namespace detail {
namespace objc {

// Wrapper around NSAutoreleasePool that drains the pool on destruction.
class autoreleasepool {
public:
  autoreleasepool()
      : m_pool(msg_send<id>(get_class("NSAutoreleasePool"), selector("new"))) {}

  ~autoreleasepool() {
    if (m_pool) {
      msg_send<void>(m_pool, selector("drain"));
    }
  }

  autoreleasepool(const autoreleasepool &) = delete;
  autoreleasepool &operator=(const autoreleasepool &) = delete;
  autoreleasepool(autoreleasepool &&) = delete;
  autoreleasepool &operator=(autoreleasepool &&) = delete;

private:
  id m_pool{};
};

} // namespace objc
} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_DARWIN)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_DARWIN_OBJC_AUTORELEASEPOOL_HH
// file end: include/webview/detail/platform/darwin/objc/autoreleasepool.hh
// file begin: include/webview/detail/platform/darwin/objc/memory.hh
#ifndef WEBVIEW_PLATFORM_DARWIN_OBJC_MEMORY_HH
#define WEBVIEW_PLATFORM_DARWIN_OBJC_MEMORY_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_DARWIN)
#include <objc/objc-runtime.h>

namespace webview {
namespace detail {
namespace objc {

inline id autorelease(id object) {
  return msg_send<id>(object, selector("autorelease"));
}

inline id retain(id object) { return msg_send<id>(object, selector("retain")); }

inline void release(id object) { msg_send<void>(object, selector("release")); }

} // namespace objc
} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_DARWIN)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_DARWIN_OBJC_MEMORY_HH
// file end: include/webview/detail/platform/darwin/objc/memory.hh
// file begin: include/webview/detail/platform/windows/dwmapi.hh
#ifndef WEBVIEW_PLATFORM_WINDOWS_DWMAPI_HH
#define WEBVIEW_PLATFORM_WINDOWS_DWMAPI_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_WINDOWS)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>

namespace webview {
namespace detail {
namespace dwmapi_symbols {

typedef enum {
  // This undocumented value is used instead of DWMWA_USE_IMMERSIVE_DARK_MODE
  // on Windows 10 older than build 19041 (2004/20H1).
  DWMWA_USE_IMMERSIVE_DARK_MODE_BEFORE_V10_0_19041 = 19,
  // Documented as being supported since Windows 11 build 22000 (21H2) but it
  // works since Windows 10 build 19041 (2004/20H1).
  DWMWA_USE_IMMERSIVE_DARK_MODE = 20
} DWMWINDOWATTRIBUTE;

using DwmSetWindowAttribute_t = HRESULT(WINAPI *)(HWND, DWORD, LPCVOID, DWORD);

constexpr auto DwmSetWindowAttribute =
    library_symbol<DwmSetWindowAttribute_t>("DwmSetWindowAttribute");

} // namespace dwmapi_symbols
} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_WINDOWS)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_WINDOWS_DWMAPI_HH
// file end: include/webview/detail/platform/windows/dwmapi.hh
// file begin: include/webview/detail/platform/windows/ntdll.hh
#ifndef WEBVIEW_PLATFORM_WINDOWS_NTDLL_HH
#define WEBVIEW_PLATFORM_WINDOWS_NTDLL_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_WINDOWS)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>

namespace webview {
namespace detail {
namespace ntdll_symbols {

using RtlGetVersion_t =
    unsigned int /*NTSTATUS*/ (WINAPI *)(RTL_OSVERSIONINFOW *);

constexpr auto RtlGetVersion = library_symbol<RtlGetVersion_t>("RtlGetVersion");

} // namespace ntdll_symbols
} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_WINDOWS)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_WINDOWS_NTDLL_HH
// file end: include/webview/detail/platform/windows/ntdll.hh
// file begin: include/webview/detail/platform/windows/shcore.hh
#ifndef WEBVIEW_PLATFORM_WINDOWS_SHCORE_HH
#define WEBVIEW_PLATFORM_WINDOWS_SHCORE_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_WINDOWS)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>

namespace webview {
namespace detail {
namespace shcore_symbols {

typedef enum { PROCESS_PER_MONITOR_DPI_AWARE = 2 } PROCESS_DPI_AWARENESS;
using SetProcessDpiAwareness_t = HRESULT(WINAPI *)(PROCESS_DPI_AWARENESS);

constexpr auto SetProcessDpiAwareness =
    library_symbol<SetProcessDpiAwareness_t>("SetProcessDpiAwareness");

} // namespace shcore_symbols
} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_WINDOWS)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_WINDOWS_SHCORE_HH
// file end: include/webview/detail/platform/windows/shcore.hh
// file begin: include/webview/detail/platform/windows/user32.hh
#ifndef WEBVIEW_PLATFORM_WINDOWS_USER32_HH
#define WEBVIEW_PLATFORM_WINDOWS_USER32_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_WINDOWS)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>

namespace webview {
namespace detail {
namespace user32_symbols {

using DPI_AWARENESS_CONTEXT = HANDLE;
using SetProcessDpiAwarenessContext_t = BOOL(WINAPI *)(DPI_AWARENESS_CONTEXT);
using SetProcessDPIAware_t = BOOL(WINAPI *)();
using GetDpiForWindow_t = UINT(WINAPI *)(HWND);
using EnableNonClientDpiScaling_t = BOOL(WINAPI *)(HWND);
using AdjustWindowRectExForDpi_t = BOOL(WINAPI *)(LPRECT, DWORD, BOOL, DWORD,
                                                  UINT);
using GetWindowDpiAwarenessContext_t = DPI_AWARENESS_CONTEXT(WINAPI *)(HWND);
using AreDpiAwarenessContextsEqual_t = BOOL(WINAPI *)(DPI_AWARENESS_CONTEXT,
                                                      DPI_AWARENESS_CONTEXT);

// Use intptr_t as the underlying type because we need to
// reinterpret_cast<DPI_AWARENESS_CONTEXT> which is a pointer.
// Available since Windows 10, version 1607
enum class dpi_awareness : intptr_t {
  per_monitor_v2_aware = -4, // Available since Windows 10, version 1703
  per_monitor_aware = -3
};

constexpr auto SetProcessDpiAwarenessContext =
    library_symbol<SetProcessDpiAwarenessContext_t>(
        "SetProcessDpiAwarenessContext");
constexpr auto SetProcessDPIAware =
    library_symbol<SetProcessDPIAware_t>("SetProcessDPIAware");
constexpr auto GetDpiForWindow =
    library_symbol<GetDpiForWindow_t>("GetDpiForWindow");
constexpr auto EnableNonClientDpiScaling =
    library_symbol<EnableNonClientDpiScaling_t>("EnableNonClientDpiScaling");
constexpr auto AdjustWindowRectExForDpi =
    library_symbol<AdjustWindowRectExForDpi_t>("AdjustWindowRectExForDpi");
constexpr auto GetWindowDpiAwarenessContext =
    library_symbol<GetWindowDpiAwarenessContext_t>(
        "GetWindowDpiAwarenessContext");
constexpr auto AreDpiAwarenessContextsEqual =
    library_symbol<AreDpiAwarenessContextsEqual_t>(
        "AreDpiAwarenessContextsEqual");

} // namespace user32_symbols
} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_WINDOWS)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_WINDOWS_USER32_HH
// file end: include/webview/detail/platform/windows/user32.hh
// file begin: include/webview/detail/basic_result.hh
#ifndef WEBVIEW_DETAIL_BASIC_RESULT_HH
#define WEBVIEW_DETAIL_BASIC_RESULT_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#include <utility>

namespace webview {
namespace detail {

template <typename Value, typename Error, typename Exception>
class basic_result {
public:
  using value_type = Value;
  using error_type = Error;
  using exception_type = Exception;

  basic_result() : basic_result(value_type{}) {}

  basic_result(const value_type &value) : m_value{value} {}
  basic_result(value_type &&value) : m_value{std::forward<value_type>(value)} {}

  basic_result(const error_type &error) : m_error{error} {}
  basic_result(error_type &&error) : m_error{std::forward<error_type>(error)} {}

  bool ok() const { return has_value() && !has_error(); }
  bool has_value() const { return m_value.has_value(); }
  bool has_error() const { return m_error.has_value(); }

  void ensure_ok() {
    if (!ok()) {
      throw exception_type{error()};
    }
  }

  const value_type &value() const {
    if (!has_value()) {
      throw bad_access{};
    }
    return m_value.get();
  }

  const error_type &error() const {
    if (!has_error()) {
      throw bad_access{};
    }
    return m_error.get();
  }

private:
  optional<value_type> m_value;
  optional<error_type> m_error;
};

template <typename Error, typename Exception>
class basic_result<void, Error, Exception> {
public:
  using value_type = void;
  using error_type = Error;
  using exception_type = Exception;

  basic_result() = default;

  basic_result(error_type &&error) : m_error{std::forward<error_type>(error)} {}

  bool ok() const { return !has_error(); }

  bool has_error() const { return m_error.has_value(); }

  void ensure_ok() {
    if (!ok()) {
      throw exception_type{error()};
    }
  }

  const error_type &error() const {
    if (!has_error()) {
      throw bad_access{};
    }
    return m_error.get();
  }

private:
  optional<error_type> m_error;
};

} // namespace detail
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_DETAIL_BASIC_RESULT_HH
// file end: include/webview/detail/basic_result.hh
// file begin: include/webview/detail/platform/darwin/objc/objc.hh
#ifndef WEBVIEW_PLATFORM_DARWIN_OBJC_OBJC_HH
#define WEBVIEW_PLATFORM_DARWIN_OBJC_OBJC_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_DARWIN)

// IWYU pragma: begin_exports

#include <objc/NSObjCRuntime.h>
#include <objc/objc-runtime.h>

#endif // defined(WEBVIEW_PLATFORM_DARWIN)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_DARWIN_OBJC_OBJC_HH
// file end: include/webview/detail/platform/darwin/objc/objc.hh
// file begin: include/webview/detail/platform/windows/theme.hh
#ifndef WEBVIEW_PLATFORM_WINDOWS_THEME_HH
#define WEBVIEW_PLATFORM_WINDOWS_THEME_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_WINDOWS)
namespace webview {
namespace detail {

inline bool is_dark_theme_enabled() {
  constexpr auto *sub_key =
      L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize";
  reg_key key(HKEY_CURRENT_USER, sub_key, 0, KEY_READ);
  if (!key.is_open()) {
    // Default is light theme
    return false;
  }
  return key.query_uint(L"AppsUseLightTheme", 1) == 0;
}

inline void apply_window_theme(HWND window) {
  auto dark_theme_enabled = is_dark_theme_enabled();

  // Use "immersive dark mode" on systems that support it.
  // Changes the color of the window's title bar (light or dark).
  BOOL use_dark_mode{dark_theme_enabled ? TRUE : FALSE};
  static native_library dwmapi{L"dwmapi.dll"};
  if (auto fn = dwmapi.get(dwmapi_symbols::DwmSetWindowAttribute)) {
    // Try the modern, documented attribute before the older, undocumented one.
    if (fn(window, dwmapi_symbols::DWMWA_USE_IMMERSIVE_DARK_MODE,
           &use_dark_mode, sizeof(use_dark_mode)) != S_OK) {
      fn(window,
         dwmapi_symbols::DWMWA_USE_IMMERSIVE_DARK_MODE_BEFORE_V10_0_19041,
         &use_dark_mode, sizeof(use_dark_mode));
    }
  }
}

} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_WINDOWS)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_WINDOWS_THEME_HH
// file end: include/webview/detail/platform/windows/theme.hh
// file begin: include/webview/detail/platform/windows/version.hh
#ifndef WEBVIEW_PLATFORM_WINDOWS_VERSION_HH
#define WEBVIEW_PLATFORM_WINDOWS_VERSION_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_WINDOWS)
#include <array>
#include <string>
#include <vector>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>

#ifdef _MSC_VER
#pragma comment(lib, "version.lib")
#endif

namespace webview {
namespace detail {

// Parses a version string with 1-4 integral components, e.g. "1.2.3.4".
// Missing or invalid components default to 0, and excess components are
// ignored.
template <typename T>
std::array<unsigned int, 4>
parse_version(const std::basic_string<T> &version) noexcept {
  using iterator = typename std::basic_string<T>::const_iterator;
  auto parse_component = [](iterator sb, iterator se) -> unsigned int {
    try {
      auto n = std::stol(std::basic_string<T>(sb, se));
      return n < 0 ? 0 : n;
    } catch (std::exception &) {
      return 0;
    }
  };
  auto end = version.end();
  auto sb = version.begin(); // subrange begin
  auto se = sb;              // subrange end
  unsigned int ci = 0;       // component index
  std::array<unsigned int, 4> components{};
  while (sb != end && se != end && ci < components.size()) {
    if (*se == static_cast<T>('.')) {
      components[ci++] = parse_component(sb, se);
      sb = ++se;
      continue;
    }
    ++se;
  }
  if (sb < se && ci < components.size()) {
    components[ci] = parse_component(sb, se);
  }
  return components;
}

template <typename T, std::size_t Length>
std::array<unsigned int, 4> parse_version(const T (&version)[Length]) noexcept {
  return parse_version(std::basic_string<T>(version, Length));
}

inline std::wstring
get_file_version_string(const std::wstring &file_path) noexcept {
  DWORD dummy_handle; // Unused
  DWORD info_buffer_length =
      GetFileVersionInfoSizeW(file_path.c_str(), &dummy_handle);
  if (info_buffer_length == 0) {
    return std::wstring();
  }
  std::vector<char> info_buffer;
  info_buffer.reserve(info_buffer_length);
  if (!GetFileVersionInfoW(file_path.c_str(), 0, info_buffer_length,
                           info_buffer.data())) {
    return std::wstring();
  }
  auto sub_block = L"\\StringFileInfo\\040904B0\\ProductVersion";
  LPWSTR version = nullptr;
  unsigned int version_length = 0;
  if (!VerQueryValueW(info_buffer.data(), sub_block,
                      reinterpret_cast<LPVOID *>(&version), &version_length)) {
    return std::wstring();
  }
  if (!version || version_length == 0) {
    return std::wstring();
  }
  return std::wstring(version, version_length);
}

// Compare the specified version against the OS version.
// Returns less than 0 if the OS version is less.
// Returns 0 if the versions are equal.
// Returns greater than 0 if the specified version is greater.
inline int compare_os_version(unsigned int major, unsigned int minor,
                              unsigned int build) {
  // Use RtlGetVersion both to bypass potential issues related to
  // VerifyVersionInfo and manifests, and because both GetVersion and
  // GetVersionEx are deprecated.
  auto ntdll = native_library(L"ntdll.dll");
  if (auto fn = ntdll.get(ntdll_symbols::RtlGetVersion)) {
    RTL_OSVERSIONINFOW vi{};
    vi.dwOSVersionInfoSize = sizeof(vi);
    if (fn(&vi) != 0) {
      return false;
    }
    if (vi.dwMajorVersion == major) {
      if (vi.dwMinorVersion == minor) {
        return static_cast<int>(vi.dwBuildNumber) - static_cast<int>(build);
      }
      return static_cast<int>(vi.dwMinorVersion) - static_cast<int>(minor);
    }
    return static_cast<int>(vi.dwMajorVersion) - static_cast<int>(major);
  }
  return false;
}

} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_WINDOWS)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_WINDOWS_VERSION_HH
// file end: include/webview/detail/platform/windows/version.hh
// file begin: include/webview/types.hh
#ifndef WEBVIEW_TYPES_HH
#define WEBVIEW_TYPES_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#include <functional>

namespace webview {

using dispatch_fn_t = std::function<void()>;

template <typename T>
using result = detail::basic_result<T, error_info, exception>;

using noresult = detail::basic_result<void, error_info, exception>;

} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_TYPES_HH
// file end: include/webview/types.hh
// file begin: include/webview/detail/platform/darwin/cocoa/NSBundle.hh
#ifndef WEBVIEW_PLATFORM_DARWIN_COCOA_NSBUNDLE_HH
#define WEBVIEW_PLATFORM_DARWIN_COCOA_NSBUNDLE_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
namespace webview {
namespace detail {
namespace cocoa {

inline id NSBundle_get_mainBundle() {
  return objc::msg_send<id>(objc::get_class("NSBundle"),
                            objc::selector("mainBundle"));
}

inline id NSBundle_get_bundlePath(id self) {
  return objc::msg_send<id>(self, objc::selector("bundlePath"));
}

} // namespace cocoa
} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_DARWIN_COCOA_NSBUNDLE_HH
// file end: include/webview/detail/platform/darwin/cocoa/NSBundle.hh
// file begin: include/webview/detail/platform/darwin/cocoa/NSEvent.hh
#ifndef WEBVIEW_PLATFORM_DARWIN_COCOA_NSEVENT_HH
#define WEBVIEW_PLATFORM_DARWIN_COCOA_NSEVENT_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
namespace webview {
namespace detail {
namespace cocoa {

enum NSEventType : NSUInteger {
  // For macOS 10.12+; replaces NSApplicationDefined (macOS 10.0–10.12)
  // with the same value
  NSEventTypeApplicationDefined = 15
};

enum NSEventModifierFlags : NSUInteger {};

inline id NSEvent_otherEventWithType(NSEventType type, NSPoint location,
                                     NSEventModifierFlags modifier_flags,
                                     NSTimeInterval timestamp,
                                     NSInteger window_number, id context,
                                     short subtype, NSInteger data1,
                                     NSInteger data2) {
  return objc::msg_send<id>(
      objc::get_class("NSEvent"),
      objc::selector("otherEventWithType:location:modifierFlags:timestamp:"
                     "windowNumber:context:subtype:data1:data2:"),
      type, location, modifier_flags, timestamp, window_number, context,
      subtype, data1, data2);
}

} // namespace cocoa
} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_DARWIN_COCOA_NSEVENT_HH
// file end: include/webview/detail/platform/darwin/cocoa/NSEvent.hh
// file begin: include/webview/detail/platform/darwin/cocoa/NSInvocation.hh
#ifndef WEBVIEW_PLATFORM_DARWIN_COCOA_NSINVOCATION_HH
#define WEBVIEW_PLATFORM_DARWIN_COCOA_NSINVOCATION_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
namespace webview {
namespace detail {
namespace cocoa {

inline id NSInvocation_invocationWithMethodSignature(id sig) {
  return objc::msg_send<id>(objc::get_class("NSInvocation"),
                            objc::selector("invocationWithMethodSignature:"),
                            sig);
}

inline void NSInvocation_set_target(id self, id target) {
  objc::msg_send<void>(self, objc::selector("setTarget:"), target);
}

inline void NSInvocation_setArgument(id self, void *location, NSInteger index) {
  objc::msg_send<void>(self, objc::selector("setArgument:atIndex:"), location,
                       index);
}

inline void NSInvocation_invoke(id self) {
  objc::msg_send<void>(self, objc::selector("invoke"));
}

} // namespace cocoa
} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_DARWIN_COCOA_NSInvocation_HH
// file end: include/webview/detail/platform/darwin/cocoa/NSInvocation.hh
// file begin: include/webview/detail/platform/darwin/cocoa/NSMethodSignature.hh
#ifndef WEBVIEW_PLATFORM_DARWIN_COCOA_NSMETHODSIGNATURE_HH
#define WEBVIEW_PLATFORM_DARWIN_COCOA_NSMETHODSIGNATURE_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
namespace webview {
namespace detail {
namespace cocoa {

inline id NSMethodSignature_signatureWithObjCTypes(const char *types) {
  return objc::msg_send<id>(objc::get_class("NSMethodSignature"),
                            objc::selector("signatureWithObjCTypes:"), types);
}

} // namespace cocoa
} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_DARWIN_COCOA_NSMETHODSIGNATURE_HH
// file end: include/webview/detail/platform/darwin/cocoa/NSMethodSignature.hh
// file begin: include/webview/detail/platform/darwin/cocoa/NSNotification.hh
#ifndef WEBVIEW_PLATFORM_DARWIN_COCOA_NSNOTIFICATION_HH
#define WEBVIEW_PLATFORM_DARWIN_COCOA_NSNOTIFICATION_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
namespace webview {
namespace detail {
namespace cocoa {

inline id NSNotification_get_object(id self) {
  return objc::msg_send<id>(self, objc::selector("object"));
}

} // namespace cocoa
} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_DARWIN_COCOA_NSNOTIFICATION_HH
// file end: include/webview/detail/platform/darwin/cocoa/NSNotification.hh
// file begin: include/webview/detail/platform/darwin/cocoa/NSNumber.hh
#ifndef WEBVIEW_PLATFORM_DARWIN_COCOA_NSNUMBER_HH
#define WEBVIEW_PLATFORM_DARWIN_COCOA_NSNUMBER_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
namespace webview {
namespace detail {
namespace cocoa {

inline id NSNumber_numberWithBool(bool value) {
  return objc::msg_send<id>(objc::get_class("NSNumber"),
                            objc::selector("numberWithBool:"),
                            static_cast<BOOL>(value));
}

} // namespace cocoa
} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_DARWIN_COCOA_NSNUMBER_HH
// file end: include/webview/detail/platform/darwin/cocoa/NSNumber.hh
// file begin: include/webview/detail/platform/darwin/cocoa/NSObject.hh
#ifndef WEBVIEW_PLATFORM_DARWIN_COCOA_NSOBJECT_HH
#define WEBVIEW_PLATFORM_DARWIN_COCOA_NSOBJECT_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
namespace webview {
namespace detail {
namespace cocoa {

inline void NSObject_setValue_forKey(id self, id value, id key) {
  objc::msg_send<void>(self, objc::selector("setValue:forKey:"), value, key);
}

} // namespace cocoa
} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_DARWIN_COCOA_NSOBJECT_HH
// file end: include/webview/detail/platform/darwin/cocoa/NSObject.hh
// file begin: include/webview/detail/platform/darwin/cocoa/NSOpenPanel.hh
#ifndef WEBVIEW_PLATFORM_DARWIN_COCOA_NSOPENPANEL_HH
#define WEBVIEW_PLATFORM_DARWIN_COCOA_NSOPENPANEL_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
namespace webview {
namespace detail {
namespace cocoa {

inline id NSOpenPanel_openPanel() {
  return objc::msg_send<id>(objc::get_class("NSOpenPanel"),
                            objc::selector("openPanel"));
}

inline void NSOpenPanel_set_canChooseFiles(id self, bool value) {
  objc::msg_send<void>(self, objc::selector("setCanChooseFiles:"),
                       static_cast<BOOL>(value));
}

inline void NSOpenPanel_set_canChooseDirectories(id self, bool value) {
  objc::msg_send<void>(self, objc::selector("setCanChooseDirectories:"),
                       static_cast<BOOL>(value));
}

inline void NSOpenPanel_set_allowsMultipleSelection(id self, bool value) {
  objc::msg_send<void>(self, objc::selector("setAllowsMultipleSelection:"),
                       static_cast<BOOL>(value));
}

inline id NSOpenPanel_get_URLs(id self) {
  return objc::msg_send<id>(self, objc::selector("URLs"));
}

} // namespace cocoa
} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_DARWIN_COCOA_NSOPENPANEL_HH
// file end: include/webview/detail/platform/darwin/cocoa/NSOpenPanel.hh
// file begin: include/webview/detail/platform/darwin/cocoa/NSString.hh
#ifndef WEBVIEW_PLATFORM_DARWIN_COCOA_NSSTRING_HH
#define WEBVIEW_PLATFORM_DARWIN_COCOA_NSSTRING_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
#include <string>

namespace webview {
namespace detail {
namespace cocoa {

enum NSStringEncoding : NSUInteger {
  NSASCIIStringEncoding = 1,
  NSUTF8StringEncoding = 4
};

inline bool NSString_hasSuffix(id self, id suffix) {
  return static_cast<bool>(
      objc::msg_send<BOOL>(self, objc::selector("hasSuffix:"), suffix));
}

inline id NSString_alloc() {
  return objc::msg_send<id>(objc::get_class("NSString"),
                            objc::selector("alloc"));
}

inline id NSString_initWithBytes(id self, const void *bytes, NSUInteger length,
                                 NSStringEncoding encoding) {
  return objc::msg_send<id>(self,
                            objc::selector("initWithBytes:length:encoding:"),
                            bytes, length, encoding);
}

inline id NSString_stringWithUTF8String(const char *utf8_string) {
  return objc::msg_send<id>(objc::get_class("NSString"),
                            objc::selector("stringWithUTF8String:"),
                            utf8_string);
}

inline id NSString_stringWithUTF8String(const std::string &utf8_string) {
  return objc::autorelease(NSString_initWithBytes(
      NSString_alloc(), utf8_string.data(),
      static_cast<NSUInteger>(utf8_string.size()), NSUTF8StringEncoding));
}

inline const char *NSString_get_UTF8String(id self) {
  return objc::msg_send<const char *>(self, objc::selector("UTF8String"));
}

} // namespace cocoa
} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_DARWIN_COCOA_NSSTRING_HH
// file end: include/webview/detail/platform/darwin/cocoa/NSString.hh
// file begin: include/webview/detail/platform/darwin/cocoa/NSURLRequest.hh
#ifndef WEBVIEW_PLATFORM_DARWIN_COCOA_NSURLREQUEST_HH
#define WEBVIEW_PLATFORM_DARWIN_COCOA_NSURLREQUEST_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
namespace webview {
namespace detail {
namespace cocoa {

inline id NSURLRequest_requestWithURL(id url) {
  return objc::msg_send<id>(objc::get_class("NSURLRequest"),
                            objc::selector("requestWithURL:"), url);
}

} // namespace cocoa
} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_DARWIN_COCOA_NSURLREQUEST_HH
// file end: include/webview/detail/platform/darwin/cocoa/NSURLRequest.hh
// file begin: include/webview/detail/platform/darwin/cocoa/NSValue.hh
#ifndef WEBVIEW_PLATFORM_DARWIN_COCOA_NSVALUE_HH
#define WEBVIEW_PLATFORM_DARWIN_COCOA_NSVALUE_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
namespace webview {
namespace detail {
namespace cocoa {

inline id NSValue_valueWithPointer(const void *pointer) {
  return objc::msg_send<id>(objc::get_class("NSValue"),
                            objc::selector("valueWithPointer:"), pointer);
}

inline void NSValue_getValue(id self, void *value, NSUInteger size) {
  objc::msg_send<void>(self, objc::selector("getValue:size:"), value, size);
}

} // namespace cocoa
} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_DARWIN_COCOA_NSVALUE_HH
// file end: include/webview/detail/platform/darwin/cocoa/NSValue.hh
// file begin: include/webview/detail/platform/darwin/cocoa/NSView.hh
#ifndef WEBVIEW_PLATFORM_DARWIN_COCOA_NSVIEW_HH
#define WEBVIEW_PLATFORM_DARWIN_COCOA_NSVIEW_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
namespace webview {
namespace detail {
namespace cocoa {

enum NSAutoresizingMaskOptions : NSUInteger {
  NSViewMinXMargin = 1,
  NSViewWidthSizable = 2,
  NSViewMaxXMargin = 4,
  NSViewMinYMargin = 8,
  NSViewHeightSizable = 16,
  NSViewMaxYMargin = 32
};

inline id NSView_alloc() {
  return objc::msg_send<id>(objc::get_class("NSView"), objc::selector("alloc"));
}

inline id NSView_initWithFrame(id self, NSRect frame_rect) {
  return objc::msg_send<id>(self, objc::selector("initWithFrame:"), frame_rect);
}

inline id NSView_withFrame(NSRect frame_rect) {
  return objc::autorelease(NSView_initWithFrame(NSView_alloc(), frame_rect));
}

inline void NSView_set_autoresizesSubviews(id self, bool resizes) {
  objc::msg_send<void>(self, objc::selector("setAutoresizesSubviews:"),
                       static_cast<BOOL>(resizes));
}

inline void NSView_addSubview(id self, id subview) {
  objc::msg_send<void>(self, objc::selector("addSubview:"), subview);
}

inline NSRect NSView_get_bounds(id self) {
  return objc::msg_send_stret<NSRect>(self, objc::selector("bounds"));
}

inline void NSView_set_frame(id self, NSRect frame) {
  objc::msg_send<void>(self, objc::selector("setFrame:"), frame);
}

inline void NSView_set_autoresizingMask(id self,
                                        NSAutoresizingMaskOptions mask) {
  objc::msg_send<void>(self, objc::selector("setAutoresizingMask:"), mask);
}

} // namespace cocoa
} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_DARWIN_COCOA_NSVIEW_HH
// file end: include/webview/detail/platform/darwin/cocoa/NSView.hh
// file begin: include/webview/detail/platform/darwin/cocoa/NSWindow.hh
#ifndef WEBVIEW_PLATFORM_DARWIN_COCOA_NSWINDOW_HH
#define WEBVIEW_PLATFORM_DARWIN_COCOA_NSWINDOW_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
#include <string>

namespace webview {
namespace detail {
namespace cocoa {

enum NSBackingStoreType : NSUInteger { NSBackingStoreBuffered = 2 };

enum NSWindowStyleMask : NSUInteger {
  NSWindowStyleMaskTitled = 1,
  NSWindowStyleMaskClosable = 2,
  NSWindowStyleMaskMiniaturizable = 4,
  NSWindowStyleMaskResizable = 8
};

inline id NSWindow_alloc() {
  return objc::msg_send<id>(objc::get_class("NSWindow"),
                            objc::selector("alloc"));
}

inline id NSWindow_initWithContentRect(id self, NSRect content_rect,
                                       NSWindowStyleMask style,
                                       NSBackingStoreType backing_store_type,
                                       bool defer) {
  return objc::msg_send<id>(
      self, objc::selector("initWithContentRect:styleMask:backing:defer:"),
      content_rect, style, backing_store_type, static_cast<BOOL>(defer));
}

inline id NSWindow_withContentRect(NSRect content_rect, NSWindowStyleMask style,
                                   NSBackingStoreType backing_store_type,
                                   bool defer) {
  return objc::autorelease(NSWindow_initWithContentRect(
      NSWindow_alloc(), content_rect, style, backing_store_type, defer));
}

inline void NSWindow_close(id self) {
  objc::msg_send<void>(self, objc::selector("close"));
}

inline NSRect NSWindow_get_frame(id self) {
  return objc::msg_send_stret<NSRect>(self, objc::selector("frame"));
}

inline void NSWindow_setFrame(id self, NSRect frame_rect, bool display,
                              bool animate) {
  objc::msg_send<void>(self, objc::selector("setFrame:display:animate:"),
                       frame_rect, static_cast<BOOL>(display),
                       static_cast<BOOL>(animate));
}

inline void NSWindow_set_styleMask(id self, NSWindowStyleMask style) {
  objc::msg_send<void>(self, objc::selector("setStyleMask:"), style);
}

inline void NSWindow_set_title(id self, const std::string &title) {
  objc::autoreleasepool arp;
  objc::msg_send<void>(
      self, objc::selector("setTitle:"),
      objc::msg_send<id>(objc::get_class("NSString"),
                         objc::selector("stringWithUTF8String:"),
                         title.c_str()));
}

inline void NSWindow_makeKeyAndOrderFront(id self, id sender = nullptr) {
  objc::msg_send<void>(self, objc::selector("makeKeyAndOrderFront:"), sender);
}

inline void NSWindow_set_contentView(id self, id view) {
  objc::msg_send<void>(self, objc::selector("setContentView:"), view);
}

inline id NSWindow_get_contentView(id self) {
  return objc::msg_send<id>(self, objc::selector("contentView"));
}

inline void NSWindow_set_delegate(id self, id delegate) {
  objc::msg_send<void>(self, objc::selector("setDelegate:"), delegate);
}

inline void NSWindow_center(id self) {
  objc::msg_send<void>(self, objc::selector("center"));
}

inline void NSWindow_set_contentMinSize(id self, NSSize size) {
  objc::msg_send<void>(self, objc::selector("setContentMinSize:"), size);
}

inline void NSWindow_set_contentMaxSize(id self, NSSize size) {
  objc::msg_send<void>(self, objc::selector("setContentMaxSize:"), size);
}

} // namespace cocoa
} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_DARWIN_COCOA_NSWINDOW_HH
// file end: include/webview/detail/platform/darwin/cocoa/NSWindow.hh
// file begin:
// include/webview/detail/platform/darwin/webkit/WKOpenPanelParameters.hh
#ifndef WEBVIEW_PLATFORM_DARWIN_WEBKIT_WKOPENPANELPARAMETERS_HH
#define WEBVIEW_PLATFORM_DARWIN_WEBKIT_WKOPENPANELPARAMETERS_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
namespace webview {
namespace detail {
namespace webkit {

inline bool WKOpenPanelParameters_get_allowsMultipleSelection(id self) {
  return static_cast<bool>(
      objc::msg_send<BOOL>(self, objc::selector("allowsMultipleSelection")));
}

inline bool WKOpenPanelParameters_get_allowsDirectories(id self) {
  return static_cast<bool>(
      objc::msg_send<BOOL>(self, objc::selector("allowsDirectories")));
}

} // namespace webkit
} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_DARWIN_WEBKIT_WKOPENPANELPARAMETERS_HH
// file end:
// include/webview/detail/platform/darwin/webkit/WKOpenPanelParameters.hh file
// begin: include/webview/detail/platform/darwin/webkit/WKScriptMessage.hh
#ifndef WEBVIEW_PLATFORM_DARWIN_WEBKIT_WKSCRIPTMESSAGE_HH
#define WEBVIEW_PLATFORM_DARWIN_WEBKIT_WKSCRIPTMESSAGE_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
namespace webview {
namespace detail {
namespace webkit {

inline id WKScriptMessage_get_body(id self) {
  return objc::msg_send<id>(self, objc::selector("body"));
}

} // namespace webkit
} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_DARWIN_WEBKIT_WKSCRIPTMESSAGE_HH
// file end: include/webview/detail/platform/darwin/webkit/WKScriptMessage.hh
// file begin:
// include/webview/detail/platform/darwin/webkit/WKUserContentController.hh
#ifndef WEBVIEW_PLATFORM_DARWIN_WEBKIT_WKUSERCONTENTCONTROLLER_HH
#define WEBVIEW_PLATFORM_DARWIN_WEBKIT_WKUSERCONTENTCONTROLLER_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
namespace webview {
namespace detail {
namespace webkit {

inline void WKUserContentController_addScriptMessageHandler(id self, id handler,
                                                            id name) {
  objc::msg_send<void>(self, objc::selector("addScriptMessageHandler:name:"),
                       handler, name);
}

inline void WKUserContentController_addUserScript(id self, id user_script) {
  objc::msg_send<void>(self, objc::selector("addUserScript:"), user_script);
}

inline void WKUserContentController_removeAllUserScripts(id self) {
  objc::msg_send<id>(self, objc::selector("removeAllUserScripts"));
}

} // namespace webkit
} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_DARWIN_WEBKIT_WKUSERCONTENTCONTROLLER_HH
// file end:
// include/webview/detail/platform/darwin/webkit/WKUserContentController.hh file
// begin: include/webview/detail/platform/darwin/webkit/WKUserScript.hh
#ifndef WEBVIEW_PLATFORM_DARWIN_WEBKIT_WKUSERSCRIPT_HH
#define WEBVIEW_PLATFORM_DARWIN_WEBKIT_WKUSERSCRIPT_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
namespace webview {
namespace detail {
namespace webkit {

enum WKUserScriptInjectionTime : NSInteger {
  WKUserScriptInjectionTimeAtDocumentStart = 0
};

inline id WKUserScript_alloc() {
  return objc::msg_send<id>(objc::get_class("WKUserScript"),
                            objc::selector("alloc"));
}

inline id WKUserScript_initWithSource(id self, id source,
                                      WKUserScriptInjectionTime injection_time,
                                      bool for_main_frame_only) {
  return objc::msg_send<id>(
      self, objc::selector("initWithSource:injectionTime:forMainFrameOnly:"),
      source, injection_time, static_cast<BOOL>(for_main_frame_only));
}

inline id WKUserScript_withSource(id source,
                                  WKUserScriptInjectionTime injection_time,
                                  bool for_main_frame_only) {
  return objc::autorelease(WKUserScript_initWithSource(
      WKUserScript_alloc(), source, injection_time, for_main_frame_only));
}

} // namespace webkit
} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_DARWIN_WEBKIT_WKUSERSCRIPT_HH
// file end: include/webview/detail/platform/darwin/webkit/WKUserScript.hh
// file begin: include/webview/detail/platform/darwin/webkit/WKWebView.hh
#ifndef WEBVIEW_PLATFORM_DARWIN_WEBKIT_WKWEBVIEW_HH
#define WEBVIEW_PLATFORM_DARWIN_WEBKIT_WKWEBVIEW_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
#include <CoreGraphics/CoreGraphics.h>

namespace webview {
namespace detail {
namespace webkit {

inline id WKWebView_alloc() {
  return objc::msg_send<id>(objc::get_class("WKWebView"),
                            objc::selector("alloc"));
}

inline id WKWebView_initWithFrame(id self, CGRect frame, id configuration) {
  return objc::msg_send<id>(self,
                            objc::selector("initWithFrame:configuration:"),
                            frame, configuration);
}

inline id WKWebView_withFrame(CGRect frame, id configuration) {
  return objc::autorelease(
      WKWebView_initWithFrame(WKWebView_alloc(), frame, configuration));
}

inline id WKWebView_get_UIDelegate(id self) {
  return objc::msg_send<id>(self, objc::selector("UIDelegate"));
}

inline void WKWebView_set_UIDelegate(id self, id ui_delegate) {
  objc::msg_send<void>(self, objc::selector("setUIDelegate:"), ui_delegate);
}

inline id WKWebView_loadHTMLString(id self, id string, id base_url) {
  return objc::msg_send<id>(self, objc::selector("loadHTMLString:baseURL:"),
                            string, base_url);
}

inline id WKWebView_get_URL(id self) {
  return objc::msg_send<id>(self, objc::selector("URL"));
}

inline id WKWebView_loadRequest(id self, id request) {
  return objc::msg_send<id>(self, objc::selector("loadRequest:"), request);
}

inline void WKWebView_evaluateJavaScript(id self, id js_string,
                                         const void *completion_handler) {
  return objc::msg_send<void>(
      self, objc::selector("evaluateJavaScript:completionHandler:"), js_string,
      completion_handler);
}

inline void WKWebView_set_inspectable(id self, bool inspectable) {
#if defined(__has_builtin)
#if __has_builtin(__builtin_available)
  if (__builtin_available(macOS 13.3, iOS 16.4, tvOS 16.4, *)) {
    objc::msg_send<void>(self, objc::selector("setInspectable:"),
                         static_cast<BOOL>(inspectable));
  }
#else
#error __builtin_available not supported by compiler
#endif
#else
#error __has_builtin not supported by compiler
#endif
}

} // namespace webkit
} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_DARWIN_WEBKIT_WKWEBVIEW_HH
// file end: include/webview/detail/platform/darwin/webkit/WKWebView.hh
// file begin:
// include/webview/detail/platform/darwin/webkit/WKWebViewConfiguration.hh
#ifndef WEBVIEW_PLATFORM_DARWIN_WEBKIT_WKWEBVIEWCONFIGURATION_HH
#define WEBVIEW_PLATFORM_DARWIN_WEBKIT_WKWEBVIEWCONFIGURATION_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
namespace webview {
namespace detail {
namespace webkit {

inline id WKWebViewConfiguration_new() {
  return objc::msg_send<id>(objc::get_class("WKWebViewConfiguration"),
                            objc::selector("new"));
}

inline id WKWebViewConfiguration_get_userContentController(id self) {
  return objc::msg_send<id>(self, objc::selector("userContentController"));
}

inline id WKWebViewConfiguration_get_preferences(id self) {
  return objc::msg_send<id>(self, objc::selector("preferences"));
}

} // namespace webkit
} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_DARWIN_WEBKIT_WKWEBVIEWCONFIGURATION_HH
// file end:
// include/webview/detail/platform/darwin/webkit/WKWebViewConfiguration.hh file
// begin: include/webview/detail/platform/windows/dpi.hh
#ifndef WEBVIEW_PLATFORM_WINDOWS_DPI_HH
#define WEBVIEW_PLATFORM_WINDOWS_DPI_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_WINDOWS)

//
// ====================================================================
//
// This implementation uses Win32 API to create a native window. It
// uses Edge/Chromium webview2 backend as a browser engine.
//
// ====================================================================
//
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>

#ifdef _MSC_VER
#pragma comment(lib, "user32.lib")
#endif

namespace webview {
namespace detail {

inline bool is_per_monitor_v2_awareness_available() {
  // Windows 10, version 1703
  return compare_os_version(10, 0, 15063) >= 0;
}

inline bool enable_dpi_awareness() {
  auto user32 = native_library(L"user32.dll");
  if (auto fn = user32.get(user32_symbols::SetProcessDpiAwarenessContext)) {
    auto dpi_awareness =
        reinterpret_cast<user32_symbols::DPI_AWARENESS_CONTEXT>(
            is_per_monitor_v2_awareness_available()
                ? user32_symbols::dpi_awareness::per_monitor_v2_aware
                : user32_symbols::dpi_awareness::per_monitor_aware);
    if (fn(dpi_awareness)) {
      return true;
    }
    return GetLastError() == ERROR_ACCESS_DENIED;
  }
  if (auto shcore = native_library(L"shcore.dll")) {
    if (auto fn = shcore.get(shcore_symbols::SetProcessDpiAwareness)) {
      auto result = fn(shcore_symbols::PROCESS_PER_MONITOR_DPI_AWARE);
      return result == S_OK || result == E_ACCESSDENIED;
    }
  }
  if (auto fn = user32.get(user32_symbols::SetProcessDPIAware)) {
    return !!fn();
  }
  return true;
}

inline bool enable_non_client_dpi_scaling_if_needed(HWND window) {
  auto user32 = native_library(L"user32.dll");
  auto get_ctx_fn = user32.get(user32_symbols::GetWindowDpiAwarenessContext);
  if (!get_ctx_fn) {
    return true;
  }
  auto awareness = get_ctx_fn(window);
  if (!awareness) {
    return false;
  }
  auto ctx_equal_fn = user32.get(user32_symbols::AreDpiAwarenessContextsEqual);
  if (!ctx_equal_fn) {
    return true;
  }
  // EnableNonClientDpiScaling is only needed with per monitor v1 awareness.
  auto per_monitor = reinterpret_cast<user32_symbols::DPI_AWARENESS_CONTEXT>(
      user32_symbols::dpi_awareness::per_monitor_aware);
  if (!ctx_equal_fn(awareness, per_monitor)) {
    return true;
  }
  auto enable_fn = user32.get(user32_symbols::EnableNonClientDpiScaling);
  if (!enable_fn) {
    return true;
  }
  return !!enable_fn(window);
}

constexpr int get_default_window_dpi() {
  return 96; // USER_DEFAULT_SCREEN_DPI
}

inline int get_window_dpi(HWND window) {
  auto user32 = native_library(L"user32.dll");
  if (auto fn = user32.get(user32_symbols::GetDpiForWindow)) {
    auto dpi = static_cast<int>(fn(window));
    return dpi;
  }
  return get_default_window_dpi();
}

constexpr int scale_value_for_dpi(int value, int from_dpi, int to_dpi) {
  return (value * to_dpi) / from_dpi;
}

constexpr SIZE scale_size(int width, int height, int from_dpi, int to_dpi) {
  return {scale_value_for_dpi(width, from_dpi, to_dpi),
          scale_value_for_dpi(height, from_dpi, to_dpi)};
}

inline SIZE make_window_frame_size(HWND window, int width, int height,
                                   int dpi) {
  auto style = GetWindowLong(window, GWL_STYLE);
  RECT r{0, 0, width, height};
  auto user32 = native_library(L"user32.dll");
  if (auto fn = user32.get(user32_symbols::AdjustWindowRectExForDpi)) {
    fn(&r, style, FALSE, 0, static_cast<UINT>(dpi));
  } else {
    AdjustWindowRect(&r, style, 0);
  }
  auto frame_width = r.right - r.left;
  auto frame_height = r.bottom - r.top;
  return {frame_width, frame_height};
}

} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_WINDOWS)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_WINDOWS_DPI_HH
// file end: include/webview/detail/platform/windows/dpi.hh
// file begin: include/webview/detail/platform/windows/webview2/loader.hh
#ifndef WEBVIEW_BACKENDS_WEBVIEW2_LOADER_HH
#define WEBVIEW_BACKENDS_WEBVIEW2_LOADER_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_WINDOWS) && defined(WEBVIEW_EDGE)
#include <string>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>

#include <objbase.h>

#include "WebView2.h" // amalgamate(skip)

#ifdef _MSC_VER
#pragma comment(lib, "ole32.lib")
#endif

namespace webview {
namespace detail {

// Enable built-in WebView2Loader implementation by default.
#ifndef WEBVIEW_MSWEBVIEW2_BUILTIN_IMPL
#define WEBVIEW_MSWEBVIEW2_BUILTIN_IMPL 1
#endif

// Link WebView2Loader.dll explicitly by default only if the built-in
// implementation is enabled.
#ifndef WEBVIEW_MSWEBVIEW2_EXPLICIT_LINK
#define WEBVIEW_MSWEBVIEW2_EXPLICIT_LINK WEBVIEW_MSWEBVIEW2_BUILTIN_IMPL
#endif

// Explicit linking of WebView2Loader.dll should be used along with
// the built-in implementation.
#if WEBVIEW_MSWEBVIEW2_BUILTIN_IMPL == 1 &&                                    \
    WEBVIEW_MSWEBVIEW2_EXPLICIT_LINK != 1
#undef WEBVIEW_MSWEBVIEW2_EXPLICIT_LINK
#error Please set WEBVIEW_MSWEBVIEW2_EXPLICIT_LINK=1.
#endif

#if WEBVIEW_MSWEBVIEW2_BUILTIN_IMPL == 1
// Gets the last component of a Windows native file path.
// For example, if the path is "C:\a\b" then the result is "b".
template <typename T>
std::basic_string<T>
get_last_native_path_component(const std::basic_string<T> &path) {
  auto pos = path.find_last_of(static_cast<T>('\\'));
  if (pos != std::basic_string<T>::npos) {
    return path.substr(pos + 1);
  }
  return std::basic_string<T>();
}
#endif // WEBVIEW_MSWEBVIEW2_BUILTIN_IMPL

namespace mswebview2 {
static constexpr IID
    IID_ICoreWebView2CreateCoreWebView2ControllerCompletedHandler{
        0x6C4819F3,
        0xC9B7,
        0x4260,
        {0x81, 0x27, 0xC9, 0xF5, 0xBD, 0xE7, 0xF6, 0x8C}};
static constexpr IID
    IID_ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler{
        0x4E8A3389,
        0xC9D8,
        0x4BD2,
        {0xB6, 0xB5, 0x12, 0x4F, 0xEE, 0x6C, 0xC1, 0x4D}};
static constexpr IID IID_ICoreWebView2PermissionRequestedEventHandler{
    0x15E1C6A3,
    0xC72A,
    0x4DF3,
    {0x91, 0xD7, 0xD0, 0x97, 0xFB, 0xEC, 0x6B, 0xFD}};
static constexpr IID IID_ICoreWebView2WebMessageReceivedEventHandler{
    0x57213F19,
    0x00E6,
    0x49FA,
    {0x8E, 0x07, 0x89, 0x8E, 0xA0, 0x1E, 0xCB, 0xD2}};
static constexpr IID
    IID_ICoreWebView2AddScriptToExecuteOnDocumentCreatedCompletedHandler{
        0xB99369F3,
        0x9B11,
        0x47B5,
        {0xBC, 0x6F, 0x8E, 0x78, 0x95, 0xFC, 0xEA, 0x17}};

#if WEBVIEW_MSWEBVIEW2_BUILTIN_IMPL == 1
enum class webview2_runtime_type { installed = 0, embedded = 1 };

namespace webview2_symbols {
using CreateWebViewEnvironmentWithOptionsInternal_t =
    HRESULT(STDMETHODCALLTYPE *)(
        bool, webview2_runtime_type, PCWSTR, IUnknown *,
        ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler *);
using DllCanUnloadNow_t = HRESULT(STDMETHODCALLTYPE *)();

static constexpr auto CreateWebViewEnvironmentWithOptionsInternal =
    library_symbol<CreateWebViewEnvironmentWithOptionsInternal_t>(
        "CreateWebViewEnvironmentWithOptionsInternal");
static constexpr auto DllCanUnloadNow =
    library_symbol<DllCanUnloadNow_t>("DllCanUnloadNow");
} // namespace webview2_symbols
#endif // WEBVIEW_MSWEBVIEW2_BUILTIN_IMPL

#if WEBVIEW_MSWEBVIEW2_EXPLICIT_LINK == 1
namespace webview2_symbols {
using CreateCoreWebView2EnvironmentWithOptions_t = HRESULT(STDMETHODCALLTYPE *)(
    PCWSTR, PCWSTR, ICoreWebView2EnvironmentOptions *,
    ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler *);
using GetAvailableCoreWebView2BrowserVersionString_t =
    HRESULT(STDMETHODCALLTYPE *)(PCWSTR, LPWSTR *);

static constexpr auto CreateCoreWebView2EnvironmentWithOptions =
    library_symbol<CreateCoreWebView2EnvironmentWithOptions_t>(
        "CreateCoreWebView2EnvironmentWithOptions");
static constexpr auto GetAvailableCoreWebView2BrowserVersionString =
    library_symbol<GetAvailableCoreWebView2BrowserVersionString_t>(
        "GetAvailableCoreWebView2BrowserVersionString");
} // namespace webview2_symbols
#endif // WEBVIEW_MSWEBVIEW2_EXPLICIT_LINK

class loader {
public:
  HRESULT create_environment_with_options(
      PCWSTR browser_dir, PCWSTR user_data_dir,
      ICoreWebView2EnvironmentOptions *env_options,
      ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler
          *created_handler) const {
#if WEBVIEW_MSWEBVIEW2_EXPLICIT_LINK == 1
    if (m_lib.is_loaded()) {
      if (auto fn = m_lib.get(
              webview2_symbols::CreateCoreWebView2EnvironmentWithOptions)) {
        return fn(browser_dir, user_data_dir, env_options, created_handler);
      }
    }
#if WEBVIEW_MSWEBVIEW2_BUILTIN_IMPL == 1
    return create_environment_with_options_impl(browser_dir, user_data_dir,
                                                env_options, created_handler);
#else
    return S_FALSE;
#endif
#else
    return ::CreateCoreWebView2EnvironmentWithOptions(
        browser_dir, user_data_dir, env_options, created_handler);
#endif // WEBVIEW_MSWEBVIEW2_EXPLICIT_LINK
  }

  HRESULT
  get_available_browser_version_string(PCWSTR browser_dir,
                                       LPWSTR *version) const {
#if WEBVIEW_MSWEBVIEW2_EXPLICIT_LINK == 1
    if (m_lib.is_loaded()) {
      if (auto fn = m_lib.get(
              webview2_symbols::GetAvailableCoreWebView2BrowserVersionString)) {
        return fn(browser_dir, version);
      }
    }
#if WEBVIEW_MSWEBVIEW2_BUILTIN_IMPL == 1
    return get_available_browser_version_string_impl(browser_dir, version);
#else
    return S_FALSE;
#endif
#else
    return ::GetAvailableCoreWebView2BrowserVersionString(browser_dir, version);
#endif // WEBVIEW_MSWEBVIEW2_EXPLICIT_LINK
  }

private:
#if WEBVIEW_MSWEBVIEW2_BUILTIN_IMPL == 1
  struct client_info_t {
    bool found{};
    std::wstring dll_path;
    std::wstring version;
    webview2_runtime_type runtime_type{};

    client_info_t() = default;

    client_info_t(bool found, std::wstring dll_path, std::wstring version,
                  webview2_runtime_type runtime_type)
        : found{found}, dll_path{std::move(dll_path)},
          version{std::move(version)}, runtime_type{runtime_type} {}
  };

  HRESULT create_environment_with_options_impl(
      PCWSTR browser_dir, PCWSTR user_data_dir,
      ICoreWebView2EnvironmentOptions *env_options,
      ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler
          *created_handler) const {
    auto found_client = find_available_client(browser_dir);
    if (!found_client.found) {
      return -1;
    }
    auto client_dll = native_library(found_client.dll_path);
    if (auto fn = client_dll.get(
            webview2_symbols::CreateWebViewEnvironmentWithOptionsInternal)) {
      return fn(true, found_client.runtime_type, user_data_dir, env_options,
                created_handler);
    }
    if (auto fn = client_dll.get(webview2_symbols::DllCanUnloadNow)) {
      if (!fn()) {
        client_dll.detach();
      }
    }
    return ERROR_SUCCESS;
  }

  HRESULT
  get_available_browser_version_string_impl(PCWSTR browser_dir,
                                            LPWSTR *version) const {
    if (!version) {
      return -1;
    }
    auto found_client = find_available_client(browser_dir);
    if (!found_client.found) {
      return -1;
    }
    auto info_length_bytes =
        found_client.version.size() * sizeof(found_client.version[0]);
    auto info = static_cast<LPWSTR>(CoTaskMemAlloc(info_length_bytes));
    if (!info) {
      return -1;
    }
    CopyMemory(info, found_client.version.c_str(), info_length_bytes);
    *version = info;
    return 0;
  }

  client_info_t find_available_client(PCWSTR browser_dir) const {
    if (browser_dir) {
      return find_embedded_client(api_version, browser_dir);
    }
    auto found_client =
        find_installed_client(api_version, true, default_release_channel_guid);
    if (!found_client.found) {
      found_client = find_installed_client(api_version, false,
                                           default_release_channel_guid);
    }
    return found_client;
  }

  std::wstring make_client_dll_path(const std::wstring &dir) const {
    auto dll_path = dir;
    if (!dll_path.empty()) {
      auto last_char = dir[dir.size() - 1];
      if (last_char != L'\\' && last_char != L'/') {
        dll_path += L'\\';
      }
    }
    dll_path += L"EBWebView\\";
#if defined(_M_X64) || defined(__x86_64__)
    dll_path += L"x64";
#elif defined(_M_IX86) || defined(__i386__)
    dll_path += L"x86";
#elif defined(_M_ARM64) || defined(__aarch64__)
    dll_path += L"arm64";
#else
#error WebView2 integration for this platform is not yet supported.
#endif
    dll_path += L"\\EmbeddedBrowserWebView.dll";
    return dll_path;
  }

  client_info_t
  find_installed_client(unsigned int min_api_version, bool system,
                        const std::wstring &release_channel) const {
    std::wstring sub_key = client_state_reg_sub_key;
    sub_key += release_channel;
    auto root_key = system ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER;
    reg_key key(root_key, sub_key, 0, KEY_READ | KEY_WOW64_32KEY);
    if (!key.is_open()) {
      return {};
    }
    auto ebwebview_value = key.query_string(L"EBWebView");

    auto client_version_string =
        get_last_native_path_component(ebwebview_value);
    auto client_version = parse_version(client_version_string);
    if (client_version[2] < min_api_version) {
      // Our API version is greater than the runtime API version.
      return {};
    }

    auto client_dll_path = make_client_dll_path(ebwebview_value);
    return {true, std::move(client_dll_path), std::move(client_version_string),
            webview2_runtime_type::installed};
  }

  client_info_t find_embedded_client(unsigned int min_api_version,
                                     const std::wstring &dir) const {
    auto client_dll_path = make_client_dll_path(dir);

    auto client_version_string = get_file_version_string(client_dll_path);
    auto client_version = parse_version(client_version_string);
    if (client_version[2] < min_api_version) {
      // Our API version is greater than the runtime API version.
      return {};
    }

    return {true, std::move(client_dll_path), std::move(client_version_string),
            webview2_runtime_type::embedded};
  }

  // The minimum WebView2 API version we need regardless of the SDK release
  // actually used. The number comes from the SDK release version,
  // e.g. 1.0.1150.38. To be safe the SDK should have a number that is greater
  // than or equal to this number. The Edge browser webview client must
  // have a number greater than or equal to this number.
  static constexpr unsigned int api_version = 1150;

  static constexpr auto client_state_reg_sub_key =
      L"SOFTWARE\\Microsoft\\EdgeUpdate\\ClientState\\";

  // GUID for the stable release channel.
  static constexpr auto stable_release_guid =
      L"{F3017226-FE2A-4295-8BDF-00C3A9A7E4C5}";

  static constexpr auto default_release_channel_guid = stable_release_guid;
#endif // WEBVIEW_MSWEBVIEW2_BUILTIN_IMPL

#if WEBVIEW_MSWEBVIEW2_EXPLICIT_LINK == 1
  native_library m_lib{L"WebView2Loader.dll"};
#endif
};

namespace cast_info {
static constexpr auto controller_completed =
    cast_info_t<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>{
        IID_ICoreWebView2CreateCoreWebView2ControllerCompletedHandler};

static constexpr auto environment_completed =
    cast_info_t<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>{
        IID_ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler};

static constexpr auto message_received =
    cast_info_t<ICoreWebView2WebMessageReceivedEventHandler>{
        IID_ICoreWebView2WebMessageReceivedEventHandler};

static constexpr auto permission_requested =
    cast_info_t<ICoreWebView2PermissionRequestedEventHandler>{
        IID_ICoreWebView2PermissionRequestedEventHandler};

static constexpr auto add_script_to_execute_on_document_created_completed =
    cast_info_t<
        ICoreWebView2AddScriptToExecuteOnDocumentCreatedCompletedHandler>{
        IID_ICoreWebView2AddScriptToExecuteOnDocumentCreatedCompletedHandler};
} // namespace cast_info

} // namespace mswebview2
} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_WINDOWS) && defined(WEBVIEW_EDGE)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_BACKENDS_WEBVIEW2_LOADER_HH
// file end: include/webview/detail/platform/windows/webview2/loader.hh
// file begin: include/webview/detail/engine_base.hh
#ifndef WEBVIEW_DETAIL_ENGINE_BASE_HH
#define WEBVIEW_DETAIL_ENGINE_BASE_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#include <atomic>
#include <functional>
#include <list>
#include <map>
#include <string>

namespace webview {
namespace detail {

class engine_base {
public:
  engine_base(bool owns_window) : m_owns_window{owns_window} {}

  virtual ~engine_base() = default;

  noresult navigate(const std::string &url) {
    if (url.empty()) {
      return navigate_impl("about:blank");
    }
    return navigate_impl(url);
  }

  using binding_t = std::function<void(std::string, std::string, void *)>;
  class binding_ctx_t {
  public:
    binding_ctx_t(binding_t callback, void *arg)
        : m_callback(callback), m_arg(arg) {}
    void call(std::string id, std::string args) const {
      if (m_callback) {
        m_callback(id, args, m_arg);
      }
    }

  private:
    // This function is called upon execution of the bound JS function
    binding_t m_callback;
    // This user-supplied argument is passed to the callback
    void *m_arg;
  };

  using sync_binding_t = std::function<std::string(std::string)>;

  // Synchronous bind
  noresult bind(const std::string &name, sync_binding_t fn) {
    auto wrapper = [this, fn](const std::string &id, const std::string &req,
                              void * /*arg*/) { resolve(id, 0, fn(req)); };
    return bind(name, wrapper, nullptr);
  }

  // Asynchronous bind
  noresult bind(const std::string &name, binding_t fn, void *arg) {
    // NOLINTNEXTLINE(readability-container-contains): contains() requires C++20
    if (bindings.count(name) > 0) {
      return error_info{WEBVIEW_ERROR_DUPLICATE};
    }
    bindings.emplace(name, binding_ctx_t(fn, arg));
    replace_bind_script();
    // Notify that a binding was created if the init script has already
    // set things up.
    eval("if (window.__webview__) {\n\
window.__webview__.onBind(" +
         json_escape(name) + ")\n\
}");
    return {};
  }

  noresult unbind(const std::string &name) {
    auto found = bindings.find(name);
    if (found == bindings.end()) {
      return error_info{WEBVIEW_ERROR_NOT_FOUND};
    }
    bindings.erase(found);
    replace_bind_script();
    // Notify that a binding was created if the init script has already
    // set things up.
    eval("if (window.__webview__) {\n\
window.__webview__.onUnbind(" +
         json_escape(name) + ")\n\
}");
    return {};
  }

  noresult resolve(const std::string &id, int status,
                   const std::string &result) {
    // NOLINTNEXTLINE(modernize-avoid-bind): Lambda with move requires C++14
    return dispatch(std::bind(
        [id, status, this](std::string escaped_result) {
          std::string js = "window.__webview__.onReply(" + json_escape(id) +
                           ", " + std::to_string(status) + ", " +
                           escaped_result + ")";
          eval(js);
        },
        result.empty() ? "undefined" : json_escape(result)));
  }

  result<void *> window() { return window_impl(); }
  result<void *> widget() { return widget_impl(); }
  result<void *> browser_controller() { return browser_controller_impl(); }
  noresult run() { return run_impl(); }
  noresult terminate() { return terminate_impl(); }
  noresult dispatch(std::function<void()> f) { return dispatch_impl(f); }
  noresult set_title(const std::string &title) { return set_title_impl(title); }

  noresult set_size(int width, int height, webview_hint_t hints) {
    auto res = set_size_impl(width, height, hints);
    m_is_size_set = true;
    return res;
  }

  noresult set_html(const std::string &html) { return set_html_impl(html); }

  noresult init(const std::string &js) {
    add_user_script(js);
    return {};
  }

  noresult eval(const std::string &js) { return eval_impl(js); }

protected:
  virtual noresult navigate_impl(const std::string &url) = 0;
  virtual result<void *> window_impl() = 0;
  virtual result<void *> widget_impl() = 0;
  virtual result<void *> browser_controller_impl() = 0;
  virtual noresult run_impl() = 0;
  virtual noresult terminate_impl() = 0;
  virtual noresult dispatch_impl(std::function<void()> f) = 0;
  virtual noresult set_title_impl(const std::string &title) = 0;
  virtual noresult set_size_impl(int width, int height,
                                 webview_hint_t hints) = 0;
  virtual noresult set_html_impl(const std::string &html) = 0;
  virtual noresult eval_impl(const std::string &js) = 0;

  virtual user_script *add_user_script(const std::string &js) {
    return std::addressof(*m_user_scripts.emplace(m_user_scripts.end(),
                                                  add_user_script_impl(js)));
  }

  virtual user_script add_user_script_impl(const std::string &js) = 0;

  virtual void
  remove_all_user_scripts_impl(const std::list<user_script> &scripts) = 0;

  virtual bool are_user_scripts_equal_impl(const user_script &first,
                                           const user_script &second) = 0;

  virtual user_script *replace_user_script(const user_script &old_script,
                                           const std::string &new_script_code) {
    remove_all_user_scripts_impl(m_user_scripts);
    user_script *old_script_ptr{};
    for (auto &script : m_user_scripts) {
      auto is_old_script = are_user_scripts_equal_impl(script, old_script);
      script = add_user_script_impl(is_old_script ? new_script_code
                                                  : script.get_code());
      if (is_old_script) {
        old_script_ptr = std::addressof(script);
      }
    }
    return old_script_ptr;
  }

  void replace_bind_script() {
    if (m_bind_script) {
      m_bind_script = replace_user_script(*m_bind_script, create_bind_script());
    } else {
      m_bind_script = add_user_script(create_bind_script());
    }
  }

  void add_init_script(const std::string &post_fn) {
    add_user_script(create_init_script(post_fn));
    m_is_init_script_added = true;
  }

  std::string create_init_script(const std::string &post_fn) {
    auto js = std::string{} + "(function() {\n\
  'use strict';\n\
  function generateId() {\n\
    var crypto = window.crypto || window.msCrypto;\n\
    var bytes = new Uint8Array(16);\n\
    crypto.getRandomValues(bytes);\n\
    return Array.prototype.slice.call(bytes).map(function(n) {\n\
      var s = n.toString(16);\n\
      return ((s.length % 2) == 1 ? '0' : '') + s;\n\
    }).join('');\n\
  }\n\
  var Webview = (function() {\n\
    var _promises = {};\n\
    function Webview_() {}\n\
    Webview_.prototype.post = function(message) {\n\
      return (" +
              post_fn + ")(message);\n\
    };\n\
    Webview_.prototype.call = function(method) {\n\
      var _id = generateId();\n\
      var _params = Array.prototype.slice.call(arguments, 1);\n\
      var promise = new Promise(function(resolve, reject) {\n\
        _promises[_id] = { resolve, reject };\n\
      });\n\
      this.post(JSON.stringify({\n\
        id: _id,\n\
        method: method,\n\
        params: _params\n\
      }));\n\
      return promise;\n\
    };\n\
    Webview_.prototype.onReply = function(id, status, result) {\n\
      var promise = _promises[id];\n\
      if (result !== undefined) {\n\
        try {\n\
          result = JSON.parse(result);\n\
        } catch (e) {\n\
          promise.reject(new Error(\"Failed to parse binding result as JSON\"));\n\
          return;\n\
        }\n\
      }\n\
      if (status === 0) {\n\
        promise.resolve(result);\n\
      } else {\n\
        promise.reject(result);\n\
      }\n\
    };\n\
    Webview_.prototype.onBind = function(name) {\n\
      if (window.hasOwnProperty(name)) {\n\
        throw new Error('Property \"' + name + '\" already exists');\n\
      }\n\
      window[name] = (function() {\n\
        var params = [name].concat(Array.prototype.slice.call(arguments));\n\
        return Webview_.prototype.call.apply(this, params);\n\
      }).bind(this);\n\
    };\n\
    Webview_.prototype.onUnbind = function(name) {\n\
      if (!window.hasOwnProperty(name)) {\n\
        throw new Error('Property \"' + name + '\" does not exist');\n\
      }\n\
      delete window[name];\n\
    };\n\
    return Webview_;\n\
  })();\n\
  window.__webview__ = new Webview();\n\
})()";
    return js;
  }

  std::string create_bind_script() {
    std::string js_names = "[";
    bool first = true;
    for (const auto &binding : bindings) {
      if (first) {
        first = false;
      } else {
        js_names += ",";
      }
      js_names += json_escape(binding.first);
    }
    js_names += "]";

    auto js = std::string{} + "(function() {\n\
  'use strict';\n\
  var methods = " +
              js_names + ";\n\
  methods.forEach(function(name) {\n\
    window.__webview__.onBind(name);\n\
  });\n\
})()";
    return js;
  }

  virtual void on_message(const std::string &msg) {
    auto id = json_parse(msg, "id", 0);
    auto name = json_parse(msg, "method", 0);
    auto args = json_parse(msg, "params", 0);
    auto found = bindings.find(name);
    if (found == bindings.end()) {
      return;
    }
    const auto &context = found->second;
    dispatch([=] { context.call(id, args); });
  }

  virtual void on_window_created() { inc_window_count(); }

  virtual void on_window_destroyed(bool skip_termination = false) {
    if (dec_window_count() <= 0) {
      if (!skip_termination) {
        terminate();
      }
    }
  }

  // Runs the event loop until the currently queued events have been processed.
  void deplete_run_loop_event_queue() {
    bool done{};
    dispatch([&] { done = true; });
    run_event_loop_while([&] { return !done; });
  }

  // Runs the event loop while the passed-in function returns true.
  virtual void run_event_loop_while(std::function<bool()> fn) = 0;

  void dispatch_size_default() {
    if (!owns_window() || !m_is_init_script_added) {
      return;
    };
    dispatch([this]() {
      if (!m_is_size_set) {
        set_size(m_initial_width, m_initial_height, WEBVIEW_HINT_NONE);
      }
    });
  }

  void set_default_size_guard(bool guarded) { m_is_size_set = guarded; }

  bool owns_window() const { return m_owns_window; }

private:
  static std::atomic_uint &window_ref_count() {
    static std::atomic_uint ref_count{0};
    return ref_count;
  }

  static unsigned int inc_window_count() { return ++window_ref_count(); }

  static unsigned int dec_window_count() {
    auto &count = window_ref_count();
    if (count > 0) {
      return --count;
    }
    return 0;
  }

  std::map<std::string, binding_ctx_t> bindings;
  user_script *m_bind_script{};
  std::list<user_script> m_user_scripts;

  bool m_is_init_script_added{};
  bool m_is_size_set{};
  bool m_owns_window{};
  static const int m_initial_width = 640;
  static const int m_initial_height = 480;
};

} // namespace detail
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_DETAIL_ENGINE_BASE_HH
// file end: include/webview/detail/engine_base.hh
// file begin: include/webview/detail/platform/darwin/cocoa/NSApplication.hh
#ifndef WEBVIEW_PLATFORM_DARWIN_COCOA_NSAPPLICATION_HH
#define WEBVIEW_PLATFORM_DARWIN_COCOA_NSAPPLICATION_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
namespace webview {
namespace detail {
namespace cocoa {

enum NSApplicationActivationPolicy : NSInteger {
  NSApplicationActivationPolicyRegular = 0
};

enum NSEventMask : NSUInteger { NSEventMaskAny = NSUIntegerMax };

enum NSModalResponse : NSInteger { NSModalResponseOK = 1 };

namespace NSRunLoopMode {
inline id NSDefaultRunLoopMode() {
  return NSString_stringWithUTF8String("kCFRunLoopDefaultMode");
}
} // namespace NSRunLoopMode

inline void NSApplication_set_delegate(id self, id delegate) {
  objc::msg_send<void>(self, objc::selector("setDelegate:"), delegate);
}

inline void NSApplication_run(id self) {
  objc::msg_send<void>(self, objc::selector("run"));
}

inline void NSApplication_stop(id self, id sender = nullptr) {
  objc::msg_send<void>(self, objc::selector("stop:"), sender);
}

inline id NSApplication_get_sharedApplication() {
  return objc::msg_send<id>(objc::get_class("NSApplication"),
                            objc::selector("sharedApplication"));
}

inline void NSApplication_sendEvent(id self, id event) {
  objc::msg_send<void>(self, objc::selector("sendEvent:"), event);
}

inline id NSApplication_nextEventMatchingMask(id self, NSEventMask mask,
                                              id expiration, id mode,
                                              bool dequeue) {
  return objc::msg_send<id>(
      self, objc::selector("nextEventMatchingMask:untilDate:inMode:dequeue:"),
      mask, expiration, mode, dequeue);
}

inline void
NSApplication_setActivationPolicy(id self,
                                  NSApplicationActivationPolicy policy) {
  objc::msg_send<void>(self, objc::selector("setActivationPolicy:"), policy);
}

inline void NSApplication_activateIgnoringOtherApps(id self, bool ignore) {
  objc::msg_send<void>(self, objc::selector("activateIgnoringOtherApps:"),
                       static_cast<BOOL>(ignore));
}

inline void NSApplication_postEvent(id self, id event, bool at_start) {
  objc::msg_send<void>(self, objc::selector("postEvent:atStart:"), event,
                       static_cast<BOOL>(at_start));
}

} // namespace cocoa
} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_DARWIN_COCOA_NSAPPLICATION_HH
// file end: include/webview/detail/platform/darwin/cocoa/NSApplication.hh
// file begin: include/webview/detail/platform/darwin/cocoa/NSURL.hh
#ifndef WEBVIEW_PLATFORM_DARWIN_COCOA_NSURL_HH
#define WEBVIEW_PLATFORM_DARWIN_COCOA_NSURL_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
#include <string>

namespace webview {
namespace detail {
namespace cocoa {

inline id NSURL_URLWithString(id string) {
  return objc::msg_send<id>(objc::get_class("NSURL"),
                            objc::selector("URLWithString:"), string);
}

inline id NSURL_URLWithString(const char *string) {
  return NSURL_URLWithString(NSString_stringWithUTF8String(string));
}

inline id NSURL_URLWithString(const std::string &string) {
  return NSURL_URLWithString(NSString_stringWithUTF8String(string));
}

} // namespace cocoa
} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_DARWIN_COCOA_NSURL_HH
// file end: include/webview/detail/platform/darwin/cocoa/NSURL.hh
// file begin: include/webview/detail/platform/darwin/webkit/webkit.hh
#ifndef WEBVIEW_PLATFORM_DARWIN_WEBKIT_HH
#define WEBVIEW_PLATFORM_DARWIN_WEBKIT_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)

// IWYU pragma: begin_exports

#endif // defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_DARWIN_WEBKIT_HH
// file end: include/webview/detail/platform/darwin/webkit/webkit.hh
// file begin: include/webview/detail/backends/gtk_webkitgtk.hh
#ifndef WEBVIEW_BACKENDS_GTK_WEBKITGTK_HH
#define WEBVIEW_BACKENDS_GTK_WEBKITGTK_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_LINUX) && defined(WEBVIEW_GTK)

//
// ====================================================================
//
// This implementation uses webkit2gtk backend. It requires GTK and
// WebKitGTK libraries. Proper compiler flags can be retrieved via:
//
//   pkg-config --cflags --libs gtk4 webkitgtk-6.0
//   pkg-config --cflags --libs gtk+-3.0 webkit2gtk-4.1
//   pkg-config --cflags --libs gtk+-3.0 webkit2gtk-4.0
//
// ====================================================================
//
#include <functional>
#include <list>
#include <memory>
#include <string>

#include <gtk/gtk.h>

#if GTK_MAJOR_VERSION >= 4

#include <jsc/jsc.h>
#include <webkit/webkit.h>

#elif GTK_MAJOR_VERSION >= 3

#include <JavaScriptCore/JavaScript.h>
#include <webkit2/webkit2.h>

#endif

#include <fcntl.h>
#include <sys/stat.h>

namespace webview {
namespace detail {

class user_script::impl {
public:
  impl(WebKitUserScript *script) : m_script{script} {
    webkit_user_script_ref(script);
  }

  ~impl() { webkit_user_script_unref(m_script); }

  impl(const impl &) = delete;
  impl &operator=(const impl &) = delete;
  impl(impl &&) = delete;
  impl &operator=(impl &&) = delete;

  WebKitUserScript *get_native() const { return m_script; }

private:
  WebKitUserScript *m_script{};
};

class gtk_webkit_engine : public engine_base {
public:
  gtk_webkit_engine(bool debug, void *window) : engine_base{!window} {
    window_init(window);
    window_settings(debug);
    dispatch_size_default();
  }

  gtk_webkit_engine(const gtk_webkit_engine &) = delete;
  gtk_webkit_engine &operator=(const gtk_webkit_engine &) = delete;
  gtk_webkit_engine(gtk_webkit_engine &&) = delete;
  gtk_webkit_engine &operator=(gtk_webkit_engine &&) = delete;

  virtual ~gtk_webkit_engine() {
    if (m_window) {
      if (owns_window()) {
        // Disconnect handlers to avoid callbacks invoked during destruction.
        g_signal_handlers_disconnect_by_data(GTK_WINDOW(m_window), this);
        gtk_window_close(GTK_WINDOW(m_window));
        on_window_destroyed(true);
      } else {
        gtk_compat::window_remove_child(GTK_WINDOW(m_window),
                                        GTK_WIDGET(m_webview));
      }
    }
    if (m_webview) {
      g_object_unref(m_webview);
    }
    if (owns_window()) {
      // Needed for the window to close immediately.
      deplete_run_loop_event_queue();
    }
  }

protected:
  result<void *> window_impl() override {
    if (m_window) {
      return m_window;
    }
    return error_info{WEBVIEW_ERROR_INVALID_STATE};
  }

  result<void *> widget_impl() override {
    if (m_webview) {
      return m_webview;
    }
    return error_info{WEBVIEW_ERROR_INVALID_STATE};
  }

  result<void *> browser_controller_impl() override {
    if (m_webview) {
      return m_webview;
    }
    return error_info{WEBVIEW_ERROR_INVALID_STATE};
  }

  noresult run_impl() override {
    m_stop_run_loop = false;
    while (!m_stop_run_loop) {
      g_main_context_iteration(nullptr, TRUE);
    }
    return {};
  }

  noresult terminate_impl() override {
    return dispatch_impl([&] { m_stop_run_loop = true; });
  }

  noresult dispatch_impl(std::function<void()> f) override {
    g_idle_add_full(G_PRIORITY_HIGH_IDLE, (GSourceFunc)([](void *fn) -> int {
                      (*static_cast<dispatch_fn_t *>(fn))();
                      return G_SOURCE_REMOVE;
                    }),
                    new std::function<void()>(f),
                    [](void *fn) { delete static_cast<dispatch_fn_t *>(fn); });
    return {};
  }

  noresult set_title_impl(const std::string &title) override {
    gtk_window_set_title(GTK_WINDOW(m_window), title.c_str());
    return {};
  }

  noresult set_size_impl(int width, int height, webview_hint_t hints) override {
    gtk_window_set_resizable(GTK_WINDOW(m_window), hints != WEBVIEW_HINT_FIXED);
    if (hints == WEBVIEW_HINT_NONE || hints == WEBVIEW_HINT_FIXED) {
      gtk_compat::window_set_size(GTK_WINDOW(m_window), width, height);
    } else if (hints == WEBVIEW_HINT_MIN) {
      gtk_widget_set_size_request(m_window, width, height);
    } else if (hints == WEBVIEW_HINT_MAX) {
      gtk_compat::window_set_max_size(GTK_WINDOW(m_window), width, height);
    } else {
      return error_info{WEBVIEW_ERROR_INVALID_ARGUMENT, "Invalid hint"};
    }
    return window_show();
  }

  noresult navigate_impl(const std::string &url) override {
    webkit_web_view_load_uri(WEBKIT_WEB_VIEW(m_webview), url.c_str());
    return {};
  }

  noresult set_html_impl(const std::string &html) override {
    webkit_web_view_load_html(WEBKIT_WEB_VIEW(m_webview), html.c_str(),
                              nullptr);
    return {};
  }

  noresult eval_impl(const std::string &js) override {
    // URI is null before content has begun loading.
    if (!webkit_web_view_get_uri(WEBKIT_WEB_VIEW(m_webview))) {
      return {};
    }
#if (WEBKIT_MAJOR_VERSION == 2 && WEBKIT_MINOR_VERSION >= 40) ||               \
    WEBKIT_MAJOR_VERSION > 2
    webkit_web_view_evaluate_javascript(WEBKIT_WEB_VIEW(m_webview), js.c_str(),
                                        static_cast<gssize>(js.size()), nullptr,
                                        nullptr, nullptr, nullptr, nullptr);
#else
    webkit_web_view_run_javascript(WEBKIT_WEB_VIEW(m_webview), js.c_str(),
                                   nullptr, nullptr, nullptr);
#endif
    return {};
  }

  user_script add_user_script_impl(const std::string &js) override {
    auto *wk_script = webkit_user_script_new(
        js.c_str(), WEBKIT_USER_CONTENT_INJECT_TOP_FRAME,
        WEBKIT_USER_SCRIPT_INJECT_AT_DOCUMENT_START, nullptr, nullptr);
    webkit_user_content_manager_add_script(m_user_content_manager, wk_script);
    user_script script{
        js, user_script::impl_ptr{new user_script::impl{wk_script},
                                  [](user_script::impl *p) { delete p; }}};
    webkit_user_script_unref(wk_script);
    return script;
  }

  void remove_all_user_scripts_impl(
      const std::list<user_script> & /*scripts*/) override {
    webkit_user_content_manager_remove_all_scripts(m_user_content_manager);
  }

  bool are_user_scripts_equal_impl(const user_script &first,
                                   const user_script &second) override {
    auto *wk_first = first.get_impl().get_native();
    auto *wk_second = second.get_impl().get_native();
    return wk_first == wk_second;
  }

private:
#if GTK_MAJOR_VERSION >= 4
  static char *get_string_from_js_result(JSCValue *r) {
    return jsc_value_to_string(r);
  }
#else
  static char *get_string_from_js_result(WebKitJavascriptResult *r) {
    char *s;
#if (WEBKIT_MAJOR_VERSION == 2 && WEBKIT_MINOR_VERSION >= 22) ||               \
    WEBKIT_MAJOR_VERSION > 2
    JSCValue *value = webkit_javascript_result_get_js_value(r);
    s = jsc_value_to_string(value);
#else
    JSGlobalContextRef ctx = webkit_javascript_result_get_global_context(r);
    JSValueRef value = webkit_javascript_result_get_value(r);
    JSStringRef js = JSValueToStringCopy(ctx, value, nullptr);
    size_t n = JSStringGetMaximumUTF8CStringSize(js);
    s = g_new(char, n);
    JSStringGetUTF8CString(js, s, n);
    JSStringRelease(js);
#endif
    return s;
  }
#endif

  void window_init(void *window) {
    m_window = static_cast<GtkWidget *>(window);
    if (owns_window()) {
      if (!gtk_compat::init_check()) {
        throw exception{WEBVIEW_ERROR_UNSPECIFIED, "GTK init failed"};
      }
      m_window = gtk_compat::window_new();
      on_window_created();
      auto on_window_destroy = +[](GtkWidget *, gpointer arg) {
        auto *w = static_cast<gtk_webkit_engine *>(arg);
        w->m_window = nullptr;
        w->on_window_destroyed();
      };
      g_signal_connect(G_OBJECT(m_window), "destroy",
                       G_CALLBACK(on_window_destroy), this);
    }
    webkit_dmabuf::apply_webkit_dmabuf_workaround();
    // Initialize webview widget
    m_webview = webkit_web_view_new();
    g_object_ref_sink(m_webview);
    WebKitUserContentManager *manager = m_user_content_manager =
        webkit_web_view_get_user_content_manager(WEBKIT_WEB_VIEW(m_webview));
    webkitgtk_compat::connect_script_message_received(
        manager, "__webview__",
        [this](WebKitUserContentManager *, const std::string &r) {
          on_message(r);
        });
    webkitgtk_compat::user_content_manager_register_script_message_handler(
        manager, "__webview__");
    add_init_script("function(message) {\n\
  return window.webkit.messageHandlers.__webview__.postMessage(message);\n\
}");
  }

  void window_settings(bool debug) {
    WebKitSettings *settings =
        webkit_web_view_get_settings(WEBKIT_WEB_VIEW(m_webview));
    webkit_settings_set_javascript_can_access_clipboard(settings, true);
    if (debug) {
      webkit_settings_set_enable_write_console_messages_to_stdout(settings,
                                                                  true);
      webkit_settings_set_enable_developer_extras(settings, true);
    }
  }

  noresult window_show() {
    if (m_is_window_shown) {
      return {};
    }
    gtk_compat::window_set_child(GTK_WINDOW(m_window), GTK_WIDGET(m_webview));
    gtk_compat::widget_set_visible(GTK_WIDGET(m_webview), true);

    if (owns_window()) {
      gtk_widget_grab_focus(GTK_WIDGET(m_webview));
      gtk_compat::widget_set_visible(GTK_WIDGET(m_window), true);
    }
    m_is_window_shown = true;
    return {};
  }

  void run_event_loop_while(std::function<bool()> fn) override {
    while (fn()) {
      g_main_context_iteration(nullptr, TRUE);
    }
  }

  GtkWidget *m_window{};
  GtkWidget *m_webview{};
  WebKitUserContentManager *m_user_content_manager{};
  bool m_stop_run_loop{};
  bool m_is_window_shown{};
};

} // namespace detail

using browser_engine = detail::gtk_webkit_engine;

} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_LINUX) && defined(WEBVIEW_GTK)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_BACKENDS_GTK_WEBKITGTK_HH
// file end: include/webview/detail/backends/gtk_webkitgtk.hh
// file begin: include/webview/detail/backends/win32_edge.hh
#ifndef WEBVIEW_BACKENDS_WIN32_EDGE_HH
#define WEBVIEW_BACKENDS_WIN32_EDGE_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_WINDOWS) && defined(WEBVIEW_EDGE)

//
// ====================================================================
//
// This implementation uses Win32 API to create a native window. It
// uses Edge/Chromium webview2 backend as a browser engine.
//
// ====================================================================
//
#include <atomic>
#include <cstdlib>
#include <functional>
#include <list>
#include <memory>
#include <utility>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>

#include <objbase.h>
#include <shlobj.h>
#include <shlwapi.h>

#ifdef _MSC_VER
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "version.lib")
#endif

namespace webview {
namespace detail {

using msg_cb_t = std::function<void(const std::string)>;

class webview2_com_handler
    : public ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler,
      public ICoreWebView2CreateCoreWebView2ControllerCompletedHandler,
      public ICoreWebView2WebMessageReceivedEventHandler,
      public ICoreWebView2PermissionRequestedEventHandler {
  using webview2_com_handler_cb_t =
      std::function<void(ICoreWebView2Controller *, ICoreWebView2 *webview)>;

public:
  webview2_com_handler(HWND hwnd, msg_cb_t msgCb, webview2_com_handler_cb_t cb)
      : m_window(hwnd), m_msgCb(msgCb), m_cb(cb) {}

  virtual ~webview2_com_handler() = default;
  webview2_com_handler(const webview2_com_handler &other) = delete;
  webview2_com_handler &operator=(const webview2_com_handler &other) = delete;
  webview2_com_handler(webview2_com_handler &&other) = delete;
  webview2_com_handler &operator=(webview2_com_handler &&other) = delete;

  ULONG STDMETHODCALLTYPE AddRef() { return ++m_ref_count; }
  ULONG STDMETHODCALLTYPE Release() {
    if (m_ref_count > 1) {
      return --m_ref_count;
    }
    delete this;
    return 0;
  }
  HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, LPVOID *ppv) {
    using namespace mswebview2::cast_info;

    if (!ppv) {
      return E_POINTER;
    }

    // All of the COM interfaces we implement should be added here regardless
    // of whether they are required.
    // This is just to be on the safe side in case the WebView2 Runtime ever
    // requests a pointer to an interface we implement.
    // The WebView2 Runtime must at the very least be able to get a pointer to
    // ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler when we use
    // our custom WebView2 loader implementation, and observations have shown
    // that it is the only interface requested in this case. None have been
    // observed to be requested when using the official WebView2 loader.

    if (cast_if_equal_iid(this, riid, controller_completed, ppv) ||
        cast_if_equal_iid(this, riid, environment_completed, ppv) ||
        cast_if_equal_iid(this, riid, message_received, ppv) ||
        cast_if_equal_iid(this, riid, permission_requested, ppv)) {
      return S_OK;
    }

    return E_NOINTERFACE;
  }
  HRESULT STDMETHODCALLTYPE Invoke(HRESULT res, ICoreWebView2Environment *env) {
    if (SUCCEEDED(res)) {
      res = env->CreateCoreWebView2Controller(m_window, this);
      if (SUCCEEDED(res)) {
        return S_OK;
      }
    }
    try_create_environment();
    return S_OK;
  }
  HRESULT STDMETHODCALLTYPE Invoke(HRESULT res,
                                   ICoreWebView2Controller *controller) {
    if (FAILED(res)) {
      // See try_create_environment() regarding
      // HRESULT_FROM_WIN32(ERROR_INVALID_STATE).
      // The result is E_ABORT if the parent window has been destroyed already.
      switch (res) {
      case HRESULT_FROM_WIN32(ERROR_INVALID_STATE):
      case E_ABORT:
        return S_OK;
      }
      try_create_environment();
      return S_OK;
    }

    ICoreWebView2 *webview;
    ::EventRegistrationToken token;
    controller->get_CoreWebView2(&webview);
    webview->add_WebMessageReceived(this, &token);
    webview->add_PermissionRequested(this, &token);

    m_cb(controller, webview);
    return S_OK;
  }
  HRESULT STDMETHODCALLTYPE
  Invoke(ICoreWebView2 * /*sender*/,
         ICoreWebView2WebMessageReceivedEventArgs *args) {
    LPWSTR message{};
    auto res = args->TryGetWebMessageAsString(&message);
    if (SUCCEEDED(res)) {
      m_msgCb(narrow_string(message));
    }

    CoTaskMemFree(message);
    return S_OK;
  }
  HRESULT STDMETHODCALLTYPE
  Invoke(ICoreWebView2 * /*sender*/,
         ICoreWebView2PermissionRequestedEventArgs *args) {
    COREWEBVIEW2_PERMISSION_KIND kind;
    args->get_PermissionKind(&kind);
    if (kind == COREWEBVIEW2_PERMISSION_KIND_CLIPBOARD_READ) {
      args->put_State(COREWEBVIEW2_PERMISSION_STATE_ALLOW);
    }
    return S_OK;
  }

  // Set the function that will perform the initiating logic for creating
  // the WebView2 environment.
  void set_attempt_handler(std::function<HRESULT()> attempt_handler) noexcept {
    m_attempt_handler = attempt_handler;
  }

  // Retry creating a WebView2 environment.
  // The initiating logic for creating the environment is defined by the
  // caller of set_attempt_handler().
  void try_create_environment() noexcept {
    // WebView creation fails with HRESULT_FROM_WIN32(ERROR_INVALID_STATE) if
    // a running instance using the same user data folder exists, and the
    // Environment objects have different EnvironmentOptions.
    // Source:
    // https://docs.microsoft.com/en-us/microsoft-edge/webview2/reference/win32/icorewebview2environment?view=webview2-1.0.1150.38
    if (m_attempts < m_max_attempts) {
      ++m_attempts;
      auto res = m_attempt_handler();
      if (SUCCEEDED(res)) {
        return;
      }
      // Not entirely sure if this error code only applies to
      // CreateCoreWebView2Controller so we check here as well.
      if (res == HRESULT_FROM_WIN32(ERROR_INVALID_STATE)) {
        return;
      }
      // Wait for m_sleep_ms before trying again.
      Sleep(m_sleep_ms);
      try_create_environment();
      return;
    }
    // Give up.
    m_cb(nullptr, nullptr);
  }

private:
  HWND m_window;
  msg_cb_t m_msgCb;
  webview2_com_handler_cb_t m_cb;
  std::atomic<ULONG> m_ref_count{1};
  std::function<HRESULT()> m_attempt_handler;
  unsigned int m_max_attempts = 60;
  unsigned int m_sleep_ms = 200;
  unsigned int m_attempts = 0;
};

class webview2_user_script_added_handler
    : public ICoreWebView2AddScriptToExecuteOnDocumentCreatedCompletedHandler {
public:
  using callback_fn = std::function<void(HRESULT errorCode, LPCWSTR id)>;

  webview2_user_script_added_handler(callback_fn cb) : m_cb{cb} {}

  virtual ~webview2_user_script_added_handler() = default;
  webview2_user_script_added_handler(
      const webview2_user_script_added_handler &other) = delete;
  webview2_user_script_added_handler &
  operator=(const webview2_user_script_added_handler &other) = delete;
  webview2_user_script_added_handler(
      webview2_user_script_added_handler &&other) = delete;
  webview2_user_script_added_handler &
  operator=(webview2_user_script_added_handler &&other) = delete;

  ULONG STDMETHODCALLTYPE AddRef() { return ++m_ref_count; }
  ULONG STDMETHODCALLTYPE Release() {
    if (m_ref_count > 1) {
      return --m_ref_count;
    }
    delete this;
    return 0;
  }

  HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, LPVOID *ppv) {
    using namespace mswebview2::cast_info;

    if (!ppv) {
      return E_POINTER;
    }

    if (cast_if_equal_iid(this, riid,
                          add_script_to_execute_on_document_created_completed,
                          ppv)) {
      return S_OK;
    }

    return E_NOINTERFACE;
  }

  HRESULT STDMETHODCALLTYPE Invoke(HRESULT res, LPCWSTR id) {
    m_cb(res, id);
    return S_OK;
  }

private:
  callback_fn m_cb;
  std::atomic<ULONG> m_ref_count{1};
};

class user_script::impl {
public:
  impl(const std::wstring &id, const std::wstring &code)
      : m_id{id}, m_code{code} {}

  impl(const impl &) = delete;
  impl &operator=(const impl &) = delete;
  impl(impl &&) = delete;
  impl &operator=(impl &&) = delete;

  const std::wstring &get_id() const { return m_id; }
  const std::wstring &get_code() const { return m_code; }

private:
  std::wstring m_id;
  std::wstring m_code;
};

class win32_edge_engine : public engine_base {
public:
  win32_edge_engine(bool debug, void *window) : engine_base{!window} {
    window_init(window);
    window_settings(debug);
    dispatch_size_default();
  }

  virtual ~win32_edge_engine() {
    if (m_com_handler) {
      m_com_handler->Release();
      m_com_handler = nullptr;
    }
    if (m_webview) {
      m_webview->Release();
      m_webview = nullptr;
    }
    if (m_controller) {
      m_controller->Release();
      m_controller = nullptr;
    }
    // Replace wndproc to avoid callbacks and other bad things during
    // destruction.
    auto wndproc = reinterpret_cast<LONG_PTR>(
        +[](HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) -> LRESULT {
          return DefWindowProcW(hwnd, msg, wp, lp);
        });
    if (m_widget) {
      SetWindowLongPtrW(m_widget, GWLP_WNDPROC, wndproc);
    }
    if (m_window && owns_window()) {
      SetWindowLongPtrW(m_window, GWLP_WNDPROC, wndproc);
    }
    if (m_widget) {
      DestroyWindow(m_widget);
      m_widget = nullptr;
    }
    if (m_window) {
      if (owns_window()) {
        DestroyWindow(m_window);
        on_window_destroyed(true);
      }
      m_window = nullptr;
    }
    if (owns_window()) {
      // Not strictly needed for windows to close immediately but aligns
      // behavior across backends.
      deplete_run_loop_event_queue();
    }
    // We need the message window in order to deplete the event queue.
    if (m_message_window) {
      SetWindowLongPtrW(m_message_window, GWLP_WNDPROC, wndproc);
      DestroyWindow(m_message_window);
      m_message_window = nullptr;
    }
  }

  win32_edge_engine(const win32_edge_engine &other) = delete;
  win32_edge_engine &operator=(const win32_edge_engine &other) = delete;
  win32_edge_engine(win32_edge_engine &&other) = delete;
  win32_edge_engine &operator=(win32_edge_engine &&other) = delete;

protected:
  noresult run_impl() override {
    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
      TranslateMessage(&msg);
      DispatchMessageW(&msg);
    }
    return {};
  }
  result<void *> window_impl() override {
    if (m_window) {
      return m_window;
    }
    return error_info{WEBVIEW_ERROR_INVALID_STATE};
  }
  result<void *> widget_impl() override {
    if (m_widget) {
      return m_widget;
    }
    return error_info{WEBVIEW_ERROR_INVALID_STATE};
  }
  result<void *> browser_controller_impl() override {
    if (m_controller) {
      return m_controller;
    }
    return error_info{WEBVIEW_ERROR_INVALID_STATE};
  }
  noresult terminate_impl() override {
    PostQuitMessage(0);
    return {};
  }
  noresult dispatch_impl(dispatch_fn_t f) override {
    PostMessageW(m_message_window, WM_APP, 0, (LPARAM) new dispatch_fn_t(f));
    return {};
  }

  noresult set_title_impl(const std::string &title) override {
    SetWindowTextW(m_window, widen_string(title).c_str());
    return {};
  }

  noresult set_size_impl(int width, int height, webview_hint_t hints) override {
    auto style = GetWindowLong(m_window, GWL_STYLE);
    if (hints == WEBVIEW_HINT_FIXED) {
      style &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
    } else {
      style |= (WS_THICKFRAME | WS_MAXIMIZEBOX);
    }
    SetWindowLong(m_window, GWL_STYLE, style);

    if (hints == WEBVIEW_HINT_MAX) {
      m_maxsz.x = width;
      m_maxsz.y = height;
    } else if (hints == WEBVIEW_HINT_MIN) {
      m_minsz.x = width;
      m_minsz.y = height;
    } else {
      auto dpi = get_window_dpi(m_window);
      m_dpi = dpi;
      auto scaled_size =
          scale_size(width, height, get_default_window_dpi(), dpi);
      auto frame_size =
          make_window_frame_size(m_window, scaled_size.cx, scaled_size.cy, dpi);
      SetWindowPos(m_window, nullptr, 0, 0, frame_size.cx, frame_size.cy,
                   SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE |
                       SWP_FRAMECHANGED);
    }
    return window_show();
  }

  noresult navigate_impl(const std::string &url) override {
    auto wurl = widen_string(url);
    m_webview->Navigate(wurl.c_str());
    return {};
  }

  noresult eval_impl(const std::string &js) override {
    // TODO: Skip if no content has begun loading yet. Can't check with
    //       ICoreWebView2::get_Source because it returns "about:blank".
    auto wjs = widen_string(js);
    m_webview->ExecuteScript(wjs.c_str(), nullptr);
    return {};
  }

  noresult set_html_impl(const std::string &html) override {
    m_webview->NavigateToString(widen_string(html).c_str());
    return {};
  }

  user_script add_user_script_impl(const std::string &js) override {
    auto wjs = widen_string(js);
    std::wstring script_id;
    bool done{};
    webview2_user_script_added_handler handler{[&](HRESULT res, LPCWSTR id) {
      if (SUCCEEDED(res)) {
        script_id = id;
      }
      done = true;
    }};
    auto res =
        m_webview->AddScriptToExecuteOnDocumentCreated(wjs.c_str(), &handler);
    if (SUCCEEDED(res)) {
      // We want to guard against executing the default `set_size` prematurely
      set_default_size_guard(true);
      // Sadly we need to pump the event loop in order to get the script ID.
      run_event_loop_while([&] { return !done; });
      // The user's `set_size` may have been executed from the depleted event
      // queue, and if so, guard against putting the default `set_size` back
      // onto the queue.
      if (!m_is_window_shown) {
        set_default_size_guard(false);
        dispatch_size_default();
      }
    }
    // TODO: There's a non-zero chance that we didn't get the script ID.
    //       We need to convey the error somehow.
    return user_script{
        js, user_script::impl_ptr{new user_script::impl{script_id, wjs},
                                  [](user_script::impl *p) { delete p; }}};
  }

  void
  remove_all_user_scripts_impl(const std::list<user_script> &scripts) override {
    for (const auto &script : scripts) {
      const auto &id = script.get_impl().get_id();
      m_webview->RemoveScriptToExecuteOnDocumentCreated(id.c_str());
    }
  }

  bool are_user_scripts_equal_impl(const user_script &first,
                                   const user_script &second) override {
    const auto &first_id = first.get_impl().get_id();
    const auto &second_id = second.get_impl().get_id();
    return first_id == second_id;
  }

private:
  void window_init(void *window) {
    if (!is_webview2_available()) {
      throw exception{WEBVIEW_ERROR_MISSING_DEPENDENCY,
                      "WebView2 is unavailable"};
    }

    HINSTANCE hInstance = GetModuleHandle(nullptr);

    if (owns_window()) {
      m_com_init = {COINIT_APARTMENTTHREADED};
      enable_dpi_awareness();

      HICON icon = (HICON)LoadImage(
          hInstance, IDI_APPLICATION, IMAGE_ICON, GetSystemMetrics(SM_CXICON),
          GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);

      // Create a top-level window.
      WNDCLASSEXW wc;
      ZeroMemory(&wc, sizeof(WNDCLASSEX));
      wc.cbSize = sizeof(WNDCLASSEX);
      wc.hInstance = hInstance;
      wc.lpszClassName = L"webview";
      wc.hIcon = icon;
      wc.lpfnWndProc = (WNDPROC)(+[](HWND hwnd, UINT msg, WPARAM wp,
                                     LPARAM lp) -> LRESULT {
        win32_edge_engine *w{};

        if (msg == WM_NCCREATE) {
          auto *lpcs{reinterpret_cast<LPCREATESTRUCT>(lp)};
          w = static_cast<win32_edge_engine *>(lpcs->lpCreateParams);
          w->m_window = hwnd;
          SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(w));
          enable_non_client_dpi_scaling_if_needed(hwnd);
          apply_window_theme(hwnd);
        } else {
          w = reinterpret_cast<win32_edge_engine *>(
              GetWindowLongPtrW(hwnd, GWLP_USERDATA));
        }

        if (!w) {
          return DefWindowProcW(hwnd, msg, wp, lp);
        }

        switch (msg) {
        case WM_SIZE:
          w->resize_widget();
          break;
        case WM_CLOSE:
          DestroyWindow(hwnd);
          break;
        case WM_DESTROY:
          w->m_window = nullptr;
          SetWindowLongPtrW(hwnd, GWLP_USERDATA, 0);
          w->on_window_destroyed();
          break;
        case WM_GETMINMAXINFO: {
          auto lpmmi = (LPMINMAXINFO)lp;
          if (w->m_maxsz.x > 0 && w->m_maxsz.y > 0) {
            lpmmi->ptMaxSize = w->m_maxsz;
            lpmmi->ptMaxTrackSize = w->m_maxsz;
          }
          if (w->m_minsz.x > 0 && w->m_minsz.y > 0) {
            lpmmi->ptMinTrackSize = w->m_minsz;
          }
        } break;
        case 0x02E4 /*WM_GETDPISCALEDSIZE*/: {
          auto dpi = static_cast<int>(wp);
          auto *size{reinterpret_cast<SIZE *>(lp)};
          *size = w->get_scaled_size(w->m_dpi, dpi);
          return TRUE;
        }
        case 0x02E0 /*WM_DPICHANGED*/: {
          // Windows 10: The size we get here is exactly what we supplied to
          // WM_GETDPISCALEDSIZE. Windows 11: The size we get here is NOT what
          // we supplied to WM_GETDPISCALEDSIZE. Due to this difference, don't
          // use the suggested bounds.
          auto dpi = static_cast<int>(HIWORD(wp));
          w->on_dpi_changed(dpi);
          break;
        }
        case WM_SETTINGCHANGE: {
          auto *area = reinterpret_cast<const wchar_t *>(lp);
          if (area) {
            w->on_system_setting_change(area);
          }
          break;
        }
        case WM_ACTIVATE:
          if (LOWORD(wp) != WA_INACTIVE) {
            w->focus_webview();
          }
          break;
        default:
          return DefWindowProcW(hwnd, msg, wp, lp);
        }
        return 0;
      });
      RegisterClassExW(&wc);

      CreateWindowW(L"webview", L"", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                    CW_USEDEFAULT, 0, 0, nullptr, nullptr, hInstance, this);
      if (!m_window) {
        throw exception{WEBVIEW_ERROR_INVALID_STATE, "Window is null"};
      }
      on_window_created();

      m_dpi = get_window_dpi(m_window);
    } else {
      m_window = IsWindow(static_cast<HWND>(window))
                     ? static_cast<HWND>(window)
                     : *(static_cast<HWND *>(window));
      m_dpi = get_window_dpi(m_window);
    }
    // Create a window that WebView2 will be embedded into.
    WNDCLASSEXW widget_wc{};
    widget_wc.cbSize = sizeof(WNDCLASSEX);
    widget_wc.hInstance = hInstance;
    widget_wc.lpszClassName = L"webview_widget";
    widget_wc.lpfnWndProc = (WNDPROC)(+[](HWND hwnd, UINT msg, WPARAM wp,
                                          LPARAM lp) -> LRESULT {
      win32_edge_engine *w{};

      if (msg == WM_NCCREATE) {
        auto *lpcs{reinterpret_cast<LPCREATESTRUCT>(lp)};
        w = static_cast<win32_edge_engine *>(lpcs->lpCreateParams);
        w->m_widget = hwnd;
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(w));
      } else {
        w = reinterpret_cast<win32_edge_engine *>(
            GetWindowLongPtrW(hwnd, GWLP_USERDATA));
      }

      if (!w) {
        return DefWindowProcW(hwnd, msg, wp, lp);
      }

      switch (msg) {
      case WM_SIZE:
        w->resize_webview();
        break;
      case WM_DESTROY:
        w->m_widget = nullptr;
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, 0);
        break;
      default:
        return DefWindowProcW(hwnd, msg, wp, lp);
      }
      return 0;
    });
    RegisterClassExW(&widget_wc);
    CreateWindowExW(WS_EX_CONTROLPARENT, L"webview_widget", nullptr, WS_CHILD,
                    0, 0, 0, 0, m_window, nullptr, hInstance, this);
    if (!m_widget) {
      throw exception{WEBVIEW_ERROR_INVALID_STATE, "Widget window is null"};
    }

    // Create a message-only window for internal messaging.
    WNDCLASSEXW message_wc{};
    message_wc.cbSize = sizeof(WNDCLASSEX);
    message_wc.hInstance = hInstance;
    message_wc.lpszClassName = L"webview_message";
    message_wc.lpfnWndProc = (WNDPROC)(+[](HWND hwnd, UINT msg, WPARAM wp,
                                           LPARAM lp) -> LRESULT {
      win32_edge_engine *w{};

      if (msg == WM_NCCREATE) {
        auto *lpcs{reinterpret_cast<LPCREATESTRUCT>(lp)};
        w = static_cast<win32_edge_engine *>(lpcs->lpCreateParams);
        w->m_message_window = hwnd;
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(w));
      } else {
        w = reinterpret_cast<win32_edge_engine *>(
            GetWindowLongPtrW(hwnd, GWLP_USERDATA));
      }

      if (!w) {
        return DefWindowProcW(hwnd, msg, wp, lp);
      }

      switch (msg) {
      case WM_APP:
        if (auto f = (dispatch_fn_t *)(lp)) {
          (*f)();
          delete f;
        }
        break;
      case WM_DESTROY:
        w->m_message_window = nullptr;
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, 0);
        break;
      default:
        return DefWindowProcW(hwnd, msg, wp, lp);
      }
      return 0;
    });
    RegisterClassExW(&message_wc);
    CreateWindowExW(0, L"webview_message", nullptr, 0, 0, 0, 0, 0, HWND_MESSAGE,
                    nullptr, hInstance, this);
    if (!m_message_window) {
      throw exception{WEBVIEW_ERROR_INVALID_STATE, "Message window is null"};
    }
  }

  void window_settings(bool debug) {
    auto cb =
        std::bind(&win32_edge_engine::on_message, this, std::placeholders::_1);
    embed(m_widget, debug, cb).ensure_ok();
  }

  noresult window_show() {
    if (owns_window() && !m_is_window_shown) {
      ShowWindow(m_window, SW_SHOW);
      UpdateWindow(m_window);
      SetFocus(m_window);
      m_is_window_shown = true;
    }
    return {};
  }
  noresult embed(HWND wnd, bool debug, msg_cb_t cb) {
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
    flag.test_and_set();

    wchar_t currentExePath[MAX_PATH];
    GetModuleFileNameW(nullptr, currentExePath, MAX_PATH);
    wchar_t *currentExeName = PathFindFileNameW(currentExePath);

    wchar_t dataPath[MAX_PATH];
    if (!SUCCEEDED(
            SHGetFolderPathW(nullptr, CSIDL_APPDATA, nullptr, 0, dataPath))) {
      return error_info{WEBVIEW_ERROR_UNSPECIFIED, "SHGetFolderPathW failed"};
    }
    wchar_t userDataFolder[MAX_PATH];
    PathCombineW(userDataFolder, dataPath, currentExeName);

    m_com_handler = new webview2_com_handler(
        wnd, cb,
        [&](ICoreWebView2Controller *controller, ICoreWebView2 *webview) {
          if (!controller || !webview) {
            flag.clear();
            return;
          }
          controller->AddRef();
          webview->AddRef();
          m_controller = controller;
          m_webview = webview;
          flag.clear();
        });

    m_com_handler->set_attempt_handler([&] {
      return m_webview2_loader.create_environment_with_options(
          nullptr, userDataFolder, nullptr, m_com_handler);
    });
    m_com_handler->try_create_environment();

    // Pump the message loop until WebView2 has finished initialization.
    bool got_quit_msg = false;
    MSG msg;
    while (flag.test_and_set() && GetMessageW(&msg, nullptr, 0, 0) >= 0) {
      if (msg.message == WM_QUIT) {
        got_quit_msg = true;
        break;
      }
      TranslateMessage(&msg);
      DispatchMessageW(&msg);
    }
    if (got_quit_msg) {
      return error_info{WEBVIEW_ERROR_CANCELED};
    }
    if (!m_controller || !m_webview) {
      return error_info{WEBVIEW_ERROR_INVALID_STATE};
    }
    ICoreWebView2Settings *settings = nullptr;
    auto res = m_webview->get_Settings(&settings);
    if (res != S_OK) {
      return error_info{WEBVIEW_ERROR_UNSPECIFIED, "get_Settings failed"};
    }
    res = settings->put_AreDevToolsEnabled(debug ? TRUE : FALSE);
    if (res != S_OK) {
      return error_info{WEBVIEW_ERROR_UNSPECIFIED,
                        "put_AreDevToolsEnabled failed"};
    }
    res = settings->put_IsStatusBarEnabled(FALSE);
    if (res != S_OK) {
      return error_info{WEBVIEW_ERROR_UNSPECIFIED,
                        "put_IsStatusBarEnabled failed"};
    }
    add_init_script("function(message) {\n\
  return window.chrome.webview.postMessage(message);\n\
}");
    resize_webview();
    m_controller->put_IsVisible(TRUE);
    ShowWindow(m_widget, SW_SHOW);
    UpdateWindow(m_widget);
    if (owns_window()) {
      focus_webview();
    }
    return {};
  }

  void resize_widget() {
    if (m_widget) {
      RECT r{};
      if (GetClientRect(GetParent(m_widget), &r)) {
        MoveWindow(m_widget, r.left, r.top, r.right - r.left, r.bottom - r.top,
                   TRUE);
      }
    }
  }

  void resize_webview() {
    if (m_widget && m_controller) {
      RECT bounds{};
      if (GetClientRect(m_widget, &bounds)) {
        m_controller->put_Bounds(bounds);
      }
    }
  }

  void focus_webview() {
    if (m_controller) {
      m_controller->MoveFocus(COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
    }
  }

  bool is_webview2_available() const noexcept {
    LPWSTR version_info = nullptr;
    auto res = m_webview2_loader.get_available_browser_version_string(
        nullptr, &version_info);
    // The result will be equal to HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND)
    // if the WebView2 runtime is not installed.
    auto ok = SUCCEEDED(res) && version_info;
    if (version_info) {
      CoTaskMemFree(version_info);
    }
    return ok;
  }

  void on_dpi_changed(int dpi) {
    auto scaled_size = get_scaled_size(m_dpi, dpi);
    auto frame_size =
        make_window_frame_size(m_window, scaled_size.cx, scaled_size.cy, dpi);
    SetWindowPos(m_window, nullptr, 0, 0, frame_size.cx, frame_size.cy,
                 SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE | SWP_FRAMECHANGED);
    m_dpi = dpi;
  }

  SIZE get_size() const {
    RECT bounds;
    GetClientRect(m_window, &bounds);
    auto width = bounds.right - bounds.left;
    auto height = bounds.bottom - bounds.top;
    return {width, height};
  }

  SIZE get_scaled_size(int from_dpi, int to_dpi) const {
    auto size = get_size();
    return scale_size(size.cx, size.cy, from_dpi, to_dpi);
  }

  void on_system_setting_change(const wchar_t *area) {
    // Detect light/dark mode change in system.
    if (lstrcmpW(area, L"ImmersiveColorSet") == 0) {
      apply_window_theme(m_window);
    }
  }

  void run_event_loop_while(std::function<bool()> fn) override {
    MSG msg;
    while (fn() && GetMessageW(&msg, nullptr, 0, 0) > 0) {
      TranslateMessage(&msg);
      DispatchMessageW(&msg);
    }
  }

  // The app is expected to call CoInitializeEx before
  // CreateCoreWebView2EnvironmentWithOptions.
  // Source:
  // https://docs.microsoft.com/en-us/microsoft-edge/webview2/reference/win32/webview2-idl#createcorewebview2environmentwithoptions
  com_init_wrapper m_com_init;
  HWND m_window = nullptr;
  HWND m_widget = nullptr;
  HWND m_message_window = nullptr;
  POINT m_minsz = POINT{0, 0};
  POINT m_maxsz = POINT{0, 0};
  DWORD m_main_thread = GetCurrentThreadId();
  ICoreWebView2 *m_webview = nullptr;
  ICoreWebView2Controller *m_controller = nullptr;
  webview2_com_handler *m_com_handler = nullptr;
  mswebview2::loader m_webview2_loader;
  int m_dpi{};
  bool m_is_window_shown{};
};

} // namespace detail

using browser_engine = detail::win32_edge_engine;

} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_WINDOWS) && defined(WEBVIEW_EDGE)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_BACKENDS_WIN32_EDGE_HH
// file end: include/webview/detail/backends/win32_edge.hh
// file begin: include/webview/detail/platform/darwin/cocoa/NSSavePanel.hh
#ifndef WEBVIEW_PLATFORM_DARWIN_COCOA_NSSAVEPANEL_HH
#define WEBVIEW_PLATFORM_DARWIN_COCOA_NSSAVEPANEL_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
namespace webview {
namespace detail {
namespace cocoa {

inline NSModalResponse NSSavePanel_runModal(id self) {
  return objc::msg_send<NSModalResponse>(self, objc::selector("runModal"));
}

} // namespace cocoa
} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_DARWIN_COCOA_NSSAVEPANEL_HH
// file end: include/webview/detail/platform/darwin/cocoa/NSSavePanel.hh
// file begin: include/webview/detail/platform/darwin/cocoa/cocoa.hh
#ifndef WEBVIEW_PLATFORM_DARWIN_COCOA_COCOA_HH
#define WEBVIEW_PLATFORM_DARWIN_COCOA_COCOA_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)

// IWYU pragma: begin_exports

#endif // defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_DARWIN_COCOA_COCOA_HH
// file end: include/webview/detail/platform/darwin/cocoa/cocoa.hh
// file begin: include/webview/detail/backends/cocoa_webkit.hh
#ifndef WEBVIEW_BACKENDS_COCOA_WEBKIT_HH
#define WEBVIEW_BACKENDS_COCOA_WEBKIT_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)

//
// ====================================================================
//
// This implementation uses Cocoa WKWebView backend on macOS. It is
// written using ObjC runtime and uses WKWebView class as a browser runtime.
// You should pass "-framework Webkit" flag to the compiler.
//
// ====================================================================
//
#include <atomic>
#include <functional>
#include <list>
#include <memory>
#include <string>

#include <objc/objc-runtime.h>

namespace webview {
namespace detail {

class user_script::impl {
public:
  impl(id script) : m_script{objc::retain(script)} {}

  ~impl() { objc::release(m_script); }

  impl(const impl &) = delete;
  impl &operator=(const impl &) = delete;
  impl(impl &&) = delete;
  impl &operator=(impl &&) = delete;

  id get_native() const { return m_script; }

private:
  id m_script{};
};

// Encapsulate backend in its own namespace to avoid polluting the parent
// namespace when pulling in commonly-used symbols from other namespaces.
// Since those commmon symbols are used a lot, this reduces the overall
// noise in the code.
namespace cocoa_webkit {

using namespace cocoa;
using namespace webkit;

class cocoa_wkwebview_engine : public engine_base {
public:
  cocoa_wkwebview_engine(bool debug, void *window)
      : engine_base{!window}, m_app{NSApplication_get_sharedApplication()} {
    window_init(window);
    window_settings(debug);
    dispatch_size_default();
  }

  cocoa_wkwebview_engine(const cocoa_wkwebview_engine &) = delete;
  cocoa_wkwebview_engine &operator=(const cocoa_wkwebview_engine &) = delete;
  cocoa_wkwebview_engine(cocoa_wkwebview_engine &&) = delete;
  cocoa_wkwebview_engine &operator=(cocoa_wkwebview_engine &&) = delete;

  virtual ~cocoa_wkwebview_engine() {
    objc::autoreleasepool arp;
    if (m_window) {
      if (m_webview) {
        if (auto ui_delegate{WKWebView_get_UIDelegate(m_webview)}) {
          WKWebView_set_UIDelegate(m_webview, nullptr);
          objc::release(ui_delegate);
        }
        objc::release(m_webview);
        m_webview = nullptr;
      }
      if (m_widget) {
        if (m_widget == NSWindow_get_contentView(m_window)) {
          NSWindow_set_contentView(m_window, nullptr);
        }
        objc::release(m_widget);
        m_widget = nullptr;
      }
      if (owns_window()) {
        // Replace delegate to avoid callbacks and other bad things during
        // destruction.
        NSWindow_set_delegate(m_window, nullptr);
        NSWindow_close(m_window);
        on_window_destroyed(true);
      }
      m_window = nullptr;
    }
    if (m_window_delegate) {
      objc::release(m_window_delegate);
      m_window_delegate = nullptr;
    }
    if (m_app_delegate) {
      NSApplication_set_delegate(m_app, nullptr);
      // Make sure to release the delegate we created.
      objc::release(m_app_delegate);
      m_app_delegate = nullptr;
    }
    if (owns_window()) {
      // Needed for the window to close immediately.
      deplete_run_loop_event_queue();
    }
    // TODO: Figure out why m_manager is still alive after the autoreleasepool
    // has been drained.
  }

protected:
  result<void *> window_impl() override {
    if (m_window) {
      return m_window;
    }
    return error_info{WEBVIEW_ERROR_INVALID_STATE};
  }

  result<void *> widget_impl() override {
    if (m_widget) {
      return m_widget;
    }
    return error_info{WEBVIEW_ERROR_INVALID_STATE};
  }

  result<void *> browser_controller_impl() override {
    if (m_webview) {
      return m_webview;
    }
    return error_info{WEBVIEW_ERROR_INVALID_STATE};
  }

  noresult terminate_impl() override {
    stop_run_loop();
    return {};
  }

  noresult run_impl() override {
    NSApplication_run(m_app);
    return {};
  }

  noresult dispatch_impl(std::function<void()> f) override {
    dispatch_async_f(dispatch_get_main_queue(), new dispatch_fn_t(f),
                     (dispatch_function_t)([](void *arg) {
                       auto f = static_cast<dispatch_fn_t *>(arg);
                       (*f)();
                       delete f;
                     }));
    return {};
  }

  noresult set_title_impl(const std::string &title) override {
    NSWindow_set_title(m_window, title);
    return {};
  }
  noresult set_size_impl(int width, int height, webview_hint_t hints) override {
    objc::autoreleasepool arp;

    auto style = static_cast<NSWindowStyleMask>(
        NSWindowStyleMaskTitled | NSWindowStyleMaskClosable |
        NSWindowStyleMaskMiniaturizable);
    if (hints != WEBVIEW_HINT_FIXED) {
      style =
          static_cast<NSWindowStyleMask>(style | NSWindowStyleMaskResizable);
    }
    NSWindow_set_styleMask(m_window, style);

    if (hints == WEBVIEW_HINT_MIN) {
      NSWindow_set_contentMinSize(m_window, NSSizeMake(width, height));
    } else if (hints == WEBVIEW_HINT_MAX) {
      NSWindow_set_contentMaxSize(m_window, NSSizeMake(width, height));
    } else {
      auto rect{NSWindow_get_frame(m_window)};
      NSWindow_setFrame(m_window,
                        NSRectMake(rect.origin.x, rect.origin.y, width, height),
                        true, false);
    }
    NSWindow_center(m_window);

    return window_show();
  }
  noresult navigate_impl(const std::string &url) override {
    objc::autoreleasepool arp;

    WKWebView_loadRequest(
        m_webview, NSURLRequest_requestWithURL(NSURL_URLWithString(url)));

    return {};
  }
  noresult set_html_impl(const std::string &html) override {
    objc::autoreleasepool arp;
    WKWebView_loadHTMLString(m_webview, NSString_stringWithUTF8String(html),
                             nullptr);
    return {};
  }
  noresult eval_impl(const std::string &js) override {
    objc::autoreleasepool arp;
    // URI is null before content has begun loading.
    auto nsurl{WKWebView_get_URL(m_webview)};
    if (!nsurl) {
      return {};
    }
    WKWebView_evaluateJavaScript(m_webview, NSString_stringWithUTF8String(js),
                                 nullptr);
    return {};
  }

  user_script add_user_script_impl(const std::string &js) override {
    objc::autoreleasepool arp;
    auto wk_script{WKUserScript_withSource(
        NSString_stringWithUTF8String(js),
        WKUserScriptInjectionTimeAtDocumentStart, true)};
    // Script is retained when added.
    WKUserContentController_addUserScript(m_manager, wk_script);
    user_script script{
        js, user_script::impl_ptr{new user_script::impl{wk_script},
                                  [](user_script::impl *p) { delete p; }}};
    return script;
  }

  void remove_all_user_scripts_impl(
      const std::list<user_script> & /*scripts*/) override {
    objc::autoreleasepool arp;
    // Removing scripts decreases the retain count of each script.
    WKUserContentController_removeAllUserScripts(m_manager);
  }

  bool are_user_scripts_equal_impl(const user_script &first,
                                   const user_script &second) override {
    auto *wk_first = first.get_impl().get_native();
    auto *wk_second = second.get_impl().get_native();
    return wk_first == wk_second;
  }

private:
  id create_app_delegate() {
    objc::autoreleasepool arp;
    constexpr auto class_name = "WebviewAppDelegate";
    // Avoid crash due to registering same class twice
    auto cls = objc_lookUpClass(class_name);
    if (!cls) {
      // Note: Avoid registering the class name "AppDelegate" as it is the
      // default name in projects created with Xcode, and using the same name
      // causes objc_registerClassPair to crash.
      cls =
          objc_allocateClassPair(objc::get_class("NSResponder"), class_name, 0);
      class_addProtocol(cls, objc_getProtocol("NSTouchBarProvider"));
      class_addMethod(
          cls,
          objc::selector("applicationShouldTerminateAfterLastWindowClosed:"),
          (IMP)(+[](id, SEL, id) -> BOOL { return NO; }), "c@:@");
      class_addMethod(cls, objc::selector("applicationDidFinishLaunching:"),
                      (IMP)(+[](id self, SEL, id notification) {
                        auto app{NSNotification_get_object(notification)};
                        auto w = get_associated_webview(self);
                        w->on_application_did_finish_launching(self, app);
                      }),
                      "v@:@");
      objc_registerClassPair(cls);
    }
    return objc::Class_new(cls);
  }
  id create_script_message_handler() {
    objc::autoreleasepool arp;
    constexpr auto class_name = "WebviewWKScriptMessageHandler";
    // Avoid crash due to registering same class twice
    auto cls = objc_lookUpClass(class_name);
    if (!cls) {
      cls =
          objc_allocateClassPair(objc::get_class("NSResponder"), class_name, 0);
      class_addProtocol(cls, objc_getProtocol("WKScriptMessageHandler"));
      class_addMethod(
          cls, objc::selector("userContentController:didReceiveScriptMessage:"),
          (IMP)(+[](id self, SEL, id, id msg) {
            auto w = get_associated_webview(self);
            w->on_message(
                NSString_get_UTF8String(WKScriptMessage_get_body(msg)));
          }),
          "v@:@@");
      objc_registerClassPair(cls);
    }
    auto instance{objc::Class_new(cls)};
    set_associated_webview(instance, this);
    return instance;
  }
  static id create_webkit_ui_delegate() {
    objc::autoreleasepool arp;
    constexpr auto class_name = "WebviewWKUIDelegate";
    // Avoid crash due to registering same class twice
    auto cls = objc_lookUpClass(class_name);
    if (!cls) {
      cls = objc_allocateClassPair(objc::get_class("NSObject"), class_name, 0);
      class_addProtocol(cls, objc_getProtocol("WKUIDelegate"));
      class_addMethod(
          cls,
          objc::selector("webView:runOpenPanelWithParameters:initiatedByFrame:"
                         "completionHandler:"),
          (IMP)(+[](id, SEL, id, id parameters, id, id completion_handler) {
            auto allows_multiple_selection{
                WKOpenPanelParameters_get_allowsMultipleSelection(parameters)};
            auto allows_directories{
                WKOpenPanelParameters_get_allowsDirectories(parameters)};

            // Show a panel for selecting files.
            auto panel{NSOpenPanel_openPanel()};
            NSOpenPanel_set_canChooseFiles(panel, true);
            NSOpenPanel_set_canChooseDirectories(panel, allows_directories);
            NSOpenPanel_set_allowsMultipleSelection(panel,
                                                    allows_multiple_selection);
            auto modal_response{NSSavePanel_runModal(panel)};

            // Get the URLs for the selected files. If the modal was canceled
            // then we pass null to the completion handler to signify
            // cancellation.
            id urls{modal_response == NSModalResponseOK
                        ? NSOpenPanel_get_URLs(panel)
                        : nullptr};

            // Invoke the completion handler block.
            auto sig{NSMethodSignature_signatureWithObjCTypes("v@?@")};
            auto invocation{NSInvocation_invocationWithMethodSignature(sig)};
            NSInvocation_set_target(invocation, completion_handler);
            NSInvocation_setArgument(invocation, &urls, 1);
            NSInvocation_invoke(invocation);
          }),
          "v@:@@@@");
      objc_registerClassPair(cls);
    }
    return objc::Class_new(cls);
  }
  static id create_window_delegate() {
    objc::autoreleasepool arp;
    constexpr auto class_name = "WebviewNSWindowDelegate";
    // Avoid crash due to registering same class twice
    auto cls = objc_lookUpClass(class_name);
    if (!cls) {
      cls = objc_allocateClassPair(objc::get_class("NSObject"), class_name, 0);
      class_addProtocol(cls, objc_getProtocol("NSWindowDelegate"));
      class_addMethod(cls, objc::selector("windowWillClose:"),
                      (IMP)(+[](id self, SEL, id notification) {
                        auto window{NSNotification_get_object(notification)};
                        auto w = get_associated_webview(self);
                        w->on_window_will_close(self, window);
                      }),
                      "v@:@");
      objc_registerClassPair(cls);
    }
    return objc::Class_new(cls);
  }
  static cocoa_wkwebview_engine *get_associated_webview(id object) {
    objc::autoreleasepool arp;
    if (id assoc_obj{objc_getAssociatedObject(object, "webview")}) {
      cocoa_wkwebview_engine *w{};
      NSValue_getValue(assoc_obj, &w, sizeof(w));
      return w;
    }
    return nullptr;
  }
  static void set_associated_webview(id object, cocoa_wkwebview_engine *w) {
    objc::autoreleasepool arp;
    objc_setAssociatedObject(object, "webview", NSValue_valueWithPointer(w),
                             OBJC_ASSOCIATION_RETAIN);
  }
  static bool is_app_bundled() noexcept {
    auto bundle = NSBundle_get_mainBundle();
    if (!bundle) {
      return false;
    }
    auto bundle_path = NSBundle_get_bundlePath(bundle);
    auto bundled =
        NSString_hasSuffix(bundle_path, NSString_stringWithUTF8String(".app"));
    return !!bundled;
  }
  void on_application_did_finish_launching(id /*delegate*/, id app) {
    // See comments related to application lifecycle in create_app_delegate().
    if (owns_window()) {
      // Stop the main run loop so that we can return
      // from the constructor.
      stop_run_loop();
    }

    // Activate the app if it is not bundled.
    // Bundled apps launched from Finder are activated automatically but
    // otherwise not. Activating the app even when it has been launched from
    // Finder does not seem to be harmful but calling this function is rarely
    // needed as proper activation is normally taken care of for us.
    // Bundled apps have a default activation policy of
    // NSApplicationActivationPolicyRegular while non-bundled apps have a
    // default activation policy of NSApplicationActivationPolicyProhibited.
    if (!is_app_bundled()) {
      // "setActivationPolicy:" must be invoked before
      // "activateIgnoringOtherApps:" for activation to work.
      NSApplication_setActivationPolicy(app,
                                        NSApplicationActivationPolicyRegular);
      // Activate the app regardless of other active apps.
      // This can be obtrusive so we only do it when necessary.
      NSApplication_activateIgnoringOtherApps(app, true);
    }

    window_init_proceed();
  }
  void on_window_will_close(id /*delegate*/, id /*window*/) {
    // Widget destroyed along with window.
    m_widget = nullptr;
    m_webview = nullptr;
    m_window = nullptr;
    dispatch([this] { on_window_destroyed(); });
  }
  void window_settings(bool debug) {
    objc::autoreleasepool arp;

    auto config{objc::autorelease(WKWebViewConfiguration_new())};

    m_manager = WKWebViewConfiguration_get_userContentController(config);

    auto preferences = WKWebViewConfiguration_get_preferences(config);
    auto yes_value = NSNumber_numberWithBool(true);

    if (debug) {
      NSObject_setValue_forKey(
          preferences, yes_value,
          NSString_stringWithUTF8String("developerExtrasEnabled"));
    }

    NSObject_setValue_forKey(
        preferences, yes_value,
        NSString_stringWithUTF8String("fullScreenEnabled"));

#if defined(__has_builtin)
#if __has_builtin(__builtin_available)
    if (__builtin_available(macOS 10.13, *)) {
      NSObject_setValue_forKey(
          preferences, yes_value,
          NSString_stringWithUTF8String("javaScriptCanAccessClipboard"));
      NSObject_setValue_forKey(
          preferences, yes_value,
          NSString_stringWithUTF8String("DOMPasteAllowed"));
    }
#else
#error __builtin_available not supported by compiler
#endif
#else
#error __has_builtin not supported by compiler
#endif

    auto ui_delegate = create_webkit_ui_delegate();
    m_webview =
        objc::retain(WKWebView_withFrame(CGRectMake(0, 0, 0, 0), config));
    // Autoresizing mask is needed to prevent the Web Inspector pane from
    // pushing the main web view out of bounds
    auto autoresizing_mask{static_cast<NSAutoresizingMaskOptions>(
        NSViewWidthSizable | NSViewMaxXMargin | NSViewHeightSizable |
        NSViewMaxYMargin)};
    NSView_set_autoresizingMask(m_webview, autoresizing_mask);
    set_associated_webview(ui_delegate, this);
    WKWebView_set_UIDelegate(m_webview, ui_delegate);

    if (debug) {
      // Explicitly make WKWebView inspectable via Safari on OS versions that
      // disable the feature by default (macOS 13.3 and later) and support
      // enabling it. According to Apple, the behavior on older OS versions is
      // for content to always be inspectable in "debug builds".
      // Testing shows that this is true for macOS 12.6 but somehow not 10.15.
      // https://webkit.org/blog/13936/enabling-the-inspection-of-web-content-in-apps/
      WKWebView_set_inspectable(m_webview, true);
    }

    auto script_message_handler =
        objc::autorelease(create_script_message_handler());
    WKUserContentController_addScriptMessageHandler(
        m_manager, script_message_handler,
        NSString_stringWithUTF8String("__webview__"));

    add_init_script("function(message) {\n\
  return window.webkit.messageHandlers.__webview__.postMessage(message);\n\
}");
    set_up_widget();
    NSWindow_set_contentView(m_window, m_widget);
    if (owns_window()) {
      NSWindow_makeKeyAndOrderFront(m_window);
    }
  }
  void set_up_widget() {
    objc::autoreleasepool arp;
    // Create a new view that can contain both the web view and the Web
    // Inspector pane
    m_widget = objc::retain(NSView_withFrame(NSRectMake(0, 0, 0, 0)));
    // Autoresizing is needed because the Web Inspector pane is a sibling of the
    // web view
    NSView_set_autoresizesSubviews(m_widget, true);
    NSView_addSubview(m_widget, m_webview);
    NSView_set_frame(m_webview, NSView_get_bounds(m_widget));
  }
  void stop_run_loop() {
    objc::autoreleasepool arp;
    // Request the run loop to stop. This doesn't immediately stop the loop.
    NSApplication_stop(m_app);
    // The run loop will stop after processing an NSEvent.
    auto event{NSEvent_otherEventWithType(
        NSEventTypeApplicationDefined, NSPointMake(0, 0),
        NSEventModifierFlags{}, 0, 0, nullptr, 0, 0, 0)};
    NSApplication_postEvent(m_app, event, true);
  }
  static bool get_and_set_is_first_instance() noexcept {
    static std::atomic_bool first{true};
    bool temp = first;
    if (temp) {
      first = false;
    }
    return temp;
  }
  void window_init(void *window) {
    objc::autoreleasepool arp;

    m_window = static_cast<id>(window);
    if (!owns_window()) {
      return;
    }

    // Skip application setup if this isn't the first instance of this class
    // because the launch event is only sent once.
    if (!get_and_set_is_first_instance()) {
      window_init_proceed();
      return;
    }

    m_app_delegate = create_app_delegate();
    set_associated_webview(m_app_delegate, this);
    NSApplication_set_delegate(m_app, m_app_delegate);

    // Start the main run loop so that the app delegate gets the
    // NSApplicationDidFinishLaunchingNotification notification after the run
    // loop has started in order to perform further initialization.
    // We need to return from this constructor so this run loop is only
    // temporary.
    NSApplication_run(m_app);
  }
  void window_init_proceed() {
    objc::autoreleasepool arp;

    m_window = objc::retain(NSWindow_withContentRect(
        NSRectMake(0, 0, 0, 0), NSWindowStyleMaskTitled, NSBackingStoreBuffered,
        false));
    m_window_delegate = create_window_delegate();
    set_associated_webview(m_window_delegate, this);
    NSWindow_set_delegate(m_window, m_window_delegate);
    on_window_created();
  }

  noresult window_show() {
    objc::autoreleasepool arp;
    if (m_is_window_shown) {
      return {};
    }
    m_is_window_shown = true;
    return {};
  }

  void run_event_loop_while(std::function<bool()> fn) override {
    objc::autoreleasepool arp;
    while (fn()) {
      objc::autoreleasepool arp2;
      if (auto event{NSApplication_nextEventMatchingMask(
              m_app, NSEventMaskAny, nullptr,
              NSRunLoopMode::NSDefaultRunLoopMode(), true)}) {
        NSApplication_sendEvent(m_app, event);
      }
    }
  }

  id m_app{};
  id m_app_delegate{};
  id m_window_delegate{};
  id m_window{};
  id m_widget{};
  id m_webview{};
  id m_manager{};
  bool m_is_window_shown{};
};

} // namespace cocoa_webkit
} // namespace detail

using browser_engine = detail::cocoa_webkit::cocoa_wkwebview_engine;

} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_DARWIN) && defined(WEBVIEW_COCOA)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_BACKENDS_COCOA_WEBKIT_HH
// file end: include/webview/detail/backends/cocoa_webkit.hh
// file begin: include/webview/backends.hh
#ifndef WEBVIEW_BACKENDS_HH
#define WEBVIEW_BACKENDS_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
namespace webview {
using webview = browser_engine;
}

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_BACKENDS_HH
// file end: include/webview/backends.hh
// file begin: include/webview/c_api_impl.hh
#ifndef WEBVIEW_C_API_IMPL_HH
#define WEBVIEW_C_API_IMPL_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
namespace webview {
namespace detail {

// The library's version information.
constexpr const webview_version_info_t library_version_info{
    {WEBVIEW_VERSION_MAJOR, WEBVIEW_VERSION_MINOR, WEBVIEW_VERSION_PATCH},
    WEBVIEW_VERSION_NUMBER,
    WEBVIEW_VERSION_PRE_RELEASE,
    WEBVIEW_VERSION_BUILD_METADATA};

template <typename WorkFn, typename ResultFn>
webview_error_t api_filter(WorkFn &&do_work, ResultFn &&put_result) noexcept {
  try {
    auto result = do_work();
    if (result.ok()) {
      put_result(result.value());
      return WEBVIEW_ERROR_OK;
    }
    return result.error().code();
  } catch (const exception &e) {
    return e.error().code();
  } catch (...) {
    return WEBVIEW_ERROR_UNSPECIFIED;
  }
}

template <typename WorkFn>
webview_error_t api_filter(WorkFn &&do_work) noexcept {
  try {
    auto result = do_work();
    if (result.ok()) {
      return WEBVIEW_ERROR_OK;
    }
    return result.error().code();
  } catch (const exception &e) {
    return e.error().code();
  } catch (...) {
    return WEBVIEW_ERROR_UNSPECIFIED;
  }
}

inline webview *cast_to_webview(void *w) {
  if (!w) {
    throw exception{WEBVIEW_ERROR_INVALID_ARGUMENT,
                    "Cannot cast null pointer to webview instance"};
  }
  return static_cast<webview *>(w);
}

} // namespace detail
} // namespace webview

WEBVIEW_API webview_t webview_create(int debug, void *wnd) {
  using namespace webview::detail;
  webview::webview *w{};
  auto err = api_filter(
      [=]() -> webview::result<webview::webview *> {
        return new webview::webview{static_cast<bool>(debug), wnd};
      },
      [&](webview::webview *w_) { w = w_; });
  if (err == WEBVIEW_ERROR_OK) {
    return w;
  }
  return nullptr;
}

WEBVIEW_API webview_error_t webview_destroy(webview_t w) {
  using namespace webview::detail;
  return api_filter([=]() -> webview::noresult {
    delete cast_to_webview(w);
    return {};
  });
}

WEBVIEW_API webview_error_t webview_run(webview_t w) {
  using namespace webview::detail;
  return api_filter([=] { return cast_to_webview(w)->run(); });
}

WEBVIEW_API webview_error_t webview_terminate(webview_t w) {
  using namespace webview::detail;
  return api_filter([=] { return cast_to_webview(w)->terminate(); });
}

WEBVIEW_API webview_error_t webview_dispatch(webview_t w,
                                             void (*fn)(webview_t, void *),
                                             void *arg) {
  using namespace webview::detail;
  if (!fn) {
    return WEBVIEW_ERROR_INVALID_ARGUMENT;
  }
  return api_filter(
      [=] { return cast_to_webview(w)->dispatch([=]() { fn(w, arg); }); });
}

WEBVIEW_API void *webview_get_window(webview_t w) {
  using namespace webview::detail;
  void *window = nullptr;
  auto err = api_filter([=] { return cast_to_webview(w)->window(); },
                        [&](void *value) { window = value; });
  if (err == WEBVIEW_ERROR_OK) {
    return window;
  }
  return nullptr;
}

WEBVIEW_API void *webview_get_native_handle(webview_t w,
                                            webview_native_handle_kind_t kind) {
  using namespace webview::detail;
  void *handle{};
  auto err = api_filter(
      [=]() -> webview::result<void *> {
        auto *w_ = cast_to_webview(w);
        switch (kind) {
        case WEBVIEW_NATIVE_HANDLE_KIND_UI_WINDOW:
          return w_->window();
        case WEBVIEW_NATIVE_HANDLE_KIND_UI_WIDGET:
          return w_->widget();
        case WEBVIEW_NATIVE_HANDLE_KIND_BROWSER_CONTROLLER:
          return w_->browser_controller();
        default:
          return webview::error_info{WEBVIEW_ERROR_INVALID_ARGUMENT};
        }
      },
      [&](void *handle_) { handle = handle_; });
  if (err == WEBVIEW_ERROR_OK) {
    return handle;
  }
  return nullptr;
}

WEBVIEW_API webview_error_t webview_set_title(webview_t w, const char *title) {
  using namespace webview::detail;
  if (!title) {
    return WEBVIEW_ERROR_INVALID_ARGUMENT;
  }
  return api_filter([=] { return cast_to_webview(w)->set_title(title); });
}

WEBVIEW_API webview_error_t webview_set_size(webview_t w, int width, int height,
                                             webview_hint_t hints) {
  using namespace webview::detail;
  return api_filter(
      [=] { return cast_to_webview(w)->set_size(width, height, hints); });
}

WEBVIEW_API webview_error_t webview_navigate(webview_t w, const char *url) {
  using namespace webview::detail;
  if (!url) {
    return WEBVIEW_ERROR_INVALID_ARGUMENT;
  }
  return api_filter([=] { return cast_to_webview(w)->navigate(url); });
}

WEBVIEW_API webview_error_t webview_set_html(webview_t w, const char *html) {
  using namespace webview::detail;
  if (!html) {
    return WEBVIEW_ERROR_INVALID_ARGUMENT;
  }
  return api_filter([=] { return cast_to_webview(w)->set_html(html); });
}

WEBVIEW_API webview_error_t webview_init(webview_t w, const char *js) {
  using namespace webview::detail;
  if (!js) {
    return WEBVIEW_ERROR_INVALID_ARGUMENT;
  }
  return api_filter([=] { return cast_to_webview(w)->init(js); });
}

WEBVIEW_API webview_error_t webview_eval(webview_t w, const char *js) {
  using namespace webview::detail;
  if (!js) {
    return WEBVIEW_ERROR_INVALID_ARGUMENT;
  }
  return api_filter([=] { return cast_to_webview(w)->eval(js); });
}

WEBVIEW_API webview_error_t webview_bind(webview_t w, const char *name,
                                         void (*fn)(const char *id,
                                                    const char *req, void *arg),
                                         void *arg) {
  using namespace webview::detail;
  if (!name || !fn) {
    return WEBVIEW_ERROR_INVALID_ARGUMENT;
  }
  return api_filter([=] {
    return cast_to_webview(w)->bind(
        name,
        [=](const std::string &seq, const std::string &req, void *arg_) {
          fn(seq.c_str(), req.c_str(), arg_);
        },
        arg);
  });
}

WEBVIEW_API webview_error_t webview_unbind(webview_t w, const char *name) {
  using namespace webview::detail;
  if (!name) {
    return WEBVIEW_ERROR_INVALID_ARGUMENT;
  }
  return api_filter([=] { return cast_to_webview(w)->unbind(name); });
}

WEBVIEW_API webview_error_t webview_return(webview_t w, const char *id,
                                           int status, const char *result) {
  using namespace webview::detail;
  if (!id || !result) {
    return WEBVIEW_ERROR_INVALID_ARGUMENT;
  }
  return api_filter(
      [=] { return cast_to_webview(w)->resolve(id, status, result); });
}

WEBVIEW_API const webview_version_info_t *webview_version(void) {
  return &webview::detail::library_version_info;
}

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_C_API_IMPL_HH
// file end: include/webview/c_api_impl.hh
// file begin: include/webview/webview.h
#ifndef WEBVIEW_H
#define WEBVIEW_H
#endif // WEBVIEW_H
// file end: include/webview/webview.h
// file begin: src/webview.cc

// file end: src/webview.cc

#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <functional>

#define SString std::stringstream
#define String std::string
#define WebView webview::webview

class App
{
private:
  const String readFile(String path)
  {
    try
    {
      SString content;
      std::fstream file(path, std::ios::in);
      content << file.rdbuf();
      return content.str().c_str();
    }
    catch (...)
    {
      printf("Failed to read file: %s\n", path.c_str());
      return "";
    }
  }
  const String writeAfterTag(const String code, const String tag, const String content)
  {
    String before, after;
    before = code.substr(0, code.find(tag));
    after = code.substr(code.find(tag));
    return before + content + after;
  }
  WebView *wv;
  std::unordered_map<String, webview::detail::engine_base::sync_binding_t> binds;
  void updateCode(const String _HTML = "", const String _CSS = "", const String _JS = "")
  {
    String _Code;
    _Code = _HTML;
    const String Tcss = "<style>" + _CSS + "</style>";
    const String Tjs = "<script>" + _JS + "</script>";
    _Code = wirteAfterTag(_Code, "<body>", Tcss);
    _Code = wirteAfterTag(_Code, "</body>", Tjs);
    this->wv->set_html(_Code);
  }
  void updateBinds()
  {
    for (auto &bind : binds)
      wv->bind(bind.first, bind.second);
  }

public:
  App(bool Debug = false) { wv = new WebView(Debug, nullptr); }
  ~App() { delete wv; }
  void setSources(String HTMLPath, String CSSPath, String JSPath) { updateCode(readFile(HTMLPath), readFile(CSSPath), readFile(JSPath)); }
  void setCodes(String HTML, String CSS, String JS) { updateCode(HTML, CSS, JS); }
  void setSize(int width, int height, bool resizable) { this->wv->set_size(width, height, resizable ? WEBVIEW_HINT_MAX : WEBVIEW_HINT_NONE); }
  void setTitle(String title) { this->wv->set_title(title); }
  void setBind(String key, webview::detail::engine_base::sync_binding_t value)
  {
    this->binds[key] = value;
    updateBinds();
  }
  void run() { wv->run(); }
  void close() { wv->terminate(); }
};


#undef SString
#undef String
#undef WebView
