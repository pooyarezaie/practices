// writen by Sadegh Jazayeri
//
#ifndef __POLYNOMIAL_HPP
#define __POLYNOMIAL_HPP

#include <iostream>
#include <vector>

template <typename T>
class basic_pol{
private:
    std::vector<T> coefficients;
public:
    size_t size()const;
    T operator[](int)const;
    T& operator[](int);

    basic_pol<T> operator+(const basic_pol&)const;
    basic_pol<T> operator-(const basic_pol&)const;
    basic_pol<T> operator*(const basic_pol&)const;
    basic_pol<T> operator-()const;

    void print(char)const;
};

template<typename T>
size_t basic_pol<T>::size()const{
    return coefficients.size();
}

template<typename T>
T basic_pol<T>::operator[](int i)const{
    if(coefficients.size() < i+1)
        return 0;
    return coefficients[i];
}

template<typename T>
T& basic_pol<T>::operator[](int i){
    if(coefficients.size() < i+1)
        coefficients.resize(i+1, 0);
    return coefficients[i];
}

template<typename T>
basic_pol<T> basic_pol<T>::operator+(const basic_pol<T>& p)const{
    basic_pol<T> result = p;
    for(int i = 0; i < size(); i++){
        result[i]+=coefficients[i];
    }
    return result;
}

template<typename T>
basic_pol<T> basic_pol<T>::operator-(const basic_pol<T>& p) const{
    return (*this)+(-p);
}

template<typename T>
basic_pol<T> basic_pol<T>::operator-()const{
    basic_pol<T> result = (*this);
    for(int i = 0; i < result.size(); i++){
        result[i]=-result[i];
    }
    return result;
}

template<typename T>
basic_pol<T> basic_pol<T>::operator*(const basic_pol<T>& p)const{
    basic_pol<T> result;
    for(int i = 0; i < size(); i++){
        for(int j = 0; j < p.size(); j++){
            result[i+j]+=coefficients[i]*p[j];
        }
    }
    return result;
}

template<typename T>
void basic_pol<T>::print(char var)const{
    for(int i = coefficients.size()-1; i >=0; i--){
        if(coefficients[i]==0)
            continue;
        std::cout<<(coefficients[i]>0?"+":"")<<coefficients[i]
            <<var<<"^"<<i;
    }
}

typedef basic_pol<long long> Polynomial;

#endif
