// File to test the speed of reading and writing to a SD Cards on the rosco_m68k
// Malcolm Harrow November 2023
// MIT license

// To compile for the rosco_m68k use:
//     ROSCO_M68K_DIR=~/rosco_m68k make

// Compiling this on the Mac will run some basic test, the answer should be:
//      Write speed: 96.513
//      Read speed: 151.032

#include <stdio.h>
#include <stdlib.h>

#ifdef ROSCO
#define USE_FILELIB_STDIO_COMPAT_NAMES 1
#include <sdfat.h>
#define _TIMER_100HZ  0x40c
#define FILENAME "/testfile"
#else
#define FILENAME "testfile"
#endif


// Keep to 2Gb or less to avoid blowing up the integer math later
#define DATASIZE 2*1024*1024
#define TIMES_TO_WRITE 1

int gettimer() {
#ifdef ROSCO
    return *(int *)_TIMER_100HZ;
#else
    static int t = 0;
    t += 1;
    if (t == 1) return 0;
    if (t == 2) return 1061;
    if (t == 3) return 0;
    if (t == 4) return 678;
    
    return 0;
#endif
}

// Returns the number of 100ths of a second passed
int getduration(int start, int end) {
    return (end > start) ? (end - start) : (0xFFFF - start + end);
}

#ifdef ROSCO
int kmain() {
    if (!SD_check_support()) {
        printf("This program requires SD card support in firmware\n");
        exit(1);
    }

    if (!SD_FAT_initialize()) {
        printf("Failed to initialise SD Card Library\n");
        exit(1);
    }

    printf("Init SD Card ok\n");
#else
int main() {
#endif
    char dataset[DATASIZE];

    for (int i = 0; i<DATASIZE; i++) {
        dataset[i] = i & 0xFF;
    }

    FILE *fp = NULL;
    int res = 0;
    int start = 0;
    int end = 0;
    char buf[100];

    if ((fp = fopen(FILENAME, "w")) == NULL) {
        printf("Failed to open for write '%s'\n", FILENAME);
        exit(1);
    }

    printf("rosco_m68k SD Card speed test\n");
    printf("Data file size used is %dK\n", DATASIZE * TIMES_TO_WRITE / 1024);
    printf("Expect each test to take about %d seconds\n\n", DATASIZE * TIMES_TO_WRITE / 1024 / 25); // just a guess
    start = gettimer();

    res = fwrite(dataset, DATASIZE, TIMES_TO_WRITE, fp);
#ifdef ROSCO
    if (res != DATASIZE * TIMES_TO_WRITE) {
#else
    if (res != TIMES_TO_WRITE) {
#endif
        printf("Failed to write a complete dataset '%s'\n", FILENAME);
        exit(1);
    }

    end = gettimer();

    fclose(fp);

    // Now we have some integer maths and magic to try to get and answer we can print and our
    // printf by default doesnt understand floats

    uint32_t t_write = (DATASIZE * TIMES_TO_WRITE * 1000) / getduration(start, end);
    t_write = ((t_write * 1000 / 1024) + 5) / 10;
#ifdef ROSCO
    snprintf(buf, 30, "Write speed: %lu.%03lu Kps\n", t_write/1000, t_write%1000);
#else
    snprintf(buf, 30, "Write speed: %u.%03u Kps\n", t_write/1000, t_write%1000);
#endif
    printf("%s", buf);

    if ((fp = fopen(FILENAME, "r")) == NULL) {
        printf("Failed to open for read '%s'\n", FILENAME);
        exit(1);
    }

    start = gettimer();

    res = fread(dataset, DATASIZE, 1, fp);
#ifdef ROSCO
    if (res != DATASIZE * TIMES_TO_WRITE) {
#else
    if (res != TIMES_TO_WRITE) {
#endif
        printf("Failed to read a complete dataset '%s'\n", FILENAME);
        exit(1);
    }

    end = gettimer();

    fclose(fp);

    uint32_t t_read = (DATASIZE * TIMES_TO_WRITE * 1000) / getduration(start, end);
    t_read = ((t_read * 1000 / 1024) + 5) / 10;
#ifdef ROSCO
    snprintf(buf, 30, "Read speed: %lu.%03lu Kps\n", t_read/1000, t_read%1000);
#else
    snprintf(buf, 30, "Read speed: %u.%03u Kps\n", t_read/1000, t_read%1000);
#endif
    printf("%s", buf);
    
    return 0;
}
