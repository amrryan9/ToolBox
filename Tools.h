#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <matrix.h>
#include <random>
#include <algorithm>
#include <functional>
#include <cmath>
#include <cerrno>
#include <cfenv>

using namespace std;

enum angle_unit{ RAD, DEG};
enum FILES { CIR, DOD };

struct RECORD
{
	RECORD()
	{
		Receiver_pt = 0;
		LOS_FLAG = false;
	}
	RECORD(size_t tx_pt, bool los)
	{
		Receiver_pt = tx_pt;
		LOS_FLAG = los;
	}
	void SHOW()
	{
		cout << " RECEIVER POINT # : " << Receiver_pt << " LOS FLAG : " << LOS_FLAG << endl;
	}
	size_t Receiver_pt;
	bool LOS_FLAG;
};
struct FILE_COLUMN
{
	FILE_COLUMN() {}
	FILE_COLUMN(std::string name, unsigned char place, double value)
	{
		NAME = name;
		PLACE = place;
		VALUE = value;
	}
	void SET_FILE_COLUMN(std::string name, unsigned char place, double value)
	{
		NAME = name;
		PLACE = place;
		VALUE = value;
	}
	const unsigned char Place() { return PLACE; }
	std::string		NAME;
	unsigned char	PLACE;
	double
		VALUE;
};

struct FileReader
{
	size_t Limit;
	std::vector <std::string>* buffer;
	std::streampos position;
	std::string File_To_Read;
	int Line_Pointer;//Where is in the block now
	int File_Line_Pointer;// Where is in the file now
	size_t edge;//upto the previous buffer
	FileReader(std::string file_full_path,  size_t limit = 1000)
	{
		File_To_Read=file_full_path ;
		Limit = limit;
		position = static_cast<streampos>(0);
		buffer = nullptr;
		Line_Pointer = -1;
		File_Line_Pointer = 0;
		edge = 0;
	}
	size_t ReadSection()
	{
		std::ifstream in_file{ File_To_Read };
		if (!in_file)
		{
			cout << " file is not open." << std::endl;
			std::cerr << " file is not open." << std::endl;
			exit(1);
		}
		if (buffer != nullptr)
		{
			edge = edge + buffer->size();// collect the previous buffer size
			delete buffer;
		}
		buffer = new std::vector <std::string>();
		in_file.seekg(position); 
		std::string line{};
		for (size_t i = 0; i < Limit; i++) {
			if (getline(in_file, line, '\n'))
			{
				
				std::istringstream input2;
				input2.str(line);
				for (std::string line_section; std::getline(input2, line_section, ' '); ) {
					line_section.erase(std::remove(line_section.begin(), line_section.end(), '\t'), line_section.end());
					buffer->push_back(line_section);
				}
			}
		}
		position = in_file.tellg();
		in_file.close();
		if (buffer->size() == 0)
		{
		//	cout << "End of File reached" << endl;
			File_Line_Pointer = -1;
			delete buffer;

		}
		return buffer->size();
	}
	std::string GetLine()
	{
		if ((Line_Pointer == -1)&& File_Line_Pointer != -1) //Line_Pointer >= Limit || 
		{
			if (ReadSection() > 0)
			{
				Line_Pointer = 0;//reset the package pointer
			}
		}
		
		File_Line_Pointer++;
		if ((Line_Pointer) < buffer->size()-1)
		{
			Line_Pointer++;
			return buffer->at(Line_Pointer - 1);
		}
		else
		{
			Line_Pointer = -1;
			return buffer->at(buffer->size() - 1);
		}
		
	}
	bool GetLine(int line_number, std::string& line)
	{
		line.clear();
		while ((line_number > File_Line_Pointer  || File_Line_Pointer==0 || (File_Line_Pointer-edge >= buffer->size())) && File_Line_Pointer != -1)
		{
			GetLine();
		}
		if (line_number <= File_Line_Pointer && (line_number-edge) <= (File_Line_Pointer - edge) && File_Line_Pointer != -1)
		{
			line= buffer->at(line_number -edge);
			return true;
		}
		return false;

	}
};
class Tools
{
public:
	Tools() = default;
	static unsigned int ConvertToUnsigned(std::string str)
	{
		unsigned x = 0;
		unsigned y = 0;
		unsigned C[3] = { 0,0,0 };
		for (char c : str)
		{
			if (x < 3)
			{
				if (c == '0')
					C[x] = 0;
				else if (c == '1')
					C[x] = 1;
				else if (c == '2')
					C[x] = 2;
				else if (c == '3')
					C[x] = 3;
				else if (c == '4')
					C[x] = 4;
				else if (c == '5')
					C[x] = 5;
				else if (c == '6')
					C[x] = 6;
				else if (c == '7')
					C[x] = 7;
				else if (c == '8')
					C[x] = 8;
				else if (c == '9')
					C[x] = 9;
				else
				{
					cout << " ERROR : SET : CONVERTER : Out Of Range" << endl;
					return 0;
				}
			}
			else
			{
				cout << " ERROR : SET : CONVERTER : Out Of Range" << endl;
				return 0;
			}
			x++;
		}
		y = C[0] * 100 + C[1] * 10 + C[2];
		return y;
	}
	static unsigned int ConvertToUnsigned_general(std::string str)
	{
		unsigned y = 0;
		std::vector<unsigned> C;
		for (char c : str)
		{
			if (c == '0')
			{
				if (C.size() > 0) C.push_back(0);
			}
			else if (c == '1')
				C.push_back(1);
			else if (c == '2')
				C.push_back(2);
			else if (c == '3')
				C.push_back(3);
			else if (c == '4')
				C.push_back(4);
			else if (c == '5')
				C.push_back(5);
			else if (c == '6')
				C.push_back(6);
			else if (c == '7')
				C.push_back(7);
			else if (c == '8')
				C.push_back(8);
			else if (c == '9')
				C.push_back(9);
		}
		double C_size = static_cast<double>(C.size() - 1);
		for (auto p : C)
		{
			y = y + p * pow(10, C_size);
			C_size--;
		}
		return y;
	}
	static double ConvertToDouble(std::string str)
	{
		string mantessa, exponenet;
		std::array<std::string, 2>Parts;
		std::array<double, 2>Dici_Parts;
		vector<double>Number;
		double mant, expo;
		size_t position_e = str.find_first_of("e", 0);
		size_t position_E = str.find_first_of("E", 0);
		size_t e;

		if (position_e != std::string::npos && position_E == std::string::npos)
		{
			e = position_e;
		}
		else if (position_e == std::string::npos && position_E != std::string::npos)
		{
			e = position_E;
		}
		else
		{
			e = str.size();
		}
		if (e < str.size() - 1)
		{
			mantessa = str.substr(0, e);
			exponenet = str.substr(e + 1, str.size() - e);
		}
		else if (e <= str.size())
		{
			mantessa = str.substr(0, e);
			exponenet = "0";
		}
		else
		{
			return 0.0;
		}
		//cout << " Matessa : " << mantessa << " Exponenet : " << exponenet << endl;
		Parts[0] = mantessa;
		Parts[1] = exponenet;
		std::string before, after;
		double sign = 1.0;
		size_t dicimal_point = 0;

		for (auto p : Parts)
		{
			if (p.size() > 0)
			{
				sign = 1.0;
				if (p.at(0) == '-')
				{
					sign = -1;
					p.erase(0, 1);
				}

				dicimal_point = p.find_first_of('.', 0);
				before = {};
				after = {};
				if (dicimal_point != std::string::npos)//  && dicimal_point < str.size() - 1)
				{
					after = p.substr(dicimal_point + 1, p.size() - dicimal_point);
					before = p.substr(0, dicimal_point);
					Dici_Parts[0] = static_cast<double>(ConvertToUnsigned_general(before));
					Dici_Parts[1] = static_cast<double>(ConvertToUnsigned_general(after)) / pow(10, after.size());
				}
				else
				{
					after = "";
					before = p;
					Dici_Parts[0] = static_cast<double>(ConvertToUnsigned_general(before));
					Dici_Parts[1] = 0.0;
				}
				Number.push_back((Dici_Parts[0] + Dici_Parts[1]) * sign);
			}
			else
			{
				Number.push_back(0.0);
			}
		}
		if (Number.size() > 1)
		{
			return Number.at(0) * pow(10, Number.at(1));
		}
		else if (Number.size() > 0)
		{
			return Number.at(0);
		}
		else
		{
			return 0.0;
		}
	}
	static std::string ConvertToString(int x)
	{
		std::string result{ "" };
		int y = x; int z{ 0 };
		if (y == 0)
			return "0";
		else {
			while (y > 0)
			{
				z = y % (10);
				if (z == 0)result = "0" + result;
				else if (z == 1)result = "1" + result;
				else if (z == 2)result = "2" + result;
				else if (z == 3)result = "3" + result;
				else if (z == 4)result = "4" + result;
				else if (z == 5)result = "5" + result;
				else if (z == 6)result = "6" + result;
				else if (z == 7)result = "7" + result;
				else if (z == 8)result = "8" + result;
				else if (z == 9)result = "9" + result;
				y = static_cast<unsigned> (floor(static_cast<float>(y) / 10));
			}
			return result;
		}
	}
	static std::string ConvertToString(float x, unsigned dicimal_digits)
	{
		int P1 = int(x);
		int P2 = static_cast<int>((x - static_cast<float>(P1))*10*dicimal_digits);
		return ConvertToString(P1) + "." + ConvertToString(P2);

	}
	static void WriteDoubleVectorToFile(std::string file_out, std::vector<double> data)
	{
		// this function writes the vector to a file
		std::ofstream out{ file_out, std::ios_base::out | std::ios_base::trunc };
		std::ostream_iterator<string> out_iter2{ out, "" };
		vector<std::string>row;
		std::stringstream converter;
		std::string d;
		for (double r : data)
		{
			converter << r; converter >> d; converter.clear();
			row.push_back(d);
			row.push_back("\t");
			std::copy(std::begin(row), std::end(row), out_iter2);
			out_iter2 = "\n";
			row.clear();
		}

	}
	
	static std::string GetWord(std::string source, size_t position)
	{
		if (source.size() > position)
		{
			string s = source;
			vector<string>words;
			size_t i{ 0 };
			while (s.size() > 0)
			{
				i = s.find_first_of(" ");
				if (i >= s.size()) {
					words.push_back(s); s.erase(0,s.size());
				}
				else if (i > 0 && i < s.size()) { words.push_back(s.substr(0, i)); s.erase(0, i + 1); }
				else s.erase(0, i + 1);
			}
			if (words.size() > position)return words.at(position);
		}
		cout << " Error in conversion GetWord at position: "<<position <<" from string :"+ source<< endl;
		return "";
	}
	
	static size_t Permute(size_t n,size_t s)
	{
		float N = static_cast<float>(n);
		float S = static_cast<float>(s);
		float L{ 0 };
		float M{ 0 };
		float T{ 0 };
		float R{ 0 };
		float Limit = (((S / 10) - 100) * 11) + 1 + 100;
		size_t Result{ 0 };
		if (n > 100)
		{
			L = (N - 100) / 11;
			M = floor(L);
			T = (M * 10) + 1000;
			R = round(((L - M) * 11) - 1);
			if (N<=Limit)//T < S || (T==S && R==0))
			{
				if (R < 0)
				{
					Result = static_cast<size_t>(M+100);
				}
				else
				{
					Result = static_cast<size_t>(T + R);
				}
			}
			else Result = static_cast<size_t>(N-(S-1000)-1);
		}
		else
			Result = n;
	//	cout << " " << N << " " << S << " " << L << " " << M << " " << (L - M) <<" "<< R << " "<< T << endl;
	//	cout << " " << n << " IS PERMUTED TO :" << Result << endl;
		return Result;

	}
	static size_t RePermute(size_t n, size_t s)
	{
		float N = static_cast<float>(n);
		float S = static_cast<float>(s);
		
		size_t Result{ 0 };
		if (n > 100 && n <= static_cast<size_t>(S / 10))
		{
			Result = (n - 100) * 10 + n;
		}
		else if (n <= 100)
			Result = n;
		else if (n >= 1000)
		{
			float range = N - 1000;
			size_t tens = static_cast<size_t>(floor(range / 10));
			Result = n - 900 + tens+1;
		}
		else
			Result = n+(s-999);
		//	cout << " " << N << " " << S << " " << L << " " << M << " " << (L - M) <<" "<< R << " "<< T << endl;
		//	cout << " " << n << " IS PERMUTED TO :" << Result << endl;
		return Result;

	}
	static double Round(double n, float d)
	{
		double value = static_cast<int>((n)*pow(10.0, d) + .5);
		return static_cast<double>(value / pow(10.0, d));
	}
	static double Round2(double n, float d)
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
			return Tools::Round(n, d + 4);


	}
	static string PrintComplex(Complex n, ComplexForm form, float d)
	{
		std::stringstream converter;
		string A, B, joint;
		double a{ 0.0 }, b{ 0.0 };
		switch (form)
		{
		case RECT:
			a = Tools::Round2(std::real(n), d);
			b = Tools::Round2(std::imag(n), d);
			//	cout << " n= " << n << "a = " << a << "b = " << b <<endl;
			if (b < 0 || b == 0)
			{
				joint = "-j";
				b = b * -1;
			}
			else
				joint = "+j";
			break;
		case POLAR:
			a = Tools::Round2(std::abs(n), d);
			b = Tools::Round2(std::arg(n), d);
			joint = "|_ ";
			break;
		}
		converter << a; converter >> A; converter.clear();
		converter << b; converter >> B; converter.clear();
		return A + joint + B;
	}
	static float RoundAngles(angle_unit unit, float angle, size_t round_digits)
	{
		float pi;
		switch (unit)
		{
		case RAD:
			pi = 22 / 7;
			break;
		case DEG:
			pi = 180;
			angle = angle * (180 * 7 / 22);
			break;
		default:
			break;
		}
		while (angle > 2*pi)
		{
			angle = angle - (2 * pi);
		}
		while (angle < 0)
		{
			angle = angle + (2 * pi);
		}
		float fraction = pow(10, round_digits);
		angle = angle * fraction;
		return (std::round(angle)) / fraction;
	}
	static std::vector<string> Read_ssv_File(string file_full_path)// // Space Separated File
	{
		std::string file_full_path_copy{ file_full_path };
		std::string file_name, parent_folder;
		if (file_full_path.find('/') == -1)
			if (file_full_path.find('\\') == -1)
			{
				file_name = file_full_path;
				parent_folder = "./";
			}
			else
			{
				file_name = file_full_path_copy.substr(file_full_path.find_last_of('\\') + 1);;
				parent_folder = file_full_path_copy.replace(file_full_path.find_last_of('\\'), file_name.size() + 1, "");;
			}
		else
		{
			file_name = file_full_path_copy.substr(file_full_path.find_last_of('/') + 1);;
			parent_folder = file_full_path_copy.replace(file_full_path.find_last_of('/'), file_name.size() + 1, "");;
		}

	//	cout << file_name << endl;
	//	cout << parent_folder << endl;

		std::ifstream in_file{ file_full_path };
		if (!in_file)
		{
			cout << file_full_path << " file is not open." << std::endl;
			std::cerr << file_full_path <<" file is not open." << std::endl;
			exit(1);
		}

		std::vector<string> file_contents{ std::istream_iterator<string>(in_file),std::istream_iterator<string>() };
		return file_contents;
	}
	static FileReader* Read_ssv_LargeFile(string file_full_path)// // Space Separated File
	{
		std::string file_full_path_copy{ file_full_path };
		std::string file_name, parent_folder;
		if (file_full_path.find('/') == -1)
			if (file_full_path.find('\\') == -1)
			{
				file_name = file_full_path;
				parent_folder = "./";
			}
			else
			{
				file_name = file_full_path_copy.substr(file_full_path.find_last_of('\\') + 1);;
				parent_folder = file_full_path_copy.replace(file_full_path.find_last_of('\\'), file_name.size() + 1, "");;
			}
		else
		{
			file_name = file_full_path_copy.substr(file_full_path.find_last_of('/') + 1);;
			parent_folder = file_full_path_copy.replace(file_full_path.find_last_of('/'), file_name.size() + 1, "");;
		}
		FileReader* READER=new FileReader(file_full_path);
		return READER;
	}
	static bool Wirte_ssv_File(string file_full_path, std::vector<string>& row)// // Space Separated File
	{
		std::string parent_directory = file_full_path;
		size_t position = file_full_path.find_last_of("/");
		std::string file_name = parent_directory.substr(position);
		parent_directory.erase(position, file_name.size());
		file_name.erase(0, 1);
		if (!std::filesystem::exists(parent_directory))
		{
			if (std::filesystem::create_directory(parent_directory))
				cout << parent_directory << " Directory Created" << endl;
			else 
			{ 
				cout << parent_directory << " Directory can't be created " << endl; return false;
			}
		}
		std::ofstream out{ file_full_path , std::ios_base::out | std::ios_base::trunc };//+ extension
		std::ostream_iterator<string> out_iter2{ out};//, " " 

		std::copy(std::begin(row), std::end(row), out_iter2);
		out_iter2 = "\n";
		return true;
	}

	static float Std(valarray<float> x)
	{
		size_t N=x.size();
		valarray<float> y;
		float mu{ 0 };
		if (N > 1)
		{
			y.resize(N);
			mu = x.sum() / static_cast<float>(N);
			size_t j{ 0 };
			for (auto& i : x)
			{
				y[j] = pow(abs(i - mu), 2);
				j++;
			}
			return sqrt((1.0 / static_cast<float>(N - 1)) * y.sum());
		}
		return 0;
	}
	public:
		
		
};

