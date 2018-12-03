# Yoda Monitor
Collect and upload system data in real time. Currently supports the following features:
- collect top data
- collect system memory
- collect application smap

## Build
```bash
 git clone git@github.com:Rokid/yoda-monitor.git
 cd yoda-monitor
 cmake .
 make
```

## Arguments
```text
-version print version
-sysroot to specific samples sysroot
-taskJson to start task from local json path
-disableUpload set 1 to disable upload data
-serverAddress specific server address
-serverPort specific server port
-sn mock an sn
```

## test
```bash
./yoda-sixsix -sysroot ./sample -taskJson ./sample/yoda_sisix_conf.json
```

##Liscense
[Apache 2.0 license](https://www.apache.org/licenses/LICENSE-2.0)