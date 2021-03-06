#pragma once
#include<vector>
#include<iostream>
#include<iomanip>
#include <fstream>
#include <iterator>
#include <sstream>
#include <numeric>
#include <filesystem>
#include <variant>
#include<complex>
#include <type_traits>
#include <typeinfo>
#include <type_traits>
#include <valarray>


/*

private:
T* elements; // Array of type T
size_t count; // Number of array elements
public:
explicit Array(size_t arraySize); // Constructor
Array(const Array& other); // Copy Constructor
Array(Array&& other); // Move Constructor
virtual ~Array(); // Destructor
T& operator[](size_t index); // Subscript operator
const T& operator[](size_t index) const; // Subscript operator-const arrays
Array& operator=(const Array& rhs); // Assignment operator
Array& operator=(Array&& rhs); // Move assignment operator
size_t size() { return count; } // Accessor for count

*/

using namespace std;


enum ComplexForm { RECT, POLAR };
enum FileForm { PLAIN, DECORATED };
typedef std::complex<double> Complex;
typedef std::complex<float> ComplexF;
typedef std::valarray<Complex> CArray;
typedef std::valarray<ComplexF> CArrayF;


typedef std::vector<Complex> Complex_vector;
typedef std::vector<double> Double_vector;
typedef std::vector<float> Float_vector;
typedef std::vector<int> Int_vector;
typedef std::vector<size_t> Size_t_vector;

#define Vector std::vector<T>
#define Matrix matrix<T>

template <class T>class matrix
{
public: // Constructors
	explicit matrix(){SelfReset();}
	matrix(const Vector& vec)  // make a single column matrix of a vector
	{
		SelfReset();
		size_t i{ 0 };
		for (auto v : vec) { AddItem(i, 0, v); i++; }
	}
	matrix(const matrix& m)// Copy Constructor
	{
		SelfReset();
		for (size_t i = 0; i < m.Rows_Count; i++)
			for (size_t j = 0; j < m.Columns_Count; j++)
				this->AddItem(i, j, m.GetItem(i, j));
	}
	matrix(matrix&& m) // Move Constructor
	{
		SelfReset();
		if (&m != nullptr)
			*this = m;
		else
			cout << "Error Moving" <<endl;
	}
	bool Increment(const Matrix& m) //Increment the current matrix with the given m
	{
		Matrix m2;
		if (this->Columns_Count == m.Columns_Count && this->Rows_Count == m.Rows_Count)
		{
			for (size_t r = 0; r < this->Rows_Count; r++)
				for (size_t c = 0; c < this->Columns_Count; c++)
					*(this->GetItemPointer(r, c)) = this->GetItem(r, c) + m.GetItem(r, c);
			return true;
		}
		else
		{
			this->CopyDimensions(m);
			return false;
		}
	}
	void CopyDimensions(const Matrix& m)
	{
		if (this->Columns_Count == 0 && this->Rows_Count == 0)
		{
			for (size_t r = 0; r < m.Rows_Count; r++)
				for (size_t c = 0; c < m.Columns_Count; c++)
					this->AddItem(r, c, m.GetItem(r, c) + m.GetItem(r, c));
		}
	}

public:	//***** Operator Overloading*******************
	Matrix operator=(const Matrix& m)
	{
	//	cout << "ASSIGN" << endl;
		this->SelfReset();
		for (unsigned r = 0; r < m.Rows.size(); r++)
		{
			for (unsigned c = 0; c < m.Rows.at(r).size(); c++)
			{
				this->AddItem(r, c, m.GetItem(r, c));
			}
		}
		return *this;
	}
	Matrix& operator=(      Matrix&& m)
	{
	//	cout << " MOVE " << endl;
		this->SelfReset();
		if (&m != nullptr)
		{
			for (size_t i = 0; i < m.Rows_Count; i++)
				for (size_t j = 0; j < m.Columns_Count; j++)
				{
					this->AddItem(i, j, static_cast<T>(0));
					(*this->GetItemPointer(i, j)) = std::move(*(m.GetItemPointer(i, j)));
				}
		}
		else
		{
			cout << " ERROR IN MOVING, The Parameter has nullptr" << endl;
		}
		return *this;
	}
	Matrix operator+(const Matrix& m)
	{
		Matrix m2;
		if (this->Columns_Count == m.Columns_Count && this->Rows_Count == m.Rows_Count)
		{
			for (size_t r = 0; r < this->Rows_Count; r++)
				for (size_t c = 0; c < this->Columns_Count; c++)
					m2.AddItem(r, c, this->GetItem(r, c) + m.GetItem(r, c));
		}
		return m2;
	}
	Matrix operator*(const Matrix& m)
	{
		//cout << " MULTIPLY " << endl;
		Matrix m2;
		T x;
		if (this->Columns_Count == m.Rows_Count)
		{
			for (size_t r = 0; r < this->Rows_Count; r++)
				for (size_t i = 0; i < m.Columns_Count; i++)
				{
					x = static_cast<T>(0);
					for (size_t c = 0; c < m.Rows_Count; c++)
					{
						x = x + this->GetItem(r, c) * m.GetItem(c, i);
					}
					m2.AddItem(r, i, x);
				}
		}
		else
			cout << " Matrix Dimensions don't match" << endl;
		return m2;
	}
	Matrix operator*(const T x)
	{
		Matrix m2;
		for (size_t r = 0; r < this->Rows_Count; r++)
			for (size_t c = 0; c < this->Columns_Count; c++)
			{
				m2.AddItem(r, c, x * this->GetItem(r, c));
			}
		return m2;
	}
	Matrix operator/(const Matrix& m)
	{
		return (*this)*m.Inverse();
	}
	Matrix operator/(const T x)
	{
		return (*this) * (static_cast<T>(1)/x);
	}
	bool write(std::ofstream& ofile)const
	{
		ofile.write(reinterpret_cast<const char*>(&Rows_Count), sizeof(unsigned));
		ofile.write(reinterpret_cast<const char*>(&Columns_Count), sizeof(unsigned));
		for (auto& r : this->Rows)
			ofile.write(reinterpret_cast<const char*>(&r[0]), static_cast<long unsigned>(static_cast<long unsigned >(Columns_Count) * static_cast<long unsigned>(sizeof(T))));
		return !ofile.fail();
	}
	bool read(std::ifstream& ifile)
	{
		unsigned myRows_Count = 0;
		unsigned myColumns_Count = 0;
		ifile.read(reinterpret_cast<char*>(&myRows_Count), sizeof(unsigned));
		ifile.read(reinterpret_cast<char*>(&myColumns_Count), sizeof(unsigned));
		std::vector<Vector> myRows;
		myRows.resize(myRows_Count);
		for (Vector& r : myRows)
			r.resize(myColumns_Count);
		for (Vector& r : myRows)
			ifile.read(reinterpret_cast<char*>(&r[0]), static_cast<long unsigned>(static_cast<long unsigned >(myColumns_Count) * static_cast<long unsigned>(sizeof(T))));
		this->SelfReset();
		for (size_t i=0;i< myRows_Count;i++)
			for (size_t j = 0; j < myColumns_Count; j++)
				this->AddItem(i, j, myRows.at(i).at(j));
		return !ifile.fail();
	}


public: // Matrix Operations
	Matrix Conjugate() const
	{
		if (this->Rows_Count > 0)
		{
			if (typeid(this->GetItem(0, 0)).hash_code() == 3783695017)
			{
				Matrix m = *this;
				for (unsigned r = 0; r < this->Rows.size(); r++)
				{
					for (unsigned c = 0; c < this->Rows.at(r).size(); c++)
					{
						//cout << typeid(this->GetItem(r, c)).hash_code() << endl;
						//cout << " Converting now" << endl;
						m.AddItem(r, c, std::conj(this->GetItem(r, c)));
					}
				}
				return m;
			}
		}
		return *this;
	}
	Matrix Transposed() const
	{
		//cout << " TRANSPOSE" << endl;
		Matrix m;
		for (unsigned r = 0; r < this->Rows.size(); r++)
		{
			for (unsigned c = 0; c < this->Rows.at(r).size(); c++)
			{
				m.AddItem(c, r, (this->GetItem(r, c)));
			}
		}
		return m;
	}
	Matrix ConjugateTransposed() const
	{
		Matrix m;
		m = this->Conjugate();
		return m.Transposed();
	}
	Matrix Identity() const
	{
		Matrix m;
		if (this->Columns_Count == this->Rows_Count)
		{
			for (size_t i = 0; i < this->Columns_Count; i++)
			{
				m.AddItem(i, i, static_cast<T>(1));
			}
		}
		return m;
	}
	Matrix Identity_Row() const
	{
		Matrix m;
		for (size_t i = 0; i < this->Rows_Count; i++)
		{
			m.AddItem(i, i, static_cast<T>(1));
		}
		return m;
	}
	Matrix Identity_Column() const
	{
		Matrix m;
		for (size_t i = 0; i < this->Columns_Count; i++)
		{
			m.AddItem(i, i, static_cast<T>(1));
		}
		return m;
	}
	Matrix Inverse() const
	{
		
		if constexpr (std::is_same_v<T, int> || std::is_same_v<T, unsigned int> || std::is_same_v<T, char> || std::is_same_v<T, unsigned char> || std::is_same_v<T, wchar_t> || std::is_same_v<T, long int> || std::is_same_v<T, long unsigned>)//  
		{
			cout << " WARNING : Integer types are not supported in Inverse. use float type instead " << endl;
		}
		Matrix N;
		if (this->Determinant() == static_cast<T>(0))
		{
			cout << " Singular Matrix has no inverse, det =" << this->Determinant() << endl;
			return N;
		}

		if (this->Columns_Count == this->Rows_Count)
		{
			Matrix M = this->Identity();
			N = *this;
			/// <Perform Invers operations on both matrices>
			// Get Diagonal items
			for (size_t i = 0; i < this->Rows_Count; i++)
			{
				T Corner_Item = N.GetItem(i, i);
				if (Corner_Item == static_cast<T>(0))
				{
					for (size_t r = i + 1; r < N.Rows_Count; r++)
						if (N.GetItem(r, i) != static_cast<T>(0))
						{
							if (N.SelfSwapRows(i, r) && M.SelfSwapRows(i, r))
								break;
							else cout << " Something is Wrong" << endl;
						}
				}
				for (size_t j = 0; j < N.Columns_Count; j++)
				{
					T& p1 = static_cast<Vector&>(M.Rows.at(i)).at(j);
					T& p2 = static_cast<Vector&>(N.Rows.at(i)).at(j);
					
					p1 = p1 / Corner_Item;
					p2 = p2 / Corner_Item;
					
				}
				static_cast<Vector&>(N.Rows.at(i)).at(i) = static_cast<T>(1);
				//cout << "after the first division at each row" << endl;
				//cout << " Row #:" << i << " is devided by " << Corner_Item << endl;
				//N.Show();
				//M.Show();
				//cout << "------------------------------------" << endl;
				for (size_t i2 = 0; i2 < this->Rows_Count; i2++)
				{
					if (i2 != i)
					{
						T Corner_Item2 = N.GetItem(i2, i);
						if (Corner_Item2 != static_cast<T>(0))
						{
							for (size_t j2 = 0; j2 < N.Columns_Count; j2++)
							{
								T& p1 = static_cast<Vector&>(M.Rows.at(i)).at(j2);
								T& p2 = static_cast<Vector&>(N.Rows.at(i)).at(j2);
								T& p3 = static_cast<Vector&>(M.Rows.at(i2)).at(j2);
								T& p4 = static_cast<Vector&>(N.Rows.at(i2)).at(j2);
								p3 = p3-(p1 * Corner_Item2);
								p4 = p4-(p2 * Corner_Item2);
							}
						}
					}
				}
			}
			return M;
		}
		cout << " Non-Square Matrix" << endl;
		return N;
	}
public: // Getters	
	T AddItem(unsigned r, unsigned c, T value)
	{
		if (r >= Rows_Count)
		{
			Rows.resize(r + 1);
			Rows_Count = r + 1;
		}
		else
		{
			Rows.resize(Rows_Count);
		}
		if (c >= Columns_Count)
		{
			for (Vector& row : Rows)
			{
				row.resize(c + 1);
			}
			Columns_Count = c + 1;
		}
		else
		{
			for (Vector& row : Rows)
			{
				row.resize(Columns_Count);
			}
		}
		if (r < Rows.size())
		{
			if (c < static_cast<Vector>(Rows.at(r)).size())
			{
				(Rows.at(r)).at(c) = value;
			}
			else
			{
				cout << "PROBLEM  WITH COLUMN SIZE : matrix.h (K) " << endl;
				cout << Rows.at(r).size() << " greater than " << c << endl;
			}
		}
		else
		{
			cout << "PROBLEM  WITH ROW SIZE : matrix.h (M) " << endl;
			cout << Rows.size() << " Should be greater than " << r << endl;
		}
		return value;// true;
	}
	T Average_All_Items()
	{
		T m = static_cast<T>(0);
		for (size_t i = 0; i < this->Rows_Count; i++)
		{
			for (size_t j = 0; j < this->Columns_Count; j++)
			{
				m = m + this->GetItem(i, j);
			}
		}
		if (this->Rows_Count * this->Columns_Count > 0)
			return (m / static_cast<double>(this->Rows_Count * this->Columns_Count));
		else
			return static_cast<T>(0);
	}
	T Determinant(void)const
	{
		//	this->Show_general();
		float s = -1;
		Matrix m;
		Vector d;
		if ((this->Columns_Count == this->Rows_Count) && (this->Columns_Count > 0))
		{
			if (this->Columns_Count == 1) return this->GetItem(0, 0);
			else
			{
				for (size_t i = 0; i < this->Columns_Count; i++)
				{
					for (size_t j = 1; j < this->Rows_Count; j++)
					{
						for (size_t k = 0; k < this->Columns_Count; k++)
						{
							if (k != i)
							{
								if (k > i)m.AddItem(j - 1, k - 1, this->GetItem(j, k));
								else m.AddItem(j - 1, k, this->GetItem(j, k));
							}
						}
					}
					s = s * -1;
					//	cout << m.GetDeterminant() << endl;
					//	cout << " item at 0 , "<<i<<")"<< this->GetItem(0, i) << endl;
					d.push_back(m.Determinant() * static_cast<T>(s) * this->GetItem(0, i));
					m.SelfReset();
				}
				return std::accumulate(d.begin(), d.end(), static_cast<T>(0));
			}
		}

		return static_cast<T>(0);
	}
	T GetItem(unsigned r, unsigned c)const // get a copy
	{

		if (r < this->Rows_Count && c < this->Columns_Count)return this->Rows.at(r).at(c);
		cout << " Error : item is beyond matrix boundry" << endl;
		return static_cast<T>(0);
	}
	T* GetItemPointer(unsigned r, unsigned c) // get a ref
	{
		Matrix m;
		if (r < this->Rows_Count && c < this->Columns_Count)return &(this->Rows.at(r).at(c));
		cout << " Error : item is beyond matrix boundry" << endl;
		exit(1);
		return &(this->Rows.at(0).at(0));
	}
public: //Output & Display
	void Show(int round_digits=-1)
	{
		if (this->Rows_Count > 0)
		{
			if (typeid(this->GetItem(0, 0)).hash_code() == 3783695017)
			{
				(static_cast<Matrix>(*this)).Show(RECT, round_digits);
			}
			else
			{
				Show_NonComplex(round_digits);
			}
		}
	}
	void Show_NonComplex(int round_digits)
	{
		std::string title = "  ***************** Matrix (whatever)***************";
		std::string title2 = "          COL#";
		std::string title3 = "  ******* ROW# *************************************";
		
		std::cout << title;
		for (int tt = 0; tt < (24 + static_cast<int>(this->Columns_Count) * 10 - static_cast<int>(title.size())); tt++)cout << "*";
		cout << endl;

		std::cout << title2;
		for (int cc = 0; cc < this->Columns_Count; cc++)
			cout << setw(10) << cc;
		cout <<endl;

		std::cout << title3;
		for (int tt = 0; tt < (24 + static_cast<int>(this->Columns_Count) * 10 - static_cast<int>(title.size())); tt++)cout << "*";
		cout << endl;

		size_t i = 0;
		for (Vector r : Rows)
		{
			std::cout << setw(10) << " ";
			std::cout << setw(4) << i;

			for (auto& c : r)
			{
				//std::cout  << " ";
				if (round_digits > -1)
				{
					cout << fixed;
					std::cout << setw(10) << setprecision(round_digits) << Round2(static_cast<Complex>(c).real(), round_digits);
				}
				else
					std::cout << setw(10) << Round2(static_cast<Complex>(c).real(),3);
			}
			i++;
			std::cout << std::endl;
		}
		std::cout << "  **************************************************";
		for (int tt = 0; tt < (24 + static_cast<int>(this->Columns_Count) * 10 - static_cast<int>(title.size())); tt++)cout << "*";
		cout << endl;
	}
	void Show(ComplexForm form, int round_digits = -1)
	{
		
		string title, title2, title3;
		size_t i = 0;
		switch (form)
		{
		case RECT:
			title = "  ************* Matrix (real , imaginary)***********";
			title2 = "          COL# ";
			title3 = "  ******* ROW# *************************************";
			
			std::cout << title ;
			for (int tt = 0; tt < (24 + static_cast<int>(this->Columns_Count) * 26 - static_cast<int>(title.size())); tt++)cout << "*";
			cout << endl;

			std::cout << title2;
			for (int cc = 0; cc < this->Columns_Count; cc++)
				cout << setw(12) << cc<<setw(14)<<" ";
			cout << endl;

			std::cout << title3;
			for (int tt = 0; tt < (24 + static_cast<int>(this->Columns_Count) * 26 - static_cast<int>(title.size())); tt++)cout << "*";
			cout << endl;

			for (auto r : Rows)
			{
				std::cout << setw(10) << " ";
				std::cout << setw(4) << i;
				for (auto c_auto : r)
				{
					Complex c = static_cast<Complex>(c_auto);
					double Real{ 0.0 };
					double Imaginary{ 0.0 };
					std::cout << " (";
					if (round_digits > -1)
					{
						cout << fixed;
						Real= Round2(c.real(), static_cast<float>(round_digits));
						Imaginary = Round2(c.imag(), static_cast<float>(round_digits));
						if (Imaginary < 0)std::cout << setw(10) << setprecision(round_digits) << Real << " -j" << setw(10) << setprecision(round_digits) << -1 * Imaginary << ")";
						else             std::cout << setw(10) << setprecision(round_digits) << Real << " +j" << setw(10) << setprecision(round_digits) << Imaginary << ")";
					}
					else
					{
						Real = Round(c.real(), 3);
						Imaginary = Round(c.imag(), 3);
						if (Imaginary < 0)std::cout << setw(10) << Real << " -j" << setw(10) << -1 * Imaginary << ")";
						else             std::cout << setw(10) << Real << " +j" << setw(10) << Imaginary << ")";
					}
				}
				i++;
				std::cout << std::endl;
			}
			std::cout << "  **************************************************";
			for (int tt = 0; tt < (24 + static_cast<int>(this->Columns_Count) * 26 - static_cast<int>(title.size())); tt++)cout << "*";
			cout << endl;
			break;
		case POLAR:
			title = "  ************* Matrix (magnitude |_ phase(rad))****";	
			title2 = "          COL# ";
			title3 = "  ******* ROW# *************************************";

			std::cout << title;
			for (int tt = 0; tt < (24 + static_cast<int>(this->Columns_Count) * 21 - static_cast<int>(title.size())); tt++)cout << "*";
			cout << endl;

			std::cout << title2;
			for (int cc = 0; cc < this->Columns_Count; cc++)
				cout << setw(10) << cc<<setw(11)<<" ";
			cout << endl;

			std::cout << title3;
			for (int tt = 0; tt < (24 + static_cast<int>(this->Columns_Count) * 21 - static_cast<int>(title.size())); tt++)cout << "*";
			cout << endl;
			double phase, mag;
			for (auto r : Rows)
			{
				std::cout << setw(10) << " ";
				std::cout << setw(4) << i;
				for (auto c_auto : r)
				{
					Complex c = static_cast<Complex>(c_auto);
					std::cout << " ";
					if (round_digits > -1)
					{
						cout << fixed;
						phase = Round(std::arg(c), static_cast<float>(round_digits));
						mag = Round(std::abs(c), static_cast<float>(round_digits));
						std::cout << setw(10) << setprecision(round_digits) << mag << " |_";
						cout << setw(7) << setprecision(round_digits) << phase;
					}
					else
					{
						phase = Round(std::arg(c), 3);
						mag = Round2(std::abs(c), 3);
						std::cout << setw(10) << mag << " |_";
						cout << setw(7) << phase;
					}
					
				}
				i++;
				std::cout << std::endl;
			}
			std::cout << "  **************************************************";
			for (int tt = 0; tt < (24 + static_cast<int>(this->Columns_Count) * 21 - static_cast<int>(title.size())); tt++)cout << "*";
			cout << endl;
			break;
		}
		
	}
	void WriteFile(std::string file_out, FileForm form = FileForm::PLAIN)
	{
		if (this->Rows_Count > 0)
		{
			if (typeid(this->GetItem(0, 0)).hash_code() == 3783695017)
			{
				(static_cast<Matrix>(*this)).WriteFile(file_out,RECT,form);
			}
			else
			{
				WriteFile_NonComplex(file_out, form);
			}
		}
	}
	void WriteFile(std::string file_out, ComplexForm form, FileForm file_form = FileForm::PLAIN,string header=".")
	{
		
		// this function writes the matrix to a file
		std::string parent_directory = file_out;
		size_t position = file_out.find_last_of("/");
		std::string file_name = parent_directory.substr(position);
		parent_directory.erase(position, file_name.size());
		file_name.erase(0, 1);
		if (!std::filesystem::exists(parent_directory))
		{
			if (std::filesystem::create_directory(parent_directory))cout << parent_directory << " Directory Created" << endl; else { cout << parent_directory << " Directory can't be created " << endl; exit(0); }
		}
		
		
		vector<std::string>row;
		std::stringstream converter;
		std::string s, i_s;
		size_t i_index = 0;
		string first_part,second_part;
		switch (form)
		{
		case RECT:
			first_part = "(real)"; second_part = "(i img)";
			break;
		case POLAR:
			first_part = "(mag)"; second_part = "(phase)";
			break;
		}
		string separator, extension;
		switch (file_form)
		{
		case FileForm::PLAIN:
			separator = "\t";
			extension = ".txt";
			header = "";
			break;
		case FileForm::DECORATED:
			separator = ",";
			extension = ".csv";
			if (header == ".")
			{
				header = "Raw #\\Column #";///
				for (size_t i = 0; i < this->Columns_Count; i++)
				{
					converter << i; converter >> i_s; converter.clear();
					header = header + "," + i_s + first_part + "," + i_s + second_part;
				}
			}
			row.push_back(header);
			
			break;
		}
		std::ofstream out{ file_out + extension, std::ios_base::out | std::ios_base::trunc };
		std::ostream_iterator<string> out_iter2{ out, " " };
		if (file_form == FileForm::DECORATED)
		{
			std::copy(std::begin(row), std::end(row), out_iter2);
			out_iter2 = "\n";
			row.clear();
		}
		switch (form)
		{
		case RECT:
			for (auto& r : Rows)
			{
				converter << i_index; converter >> i_s; converter.clear(); row.push_back(i_s);
				row.push_back(separator);
				for (auto ii : r)
				{
					Complex i = static_cast<Complex>(ii);
					converter << Round2(i.real(),3); converter >> s; converter.clear();
					row.push_back(s);
					row.push_back(separator);
					converter << Round2(i.imag(),3); converter >> s; converter.clear();
					row.push_back(s);
					row.push_back(separator);
				}
				std::copy(std::begin(row), std::end(row), out_iter2);
				out_iter2 = "\n";
				row.clear();
				i_index++;
			}
			break;
		case POLAR:
			double phase;
			double mag;
			for (auto& r : Rows)
			{
				converter << i_index; converter >> i_s; converter.clear(); row.push_back(i_s);
				row.push_back(separator);
				for (auto ii : r)
				{
					Complex i = static_cast<Complex>(ii);
					phase = Round(std::arg(i),3);
					mag = Round2(std::abs(i),3);
					converter << mag; converter >> s; converter.clear();
					row.push_back(s);
					row.push_back(separator);
					converter << phase; converter >> s; converter.clear();
					row.push_back(s);
					row.push_back(separator);
				}
				std::copy(std::begin(row), std::end(row), out_iter2);
				out_iter2 = "\n";
				row.clear();
				i_index++;
			}
		}
	}
	void WriteFile_NonComplex(std::string file_out, FileForm file_form = FileForm::PLAIN, string header = ".")
	{
		
		// this function writes the matrix to a file
		// check if the parent directory exists
		std::string parent_directory = file_out;
		size_t position = file_out.find_last_of("/");
		std::string file_name = parent_directory.substr(position);
		parent_directory.erase(position, file_name.size());
		file_name.erase(0, 1);
		if (!std::filesystem::exists(parent_directory))
		{
			if (std::filesystem::create_directory(parent_directory))cout << parent_directory << " Directory Created" << endl; else { cout << parent_directory << " Directory can't be created " << endl; exit(0); }
		}

		vector<std::string>row;
		std::stringstream converter;
		std::string s, i_s;
		size_t i_index = 0;
		string separator, extension;
		switch (file_form)
		{
		case FileForm::PLAIN:
			separator = "\t";
			extension = ".txt";
			header = "";
			break;
		case FileForm::DECORATED:
			separator = ",";
			extension = ".csv";
			if (header == ".")
			{
				header = "Raw #\\Column #";///
				for (size_t i = 0; i < this->Columns_Count; i++)
				{
					converter << i; converter >> i_s; converter.clear();
					header = header + "," + i_s;
				}
			}
			row.push_back(header);
			break;
		}
		std::ofstream out{ file_out + extension, std::ios_base::out | std::ios_base::trunc };
		std::ostream_iterator<string> out_iter2{ out, " " };
		if (file_form == FileForm::DECORATED)
		{
			std::copy(std::begin(row), std::end(row), out_iter2);
			out_iter2 = "\n";
			row.clear();
		}

		for (auto r : Rows)
		{
			converter << i_index; converter >> i_s; converter.clear(); row.push_back(i_s); row.push_back(separator);

			for (auto i : r)
			{
				converter << i; converter >> s; converter.clear();
				row.push_back(s);
				row.push_back(separator);
			}
			std::copy(std::begin(row), std::end(row), out_iter2);
			out_iter2 = "\n";
			row.clear();
			i_index++;
		}
	}
public: // Convert to Tables. A Table that contains the rows of the item matrices in one table record per each matrix item
	static Matrix AppendToTable(Matrix& table, Matrix& item)
	{
		unsigned current_row = table.Rows_Count;
		unsigned i = 0;
		for (unsigned r = 0; r < item.Rows_Count; r++)
			for (unsigned c = 0; c < item.Columns_Count; c++)
			{
				table.AddItem(current_row, i, item.GetItem(r, c));
				i++;
			}
		return table;
	}
	~matrix() { SelfReset(); }
public: //Selfies
	bool GT(T lhs, T rhs)
	{
		Complex R = static_cast<Complex>(rhs);
		Complex L = static_cast<Complex>(lhs);
		if (L.real() > R.real())return true;
		else
			return false;
	}
	void SelfAppendColumns(Matrix& m)
	{
		unsigned columns = this->Columns_Count;
		for (unsigned r = 0; r < m.Rows.size(); r++)
			for (unsigned c = 0; c < m.Rows.at(r).size(); c++)
				this->AddItem(r, c + columns, m.GetItem(r, c));
	}
	bool SelfSwapRows(size_t n, size_t m)
	{
		if (n < this->Rows_Count && m < this->Rows_Count)
		{
			Vector temp_row;
			for (size_t j = 0; j < this->Columns_Count; j++)
			{
				temp_row.push_back(this->GetItem(n, j));
				static_cast<Vector&>(this->Rows.at(n)).at(j) = this->GetItem(m, j);
				static_cast<Vector&>(this->Rows.at(m)).at(j) = temp_row.at(j);
			}
			return true;
		}
		else
		{
			//		cout << " ERROR: The matrix rows count is less that the swapping rows" << endl;
			//		cout << " n " << n << " m " << m << " this->Rows_Count " << this->Rows_Count << endl;
			return false;
		}
	}
	bool SelfSwapColumns(size_t n, size_t m)
	{
		if (n < this->Columns_Count && m < this->Columns_Count)
		{
			Vector temp_column;
			for (size_t j = 0; j < this->Rows_Count; j++)
			{
				temp_column.push_back(this->GetItem(j, n));
				static_cast<Vector&>(this->Rows.at(j)).at(n) = this->GetItem(j,m);
				static_cast<Vector&>(this->Rows.at(j)).at(m) = temp_column.at(j);
			}
			return true;
		}
		else
		{
			//		cout << " ERROR: The matrix rows count is less that the swapping rows" << endl;
			//		cout << " n " << n << " m " << m << " this->Rows_Count " << this->Rows_Count << endl;
			return false;
		}
	}
	void SelfSortOnColumn(size_t n)
	{
		if (this->Columns_Count > n)
		{
			int j;
			for (int i = 1; i < this->Rows_Count; i++)
			{
				j = i - 1;
				while (j >= 0)
				{
					if (GT(this->GetItem(j, n), this->GetItem(i, n)))
					{
						//	cout << this->GetItem(j, n) << " is greater than " << this->GetItem(i, n) <<endl;
						this->SelfSwapRows(i, j);
						i = j - 1; break;
						//	cout << "i = " << i << " j = " << j << endl;
					}
					j--;
				}
			}
		}
		//	else
		//		cout << " ERROR: The matrix columns count is less that the sorting column" << endl;
	}
	bool SelfDeleteRow(size_t n)
	{
		//	cout << " Delete row :  " << n << endl;
		size_t l1, l2;
		if (this->Rows_Count > n)
		{
			l1 = n;
			static_cast<Vector>(this->Rows.at(n)).clear();
			this->Rows.erase(this->Rows.begin() + n);
			this->Rows_Count--;
			return true;
		}
		else
		{
			//		cout << " ERROR: The matrix row is not found" << endl;
			return false;
		}
	}
	void SelfReset()
	{
		for (Vector& r : this->Rows)
			r.clear();
		this->Rows.clear();
		this->Columns_Count = 0;
		this->Rows_Count = 0;
	}
public: //Tools
	static float Round(double n, float d);
	static double Round2(double n, float d);
	static string PrintComplex(Complex n, ComplexForm form, float d);

public: // Statistics
	T Mean()// mean of all items
	{
		T sum = static_cast<T>(0);
		for(auto& r:Rows)
			for (auto& c : r)
			{
				sum = sum + c;
			}
		return sum / static_cast<T>(Columns_Count * Rows_Count);
	}
	T Var()// mean of all items
	{
		T mean = Mean();
		T sum = static_cast<T>(0);
		for (auto& r : Rows)
			for (auto& c : r)
			{
				sum = sum + pow(abs(c-mean),2);
			}
		return sum / static_cast<T>((Columns_Count * Rows_Count)-1);
	}
	T Std()// standard deviation of all items
	{
		return sqrt(this->Var());
	}
	T Max()// Maximum of all items
	{
		if (typeid(this->GetItem(0, 0)).hash_code() == 3783695017)
		{
			return Max_Complex();
		}
		else
		{
			return Max_nonComplex();
		}
	}
	T Max_Complex()
	{
		T max = static_cast<T>(0);
		if (this->Rows_Count > 0 && this->Columns_Count > 0)
		{
			max = this->GetItem(0, 0);
			for (auto& r : Rows)
				for (auto& c : r)
				{
					if (std::abs(static_cast<Complex>(max)) < std::abs(static_cast<Complex>(c)))
					{
						max = c;
					}
				}
		}
		return max;
	}
	T Max_nonComplex()
	{
		T max = static_cast<T>(0);
		if (this->Rows_Count > 0 && this->Columns_Count > 0)
		{
			max = this->GetItem(0, 0);
			for (auto& r : Rows)
				for (auto& c : r)
				{
					if (std::real(static_cast<Complex>(max)) < std::real(static_cast<Complex>(c)))
					{
						max = c;
					}
				}
		}
		return max;
	}
	T Min()// Minimum of all items
	{
		if (typeid(this->GetItem(0, 0)).hash_code() == 3783695017)
		{
			return Min_Complex();
		}
		else
		{
			return Min_nonComplex();
		}
	}
	T Min_Complex()
	{
		T min = static_cast<T>(0);
		if (this->Rows_Count > 0 && this->Columns_Count > 0)
		{
			min = this->GetItem(0, 0);
			for (auto& r : Rows)
					for (auto& c : r)
					{
						if (std::abs(static_cast<Complex>(min)) > std::abs(static_cast<Complex>(c)))
						{
							min = c;
						}
					}
		}
		return min;
	}
	T Min_nonComplex()
	{
		T min = static_cast<T>(0);
		if (this->Rows_Count > 0 && this->Columns_Count > 0)
		{
			min = this->GetItem(0, 0);
			for (auto& r : Rows)
				for (auto& c : r)
				{
					if (std::real(static_cast<Complex>(min)) > std::real(static_cast<Complex>(c)))
					{
						min = c;
					}
				}
		}
		return min;
	}
	public: // Complex matrix specials
		static matrix<Complex> ConvertToMatrix(CArray& x)
		{
			matrix<Complex> result;
			size_t i{ 0 };
			for (auto& v : x)
			{
				result.AddItem(i, 0, v);
				i++;
			}
			return result;
		}
		static CArray ConvertFromMatrix(matrix<Complex>& x)
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

public:
	std::vector<Vector> Rows;
	unsigned Rows_Count;
	unsigned Columns_Count;
};

//matrix<complex<double>> operator/(matrix<complex<double>>, const double);
//matrix<double> operator/(matrix<double> matrix, const double);
//matrix<Complex> operator/(matrix<Complex>, matrix<double>);


template<class T, class T2>
Matrix AppendToTable(Matrix& table, Matrix& item, vector<T2> item_extension)
{
	unsigned current_row = table.Rows_Count;
	unsigned i = 0;
	bool valid_check = false;

	for (unsigned r = 0; r < item.Rows.size(); r++)
		for (unsigned c = 0; c < item.Rows.at(r).size(); c++)
		{
			table.AddItem(current_row, i, item.GetItem(r, c));
			i++;
		}
		size_t j = 0;
		for (auto g : item_extension)
		{
			table.AddItem(current_row, i + j, static_cast<T>(g));
			j++;
		}
	return table;
}
template<class T, class T2>
Matrix operator/(const Matrix& lhs, const matrix<T2>& rhs)
{
	// convert to this type
	Matrix denumerator;
	for (size_t i = 0; i < rhs.Rows_Count; i++)
		for (size_t j = 0; j < rhs.Columns_Count; j++)
			denumerator.AddItem(i, j, static_cast<T>(rhs.GetItem(i, j)));
	return (lhs / denumerator);
}

typedef matrix<Complex> Complex_matrix;
typedef matrix<ComplexF> ComplexF_matrix;
typedef matrix<double> Double_matrix;
typedef matrix<float> Float_matrix;
typedef matrix<int> Int_matrix;
typedef matrix<size_t> Size_t_matrix;

template <class T>
float Matrix::Round(double n, float d)
{
	double value = static_cast<int>((n)*pow(10.0, d) + .5);
	return static_cast<float>(value / pow(10.0, d));
}
template <class T>
double Matrix::Round2(double n, float d)
{
	std::stringstream converter;
	string number, exponent;
	int e{ 0 };
	double n2{ n };
	float m, m_rounded;
	converter << n; converter >> number; converter.clear();
	int position = number.find("e");
	if (position < 0)position = number.find("E");
	if (position > 0)
	{
		exponent = number.substr(position + 1);
		converter << exponent; converter >> e;
		m = n * pow(10, -1 * e);
		m_rounded = Round(m, d);
		return m_rounded * pow(10, e);
	}
	else // Make sure the rounding dosn't kill the value
		return Round(n, d);


}
template <class T>
string Matrix::PrintComplex(Complex n, ComplexForm form, float d)
{
	std::stringstream converter;
	string A, B, joint;
	double a{ 0.0 }, b{ 0.0 };
	switch (form)
	{
	case RECT:
		a = Round2(std::real(n), d);
		b = Round2(std::imag(n), d);
		if (b < 0 || b == 0)
		{
			joint = "-j";
			b = b * -1;
		}
		else
			joint = "+j";
		break;
	case POLAR:
		a = std::abs(n);
		b = std::arg(n);
		joint = "|_ ";
		break;
	}
	converter << a; converter >> A; converter.clear();
	converter << b; converter >> B; converter.clear();
	return A + joint + B;
}
template <class T>
Matrix Kronecker(Matrix& A, Matrix& B)
{
	Matrix R;
	for (size_t i1=0;i1<A.Rows_Count;i1++)
		for (size_t j1 = 0; j1 < A.Columns_Count; j1++)
			for (size_t i2 = 0; i2 < B.Rows_Count; i2++)
				for (size_t j2 = 0; j2 < B.Columns_Count; j2++)
				{
					R.AddItem(i2+(i1*B.Rows_Count),j2+(j1*B.Columns_Count),A.GetItem(i1, j1)* B.GetItem(i2, j2));
				}
	return R;
}

template <class T>
valarray<T> Kronecker(valarray<T>& A, valarray<T>& B) // Kronker for vectors only
{
	size_t N1 = A.size();
	size_t N2 = B.size();
	valarray<T> R(N1*N2);
	size_t k{ 0 };
	for (size_t i1 = 0; i1 < N1; i1++)
		for (size_t j1 = 0; j1 < N2; j1++)
		{
			R[k]= A[i1] * B[j1];
			k++;
		}
	return R;
}

template <class T>
valarray<complex<T>> Conjugate(valarray<complex<T>>& A) // Kronker for vectors only
{
	size_t N1 = A.size();
	valarray<complex<T>> R(N1);
	size_t k{ 0 };
	for (size_t i1 = 0; i1 < N1; i1++)
	{
		R[k] = std::conj(A[i1]);
		k++;
	}
	return R;
}

template <class T>
matrix<T> ConvertToMatrix(valarray<T>& x)
{
	matrix<T> result;
	size_t i{ 0 };
	for (auto& v : x)
	{
		result.AddItem(i, 0, v);
		i++;
	}
	return result;
}
template <class T>
valarray<T> ConvertFromMatrix(matrix<T>& x)
{
	valarray<T> result;
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
template <class T>
T Std(valarray<T> x)
{
	T mu;
	valarray<T> y;
	size_t Size = x.size();
	if (Size > 1)
	{
		y.resize(Size);
		mu = x.sum() / static_cast<T>(Size);
		
		size_t j{ 0 };
		for (auto& i : x)
		{
			y[j] = pow(abs(i - mu), 2);
			j++;
		}
		return sqrt(static_cast<T>(1.0 / static_cast<float>(Size - 1)) * y.sum());
	}
	return T(0);
}
template <class T>
float Norm(Matrix m, int p)
{
	float sum=0;
	for (auto& r : m.Rows)
		for (auto& c : r)
			sum = sum + pow(abs(c),p);
	float root_power = 1.0 / static_cast<float>(p);
	return pow(sum, root_power);
}

template <class T>
Matrix Normalize(Matrix m)
{
	float norm_2 = Norm<T>(m, 2);
	if (norm_2 > 0)
	{
		return (m * (1 / norm_2));
	}
	return m;
}