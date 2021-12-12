#include <stdio.h>
#include "Setting.h"

int main() {
    struct coordinate* l_coordinate;
    struct coordinate* r_coordinate;
    struct gisinfo* DS_dms;
    int DS[32] = {0, };
    int count;
    char filename[100];

    l_coordinate = (struct coordinate*)malloc(sizeof(struct coordinate) * 32);
    r_coordinate = (struct coordinate*)malloc(sizeof(struct coordinate) * 32);
    DS_dms = (struct gisinfo*)malloc(sizeof(struct gisinfo) * 32);

    printf("filename: ");
#pragma warning(suppress : 4996)
    scanf("%[^\n]", &filename);

    count = getGIS(filename, l_coordinate, r_coordinate);
    getDistance(DS, count, l_coordinate, r_coordinate);

    getDMS(DS, count, DS_dms);

    int temp = 0;
    while (temp < count) {
        printf("(%03dd %02dm %02ds %c, %02dd %02dm %02ds %c) (%03dd %02dm %02ds %c, %02dd %02dm %02ds %c) %06d %03dd %02dm %02ds\n",
            l_coordinate[temp].longitude.degree, l_coordinate[temp].longitude.minute, l_coordinate[temp].longitude.second, l_coordinate[temp].longitude.direction, 
            l_coordinate[temp].latitude.degree, l_coordinate[temp].latitude.minute, l_coordinate[temp].latitude.second, l_coordinate[temp].latitude.direction, 
            r_coordinate[temp].longitude.degree, r_coordinate[temp].longitude.minute, r_coordinate[temp].longitude.second, r_coordinate[temp].longitude.direction,
            r_coordinate[temp].latitude.degree, r_coordinate[temp].latitude.minute, r_coordinate[temp].latitude.second, r_coordinate[temp].latitude.direction, 
            DS[temp], DS_dms[temp].degree, DS_dms[temp].minute, DS_dms[temp].second);

        temp++;
    }

    return 0;
}

