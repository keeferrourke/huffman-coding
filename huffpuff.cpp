/* huffpuff.cpp
 * Written by:  Keefer Rourke
 * License:     GPLv3
 *
 * COPYRIGHT    Keefer Rourke 2015
 *
 * Description: This program will take a plain-text file as input and
 *              using Huffman coding, it will compress the file to an
 *              output binary file that is smaller in size.
 *
 *              This program is also capable of reinflating the binary
 *              file to its original plain-text format so that it can
 *              be read.
 *
 * Disclaimer:  This program is free software: you can redistribute it
 *              and/or modify it under the terms of the GNU General
 *              Public License as published by the Free Software
 *              Foundation, either version 3 of the License, or (at
 *              your option) any later version.
 *
 *              This program is distributed in the hope that it will
 *              be useful, but WITHOUT ANY WARRANTY; without even the
 *              implied warranty of MERCHANTABILITY or FITNESS FOR A
 *              PARTICULAR PURPOSE.  See the GNU General Public License
 *              for more details.
 *
 *              You should have received a copy of the GNU General
 *              Public License along with this program.  If not, see
 *              <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include "lib/hufftree.hpp"
#include "lib/huff.hpp"
#include "lib/puff.hpp"

using namespace std;

bool isEmpty(string file);
void printUse();

int main(int argc, char * argv[])
{
    // check that the number of arguments is valid, if not then print usage instructions and exit
    if (argc < 3 || argc > 4)
    {
        cout << "Error. Invalid number of arguments." << endl;
        printUse();
        return 0;
    }
    // argv[2] is the input file name, and argv[3] is the output file name
    // if user specifies that they want to extract a file, extract the file
    // the function huffExtract(string, string) is in huff.hpp
    else if ((strcmp(argv[1], "-x")) == 0 || (strcmp(argv[1], "--extract")) == 0 || (strcmp(argv[1], "--decompress")) == 0 || (strcmp(argv[1], "--inflate")) == 0)
       {
           string infilename = argv[2];
           // check if specified infile is empty, quit if true
           if (isEmpty(infilename))
           {
               cerr << "Error: " << infilename << " is an empty file." << endl;
               return 0;
           }

           if (argc > 3)
           {
               string outfilename = argv[3];
               huffExtract(infilename, outfilename);
           }
           else
               huffExtract(infilename);
       }
    // if user specifies that they want to compress a file, compress the file
    // the function huffCompress(string, string) is in puff.hpp
    else if ((strcmp(argv[1], "-c")) == 0 || (strcmp(argv[1], "--compress")) == 0)
    {
        string infilename = argv[2];
        // check if specified infile is empty, quit if true
        if (isEmpty(infilename))
        {
            cerr << "Error: " << infilename << " is an empty file." << endl;
            return 0;
        }

        if (argc > 3)
        {
            string outfilename = argv[3];
            huffCompress(infilename, outfilename);
        }
        else
            huffCompress(infilename);
    }
    // if user specifies bad arguments, print usage
    else
    {
        cerr << "Error. Bad arguments." << endl;
        printUse();
    }

    return 0;
}

/* this function will check if a file is empty */
bool isEmpty(string file)
{
    ifstream infile(file.c_str());
    return infile.peek() == ifstream::traits_type::eof();
}

/* this function simply prints the manual for the huffpuff program
 * in the case that a user makes a syntax error while using the utility */
void printUse()
{
    cout << "\nNAME" << endl;
    cout << "   huffpuff - a Huffman coding implementation\n" << endl;
    cout << "SYNOPSIS" << endl;
    cout << "   huffpuff [-c] [--compress] [-x] [--extract] [--decompress]" << endl;
    cout << "   [--inflate] file ...\n" << endl;
    cout << "DESCRIPTION" << endl;
    cout << "   Compress plain-text files, and decompress Huffman binary files" << endl;
    cout << "   created by this programme.\n" << endl;
    cout << "OPTIONS" << endl;
    cout << "   Mandatory arguments are as follows, plus the input file name.\n" << endl;
    cout << "   -c, --compress" << endl;
    cout << "       compress a plain-text file to a smaller binary file\n" << endl;
    cout << "   -x, --extract, --decompress, --inflate" << endl;
    cout << "       decompress a binary file back to plain-text\n" << endl;
    cout << "   Optionally an output file name can be specified (see usage)\n " << endl;
    cout << "USAGE EXAMPLES" << endl;
    cout << "   huffpuff -c inputfile.txt" << endl;
    cout << "   huffpuff -x inputfile.bin" << endl;
    cout << "   huffpuff --compress inputfile.txt outputfile.bin" << endl;
    cout << "   huffpuff --inflate inputfile.bin outputfile.txt\n" << endl;
}
