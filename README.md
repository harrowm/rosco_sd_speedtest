# rosco_sd_speedtest
Some simple read/write speed tests for the rosco_m68k SPI SD Card

To compile for the rosco_m68k use:
```shell
ROSCO_M68K_DIR=~/rosco_m68k make EXTRA_CFLAGS=-DROSCO=1
```

Compiling this on the Mac/PC will run a basic static test. With:

```c
#define DATASIZE 2*1024*1024
```

on line 26, the result should be:

```shell
% cc -o test test.c
% ./test
```
``` 
rosco_m68k SD Card speed test
Data file size used is 2048K
Expect each test to take about 81 seconds

Write speed: 193.025 Kps
Read speed: 302.065 Kps
``` 