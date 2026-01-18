#include <iostream>
#include <vector>
#include <algorithm>

void sort_vector()
{
    using doubles = std::vector<double>;

    // Lambda to print vector
    auto pVec = [](const doubles &v)
    {
        for (auto &&x : v)
            std::cout << x << std::endl;
    };

    // Initialize vector
    doubles v{0, 3, 5, 4, 1, 2};

    std::cout << "--- Original ---" << std::endl;
    pVec(v);

    // Sort ascending
    std::sort(v.begin(), v.end());
    std::cout << "--- Sorted (Ascending) ---" << std::endl;
    pVec(v);

    // Sort descending
    std::sort(v.begin(), v.end(), [](double a, double b)
              { return a > b; });
    std::cout << "--- Sorted (Descending) ---" << std::endl;
    pVec(v);
}