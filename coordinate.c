#include <stdio.h>
#include <stdlib.h>
#include "Setting.h"

int getGIS(char* filename, struct coordinate* a, struct coordinate* b) {
#pragma warning(suppress : 4996)
    FILE* fp = fopen(filename, "r");
    int cur = 0;
    while (!feof(fp)) {
#pragma warning(suppress : 4996)
        fscanf(fp, "(%d%c, %d%c) (%d%c, %d%c) \n",
            &a[cur].longitude.dms, &a[cur].longitude.direction,
            &a[cur].latitude.dms, &a[cur].latitude.direction,
            &b[cur].longitude.dms, &b[cur].longitude.direction,
            &b[cur].latitude.dms, &b[cur].latitude.direction);
        cur++;
    }
    fclose(fp);
    initGIS(cur, a, b);
    return cur;
}

int initGIS(int cur, struct coordinate* l, struct coordinate* r) {
    int temp;
    while(cur--) {
        temp = l[6].longitude.dms;
        temp = l[cur].longitude.dms;
        l[cur].longitude.degree = temp / 10000; temp %= 10000;
        l[cur].longitude.minute = temp / 100; temp %= 100;
        l[cur].longitude.second = temp;
        l[cur].longitude.totalSec = l[cur].longitude.degree * 3600 + l[cur].longitude.minute * 60 + l[cur].longitude.second;
        if (l[cur].longitude.direction == 'W')
            l[cur].longitude.totalSec *= -1;

        temp = l[6].longitude.dms;
        temp = l[cur].latitude.dms;
        l[cur].latitude.degree = temp / 10000; temp %= 10000;
        l[cur].latitude.minute = temp / 100; temp %= 100;
        l[cur].latitude.second = temp;
        l[cur].latitude.totalSec = l[cur].latitude.degree * 3600 + l[cur].latitude.minute * 60 + l[cur].latitude.second;
        if (l[cur].latitude.direction == 'S')
            l[cur].latitude.totalSec *= -1;

        temp = l[6].longitude.dms;
        temp = r[cur].longitude.dms;
        r[cur].longitude.degree = temp / 10000; temp %= 10000;
        r[cur].longitude.minute = temp / 100; temp %= 100;
        r[cur].longitude.second = temp;
        r[cur].longitude.totalSec = r[cur].longitude.degree * 3600 + r[cur].longitude.minute * 60 + r[cur].longitude.second;
        if (r[cur].longitude.direction == 'W')
            r[cur].longitude.totalSec *= -1;

        temp = l[6].longitude.dms;
        temp = r[cur].latitude.dms;
        r[cur].latitude.degree = temp / 10000; temp %= 10000;
        r[cur].latitude.minute = temp / 100; temp %= 100;
        r[cur].latitude.second = temp;
        r[cur].latitude.totalSec = r[cur].latitude.degree * 3600 + r[cur].latitude.minute * 60 + r[cur].latitude.second;
        if (r[cur].latitude.direction == 'S')
            r[cur].latitude.totalSec *= -1;
    }
}
