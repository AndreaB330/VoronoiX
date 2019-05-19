#include <iostream>
#include <fstream>
#include <algorithm>
#include <set>
#include <ctime>
#include <cmath>
#include "src/core_impl.hpp"
#include <vx/model/polygon.hpp>
using namespace std;
using namespace vx;

int main() {
    ios::sync_with_stdio(false);
    vector<Vec2> v;
    set<pair<int, int>> c;
    srand(0);
    for (int i = 0; i < 1000; i++) {
        int x = rand();
        int y = rand();
        if (c.count({x, y})) {
            continue;
        }
        c.insert({x, y});
        v.emplace_back(x, y);
    }
    cerr << clock() * 1.0 / CLOCKS_PER_SEC << endl;
    core_impl core = core_impl();
    core.fit(v);
    core.add_crop(0, RAND_MAX, 0, RAND_MAX);
    core.lloyd_relaxation(1.0);
    core.lloyd_relaxation(1.0);
    cerr << clock() * 1.0 / CLOCKS_PER_SEC << endl;
    cerr << "Built" << endl;
    for (int i = 0; i < v.size(); i++) v[i] = core.get_center(i);
    for (int i = 0; i < 100000; i++) {
        int x = rand()/2 + 1000;
        int y = rand()/2 + 1000;
        Vec2 u = Vec2(x,y);
        double minimal = 1e19;
        for (auto v : v) minimal = min(minimal, u.dist_square(v));
        vector<vector<int>> ff;
        vector<vector<Vec2>> cc;
        auto f = core.get_center(core.get_nearest_face(u,ff,cc));
        double found = f.dist_square(u);
        if (found != minimal && f.x > 50) {
            cout << endl;
            cout << "pos: " << u << endl;
            cout << "found: " << f << endl;
            cout << i << " " << found << " != " << minimal << endl;
        }
    }

    return 0;
}