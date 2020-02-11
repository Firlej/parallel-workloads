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
    limiter = 1000;
    // for 60 Cmax 28959

    filename = argc >= 2 ? argv[1] : filename;
    limiter = argc >= 3 ? atoi(argv[2]) : limiter;
    string outfilename = argc >= 4 ? argv[3] : "output.txt";
    int time_limit = argc >= 5 ? atoi(argv[4]) : 1;

//    if (BENCHAMARK) {
//        printf("limiter a_time a_cmax a_sumcj b_time b_cmax b_sumcj BEST_cmax BEST_sumcj\n");
//        Arrangement::print_stats_headers();
//        for (limiter = 0; limiter <= 40000; limiter += 250) {
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
//            a.basic();
////            a.print_stats();
//            Arrangement b(JOBS);
//            b.greedy();
//
//            printf("%d %.2f %d %lld %.2f %d %lld %d %lld\n", JOBS.size(), a.time, a.cmax, a.sumcj, b.time, b.cmax, b.sumcj, b.theoretical_cmax, b.theoretical_sumcj);
//        }
//
//        return 0;
//    }

    init(filename, limiter);
    sort(JOBS.begin(), JOBS.end(), Job::cmp);
    FOR(i, JOBS.size()) {
        JOBS[i].index = i;
    }

    Arrangement a(JOBS);
    a.arrange(time_limit);
    a.print_stats();
    a.output_to_file(outfilename);

//    string cmd = "chk-pata.exe " + filename + " " + outfilename + " 1";
//    cout << endl << exec(cmd.c_str());

    return 0;
}