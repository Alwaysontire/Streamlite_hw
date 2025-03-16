#include <cmath>
#include <iostream>

class Complex {
   private:
    double re_;
    double im_;

   public:
    explicit Complex(double re = 0, double im = 0) : re_(re), im_(im) {};

    Complex operator+(const Complex& other) const {
        return Complex(re_ + other.re_, im_ + other.im_);
    }

    Complex operator-(const Complex& other) const {
        return Complex(re_ - other.re_, im_ - other.im_);
    }

    Complex operator*(const Complex& other) const {
        return Complex(re_ * other.re_ - im_ * other.im_, re_ * other.im_ + im_ * other.re_);
    }

    Complex operator/(const Complex& other) const {
        double tmp = other.re_ * other.re_ + other.im_ * other.im_;
        if (tmp == 0) {
            throw std::runtime_error("wait wait wait, divisioning by zero");
        }
        return Complex((re_ * other.re_ + im_ * other.im_) / tmp, (im_ * other.re_ - re_ * other.im_) / tmp);
    }

    Complex operator-() const {
        return Complex(-re_, -im_);
    }

    Complex operator+() const {
        return *this;
    }

    double Re() const {
        return this->re_;
    }

    double Im() const {
        return this->im_;
    }

    bool operator==(const Complex& other) const {
        if (other.im_ == this->im_ && other.re_ == this->re_) {
            return true;
        }
        return false;
    }

    bool operator!=(const Complex& other) const {
        if (other.im_ != this->im_ || other.re_ != this->re_) {
            return true;
        }
        return false;
    }
};
double Abs(const Complex& other) {
    return std::pow((other.Im() * other.Im()) + (other.Re() * other.Re()), 0.5);
}
Complex operator+(const Complex& other, double d) {
    return Complex(other.Re() + d, other.Im());
}

Complex operator+(double d, const Complex other) {
    return Complex(other.Re() + d, other.Im());
}
Complex operator-(const Complex& other, double d) {
    return Complex(other.Re() - d, other.Im());
}

Complex operator-(double d, const Complex other) {
    return Complex(d - other.Re(), other.Im());
}
Complex operator*(const Complex& other, double d) {
    return Complex(other.Re() * d, other.Im() * d);
}
Complex operator*(double d, const Complex& other) {
    return Complex(other.Re() * d, other.Im() * d);
}

Complex operator/(const Complex& other, double d) {
    return Complex(other.Re() / d, other.Im() / d);
}
Complex operator/(double d, const Complex& other) {
    denom = other.Re() * other.Re() + other.Im() * other.Im();
    return Complex(d * other.Re() / denom, -d * other.Im() / denom);
}

int main() {
    Complex fir(3, 1);
    Complex sec(1, 1);
    Complex thi = fir * sec;
    std::cout << thi.Re() << " + " << thi.Im() << "i" << '\n';
}
