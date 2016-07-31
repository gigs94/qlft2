#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <Field.h>


int main()
{
    std::ifstream infile{"input.csv"};

    while (infile)
    {
        std::string line;
        std::string stockName;
        long timestamp;
        int volume;
        int price;

        std::cout << "output: " << stockName << ":" << time << std::endl;
    };
};
