#include"matrix.h"
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
	Complex_matrix C,D,E;
	/*
	C.AddItem(0, 0, complex<double>(12.3, 3.1));
	C.AddItem(0, 1, complex<double>(1.3, -3.1));
	C.AddItem(0, 2, complex<double>(2.3, 0.1));

	C.AddItem(1, 0, complex<double>(-12.3, 3.1));
	C.AddItem(1, 1, complex<double>(10.3, 3.1));
	C.AddItem(1, 2, complex<double>(-9.3, 3.1));

	C.AddItem(2, 0, complex<double>(12.3, 3.1));
	C.AddItem(2, 1, complex<double>(10.3, -7.1));
	C.AddItem(2, 2, complex<double>(12.0, 5.1));
	*/
//	/*
	
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
//	C.Show();
//	C.WriteFile("D://Test.csv");


	//	C.Show();
	//	D=C.Conjugate();
//	D = C;
	//	C.Show();
//	D.Show();
//	D.Inverse().Show();

	//	C1.Show();
//	(D * D.Inverse()).Show();
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
///*
	std::ofstream ofile("D:/sample5.bin", std::ios::binary);
	if (ofile.is_open())
	{
		ofile.seekp(0);
		C.write(ofile);
		ofile.close();
	}
	std::ifstream ifile("D:/sample5.bin", std::ios::binary);



	if (ifile.is_open())
	{
		ifile.seekg(0);
		try {
			if (!E.read(ifile))cout << "KKKKKKKKKKKKKK" << endl;
		}
//		catch(const ifstream::failure& e){
//			ifile.close();
//			cout << " Go To Hell" << endl;
//		}
		catch (const std::ios_base::failure& fail) {
			cout << " Caught here" << endl;
			std::cout << fail.what() << '\n';
		}
		ifile.close();
	}
	
//	D.Show();
	E.Show();
//*/
/*
	WirelessPower P1(D);
	if (!P1.write(ofile))cout << "LLLLLLLLLLLLLLLLL" << endl;;
	WirelessPower P2;
	//	P2.Show();
	if (!P2.read(ifile))cout << "KKKKKKKKKKKKKKKKKKK" << endl;;
	P1.Show();
	P2.Show();
//	*/
/*
	unsigned var1 = 10; //for (auto c : buffer)cout << c; cout << endl;
	unsigned var2 = 12; //for (auto c : buffer3)cout << c;
	ofstream myFile("D:/data.bin", ios::out | ios::binary);
	if (myFile.is_open())
	{
		myFile.seekp(0);
		myFile.write(reinterpret_cast<const char*>(&var1), sizeof(unsigned));
		myFile.write(reinterpret_cast<const char*>(&var2), sizeof(unsigned));
		myFile.close();
	}
	cout << endl;
	
	
	unsigned var3=0;
	unsigned var4=0;
	ifstream myFile2("D:/data.bin", ios::in | ios::binary);
	if (myFile2.is_open())
	{
		myFile2.seekg(0);
		myFile2.read(reinterpret_cast<char*>(&var3), sizeof(unsigned));
		myFile2.read(reinterpret_cast<char*>(&var4), sizeof(unsigned));
		myFile2.close();
	}
	//for (auto c : buffer2)cout << c; cout << endl;
	cout << var3 << " " << var4 << endl;
	//for (auto c : buffer4)cout << c; cout << endl;
//*/
	return 0;
}