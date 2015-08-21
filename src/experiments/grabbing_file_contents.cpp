//grabbing_file_contents.cpp
//Keefer Rourke
//
//This is a small experiment with getting the entire contents of a
//plain-text file and shoving it into a single string (newlines included)

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

string getContents(string infilename)
{
    ifstream infile;
    infile.open(infilename.c_str());
    if (!infile.is_open())
    {
        cerr << "could not open file." << endl;
        return "";
    }
    string str;
    string contents;
    while (getline(infile, str))
    {
        contents += str;
        contents.push_back('\n');
    }
    if (infile.bad())
    {
        cerr << "error while reading file." << endl;
        return "";
    }

    return contents;
}

int main()
{
    string filename;
    cout << "enter a filename:" << endl;
    getline(cin, filename);
    
    string contents = getContents(filename);
    cout << contents << endl;

    //terminate program
    return 0;
}
