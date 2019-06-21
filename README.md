# Yoda Monitor
[![License](https://img.shields.io/badge/licence-Apache%202.0-brightgreen.svg?style=flat)](LICENSE)
[![Build Status](https://travis-ci.org/yodaos-project/edge-monitor.svg?branch=master)](https://travis-ci.org/yodaos-project/edge-monitor)

Collect and upload system data in real time. Currently supports the following features:

- Total CPU usage and usage on per CPU.
- CPU usage on per process.
- System available memory.
- System battery info.
- PSS usage on per process, map PSS to file.
- Monitor comredump file, compress and upload automatically.

## Dependencies
- libcurl android armeabi-v7a is built-in

## Build

```bash
$ git clone git@github.com:Rokid/yoda-monitor.git
$ cd yoda-monitor
$ cmake .
$ make
```

The above builds an executable `yoda-sixsix` in your CMake build directory.

## Command line arguments

| options          | description   |
|------------------|---------------|
| `-v`             | print version |
| `-conf`          | set configure json path |
| `-b`             | running in the background |
| `-l`             | set log file directory to write log to files |
| `-d`             | set log level, [verbose, info, warn, error, fatal] |

## Configure json structure
| options          | description   |
|------------------|---------------|
| `unzipRoot`      | temporary file directory, default value is `/tmp/` |
| `sysroot`        | set sysroot, default value is `/` |
| `uploadUrl`      | set coredump file upload server url, coredump will not upload if this value is not set |
| `coredumpDir`    | set coredump file generate directory, default value is `data`  |
| `disableUpload`  | set 1 to disable upload data |
| `smapInterval`   | set smap collect interval, default value is `1000` milliseconds |
| `smapSleep`      | set sleep time after collected smap, defaule value is `300 * 1000` milliseconds |
| `bufferCount`    | set ws message buf count, default count is 100 |
| `serverAddress`  | set ws server address |
| `serverPort`     | set ws server port |
| `sn`             | mock a sn, read from host by default |
| `hardware`       | mock a hardware type, read from host by default |

## Test

```bash
$ ./yoda-sixsix -conf ./sample/conf.json
```

## License

[Apache 2.0 license](https://www.apache.org/licenses/LICENSE-2.0)
