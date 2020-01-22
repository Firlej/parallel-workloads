#pragma once

#include "job.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

extern vector<Job> JOBS;

extern int JOBS_N;
extern int NODES_N;

void init(string filename, int limiter) {

    string maxjobsstr = "; MaxJobs: ";
    string maxjobsstr2 = ";       MaxJobs: ";
    string maxnodesstr = "; MaxNodes: ";
    string maxnodesstr2 = ";      MaxProcs: ";

    ifstream file(filename);
    string s;

    if (file.good()) {
        while (getline(file, s) && s[0] == ';') {
            if (s.substr(0, maxjobsstr.size()) == maxjobsstr) {
                JOBS_N = stoi(s.substr(maxjobsstr.size(), s.size() - maxjobsstr.size()));
            } else if (s.substr(0, maxnodesstr.size()) == maxnodesstr) {
                NODES_N = stoi(s.substr(maxnodesstr.size(), s.size() - maxnodesstr.size()));
            } else if (s.substr(0, maxjobsstr2.size()) == maxjobsstr2) {
                JOBS_N = stoi(s.substr(maxjobsstr2.size(), s.size() - maxjobsstr2.size()));
            } else if (s.substr(0, maxnodesstr2.size()) == maxnodesstr2) {
                NODES_N = stoi(s.substr(maxnodesstr2.size(), s.size() - maxnodesstr2.size()));
            }
        }

        int t[18] = {0};

        JOBS.resize(JOBS_N);

//        printf("%d %d\n", NODES_N, JOBS_N);

        int i = 0;

        do {
            stringstream ss;
            ss << s;

            for (int j = 0; j < 18; ++j) ss >> t[j];

            if (t[1] >= 0 && t[3] > 0 && t[4] > 0) {
                JOBS[i].index = i;
                JOBS[i].number = t[0];
                JOBS[i].submit_time = t[1];
                JOBS[i].w = t[3];
                JOBS[i].h = t[4];
                i++;
            }

            ss.str("");

        } while (getline(file, s) && i < limiter);

        JOBS_N = i;
        JOBS.resize(JOBS_N);

        file.close();
    } else {
        printf("Failed to open file.\n");
    }
}