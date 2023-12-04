# rosco_sd_speedtest
Some simple read/write speed tests for the rosco_m68k SPI SD Card

To compile for the rosco_m68k use:
```shell
ROSCO_M68K_DIR=~/rosco_m68k make EXTRA_CFLAGS=-DROSCO=1
```

Compiling this on the Mac/PC will run a basic static test, the answer should be:
    Write speed: 96.513 Kps
    Read speed: 151.032 Kps
