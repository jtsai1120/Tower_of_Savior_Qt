#ifndef COMBO_COUNTER_H
#define COMBO_COUNTER_H

#include "runestone.h"
#include "runestone_pair.h"
#include <vector>
using namespace std;

class Combo_counter {
public:
    Combo_counter();

    vector<Runestone_pair> count(vector<vector<Runestone*>> runestones);

    void show_pairs(vector<Runestone_pair> ps);

private:
    vector<Runestone_pair> primary_pairs; // 初步組合 (融合前)
    vector<Runestone_pair> merged_pairs;

    bool check_overlap_or_neighbor(Runestone_pair p1, Runestone_pair p2);
};

#endif // COMBO_COUNTER_H
