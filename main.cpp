#include <iostream>
#include <string.h>
#include <fstream>
#include "fsm.cpp"


int main(int argc, char *argv[])
{
    // Check if user supplied the file to be analyzed
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " {Add file here}" << std::endl;
        return EXIT_FAILURE;
    }

    // Open the input and output file
    std::ifstream iFile;
    std::ofstream outFile("output.txt");
    iFile.open(argv[1]);

    // Check if the file opened properly
    if (iFile.fail())
    {
        std::cerr << "Error opening file. Exiting....\n";
        exit(EXIT_FAILURE);
    }

    outFile << "TOKEN\t\t\tLEXEMES\n";
    outFile << "_______________________________\n";
    while (!iFile.eof())
    {
        std::string line;
        getline(iFile, line);

        fsm(line, outFile);
    }

    iFile.close();
    outFile.close();

    return 0;
}