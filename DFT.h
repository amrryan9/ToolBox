#pragma once
#include "matrix.h"
#include <cfenv>
#include <valarray>
typedef std::valarray<Complex> CArray;
enum class UNITARY{NO, YES};
class DFT :
    public Complex_matrix
{
public:
    DFT() {}
    DFT(Complex_matrix x)
    {
        for (size_t i = 0; i < x.Rows_Count; i++)
            for (size_t j = 0; j < x.Columns_Count; j++)
                this->AddItem(i, j, x.GetItem(i, j));
    }
    DFT(size_t N,UNITARY unitary=UNITARY::NO)
    {
        const double pi = 22.0/7.0;
        const std::complex<double> i(0, 1);
        double factor{ 1.0 };
        if (unitary == UNITARY::YES) factor = 1.0 / sqrt(static_cast<double>(N));
        std::feclearexcept(FE_ALL_EXCEPT);
        if (N > 0)
        {
            for (size_t n = 0; n < N; n++)
                for (size_t m = 0; m < N; m++)
                    this->AddItem(n, m, std::exp(i * (-2.0 * pi *( (n * m) % N )/ N))*factor);
            if(!MATH_ERREXCEPT)
            {
                cout << " Overflow " <<endl;
            }  
        }
        else
              cout << "ERROR : ZERO DIMENSIONS" << endl;
       
    }
    Complex_matrix operator*(const Complex_matrix& m)
    {
        return this->GetMatrix() * m;
    }
    Complex_matrix GetMatrix()
    {
        Complex_matrix m2;
        for (size_t i = 0; i < this->Rows_Count; i++)
            for (size_t j = 0; j < this->Rows_Count; j++)
                m2.AddItem(i, j, this->GetItem(i, j));
        return m2;
    }
    static CArray fft(CArray& x)
    {
        size_t N = x.size();
        if (N % 2 != 0)
        {
            cout << N << endl;
            cout << N % 2 << endl;
            x.resize(N + 1);
            x[N] = 0.0;
        }
        return Fft(x);
    }
    static CArray Fft(CArray& x)
    {
        const double pi = 22.0 / 7.0;
        size_t N = x.size();
        if (N <= 1) return x;
        int log2n = static_cast<int>(log2f(N));
        int n = 1 << log2n;
        CArray X;//result
        X.resize(N);
        for (unsigned int i = 0; i < n; ++i) {
            X[bitReverse(i, log2n)] = x[i];
        }

        for (int s = 1; s <= log2n; ++s) {
            int m = 1 << s;
            int m2 = m >> 1;
            Complex w(1, 0);
            Complex wm = std::polar<double>(1.0, -1 * (pi / m2));// *odd[k];//exp(-J * (pi / m2)); 
            for (int j = 0; j < m2; ++j)
            {
                for (int k = j; k < n; k += m) {
                    Complex t = w * X[k + m2];
                    Complex u = X[k];
                    X[k] = u + t;
                    X[k + m2] = u - t;
                }
                w *= wm;
            }
        }
        return X;
    }
    static Complex_matrix ConvertToMatrix(CArray& x)
    {
        Complex_matrix result;
        size_t i{ 0 };
        for (auto& v : x)
        {
            result.AddItem(i, 0, v);
            i++;
        }
        return result;
    }
    static CArray ConvertFromMatrix(Complex_matrix& x)
    {
        CArray result;
        if (x.Columns_Count == 1)
        {
            result.resize(x.Rows_Count);
            for (size_t i = 0; i < x.Rows_Count; i++)
                result[i] = x.GetItem(i, 0);
        }
        else if (x.Rows_Count == 1)
        {
            result.resize(x.Columns_Count);
            for (size_t i = 0; i < x.Columns_Count; i++)
                result[i] = x.GetItem(0, i);
        }
        else
        {
            cout << " ERROR NOT A VECTOR " << endl;
        }
        return result;
    }
    static Complex_matrix FFT(Complex_matrix& m)
    {
        CArray x = ConvertFromMatrix(m);
        CArray X = fft(x);
        return ConvertToMatrix(X);
    }
    static unsigned int bitReverse(unsigned int x, int log2n)
    {
        int n = 0;
        int mask = 0x1;
        for (int i = 0; i < log2n; i++) {
            n <<= 1;
            n |= (x & 1);
            x >>= 1;
        }
        return n;
    }
};

