// Eduards Žuga 231RDB009
#include <iostream>
#include <chrono>
#include <cmath>
#include <functional>
#include <iomanip>

using namespace std::chrono;
#pragma acc routine

float inline random(float min, float max)
{
    return min + static_cast<float>(rand()) / RAND_MAX * (max - min);
}

float montecarlo(
    const std::function<float(float)> &integrand,
    float xmin, float xmax,
    float ymin, float ymax,
    const size_t points)
{
    signed long long int count = 0;
    float y, f;
#pragma acc data copy(count)
#pragma acc parallel loop private(y, f) independent
    for (int i = 0; i < points; i++)
    {
        y = random(ymin, ymax);
        f = integrand(random(xmin, xmax));
        if (f > 0 && y < f && y > 0)
            count++;
        if (f < 0 && y > f && y < 0)
            count--;
    }
    return (float)count / (float)points * (xmax - xmin) * (ymax - ymin);
}

// Zemintegrāļa funkcija (x-0)*(x-0)*(x-9)
float func(float x) { return x * x * (x - 9); }

int main(int argc, char *argv[])
{
    std::cout << "Montekarlo metodes OpenACC integrācija uz HPC" << "\n";
    size_t points = 1'000;
    const double x[2] = {-1, 10};           // x_min, x_max
    const double y[2] = {-107.794125, 100}; // y_min, y_max
    std::cout << std::fixed << std::setprecision(6);

    while (points <= 1'000'000'000)
    {
        auto start = high_resolution_clock::now();
        auto integral = montecarlo(func, x[0], x[1], y[0], y[1], points);
        auto end = high_resolution_clock::now();
        auto elapsed = duration_cast<duration<double>>(end - start).count();
        std::cout << "Punktu skaits:\t" << points << "\n";
        std::cout << "Intervāls:\t" << "[" << x[0] << ", " << x[1] << "]" << "\n";
        std::cout << "Integrālis:\t" << integral << "\n";
        std::cout << "Laiks, s:\t " << elapsed << "\n";
        std::cout << "--------------------------\n";

        points *= 10;
    }
}