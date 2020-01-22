#pragma once

#include <string>
#include <iostream>

using namespace std;

class Job {

public:

    int index = 0;
    int number = 0;
    int submit_time = 0;

    int x = 0;
    int y = 0;

    int w = 0;
    int h = 0;

    bool placed = false;

    bool skip = true;

    int first_timestamp = 0;

    Job(int _index, int _number, int _submit_time, int _w, int _h) {
        index = _index;
        number = _number;
        submit_time = _submit_time;

        w = _w;
        h = _h;

        placed = false;
    };

    Job() {
        //
    };

    void print() const {
        printf("%s", print_string().c_str());
    }

    string print_string() const {
        return "index: " + to_string(index) + ", number: " + to_string(number) + ", submit_time: " +
               to_string(submit_time) + ", x: " + to_string(x) + ", y: " + to_string(y) +
               " w: " + to_string(w) + " h: " + to_string(h) + ", placed: " + to_string(placed) + ", end_time: " +
               to_string(x + w) + ", skip: " + to_string(skip) + "\n";
    }

    string str() {
        string procs = "";
        for (int i = y; i < y + h; ++i) {
            procs += std::to_string(i) + " ";
        }
        return to_string(number) + " " + to_string(x) + " " + to_string(x + w) + " " + procs + "\n";
    }

    void place(int _x, int _y) {
        x = _x;
        y = _y;
        placed = true;
    }

    static bool cmp(const Job &a, const Job &b) {
        if (a.submit_time != b.submit_time)
            return a.submit_time < b.submit_time;
        if (a.w != b.w)
            return a.w > b.w;
        if (a.h != b.h)
            return a.h > b.h;
        return true;
    }

    static bool overlap(const Job &a, const Job &b) {
        if (a.x >= b.x + b.w || b.x >= a.x + a.w) {
            return false;
        }
        if (a.y >= b.y + b.h || b.y >= a.y + a.h) {
            return false;
        }
        return true;
    }
};