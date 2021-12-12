#include "Setting.h"

int getDMS(int* DS, int count, struct gisinfo* dms) {
	int temp;
	while (count--) {
		temp = DS[count];
		dms[count].degree = temp / 3600; temp %= 3600;
		dms[count].minute = temp / 60; temp %= 60;
		dms[count].second = temp;
	}
}
