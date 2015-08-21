//args.cpp
//Keefer Rourke
//
//This is a small experiment with learning how commandline arguments work
//in c++, using the context of the expected arguments the final program
//will use

#include <iostream>
#include <string>

using namespace std;

void printuse();

int main(int argc, char *argv[]) //argc: number of arguments, argv: vector of arguments
{
    //some variables
    string infile;
    string outfile;
    string option;

    //check to make sure enough arguments are provided
    if (argc < 3 || argc > 4)
        printuse();
    //check to make sure the correct arguments are provided
    else if (argv[1] != "-x" || argv[1] != "-c")
        printuse();
    //do stuff with the arguments
    else
    {
        option = argv[1]; //switch for program mode
        infile = argv[2]; //name of input file
        //if user wants to specify an output file name
        if (argc > 3)
            outfile = argv[3]; //name of output file
    }
    
    if (option == "-x")
        cout << "extract file" << endl;
    else if (option == "-c")
        cout << "compress file" << endl;

    //terminate program
    return 0;
}

//show usage for the program
void printuse()
{
    cout << argv[0] "- a Huffman coding implementation" << endl;
    cout << "SYNOPSIS" << endl;
    cout << argv[0] << " [-] c --compress || x --extract --inflate [filename ...]" << endl;
    cout << "FUNCTIONS" << endl;
    cout << "\t -c, --compress" << endl;
    cout << "\t\t compress a plain-text file" << endl;
    cout << "\t -x, --extract, --inflate" << endl;
    cout << "\t\t decompress a file back to plain-text" << endl;
    cout << "OPTIONAL" << endl;
    cout << "\t Optionally an output file name can be specified" << endl;
    cout << "USAGE EXAMPLES" << endl;
    cout << argv[0] "-c inputfile.txt outputfile.bin" << endl;
    return 0;
}

