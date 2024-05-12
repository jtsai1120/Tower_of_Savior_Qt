#include "combo_counter.h"
#include <QDebug>
#include <cstdlib> // abs();
using namespace std;
Combo_counter::Combo_counter() {
}

vector<Runestone_pair> Combo_counter::count(vector<vector<Runestone *> > runestones) {
    primary_pairs.clear();
    merged_pairs.clear();

// find vertical pairs
    for (int i = 0; i < 6; i++) {
        Runestone_pair *runestone_pair = new Runestone_pair;
        for (int j = 0; j < 5; j++) {
            pair<int,int> runestone = make_pair(j, i);
            QString color = runestones[j][i]->get_color();
            if (runestone_pair->pair.empty()) {
                runestone_pair->pair.push_back(runestone);
                runestone_pair->color = color;
            } else if (color == runestone_pair->color) {
                runestone_pair->pair.push_back(runestone);
            } else {
                if (runestone_pair->pair.size() >= 3)
                    primary_pairs.push_back(*runestone_pair);
                delete runestone_pair;
                runestone_pair = new Runestone_pair;
                runestone_pair->pair.push_back(runestone);
                runestone_pair->color = color;
            }
        }
        if (runestone_pair->pair.size() >= 3)
            primary_pairs.push_back(*runestone_pair);
        delete runestone_pair;
    }

// find horizontal pairs
    for (int j = 0; j < 5; j++) {
        Runestone_pair *runestone_pair = new Runestone_pair;
        for (int i = 0; i < 6; i++) {
            pair<int,int> runestone = make_pair(j, i);
            QString color = runestones[j][i]->get_color();
            if (runestone_pair->pair.empty()) {
                runestone_pair->pair.push_back(runestone);
                runestone_pair->color = color;
            } else if (color == runestone_pair->color) {
                runestone_pair->pair.push_back(runestone);
            } else {
                if (runestone_pair->pair.size() >= 3)
                    primary_pairs.push_back(*runestone_pair);
                delete runestone_pair;
                runestone_pair = new Runestone_pair;
                runestone_pair->pair.push_back(runestone);
                runestone_pair->color = color;
            }
        }
        if (runestone_pair->pair.size() >= 3)
            primary_pairs.push_back(*runestone_pair);
        delete runestone_pair;
    }

// merge pairs in color
    while(!primary_pairs.empty()) {
        bool flag = false;
        // 從第二個開始拿來跟第一個比對
        for (int i = 1; i < int(primary_pairs.size()); i++) {
            if (check_overlap_or_neighbor(primary_pairs[0], primary_pairs[i])) {
                // 如果 overlap 或是 neighor 就把第 i 個合併到第一個當中，
                // 再刪除第 i 個後 continue，再開始讓其他人跟第一個比對，依此循環;
                for (pair<int,int> pii : primary_pairs[i].pair) {
                    primary_pairs[0].pair.push_back(pii);
                }
                primary_pairs.erase(primary_pairs.begin() + i);
                flag = true;
                break;
            }
        }
        if (flag) {
            continue;
        } else {
            // 如果沒有 continue 的話(代表第一個不能再融合)就把第一個加到 merge_pairs 當中。
            merged_pairs.push_back(primary_pairs[0]);
            primary_pairs.erase(primary_pairs.begin());
        }
    }
    //show_pairs(merged_pairs);

    return merged_pairs;
}

bool Combo_counter::check_overlap_or_neighbor(Runestone_pair p1, Runestone_pair p2) {
    if (p1.color != p2.color) return false;
    for (auto r1 : p1.pair) {
        for (auto r2 : p2.pair) {
            if ( (abs(r1.first-r2.first)<=1 && r1.second == r2.second)
              || (abs(r1.second-r2.second)<=1 && r1.first == r2.first) )
                    return true;
        } // abs()=1 : neighbor, abs()=0 : overlap
    }
    return false;
}

void Combo_counter::show_pairs(vector<Runestone_pair> ps) {
    int count = 1;
    for (auto p: ps) {
        qDebug() << "pair " << count++ << ", color " << p.color;
        for (auto i: p.pair)
            qDebug().noquote() << "(" << i.first << ", " << i.second << ") ";
        qDebug();
    }
}
