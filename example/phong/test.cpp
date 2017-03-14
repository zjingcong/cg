# include <iostream>
# include <fstream>
# include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

string read_shader_program(const char *filePath)
{
		string content;
    ifstream fileStream(filePath, std::ios::in);

    if(!fileStream.is_open()) {
        cerr << "Could not read file " << filePath << ". File does not exist." << endl;
        exit(0);
    }
    string line = "";
    while(!fileStream.eof()) {
        getline(fileStream, line);
        content.append(line + "\n");
    }
		
		return content;
}


int main()
{
	string str;
	str = read_shader_program("phong.vert");
	cout << vs << endl;

	// free(vs);

	return 0;
}

