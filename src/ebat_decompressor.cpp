#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <FullCompress.h>
#include <CsvParser.h>

int main(int argc, char **argv)
{
  char *ivalue = NULL;
  char *ovalue = NULL;
  int c;

  opterr = 0;

  while ((c = getopt (argc, argv, "i:o:")) != -1)
    switch (c)
      {
      case 'i':
        ivalue = optarg;
        break;
      case 'o':
        ovalue = optarg;
        break;
      case '?':
        if (optopt == 'c' || optopt == 'i' || optopt == 'o')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
        std::cerr << "Usage: ./compressor [-i <filename>] [-o <filename>]" << std::endl;
        std::cerr << "   defaults:  -i ebat.compressed -o ebat.new.csv" << std::endl;
        return 1;
      default:
        abort();
      }


    std::string infileName{ovalue ? ovalue : "ebat.compressed"};
    std::string outfileName{ivalue ? ivalue : "ebat.new.csv"};


    auto start = std::chrono::high_resolution_clock::now();

    FullCompress decompress{};

    {
        std::ifstream ifs{infileName};

        if (!infile.good()) {
            std::cerr << infileName << " not a valid file." << std::endl;
            return 1;
        }

        boost::archive::binary_iarchive ia(ifs);
        ia >> decompress;
    }

    std::ofstream outfile{outfileName};

    outfile << decompress;

    auto finish = std::chrono::high_resolution_clock::now();
    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(finish-start);
    std::cout << "decompress completed in " << microseconds.count()/1000000.00 << "s" << std::endl;

};
