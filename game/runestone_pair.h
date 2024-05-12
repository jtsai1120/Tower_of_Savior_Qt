#ifndef RUNESTONE_PAIR_H
#define RUNESTONE_PAIR_H

#include "runestone.h"
#include <vector>
#include <QString>

struct Runestone_pair {
    std::vector<pair<int,int>> pair; // row, col
    QString color;
};

#endif // RUNESTONE_PAIR_H
