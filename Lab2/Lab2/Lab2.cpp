#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <string>
#include <memory>

class IMatrix {
public:
    virtual ~IMatrix() = default;
    virtual void Import(const std::string& filename) = 0;
    virtual void Export(const std::string& filename) const = 0;
    virtual void Print() const = 0;
};

template <typename T = double>
class MatrixDense : public IMatrix {
private:
    unsigned _m, _n;
    T* data;

public:
    MatrixDense(unsigned m, unsigned n) : _m(m), _n(n), data(new T[m * n]()) {}
    ~MatrixDense() { delete[] data; }

    T& operator()(unsigned i, unsigned j) {
        if (i >= _m || j >= _n)
            throw std::out_of_range("Index out of range");
        return data[i * _n + j];
    }

    const T& operator()(unsigned i, unsigned j) const {
        if (i >= _m || j >= _n)
            throw std::out_of_range("Index out of range");
        return data[i * _n + j];
    }

    MatrixDense operator+(const MatrixDense& other) const {
        if (_m != other._m || _n != other._n)
            throw std::invalid_argument("Matrix dimensions do not match for addition");
        MatrixDense result(_m, _n);
        for (unsigned i = 0; i < _m * _n; ++i)
            result.data[i] = data[i] + other.data[i];
        return result;
    }

    void Import(const std::string& filename) override {
        std::ifstream file(filename);
        if (!file.is_open())
            throw std::runtime_error("Failed to open file for importing");

        file >> _m >> _n;
        delete[] data;
        data = new T[_m * _n];
        for (unsigned i = 0; i < _m * _n; ++i)
            file >> data[i];
        file.close();
    }

    void Export(const std::string& filename) const override {
        std::ofstream file(filename);
        if (!file.is_open())
            throw std::runtime_error("Failed to open file for exporting");

        file << _m << " " << _n << "\n";
        for (unsigned i = 0; i < _m; ++i) {
            for (unsigned j = 0; j < _n; ++j)
                file << (*this)(i, j) << " ";
            file << "\n";
        }
        file.close();
    }

    void Print() const override {
        for (unsigned i = 0; i < _m; ++i) {
            for (unsigned j = 0; j < _n; ++j)
                std::cout << (*this)(i, j) << " ";
            std::cout << "\n";
        }
    }
};

class MatrixDiagonal : public IMatrix {
private:
    unsigned _size;
    std::vector<double> data;

public:
    MatrixDiagonal(unsigned size) : _size(size), data(size, 0.0) {}

    double& operator()(unsigned i, unsigned j) {
        if (i >= _size || j >= _size)
            throw std::out_of_range("Index out of range");
        if (i != j)
            throw std::invalid_argument("Cannot access off-diagonal elements");
        return data[i];
    }

    const double& operator()(unsigned i, unsigned j) const {
        if (i >= _size || j >= _size)
            throw std::out_of_range("Index out of range");
        if (i != j)
            return 0.0; // Off-diagonal elements are implicitly zero
        return data[i];
    }

    void Import(const std::string& filename) override {
        std::ifstream file(filename);
        if (!file.is_open())
            throw std::runtime_error("Failed to open file for importing");

        file >> _size;
        data.resize(_size);
        for (unsigned i = 0; i < _size; ++i)
            file >> data[i];
        file.close();
    }

    void Export(const std::string& filename) const override {
        std::ofstream file(filename);
        if (!file.is_open())
            throw std::runtime_error("Failed to open file for exporting");

        file << _size << "\n";
        for (unsigned i = 0; i < _size; ++i)
            file << data[i] << " ";
        file.close();
    }

    void Print() const override {
        for (unsigned i = 0; i < _size; ++i) {
            for (unsigned j = 0; j < _size; ++j) {
                if (i == j)
                    std::cout << data[i] << " ";
                else
                    std::cout << 0 << " ";
            }
            std::cout << "\n";
        }
    }
};

void Test() {
    MatrixDense<> mat1(2, 2);
    mat1(0, 0) = 1; mat1(0, 1) = 2;
    mat1(1, 0) = 3; mat1(1, 1) = 4;

    std::cout << "Dense Matrix:\n";
    mat1.Print();

    MatrixDiagonal diag(3);
    diag(0, 0) = 5; diag(1, 1) = 10; diag(2, 2) = 15;

    std::cout << "\nDiagonal Matrix:\n";
    diag.Print();
}

int main() {
    Test();
    return 0;
}
