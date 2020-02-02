#pragma once

#include "job.h"
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <stdlib.h>

using namespace std;

extern int NODES_N;

class Timestamp {
public:

    int x;
    int free = NODES_N;
    vector<Job *> jobs;

    vector<pair<int, bool> > t;

    Timestamp(int _x) {
        x = _x;
        t.push_back(make_pair(0, true));
        t.push_back(make_pair(NODES_N, false));
    }

    Timestamp(int _x, int _free, vector<pair<int, bool> > t_prev) {
        x = _x;
        free = _free;

        for (int i = 0; i < t_prev.size(); ++i) {
            t.push_back(make_pair(t_prev[i].first, t_prev[i].second));
        }
    }

    Timestamp(int _x, int _free, vector<Job *> jobs_prev) {
        x = _x;
        free = _free;

        for (Job *j : jobs_prev) {
            jobs.push_back(j);
        }
    }

    void place_basic(Job *j) {
        free -= j->h;
        jobs.push_back(j);
    }

    void place(Job *j) {

        place_basic(j);

        bool exists = false;
        pair<int, bool> *pa;
        for (int i = 0; i < t.size(); ++i) {
            if (t[i].first == j->y) {
                exists = true;
                t[i].second = false;
                break;
            }
        }
        if (!exists) {
            t.push_back(make_pair(j->y, false));
        }

        exists = false;
        for (pair<int, bool> p : t) {
            if (p.first == j->y + j->h) {
                exists = true;
                pa = &p;
                break;
            }
        }

        if (!exists) {
            t.push_back(make_pair(j->y + j->h, true));
        }

        sort(t.begin(), t.end());

        for (int i = 0; i < t.size() - 2; ++i) {
            if (t[i].second == false && t[i + 1].second == false) {
                t.erase(t.begin() + i + 1);
                i--;
            }
        }
    }

    bool can_place(Job *job) {
        if (free < job->h) {
            return false;
        }

        int max_free = 0;

        for (int i = 0; i < t.size() - 1; ++i) {
            if (t[i].second && !t[i + 1].second) {
                max_free = max(max_free, t[i + 1].first - t[i].first);
            }
        }

        if (max_free < job->h) {
            return false;
        }

        return true;
    }

    void print() {
        printf("%s", print_string().c_str());
    }

    string print_string() {
        string s = "";
//        s += "x: " + to_string(x) + " | ";
        for (int j = 0; j < t.size(); ++j) {
            s += to_string(t[j].first) + "[" + to_string(t[j].second) + "] ";
        }
        return s;
    }

    vector<int> where_can_place(Job *job) {

        // returns a vector of y values of possible placements of job in this timestamp
        vector<int> res;
        for (int i = 0; i < t.size() - 1; ++i) {
            if (t[i].second && !t[i + 1].second) {

                int y = t[i].first;

                while (y + job->h <= t[i + 1].first) {
                    res.push_back(y);
                    y++;
                }
            }
        }

        return res;
    }
};

struct cmp_lower_bound {
    bool operator()(const Timestamp &t, unsigned int x) const {
        return t.x < x;
    }

    bool operator()(unsigned int x, const Timestamp &t) const {
        return x < t.x;
    }
};

class Timestamps {
public:

    vector<Timestamp> data;

    bool basic = false;

    Timestamps() {
        data.push_back(Timestamp(0));
    }

    void reconstruct(vector<Job> *jobs) {
        data.clear();
        data.push_back(Timestamp(0));
        for (Job &j : *jobs) {
            split_at(j.x);
            split_at(j.x + j.w);
            place_at(j.x, j.y, &j);
        }
    }

    void place_at(int x, int y, Job *j) {

        j->x = x;
        j->y = y;

        split_at(j->x + j->w);

        vector<Timestamp>::iterator it;
        it = lower_bound(data.begin(), data.end(), j->x, cmp_lower_bound());
        int i = it - data.begin();

        it = lower_bound(data.begin(), data.end(), j->x + j->w, cmp_lower_bound());
        int last = it - data.begin();

        for (; i < last; i++) {
            if (basic) {
                data[i].place_basic(j);
            }
            else {
                data[i].place(j);
            }
        }

        j->placed = true;
    }

    int job_fits_at(int x, Job *j) {

        vector<Timestamp> backup = data;

        vector<Timestamp>::iterator it;
        it = lower_bound(data.begin(), data.end(), x, cmp_lower_bound());
        int i = it - data.begin();

        vector<vector<int>> ys;

        int width = 0;
        while (i < data.size() && width <= j->w) {
            if (!data[i].can_place(j)) {
                return -1;
            }

            if (data[i].free != NODES_N) {
                ys.push_back(data[i].where_can_place(j));
            }

            if (i == data.size() - 1) {
                width += j->w;
            } else {
                width += data[i + 1].x - data[i].x;
            }
            i++;
        }

        if (ys.size() == 0) {
            return 0;
        }

        vector<int> res = common_ints(ys);

        return res.size() > 0 ? res[0] : -1;
    }

    int split_at(int x) {

        if (x > data.back().x) {
            if (basic) {
                Timestamp ts(x, data.back().free, data.back().jobs);
                data.push_back(ts);
            } else {
                Timestamp ts(x, data.back().free, data.back().t);
                data.push_back(ts);
            }
            return data.size() - 1;
        }

        vector<Timestamp>::iterator it;
        it = lower_bound(data.begin(), data.end(), x, cmp_lower_bound());
        int i = it - data.begin();

        for (; i < data.size(); ++i) {
            if (x == data[i].x) {
                return i;
            }
            if (x < data[i].x) {
                if (basic) {
                    Timestamp ts(x, data[i - 1].free, data[i - 1].jobs);
                    data.insert(data.begin() + i, ts);
                } else {
                    Timestamp ts(x, data[i - 1].free, data[i - 1].t);
                    data.insert(data.begin() + i, ts);
                }
                return i;
            }
        }

        // NOT REACHABLE
        cerr << "Error at splitting timestamps" << endl;
        exit(EXIT_FAILURE);
    }

    void print() {
        for (int i = 0; i < data.size(); ++i) {
            printf("i: %d, x: %d, free: %d | ", i, data[i].x, data[i].free);
            data[i].print();
            for (Job *j : data[i].jobs) {
                printf("%d ", j->index);
            }
            printf("\n");
        }
        printf("\n");
    }

    static vector<int> common_ints(vector<vector<int>> ys) {

        if (ys.size() == 1) {
            return ys[0];
        }

        while (ys.size() > 1) {
            for (int i = 0; i < ys.size(); ++i) {
                if (i != ys.size() - 1) {
                    vector<int> v(ys[i].size() + ys[i + 1].size());
                    vector<int>::iterator it, st;

                    it = set_intersection(ys[i].begin(),
                                          ys[i].end(),
                                          ys[i + 1].begin(),
                                          ys[i + 1].end(),
                                          v.begin());

                    vector<int> res;

                    for (st = v.begin(); st != it; ++st) {
                        res.push_back(*st);
                    }

                    ys[i] = res;

                    ys.erase(ys.begin() + i + 1);
                }
            }
        }

        return ys[0];
    }
};