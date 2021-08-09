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
//#include <type_traits>
#include <typeinfo>

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
typedef std::complex<double> Complex;
#define Vector std::vector<T>
#define Matrix matrix<T>
//#define Vector std::vector<T>
template <class T>class matrix
{
public:
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


	//***** Operator Overloading*******************
	Matrix& operator=(const Matrix& m)
	{
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
		if(&m !=nullptr)
		this = &m;
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
		return (*this) * (static_cast<T>(1)/T);
	}
	//*********************************************
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
						cout << typeid(this->GetItem(r, c)).hash_code() << endl;
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
		m =this->Conjugate();
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
					for(size_t r=i+1;r<N.Rows_Count;r++)
						if (N.GetItem(r, i) != static_cast<T>(0))
						{
							if(N.SelfSwapRows(i, r) && M.SelfSwapRows(i, r))
								break;
							else cout << " Something is Wrong" << endl;
						}
				}
				for (size_t j = 0; j < N.Columns_Count; j++)
				{
					T& p1 = static_cast<Vector&>(M.Rows.at(i)).at(j);
					T& p2 = static_cast<Vector&>(N.Rows.at(i)).at(j);
					p1=p1/ Corner_Item;
					p2=p2/ Corner_Item;
				}
				static_cast<Vector&>(N.Rows.at(i)).at(i) = static_cast<T>(1);
				//cout << "after the first division at each row" << endl;
				//cout << " Row #:" << i << " is devided by " << Corner_Item << endl;
				//N.Show();
				//M.Show();
				//cout << "------------------------------------" << endl;
				// All raws
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
	T* GetItemPointer(unsigned r, unsigned c)const // get a ref
	{
		Matrix m;
		if (r < this->Rows_Count && c < this->Columns_Count)return &(this->Rows.at(r).at(c));
		cout << " Error : item is beyond matrix boundry" << endl;
		exit(1);
		return &(this->Rows.at(0).at(0));
	}

	void Show()
	{
	//	if (this->Rows_Count > 0)
		{
	///		if (typeid(this->GetItem(0, 0)).hash_code() == 3783695017)
			{
	//			(static_cast<Matrix>(*this)).Show(RECT);
			}
	//		else
			{
				Show_NonComplex();
			}
		}
	}
	void Show_NonComplex()
	{
		std::cout << " \t ********************* matrix (whatever)**************" << std::endl;
		size_t i = 0;
		for (Vector r : Rows)
		{
			std::cout << setw(10) << " ";
			std::cout << " " << i;

			for (T& c : r)
			{
				std::cout << setw(30) << " ";
				std::cout << "\t";
				std::cout << c;
			}
			i++;
			std::cout << std::endl;
		}
		std::cout << " \t **************************************************************" << std::endl;
	}
	void Show(ComplexForm form)
	{
		size_t i = 0;
		switch (form)
		{
		case RECT:
			std::cout << " \t ********************* H matrix (real , imaginary)**************" << std::endl;
			for (auto r : Rows)
			{
				std::cout << setw(10) << " ";
				std::cout << " " << i;
				for (Complex& c : r)
				{
					std::cout << "\t(";
					if (c.real() > 0)std::cout << " ";
					if (c.imag() < 0)std::cout << c.real() << "\t-j" << setw(13) << -c.imag() << ")";
					else             std::cout << c.real() << "\t+j" << setw(13) << c.imag() << ")";

				}
				i++;
				std::cout << std::endl;
			}
			break;
		case POLAR:
			double phase, mag;
			std::cout << " \t ********************* H matrix (magnitude |_ phase(rad))**************" << std::endl;
			for (std::vector<Complex> r : Rows)
			{
				std::cout << setw(10) << " ";
				std::cout << " " << i;
				for (Complex c : r)
				{
					phase = std::arg(c);
					mag = std::abs(c);//sqrt(pow(c.real(), 2) + pow(c.imag(), 2));
					std::cout << "\t";
					std::cout << mag << "\t|_";
					if (phase > 0)std::cout << " ";
					cout << setw(5) << phase;
				}
				i++;
				std::cout << std::endl;
			}
			break;
		}

		std::cout << " \t **************************************************************" << std::endl;
	}

	void WriteFile(std::string file_out, ComplexForm form)
	{
		// this function writes the matrix to a file
		std::ofstream out{ file_out, std::ios_base::out | std::ios_base::trunc };
		std::ostream_iterator<string> out_iter2{ out, " " };
		vector<std::string>row;
		std::stringstream converter;
		std::string s, i_s;
		int i_index = 0;
		switch (form)
		{
		case RECT:
			for (std::vector<Complex>& r : Rows)
			{
				if (i_index < this->Indices.size()) { converter << this->Indices.at(i_index); converter >> i_s; converter.clear(); row.push_back(i_s); }
				row.push_back("\t");
				for (Complex& i : r)
				{
					//row.push_back("(");
					converter << i.real(); converter >> s; converter.clear();
					row.push_back(s);
					row.push_back("\t");
					converter << i.imag(); converter >> s; converter.clear();
					row.push_back(s);
					row.push_back("\t");
				}
				std::copy(std::begin(row), std::end(row), out_iter2);
				//	std::copy(std::begin(row), std::end(row), std::ostream_iterator<string>{std::cout, " "});
				out_iter2 = "\n";
				//	cout << endl;
				row.clear();
				i_index++;
			}
			break;
		case POLAR:
			double phase;
			double mag;
			for (std::vector<Complex>& r : Rows)
			{
				if (i_index < this->Indices.size()) { converter << this->Indices.at(i_index); converter >> i_s; converter.clear(); row.push_back(i_s); }
				row.push_back("\t");
				for (Complex& i : r)
				{
					phase = std::arg(i);
					mag = std::abs(i);
					converter << mag; converter >> s; converter.clear();
					row.push_back(s);
					row.push_back("\t");
					converter << phase; converter >> s; converter.clear();
					row.push_back(s);
					row.push_back("\t");
				}
				std::copy(std::begin(row), std::end(row), out_iter2);
				out_iter2 = "\n";
				row.clear();
				i_index++;
			}

		}
	}
	void WriteFile(std::string file_out)
	{
		// this function writes the matrix to a file
		// check if the parent directory exixts
		std::string parent_directory = file_out;
		size_t position = file_out.find_last_of("/");
		std::string file_name = parent_directory.substr(position);
		parent_directory.erase(position, file_name.size());
		if (!std::filesystem::exists(parent_directory))
		{
			if (std::filesystem::create_directory(parent_directory))cout << parent_directory << " Directory Created" << endl; else { cout << parent_directory << " Directory can't be created " << endl; exit(0); }
		}
		std::ofstream out{ file_out, std::ios_base::out | std::ios_base::trunc };
		std::ostream_iterator<string> out_iter2{ out, " " };
		vector<std::string>row;
		std::stringstream converter;
		std::string s, i_s;
		int i_index = 0;
		for (auto r : Rows)
		{
			if (i_index < this->Indices.size()) { converter << this->Indices.at(i_index); converter >> i_s; converter.clear(); row.push_back(i_s); row.push_back("\t"); }

			for (auto i : r)
			{
				converter << i; converter >> s; converter.clear();
				row.push_back(s);
				row.push_back("\t");
			}
			std::copy(std::begin(row), std::end(row), out_iter2);
			out_iter2 = "\n";
			row.clear();
			i_index++;
		}
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
		//	cout << " DElete row :  " << n << endl;
		size_t l1, l2;
		if (this->Rows_Count > n)
		{
			/// sawp down the found column
			l1 = n;
			//	while (l1 < this->Columns_Count - 1)
			//	{
			//		l2 = this->Columns_Count - 1;
			//		cout << " l1 " << l1 << " l2 " << l2 << endl;
			//		if (!this->SwapRows(l1, l2))
			//		{
			//			cout << " row number " << l1 << " is the last row " << endl;
			//			break;
			//		}
			//		l1++;
			//	}
				// delete last raw
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
public:
	std::vector<Vector> Rows;
	unsigned Rows_Count;
	unsigned Columns_Count;
	vector<size_t> Indices;
};

//matrix<complex<double>> operator/(matrix<complex<double>>, const double);
//matrix<double> operator/(matrix<double> matrix, const double);
//matrix<Complex> operator/(matrix<Complex>, matrix<double>);

