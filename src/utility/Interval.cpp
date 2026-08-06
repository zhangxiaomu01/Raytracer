#include "Interval.h"

const Interval Interval::empty = Interval(G_INFINITY, -G_INFINITY);
const Interval Interval::universe = Interval(-G_INFINITY, G_INFINITY);