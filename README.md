# Yoda Monitor

Collect and upload system data in real time. Currently supports the following features:

- `top` data.
- system memory.
- process smap for PSS.

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
| `-version`       | print version |
| `-sysroot`       | specific samples sysroot |
| `-taskJson`      | start task from local json path |
| `-disableUpload` | set 1 to disable upload data |
| `-serverAddress` | specific server address |
| `-serverPort`    | specific server port |
| `-sn`            | mock sn |

## Test

```bash
$ ./yoda-sixsix -sysroot ./sample -taskJson ./sample/yoda_sisix_conf.json
```

## Liscense

[Apache 2.0 license](https://www.apache.org/licenses/LICENSE-2.0)
