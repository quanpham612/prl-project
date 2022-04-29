#include "helper.h"

void showImage(cv::Mat im) {
    cv::imshow("show image", im);
    cv::waitKey(0);
    cv::destroyAllWindows();
}

double calExecTime(void (*func)(), int iters) {
    double sum, start, end;
    int i;
    sum = 0;
    for (i = 0; i < iters; ++i) {
        start = omp_get_wtime();
        func();
        end = omp_get_wtime();
        sum += (end-start);
    }
    return sum/iters;
}

void writeToCSVFile(std::string path, std::tuple<std::vector<std::string>, std::vector<double>> execTimeData) {
    std::vector<std::string> columnHeaders = std::get<0>(execTimeData);
    std::vector<double> execTimes = std::get<1>(execTimeData);
    std::fstream fout;
    fout.open(path, std::ios::out);
    int maxThreads = 4;
    // write header
    fout << " , ";
    for (int t = 1; t <= maxThreads; ++t) {
        std::string header = "threads=" + std::__cxx11::to_string(t);
        if (t != maxThreads) {
            fout << header << ", ";
        }
        else {
            fout << header << "\n";
        }
    }
    for (unsigned i = 0; i < execTimes.size(); ++i) {
        if (i % maxThreads == 0) {
            fout << columnHeaders.at(i/maxThreads) << ", ";
        }
        if ((i+1) % maxThreads != 0) {
            fout << execTimes.at(i) << ", ";
        }
        else {
            fout << execTimes.at(i) << "\n";
        }
    }
}