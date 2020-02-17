#include "read.h"
#include "chart.h"
#include "timestamp.h"
#include "arrangement.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <time.h>
#include <queue>

#define FOR(i, n) for(int i=0;i<n;++i)
#define FREE ((char)178)

using namespace std;

int JOBS_N;
int NODES_N;
bool DEBUG = false;
bool BENCHAMARK = true;

vector<Job> JOBS;

string exec(const char *cmd) {
    array<char, 256> buffer;
    string result;
    unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

int main(int argc, char *argv[]) {

    ios::sync_with_stdio(false);
    srand(time(NULL));

    string filename = "test.txt";
//    filename = "LANL-CM5-1994-4.1-cln.swf";
//    filename = "SDSC-SP2-1998-3.1-cln.swf";
//    filename = "DAS2-fs0-2003-1.swf";
//    filename = "Firlej2ship/test2.txt";
//    filename = "LPC-EGEE-2004-1.2-cln.swf";
//    filename = "LLNL-Thunder-2007-1.1-cln.swf"; // 29014 jobs make this program crash at sorting jobs after reading.
//    filename = "SHARCNET-2005-2.swf";
    int limiter = INT32_MAX;

    filename = argc >= 2 ? argv[1] : filename;
    limiter = argc >= 3 ? atoi(argv[2]) : limiter;
    string outfilename = argc >= 4 ? argv[3] : "output.txt";
    string mode = argc >= 5 ? argv[4] : "optimized";
    if (mode != "basic" && mode != "greedy" && mode != "optimized") {
        cerr << "Available mode values: basic greedy optimized" << endl;
        exit(EXIT_FAILURE);
    }
    int time_limit = argc >= 6 ? atoi(argv[5]) : 1;

//    if (BENCHAMARK) {
//        Arrangement::print_stats_headers();
//        for (limiter = 1; limiter <= 1000000000; limiter += 1000) {
//            JOBS.clear();
//            init(filename, limiter);
//
//            sort(JOBS.begin(), JOBS.end(), Job::cmp);
//
//            FOR(i, JOBS.size()) {
//                JOBS[i].index = i;
//            }
//
//            Arrangement a(JOBS);
//            a.greedy();
//            a.print_stats();
//        }
//
//        return 0;
//    }

    init(filename, limiter);
    sort(JOBS.begin(), JOBS.end(), Job::cmp);
    FOR(i, JOBS.size()) {
        JOBS[i].index = i;
//        JOBS[i].submit_time = 0;
    }

    Arrangement::print_stats_headers();
    Arrangement a(JOBS);
    if (mode == "basic") {
        a.basic();
    } else if (mode == "greedy") {
        a.greedy();
    } else if (mode == "optimized") {
        a.arrange(time_limit);
    }
    a.print_stats();
    a.output_to_file(outfilename);

//    string cmd = "chk-pata.exe " + filename + " " + outfilename + " 1";
//    cout << endl << exec(cmd.c_str());

    return 0;
}