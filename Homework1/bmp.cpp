#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;
int main(){
	ifstream in;
	in.open("pic.bmp");
	ostringstream str_stream;
	str_stream << in.rdbuf();
	string file_str = str_stream.str();
	const char* char_ptr = file_str.c_str();
	const char* width_ptr = char_ptr + 0x12;
	int width = *((int*) width_ptr);
	cout << width << '\n';

}



