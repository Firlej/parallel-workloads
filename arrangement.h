#pragma once

#include "job.h"
#include "timestamp.h"
#include "chart.h"

#include <fstream>
#include <string>
#include <vector>
#include <time.h>

#define FOR(i, n) for(int i=0;i<n;++i)

using namespace std;

int gcd(int a, int b) {

    if (a == 0)
        return b;
    return gcd(b % a, a);
}

class Arrangement {
public:

    vector<Job> jobs;
    int job_skip_count = 0;

    Timestamps t;

    Chart c;

    clock_t start = clock();

    clock_t improvements_start = clock();

    float time = 0;
    long long int pcmax = 0;
    long long int cmax = 0;
    long long int theoretical_cmax = 0;
    long long int psumcj = 0;
    long long int sumcj = 0;
    long long int theoretical_sumcj = 0;

    Arrangement(vector<Job> JOBS) : c(&jobs) {
        jobs = JOBS;

        for (Job j : jobs) {
            theoretical_cmax = max(theoretical_cmax, (long long int) j.submit_time + j.w);
            theoretical_sumcj += j.submit_time + j.w;
        }
    }

    void arrange(int time_limit) {

        greedy();

        improvements_start = clock();

        t = Timestamps();
        t.basic = true;
        t.reconstruct(&jobs);

        for (int i = t.data.size() - 2; i >= 0; i--) {
            if (t.data[i].free == NODES_N) {
                break;
            } else {
                for (Job *j : t.data[i].jobs) {
                    j->skip = false;
                }
            }
        }

        job_skip_count = 0;
        for (Job &j : jobs) {
            if (!j.skip) {
                break;
            }
            job_skip_count++;
        }

//        jobs_print();

        printf("Skipping %d jobs out of %d in further optimisations.\n", job_skip_count, jobs.size());
        job_skip_count = 0;
        Arrangement::print_stats_headers();
        calc_stats();
        print_stats();

//        c.refresh();
//        c.show();

        vector<int> heights;
        for (Job &j : jobs) {
            if (find(heights.begin(), heights.end(), j.h) == heights.end()) {
                heights.push_back(j.h);
            }
        }
        sort(heights.begin(), heights.end());
        heights.resize(distance(heights.begin(), unique(heights.begin(), heights.end())));
        int dy = heights[0];
        for (auto h : heights) {
            dy = gcd(dy, h);
        }

        int count = 0;

        clock_t last_improved = clock();

        while (true) {

            Job *j = random_job();

            if (j->h != NODES_N) {

                while (j->y + dy < 0 || j->y + j->h + dy > NODES_N) {
                    dy = rand() % 2 == 0 ? dy * -1 : dy * 1;
                }

                bool moved_vertically = true;
                do {
                    j->y += dy;
                    for (Job &b : jobs) {
                        if (j->y < 0 || j->y + j->h > NODES_N || (j->index != b.index && Job::overlap(*j, b))) {
                            j->y -= dy;
                            moved_vertically = false;
                            break;
                        }
                    }
                } while (moved_vertically);
            }

            if (j->submit_time != j->x) {
                bool moved_left = true;
                do {
                    j->x--;
                    for (Job &b : jobs) {
                        if (j->x < j->submit_time || (j->index != b.index && Job::overlap(*j, b))) {
                            j->x++;
                            moved_left = false;
                            break;
                        }
                    }
                } while (moved_left);
            }

            calc_stats();
            if (improved()) {
                last_improved = clock();
//                printf("Count: %d\n", count);
                if (pcmax != cmax || true) {
                    //     print_stats();
                    //    c.refresh();
                    //    c.show();
                }
            }

            count++;

            if (time > time_limit) {
//                jobs_print();
                break;
            }
        }
    }

    void greedy() {

        start = clock();
        for (Job &j : jobs) {

            t.data.erase(t.data.begin(), t.data.begin() + t.split_at(j.submit_time));

            int i = 0;
            while (true) {
                pair<bool, int> result = t.job_fits_at(i, &j);
                if (result.first) {
                    t.place_at(t.data[i].x, result.second, &j);
                    break;
                }
                if (result.second == i) {
                    i++;
                } else {
                    i = result.second + 1;
                }
            }
        }
        calc_stats();
    }

    void basic() {
        start = clock();
        FOR(i, jobs.size()) {
            Job *j = &jobs[i];

            int ti = t.data.size() - 1;
            if (t.data[ti].x >= j->submit_time) {
                if (t.data[ti].can_place(j)) {
                    t.data[ti].place(j);
                    j->place(t.data[ti].x, NODES_N - t.data[ti].free - j->h);
                    t.data.push_back(Timestamp(t.data[ti].x + j->w));
                }
            } else {
                t.split_at(j->submit_time);
                i--;
            }
        }
        calc_stats();
    }

    Job *random_job() {
        if (job_skip_count == jobs.size()) {
            job_skip_count = 0;
        }
        return &jobs[(rand() % (jobs.size() - job_skip_count)) + job_skip_count];
    }

    void calc_stats() {
        time = (float) (clock() - start) / CLOCKS_PER_SEC;
        pcmax = cmax;
        psumcj = sumcj;
        cmax = 0;
        sumcj = 0;
        for (Job &j : jobs) {
            cmax = max(cmax, (long long int) j.x + j.w);
            sumcj += j.x + j.w;
        }
    }

    bool improved() {
        return (pcmax != cmax || psumcj != sumcj) ? true : false;
    }

    static void print_stats_headers() {
        printf("JOBS_N TIME SUMCJ BEST_SUMCJ CMAX BEST_CMAX\n");
    }

    void print_stats() {
        printf("%d %.2f %lld %lld %lld %lld\n", JOBS_N, time, sumcj, theoretical_sumcj, cmax, theoretical_cmax);
    }

    void jobs_print() {
        for (Job j : jobs) {
            j.print();
        }
        printf("\n");
    }

    void output_to_file(string filename) {
        std::ofstream outfile(filename);

        FOR(i, jobs.size()) {
            outfile << jobs[i].str();
        }
        outfile.close();
    }
};

float seconds_passed(clock_t clk) {
    return (float) (clock() - clk) / CLOCKS_PER_SEC;
}