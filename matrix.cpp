#include"matrix.h"
/*
matrix<Complex> operator/(matrix<Complex> matrix, const double m)
{
	matrix<Complex> m2;
	for (size_t r = 0; r < matrix.Rows_Count; r++)
		for (size_t c = 0; c < matrix.Columns_Count; c++)
		{
			if (m != 0.0)m2.AddItem(r, c, Complex(matrix.GetItem(r, c).real() / m, matrix.GetItem(r, c).imag() / m));
			else
				m2.AddItem(r, c, Complex(0.0, 0.0));
		}
	return m2;
}
matrix<double> operator/(matrix<double> matrix, const double m)
{
	matrix<double> m2;
	for (size_t r = 0; r < matrix.Rows_Count; r++)
		for (size_t c = 0; c < matrix.Columns_Count; c++)
		{
			if (m != 0)m2.AddItem(r, c, matrix.GetItem(r, c) / m);
			else
			{
				m2.AddItem(r, c, (0, 0));
				cout << " Zero Divizor !!!" << endl;
			}
		}
	return m2;
}
matrix<Complex> operator/(matrix<Complex> matrix_N, matrix<double> matrix_D)
{
	matrix<Complex> m2;
	if (matrix_N.Rows_Count == matrix_D.Rows_Count && matrix_N.Columns_Count == matrix_N.Columns_Count)
		for (size_t r = 0; r < matrix_N.Rows_Count; r++)
			for (size_t c = 0; c < matrix_N.Columns_Count; c++)
			{
				if (matrix_D.GetItem(r, c) != 0.0)m2.AddItem(r, c, Complex(matrix_N.GetItem(r, c).real() / matrix_D.GetItem(r, c), matrix_N.GetItem(r, c).imag() / matrix_D.GetItem(r, c)));
				else
					m2.AddItem(r, c, Complex(0.0, 0.0));
			}
	return m2;
}

*/