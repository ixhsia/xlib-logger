# Xlib-Logger
A lightweight and configurable C++ logging library.

---
## Cloning & Building

```bash
git clone https://github.com/ixhsia/xlib-logger.git
cd xlib-logger
mkdir build && cd build
cmake ..
make
```
After building, a static library will be generated in the `build/` directory.

You can either:
- Copy the `include/log-xlib` directory and link the static library in your own project, or
- Directly include the entire source code in your project and build it as part of your codebase.

---
## Configuration
Xlib-Logger supports configuration via [TOML](https://toml.io).

You can create a `.toml` file anywhere you like and load it by calling:

```cpp
xlib::logger::LogWriter::from_file_config("path/to/config.toml");
```
You're allowed to customize all necessary configurations the library support.

