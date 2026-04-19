#include "ArtifactManager.h"
#include <iostream>
#include <fstream>
#include <string>

// This main file is provided for you.
// You do NOT need to modify it.

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << argv[1] << "\n";
        return 1;
    }

    ArtifactManager manager;
    std::string line;

    // Read input file line by line
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#') continue;
        manager.parseAndExecute(line);
    }

    file.close();

    return 0;
}
