#pragma once
#include "matrix.h"
#include <cfenv>
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
};

