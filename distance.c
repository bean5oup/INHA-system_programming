#include "Setting.h"

int getDistance(int* DS, int count, struct coordinate* l, struct coordinate* r) {
	while (count--) {
		if ((l[count].latitude.totalSec > 0 && r[count].latitude.totalSec < 0) || (l[count].latitude.totalSec < 0 && r[count].latitude.totalSec > 0))
			DS[count] = l[count].latitude.totalSec + r[count].latitude.totalSec;
		else if (l[count].latitude.totalSec > r[count].latitude.totalSec)
			DS[count] = l[count].latitude.totalSec - r[count].latitude.totalSec;
		else
			DS[count] = r[count].latitude.totalSec - l[count].latitude.totalSec;

		if ((l[count].longitude.totalSec > 0 && r[count].longitude.totalSec < 0) || (l[count].longitude.totalSec < 0 && r[count].longitude.totalSec > 0))
			DS[count] += l[count].longitude.totalSec + r[count].longitude.totalSec;
		else if (l[count].longitude.totalSec > r[count].longitude.totalSec)
			DS[count] += l[count].longitude.totalSec - r[count].longitude.totalSec;
		else
			DS[count] += r[count].longitude.totalSec - l[count].longitude.totalSec;
	}
}
