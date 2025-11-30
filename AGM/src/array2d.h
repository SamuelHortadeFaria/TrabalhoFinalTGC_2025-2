#pragma once

/* 
 * Um array de 2 dimensões genérico com tamanho fixo
 * definido na instanciação.
 * Veja: https://pbr-book.org/4ed/Utilities/Containers_and_Memory_Management#x2DArrays
 * e https://github.com/mmp/pbrt-v4/blob/91bc6ca69bcdc597d1c8678df088034bf7fbc78c/src/pbrt/util/containers.h#L146
 * Podiam incluir uma classe assim na STL, né?
 */

template <typename T>
struct Array2D {

    int width, height;
    T* data;

    Array2D(int width, int height)
        : width(width), height(height), data(new T[width * height] {}) {}

    Array2D(int width, int height, T value)
        : width(width), height(height), data(new T[width * height] {value}) {}

    // Copia
    Array2D(const Array2D& other)
        : width(other.width), height(other.height),
          data(new T[other.width * other.height]) 
    {
        std::copy(other.data, other.data + width * height, data);
    }

    // Move
    Array2D(Array2D&& other) noexcept
        : width(other.width), height(other.height), data(other.data) 
    {
        other.data = nullptr;
        other.width = other.height = 0;
    }

    // Copia
    Array2D& operator=(const Array2D& other)
    {
        if (this != &other) {
            delete[] data;
            width = other.width;
            height = other.height;
            data = new T[width * height];
            std::copy(other.data, other.data + width * height, data);
        }
        return *this;
    }

    // Move
    Array2D& operator=(Array2D&& other) noexcept
    {
        if (this != &other) {
            delete[] data;
            width = other.width;
            height = other.height;
            data = other.data;
            other.data = nullptr;
            other.width = other.height = 0;
        }
        return *this;
    }

    ~Array2D()
    {
        delete[] data;
    }

    int idx(int x, int y) const
    {
	    return y * width + x;
    }

    T& operator()(int i)
    {
        return data[i];
    }

    const T& operator()(int i) const
    {
        return data[i];
    }

    T& operator()(int x, int y)
    {
        return data[idx(x, y)];
    }

    const T& operator()(int x, int y) const
    {
        return data[idx(x, y)];
    }
};
