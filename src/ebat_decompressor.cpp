#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <FullCompress.h>
#include <CsvParser.h>

int main()
{
    auto start = std::chrono::high_resolution_clock::now();

    FullCompress decompress{};

    {
        std::ifstream ifs("ebat.compressed");
        boost::archive::binary_iarchive ia(ifs);
        ia >> decompress;
    }

    std::ofstream outfile{"ebat.new.csv"};

    outfile << decompress;

    auto finish = std::chrono::high_resolution_clock::now();
    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(finish-start);
    std::cout << "decompress completed in " << microseconds.count()/1000000.00 << "s" << std::endl;

};
