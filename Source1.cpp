#include"DFT.h"
#include <io.h>
#include <fcntl.h>

//#include"Header.h"
typedef matrix<Complex> Complex_matrix;
typedef matrix<double> Double_matrix;
typedef matrix<float> Float_matrix;
typedef matrix<int> Int_matrix;
typedef matrix<unsigned> Unsigned_matrix;
int main(void)
{
	//Double_matrix A, B , C, D,E;
	//Float_matrix A, B, C, D,E;
	//Int_matrix A, B, C, D,E;
	Complex_matrix C,D,E,I,FFF;
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
	DFT W(20);//,UNITARY::NO
//	W.Show(RECT);
	E = (W*x);
	E.Show(RECT);
	X = DFT::FFT(x);
	X.Show(RECT);
//	I = W * W.ConjugateTransposed();
//	I.Show(RECT,0);

	Int_matrix A, B;
	A.AddItem(0, 0, 1);
	A.AddItem(0, 1, 2);
	A.AddItem(0, 2, 3);
	A.AddItem(0, 3, 4);

	B.AddItem(0, 0, 0);
	B.AddItem(0, 1, 5);
	B.AddItem(0, 2, 6);
	B.AddItem(0, 3, 7);
	Kronecker<int>(A, B).Show();
	(A* B.Transposed()).Show();
	
	Float_matrix AA,AB;
	AA.AddItem(0, 0, 1);
	AA.AddItem(0, 1, 2);
	AA.AddItem(0, 2, 3);
	AA.AddItem(0, 3, 4);

	Unsigned_matrix BB;
	BB.AddItem(0, 0, static_cast<unsigned>(-1)); // Doesn't Work
	BB.AddItem(0, 1, 2);
	BB.AddItem(0, 2, 3);
	BB.AddItem(0, 3, 4);

	cout << " mean C :" << C.Mean() << endl;
	cout << " var  C :" << C.Var() << endl;
	cout << " std  C :" << C.Std() << endl;
	cout << " max  C :" << C.Max() << endl;
	cout << " min  C :" << C.Min() << endl;

	valarray<double> AAA, BBB;
	AAA.resize(3);
	BBB.resize(3);
	AAA[0] = 0.23;
	AAA[1] = 2;
	AAA[2] = 1.2e1;

	BBB=pow(AAA,2);
	for (auto& x1 : BBB)
		cout << " " << x1 << endl;
	AB = AA.Transposed();
	(Kronecker<float>(AA, AA)).Show();
	valarray<float> CCC,DDD;
	CCC=ConvertFromMatrix<float>(AA);
	DDD = Kronecker<float>(CCC, CCC);
	for (auto& x2 : DDD)
		cout <<" " << x2 << endl;
	FFF.AddItem(0, 0, 0.0757 - 1.0097i);
	FFF.AddItem(0, 1, 0.0387 + 1.0129i);
	FFF.AddItem(0, 2, -3.9858 + 0.3364i);
	FFF.Show();
	(Normalize(FFF)).Show();
	ComplexF Z(-1.23, 5.012);
	cout << Z <<" abs :"<< abs(Z)<<" arg :" << arg(Z)<<endl;
	ComplexF Z1=abs(Z) * polar<float>(1.0,0.0);
	cout << Z1 << " abs :" << abs(Z1) << " arg :" << arg(Z1) << endl;
	return 0;
}