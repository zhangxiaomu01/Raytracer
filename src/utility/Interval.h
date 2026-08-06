#ifndef INTERVAL_H
#define INTERVAL_H

#include "CommonUtil.h"

class Interval {
public:
    double m_min;
    double m_max;

    Interval() : m_min(G_INFINITY / 2), m_max(-G_INFINITY / 2) {}
    Interval(double min, double max) : m_min(min), m_max(max) {}

    double Size() const {
        return m_max - m_min;
    }

    bool Overlaps(const Interval& other) const {
        return m_max >= other.m_min && m_min <= other.m_max;
    }

    bool Contains(double x) const {
        return m_min <= x && x <= m_max;
    }

    bool Surrounds(double x) const {
        return m_min < x && x < m_max;
    }

    static const Interval empty, universe;

};

#endif