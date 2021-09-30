#include"DFT.h"
//#include"Header.h"
typedef matrix<Complex> Complex_matrix;
typedef matrix<double> Double_matrix;
typedef matrix<float> Float_matrix;
typedef matrix<int> Int_matrix;
int main(void)
{
	//Double_matrix A, B , C, D,E;
	//Float_matrix A, B, C, D,E;
	//Int_matrix A, B, C, D,E;
	Complex_matrix C,D,E,I;
//	/*
	C.AddItem(0, 0, complex<double>(12.3, 3.1));
	C.AddItem(0, 1, complex<double>(1.3, -3.1));
	C.AddItem(0, 2, complex<double>(2.3, 0.1));

	C.AddItem(1, 0, complex<double>(-12.3, 3.1));
	C.AddItem(1, 1, complex<double>(10.3, 3.1));
	C.AddItem(1, 2, complex<double>(-9.3, 3.1));

	C.AddItem(2, 0, complex<double>(12.3, 3.1));
	C.AddItem(2, 1, complex<double>(10.3, -7.1));
	C.AddItem(2, 2, complex<double>(12.0, 5.1));
//	*/
	/*
	
	C.AddItem(0, 0, 12.3);
	C.AddItem(0, 1, 1.3);
	C.AddItem(0, 2, 2.3);

	C.AddItem(1, 0, -12.3);
	C.AddItem(1, 1, 10.3);
	C.AddItem(1, 2, -9.3);

	C.AddItem(2, 0, 12.3);
	C.AddItem(2, 1, 10.3);
	C.AddItem(2, 2, 12.0);
//	*/
	/*
	C.AddItem(0, 0, 12);
	C.AddItem(0, 1, 1);
	C.AddItem(0, 2, 2);

	C.AddItem(1, 0, -12);
	C.AddItem(1, 1, 10);
	C.AddItem(1, 2, -9);

	C.AddItem(2, 0, 12);
	C.AddItem(2, 1, 10);
	C.AddItem(2, 2, 12);
	*/
	C.Show(RECT);
	C.Conjugate().Show();
//	C.WriteFile("D://Test.csv");
	//	C.Show();
	D=C.Conjugate();
//	D = C;
	//	C.Show();
	D.Show();
	D.Inverse().Show();
	//	C1.Show();
	(D * D.Inverse()).Show();
	//	D.SelfSwapRows(0, 1);
	//	D.SelfSwapColumns(0, 1);
//	D.Show();
//	std::vector<float>v;
//	v.push_back(12);
//	v.push_back(1);
//	v.push_back(2);

//	v.push_back(-12);
//	v.push_back(10);
//	v.push_back(-9);
//	Complex_matrix result;
//	(AppendToTable<Complex, float>(result, C, v)).Show();
//	DFT dd = C;
//	dd.Show();
//	DFT ff(C);
//	ff.Show();
	size_t c = 0;
	Complex_matrix x,X;
	x.AddItem(c, 0, 0); c++;
	x.AddItem(c, 0, 0); c++;
	x.AddItem(c, 0, 0); c++;
	x.AddItem(c, 0, 0); c++;
	x.AddItem(c, 0, 0); c++;
	x.AddItem(c, 0, 0); c++;
	x.AddItem(c, 0, 0); c++;
	x.AddItem(c, 0, 0); c++;

	x.AddItem(c, 0, 1); c++;
	x.AddItem(c, 0, 1); c++;
	x.AddItem(c, 0, 1); c++;
	x.AddItem(c, 0, 1); c++;

	x.AddItem(c, 0, 0); c++;
	x.AddItem(c, 0, 0); c++;
	x.AddItem(c, 0, 0); c++;
	x.AddItem(c, 0, 0); c++;
	x.AddItem(c, 0, 0); c++;
	x.AddItem(c, 0, 0); c++;
	x.AddItem(c, 0, 0); c++;
	x.AddItem(c, 0, 0); c++;
//	x.Show(RECT);
	DFT W(20,UNITARY::YES);
//	W.Show(RECT);
	E = (W*x);
//	E.Show(RECT);
	I = W * W.ConjugateTransposed();
//	I.Show(RECT,0);
	return 0;
}