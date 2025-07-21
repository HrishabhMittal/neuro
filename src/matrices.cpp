#include <array>
#include <cstddef>
#include <functional>
#include <random>

double getRandom(double min, double max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(min,max);
    return dist(gen);
}
template<size_t x,size_t y>
class Matrix {
    std::array<std::array<double,y>,x> data{};
public:
    Matrix(const std::array<std::array<double,y>,x>& mat): data(mat) {}
    Matrix() {}
    std::array<double,y>& operator[](size_t index) {
        return data[index];
    }
    const std::array<double,y>& operator[](size_t index) const {
        return data[index];
    }
    Matrix<x,y> operator+(const Matrix<x,y>& mat) const {
        Matrix<x,y> out(*this);
        for (size_t i=0;i<x;i++) {
            for (size_t j=0;j<y;j++) {
                out[i][j]+=mat[i][j];
            }
        }
        return out;
    }
    Matrix<x,y>& operator-=(const Matrix<x,y>& mat) {
        for (size_t i=0;i<x;i++) for (size_t j=0;j<y;j++) {
            data[i][j]-=mat[i][j];
        }
        return *this;
    }
    Matrix<x,y> operator-(const Matrix<x,y>& mat) const {
        Matrix<x,y> out(*this);
        for (size_t i=0;i<x;i++) {
            for (size_t j=0;j<y;j++) {
                out[i][j]-=mat[i][j];
            }
        }
        return out;
    }
    Matrix<x,y> operator*(double d) const {
        Matrix<x,y> out(*this);
        for (size_t i=0;i<x;i++) {
            for (size_t j=0;j<y;j++) {
                out[i][j]*=d;
            }
        }
        return out;
    }
    Matrix<y,x> T() const {
        Matrix<y,x> out;
        for (int i=0;i<x;i++) for (int j=0;j<y;j++) {
            out[j][i]=data[i][j];
        }
        return out;
    }
    template<size_t z>
    Matrix<x,z> operator*(const Matrix<y,z>& mat2) const {
        Matrix<x,z> out;
        for (size_t i=0;i<x;i++) {
            for (size_t k=0;k<z;k++) {
                for (size_t j=0;j<y;j++) {
                    out[i][k]+=data[i][j]*mat2[j][k];
                }
            }
        }
        return out;
    }
};
template<size_t x,size_t y>
Matrix<x,y> apply(Matrix<x,y> mat,std::function<double(double)> func) {
    for (int i=0;i<x;i++) for (int j=0;j<y;j++) {
        mat[i][j]=func(mat[i][j]);
    }
    return mat;
}
template<size_t x,size_t y>
Matrix<x,y> multiplyElementWise(Matrix<x,y> mat1,const Matrix<x,y>& mat2) {
    for (int i=0;i<x;i++) for (int j=0;j<y;j++) {
        mat1[i][j]*=mat2[i][j];
    }
    return mat1;
}
template<size_t x,size_t y>
Matrix<x,y> randomMatrix() {
    Matrix<x,y> out;
    for (size_t i=0;i<x;i++) {
        for (size_t j=0;j<y;j++) {
            out[i][j]=getRandom(0.,1.);
        }
    }
    return out;
}
template<size_t x, size_t y>
Matrix<x,y> makeMatrix(const std::array<std::array<double, y>, x>& arr) {
    return Matrix<x,y>(arr);
}
