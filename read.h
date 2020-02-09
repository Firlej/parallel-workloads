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

    vector<string> max_jobs_strings = {"; MaxJobs: ", ";       MaxJobs: ", ";         MaxJobs: "};
    vector<string> max_nodes_strings = {"; MaxNodes: ", ";      MaxProcs: ", ";        MaxProcs: ", "; MaxProcs: "};

    ifstream file(filename);
    string s;

    if (file.good()) {
        while (getline(file, s) && s[0] == ';') {
            for (string mjs  : max_jobs_strings) {
                if (s.substr(0, mjs.size()) == mjs) {
                    JOBS_N = stoi(s.substr(mjs.size(), s.size() - mjs.size()));
                }
            }
            for (string mns  : max_nodes_strings) {
                if (s.substr(0, mns.size()) == mns) {
                    NODES_N = stoi(s.substr(mns.size(), s.size() - mns.size()));
                }
            }
        }

        limiter = min(JOBS_N, limiter);

        int t[18] = {0};

        JOBS.resize(limiter);

        printf("Nodes: %d Jobs: %d\n", NODES_N, limiter);

        int i = 0;

        do {

            stringstream ss;
            ss << s;

            int j = 0;
            while (ss >> t[j]) {
//                printf("%d ", t[j]);
                j++;
            }
//            printf("\n");

            if (j == 18 && t[1] >= 0 && t[3] > 0 && t[4] > 0 && t[4] <= NODES_N && t[0] <= JOBS_N) {
                JOBS[i].index = i;
                JOBS[i].number = t[0];
                JOBS[i].submit_time = t[1];
                JOBS[i].w = t[3];
                JOBS[i].h = t[4];
                JOBS[i].print();
                i++;
            }

            ss.str("");

        } while (getline(file, s) && i < limiter);

        JOBS_N = i;
        JOBS.resize(JOBS_N);

        file.close();
    } else {
        cerr << "Failed to open file." << endl;
        exit(EXIT_FAILURE);
    }
}