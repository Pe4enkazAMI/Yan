#include <iostream>
#include <vector>
#include <cmath>
using namespace std;


template <typename T>
class Polynomial {
private:
    std::vector<T> c;

    void deleteZeroes() {
        while (!c.empty() && c.back() == T(0)) {
            c.pop_back();
        }
    }

public:
    Polynomial(const std::vector<T>& coefs) : c(coefs) {
        deleteZeroes();
    }

    Polynomial(const T& coef = T()) : c(1, coef) {
        deleteZeroes();
    }

    template <typename It>
    Polynomial(It begin, It end) : c(begin, end) {
        deleteZeroes();
    }

    void print() const {
        for (size_t i = 0; i < c.size(); ++i) {
            std::cout << c[i] << " ";
        }
        std::cout << "\n";
    }

    int Degree() const {
        return static_cast<int>(c.size()) - 1;
    }

    T operator[] (size_t i) const {
        if (i >= c.size()) {
            return T(0);
        } else {
            return c[i];
        }
    }

    friend bool operator== (const Polynomial& a, const Polynomial& b) {
        return a.c == b.c;
    }
    friend bool operator!= (const Polynomial& a, const Polynomial& b) {
        return a.c != b.c;
    }

    Polynomial& operator+= (const Polynomial& target) {
        if (c.size() < target.c.size()) {
            c.resize(target.c.size());
        }
        for (size_t i = 0; i < target.c.size(); i++) {
            c[i] += target.c[i];
        }
        deleteZeroes();
        return *this;
    }
    Polynomial& operator-= (const Polynomial& target) {
        if (c.size() < target.c.size()) {
            c.resize(target.c.size());
        }
        for (size_t i = 0; i < target.c.size(); i++) {
            c[i] -= target.c[i];
        }
        deleteZeroes();
        return *this;
    }

    friend Polynomial operator+ (Polynomial a, const Polynomial& b) {
        return a += b;
    }
    friend Polynomial operator- (Polynomial a, const Polynomial& b) {
        return a -= b;
    }

    typename std::vector<T>::const_iterator begin() const {
        return c.begin();
    }
    typename std::vector<T>::const_iterator end() const {
        return c.end();
    }

    T operator() (const T& point) const {
        T value = T(0);
        for (int i = Degree(); i >=0; --i) {
            value = c[i] + point * value;
        }
        return value;
    }

    friend Polynomial operator* (Polynomial lhs, const Polynomial& rhs) {
        return lhs *= rhs;
    }

    Polynomial& operator*= (const Polynomial& target) {
        vector<T> tmp;
        tmp.resize(Degree() + target.Degree() + 1);
        for (int i = 0; i <= Degree(); i++) {
            for (int j = 0; j <= target.Degree(); j++) {
                tmp[i+j] += c[i]*target[j];
            }
        }
        c = move(tmp);
        return *this;
    }

    template <typename U>
    friend ostream& operator<< (ostream& out, const Polynomial<U>& pol);

    friend Polynomial operator& (const Polynomial& lhs, const Polynomial& rhs) {
        Polynomial tmp(lhs[0]);
        auto tmp1(rhs);
        for (int i = 1; i <= lhs.Degree(); ++i) {
            tmp += tmp1 * lhs[i];
            tmp1 *= rhs;
        }
        return tmp;
    }

    friend Polynomial operator/ (const Polynomial& lhs, const Polynomial& rhs) {
        if (lhs.Degree() < rhs.Degree()) {
            return T(0);
        }
        Polynomial tmp;
        tmp.c.resize(lhs.Degree() - rhs.Degree() + 1);
        Polynomial tmp1(lhs);
        while (tmp1.Degree() >= rhs.Degree()) {
            T x = tmp1[tmp1.Degree()] / rhs[rhs.Degree()];
            tmp.c[tmp1.Degree() - rhs.Degree()] = x;
            vector<T> jopa(tmp1.Degree() - rhs.Degree());
            jopa.push_back(x);
            tmp1 -= rhs * jopa;
        }
        tmp.deleteZeroes();
        return tmp;
    }

    friend Polynomial operator% (const Polynomial& lhs, const Polynomial& rhs) {
        auto tmp(lhs);
        tmp -= rhs * (tmp / rhs);
        return tmp;
    }

    friend Polynomial operator, (const Polynomial& lhs, const Polynomial& rhs) {
        if (lhs.Degree() < rhs.Degree()) {
            return rhs, lhs;
        }
        if (rhs.Degree() == -1) {
            return lhs / lhs[lhs.Degree()];
        }
        return rhs, (lhs % rhs);
    }
};

template <typename T>
void print(const Polynomial<T>& pol) {
    for (auto i : pol) {
        std::cout << i << " ";
    }
    std::cout << "\n";
}

template <typename U>
ostream& operator<< (ostream& out, const Polynomial<U>& pol) {
    if (pol.Degree() != -1) {
        for (int i = pol.Degree(); i >= 0; --i) {
            if (pol[i] == U(0)) {
                continue;
            }
            if (pol[i] > U(0) && i != pol.Degree()) {
                out << '+';
            }
            if (pol[i] < U(0)) {
                out << '-';
            }
            if (i == 0 || (pol[i] != U(-1) && pol[i] != U(1))) {
                pol[i] < U(0) ? out << -pol[i] : out << pol[i];
                if (i >= 1) {
                    out << '*';
                }
            }
            if (i >= 1) {
                out << 'x';
            }
            if (i >= 2) {
                out << "^" << i;
            }
        }
        return out;
    } else {
        out << U(0);
        return out;
    }
}
