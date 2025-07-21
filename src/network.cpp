#include "matrices.cpp"
#include <cstddef>
#include <functional>
#include <ostream>
#include <stdexcept>
#include <cmath>
#include <iostream>
#include <array>

template <size_t... Ns>
struct LastDim;
template <size_t N>
struct LastDim<N> {
    static constexpr size_t value = N;
};
template <size_t First, size_t... Rest>
struct LastDim<First, Rest...> {
    static constexpr size_t value = LastDim<Rest...>::value;
};
std::function<double(double)> differentiate(std::function<double(double)> f, double eps = 1e-5) {
    return [=](double x) {
        return (f(x+eps)-f(x-eps))/(2*eps);
    };
}
double relu(double x) {
    return x>0?x:0;
}
double sigmoid(double x) {
    return 1.0/(1.0+std::exp(-x));
}
double adjustedArctan(double x) {
    return std::atan(x)/M_PI+0.5;
}
double identity(double x) {
    return x;
}
double leakyrelu(double x) {
    return x>0?x:x/5.0;
}

double MSEloss(double diff) {
    return diff*diff;
}

template <size_t... dims>
class Network;
template <size_t N>
class Network<N> {
public:
    static constexpr size_t depth=1;
    static constexpr size_t final=N;
    std::function<double(double)> loss,lossd;
    Network(std::function<double(double)> act=identity,std::function<double(double)> loss=MSEloss): loss(loss),lossd(differentiate(loss)) {}
    Network& operator[](size_t idx) {
        if (idx != 0) throw std::out_of_range("Layer index out of range");
        return *this;
    }
    Matrix<1,final> eval(const Matrix<1,N>& input) {
        return input;
    }
    Matrix<1,N> learn(Matrix<1,N> input,const Matrix<1,N>& output) {
        for (size_t i=0;i<N;i++) {
            input[0][i]=lossd(input[0][i]-output[0][i]);
        }
        return input;
    }
};
template <size_t In,size_t Out,size_t... Rest>
class Network<In,Out,Rest...> {
public:
    static constexpr size_t depth=1+Network<Out,Rest...>::depth;
    static constexpr size_t final=LastDim<Out,Rest...>::value;
    Matrix<In,Out> weight;
    Matrix<1,Out> bias;
    Network<Out,Rest...> next;
    std::function<double(double)> activation, activationd;
    Network(std::function<double(double)> act=identity,std::function<double(double)> loss=MSEloss): activation(act),activationd(differentiate(act)) {
        weight=randomMatrix<In,Out>();
        bias=randomMatrix<1,Out>();
        next=Network<Out,Rest...>(act,MSEloss);
    }
    auto& operator[](size_t idx) {
        if (idx==0) return *this;
        return next[idx-1];
    }
    Matrix<1,final> eval(const Matrix<1, In>& input) {
        auto z=input*weight+bias;
        auto a=apply(z,activation);
        if constexpr (sizeof...(Rest)>0)
            return next.eval(a);
        else
            return a;
    }
    int totaleval(const Matrix<1, In>& input, int expected) {
        auto out=eval(input);
        size_t best=0;
        double vmax=out[0][0];
        for (size_t i=1;i<final;++i) {
            if (out[0][i]>vmax) {
                vmax=out[0][i];
                best=i;
            }
        }
        return best==expected?1:0;
    }
    Matrix<1,In> learn(const Matrix<1, In>& input,const Matrix<1,final>& finaloutput) {
        auto raw=input*weight+bias;
        auto activated=apply(raw,activation);
        auto der=next.learn(activated,finaloutput);
        auto y=apply(raw,activationd);

        auto c=multiplyElementWise(der,y);
        auto old=weight;
        weight-=(input.T()*c)*0.01;
        bias-=c*0.01;
        return c*old.T();
    }
};
