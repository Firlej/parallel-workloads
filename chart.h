#pragma once

#include "job.h"
#include <string>
#include <vector>

#define FOR(i, n) for(int i=0;i<n;++i)
#define FREE ((char)178)

using namespace std;

extern int NODES_N;

class Chart {
public:

    vector<Job> *jobs;

    int total_time = 0;

    char data[1000][1000];

    Chart(vector<Job> *JOBS) {
        jobs = JOBS;
        refresh();
    }

    void refresh() {
        total_time = 0;

        for (Job j : *jobs) {
            if (j.placed && total_time < j.x + j.w + 1) {
                total_time = j.x + j.w + 1;
            }
        }

        for (int j = NODES_N - 1; j >= 0; j--) {
            for (int i = 0; i < total_time; ++i) {
                data[j][i] = FREE;
            }
        }

        for (Job j : *jobs) {
            if (j.placed) {
                set(j.x, j.x + j.w, j.y, j.y + j.h, 'A' + j.index);
            }
        }
    }

    void set(int submit_time, int end_time, int nodes_start, int nodes_end, char c) {
        for (int i = submit_time; i < end_time; ++i) {
            for (int j = nodes_start; j < nodes_end; ++j) {
                data[j][i] = c;
            }
        }
    }

    void show() {
        refresh();
        printf("%s", print_string().c_str());
    }

    string print_string() {
        string s = "t | ";
        for (int i = 0; i < total_time; ++i) {
            s += to_string(i % 10);
        }
        s += "\n";
        for (int j = NODES_N - 1; j >= 0; j--) {
            s += to_string(j % 10) + " | ";
            for (int i = 0; i < total_time; ++i) {
                s += data[j][i];
            }
            s += "\n";
        }
        s += "\n";
        return s;
    }

};