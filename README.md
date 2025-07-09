# Atom 🪐  
*A tiny, header-only C++20 framework for building cross-platform desktop apps with WebView — lighter than Electron, yet just as flexible.*

<p align="center">
  <!-- ضع الشعار في المسار التالي ثم أزل هذا التعليق -->
  <img src="assets/atom_logo.png" width="180" alt="Atom logo">
</p>

---

## 🚀 What is Atom?
**Atom** allows you to ship native Windows / Linux GUIs using nothing but **HTML + CSS + JavaScript**, while keeping memory usage to a minimum.  
It is a single-header wrapper around the ultra-lightweight [webview](https://github.com/webview/webview) library, giving you an Electron-style developer experience without bundling Chromium or Node.js.

|               | Atom | Electron |
|---------------|------|----------|
| Delivery      | Single header (`atom.hpp`) | 100 + MB runtime |
| Language      | Pure C++20                 | JS + C++ bindings |
| Typical RAM*  | **30-50 MB**               | 150-300 MB |
| Build system  | Makefile / CMake           | npm / node-gyp    |

\*Measured with the bundled demo on Windows 11 and Ubuntu 24.04.

---

## 🗂️ Repository layout
```
Atom/
├── atom/            # Library (header-only)
│   ├── atom.hpp
├── ui/              # Front-end assets
│   ├── index.html
│   ├── style.css
│   └── script.js
├── main.cpp         # Demo application
└── makefile         # One-command build on Linux
```

---

## ⚡ Quick start

### Build & run (Linux / WSL)
```bash
# Install deps (Debian/Ubuntu example)
sudo apt install g++ pkg-config libgtk-3-dev libwebkit2gtk-4.1-dev

# Clone and build
git clone https://github.com/<you>/Atom.git
cd Atom
make       # produces ./atomic
make run   # or ./atomic
```

### Build & run (Windows + MSVC)
```bat
vcpkg install webview2
cl /std:c++20 /Iatom /I%VCPKG_ROOT%\installed\x64-windows\include ^
   /link /OUT:atomic.exe main.cpp
atomic.exe
```

You should see a native window that swaps the title text when you click it — all logic lives in `ui/script.js`, while C++ functions are exposed through **bindings**.

---

## ✍️ Writing your own app

```cpp
#include "atom/atom.hpp"

int main() {
    App app(/*debug=*/true);
    app.setSources("ui/index.html", "ui/style.css", "ui/script.js");
    app.setSize(800, 600, /*resizable=*/false);
    app.setTitle("My Atom App");

    // C++ ⇄ JS binding
    app.setBind("add", [](std::string args){
        int a, b;
        sscanf(args.c_str(), "%d,%d", &a, &b);
        return std::to_string(a + b);
    });

    app.run();
}
```

```js
// ui/script.js
const result = await window.add("2,3"); // "5"
```

---

## 🛠️ API reference (core methods)

| C++ Method | Description |
|------------|-------------|
| `setSources(htmlPath, cssPath, jsPath)` | Load UI from files. |
| `setCodes(html, css, js)`              | Inline strings instead of files. |
| `setSize(width, height, resizable)`    | Fixed or resizable window. |
| `setTitle(title)`                      | Native window caption. |
| `setBind(name, callback)`              | Expose a sync C++ function to JS. |
| `run()` / `close()`                    | Start or terminate the event loop. |

---

## 📦 Packaging tips

* **Windows** – Bundle the produced `atomic.exe` together with the **WebView2 runtime** (included by default on recent Windows 11, or ship the Evergreen installer).  
* **Linux** – Ensure `libgtk-3` and `libwebkit2gtk-4.1` are present; use AppImage/Flatpak for painless distribution.


---

## 🤝 Contributing

Pull requests and issues are welcome!  
Please run `clang-format -i **/*.hpp **/*.cpp` before submitting.

---

<p align="center"><sub>© 2025 Atom — Built with ❤️ & C++20</sub></p>