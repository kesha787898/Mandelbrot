#include <cstdio>
#include "matplotlibcpp.h"
#include <complex>
#include <mutex>

namespace plt = matplotlibcpp;
const bool NEED_CSV = false;
float X_MIN = -1.294;
float X_MAX = -1.292;
float Y_MIN = -0.440;
float Y_MAX = -0.438;
/*float X_MIN = -1.5;
float X_MAX = -1;
float Y_MIN = -0.25;
float Y_MAX = 0.25;*/


struct Point {
    float x;
    float y;
    float z = 0;

    Point(float x, float y) {
        this->x = x;
        this->y = y;
    }

    Point(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

bool is_in_cardioid(float ix, float iy) {
    float theta = atan2(iy, ix - 0.25f);
    float p_card = (1 - cos(theta)) / 2;
    float p = powf((ix - 0.25f), 2) + powf(iy, 2);
    return p <= powf(p_card, 2);
}

float iterable_check(float ix, float iy, int MAX_ITER) {
    if (is_in_cardioid(ix, iy)) {
        return 0;
    }
    float zx = 0;
    float zy = 0;

    for (int iter = 0; iter < MAX_ITER; iter++) {
        if ((zx >= 2) or (zy >= 2)or(zy <= -2)or(zx <= -2)) {
            return (float) iter / MAX_ITER;
        }
        if (powf(zx,2)+powf(zy,2) >= 4) {
            return (float) iter / MAX_ITER;
        }
        float next_zx = ix + (zx + zy) * (zx - zy);
        float next_zy = iy + 2 * zx * zy;
        zx = next_zx;
        zy = next_zy;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    int SPLITS_IN_E = 250;
    int MAX_ITER = 2000000;
    if (argc >= 3) {
        SPLITS_IN_E = atoi(argv[1]);;
        MAX_ITER = atoi(argv[2]);;
    }
    std::cout << "STARTED\n";
    unsigned int start_time = clock();
    fflush(stdout);
    std::vector<Point> points = {};

    std::cout << "Calculating\n";
    fflush(stdout);
    int x_splits =  fabs(X_MAX - X_MIN) * SPLITS_IN_E;
    int y_splits =  fabs(Y_MAX - Y_MIN) * SPLITS_IN_E;
    std::mutex lock;

//#pragma omp parallel for
    for (int i = 0; i < x_splits; i++) {
        float ix = ((float) i / (float) x_splits) * (X_MAX - X_MIN) + X_MIN;
        if (i % ((x_splits / 10)) == 0) {
            std::cout << "0";
            fflush(stdout);
        }

        for (int j = 0; j < y_splits; j++) {
            float iy = (float) j / (float) y_splits * (Y_MAX - Y_MIN) + Y_MIN;
            double check = iterable_check(ix, iy, MAX_ITER);
            if (check != 0) {
                //lock.lock();
                points.emplace_back(ix, iy, check);
                //lock.unlock();
            }
        }
    }
    std::cout << "\n";
    fflush(stdout);

    std::cout << (points.size()) << '\n';
    fflush(stdout);
    /*if (NEED_CSV) {
        std::ofstream csv;
        csv.open("data.csv");
        csv << "x,y,z,\n";
        for (int i = 0; i < total_z.size(); i++) {
            csv << total_x.at(i);
            csv << ",";
            csv << total_y.at(i);
            csv << ",";
            csv << total_z.at(i);
            csv << "\n";
        }
        csv.close();
    }*/
    std::vector<float> x = {};
    std::vector<float> y = {};
    std::vector<float> z = {};

    for (auto &i : points) {
        x.push_back(i.x);
        y.push_back(i.y);
        z.push_back(log(i.z));
    }
    unsigned int end_time = clock();
    unsigned int search_time = end_time - start_time;
    std::cout << "runtime = " << search_time / 1000000.0 << " seconds\n";
    printf("DRAWING\n");
    fflush(stdout);
    plt::scatter(x, y, z);
    plt::show();

    return 1;
}
