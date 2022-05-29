#pragma once

struct gisinfo {
    int degree;
    int minute;
    int second;
    int totalSec;
    int dms;
    char direction;
};

struct coordinate {
    struct gisinfo longitude;
    struct gisinfo latitude;
};

int getGIS(char* filename, struct coordinate* a, struct coordinate* b);
int getDistance(int* DS, int count, struct coordinate* l, struct coordinate* r);
int getDMS(int* DS, int count, struct gisinfo* dms);
