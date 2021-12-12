#pragma once


template <typename T>
std::vector<T> operator+(std::vector<T> vec1, std::vector<T> vec2)
{
    std::vector<T> prod(vec1.size());
    std::transform(vec1.begin(), vec1.end(), vec2.begin(), prod.begin(), std::plus<T>());
    return prod;
}

template <typename T>
std::vector<T> operator-(std::vector<T> vec1, std::vector<T> vec2)
{
    std::vector<T> prod(vec1.size());
    std::transform(vec1.begin(), vec1.end(), vec2.begin(), prod.begin(), std::minus<T>());
    return prod;
}

template <typename T>
std::vector<T> operator*(std::vector<T> vec1, T skal)
{
    std::vector<T> prod(vec1.size());
    std::transform(vec1.begin(), vec1.end(), prod.begin(), [skal](T& el) { return el * skal; });
    return prod;
}

template <typename T>
std::vector<T> operator*(T skal, std::vector<T> vec1)
{
    std::vector<T> prod(vec1.size());
    std::transform(vec1.begin(), vec1.end(), prod.begin(), [skal](T& el) { return el * skal; });
    return prod;
}

template <typename T>
T skalar_prod(std::vector<T> vec1, std::vector<T> vec2)
{
    T sum = 0;
    for (int i = 0; i < vec1.size(); i++)
    {
        sum += vec1[i] * vec2[i];
    }
    return sum;
}

template <typename T>
T norm_squared(std::vector<T> vec)
{
    return skalar_prod(vec, vec);
}

float norm(std::vector<float> vec)
{
    return sqrt(norm_squared(vec));
}


template <typename T>
void print_vec(std::vector<T> vec)
{
    for (auto el : vec)
    {
        std::cout << el << " ";
    }
}