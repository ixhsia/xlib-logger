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
xlib::logger::LogWriter::load_config(const std::string &_file_config);
```
You're allowed to customize all necessary configurations the library support.

### How to write a configuration file

#### Version statement
It must be required at the top of the file, for example:

```toml
version = 1
```
- It must be the first line in the file.
- The version number must be an integer.
- The version number must be valid, i.e., it must be supported by the library.

#### Logger global configuration
It must be existed as a table in the file, for example:

```toml
[logger]
    level = 1
    thread = false
```
- The table name must be `logger`.

- The `level` key must be an integer, representing the minimum log level to be recorded.
  - `0`:  `XLIB_LOG_LEVEL_DEBUG`
  - `1`:  ` XLIB_LOG_LEVEL_INFO,`
  - `2`:  `XLIB_LOG_LEVEL_WARNING`
  - `3`:  `XLIB_LOG_LEVEL_ERROR,`
  - `4`:  `XLIB_LOG_LEVEL_FATAL`
  
- The `thread` key must be a boolean, indicating whether to enable thread-safe logging.

#### Logger output configuration
Any enabled output you want to use should be defined in this file, while an output method is defined as a table, it will be used immediately after the configuration file is loaded.

Up to version 1, three output methods are supported

- ##### Console output configuration

- ##### File output configuration
  ```toml
  [file]
    log_dir = "./"
    log_file = "log_xlib.log"
  ``` 
  ```toml
  [file.rolling]
    log_rolling_size = 10485760
    log_name = "log_test_${time}"
  ```