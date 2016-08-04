#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <FullCompress.h>
#include <CsvParser.h>

int main(int argc, char **argv)
{
  char *cvalue = NULL;
  char *ivalue = NULL;
  char *ovalue = NULL;
  int c;

  opterr = 0;

  while ((c = getopt (argc, argv, "i:o:c:")) != -1)
    switch (c)
      {
      case 'i':
        ivalue = optarg;
        break;
      case 'o':
        ovalue = optarg;
        break;
      case 'c':
        cvalue = optarg;
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
        std::cerr << "Usage: ./compressor [-i <filename>] [-o <filename>] [-c <block size>]" << std::endl;
        std::cerr << "   defaults:  -i ebat.csv -o ebat.compressed -c 500" << std::endl;
        return 1;
      default:
        abort();
      }

    auto start = std::chrono::high_resolution_clock::now();

    std::string infileName{ivalue ? ivalue : "ebat.csv"};
    std::string outfileName{ovalue ? ovalue : "ebat.compressed"};
    int blockSize{cvalue ? std::stoi(cvalue) : 500};

    std::ifstream infile{infileName};
    if (!infile.good()) {
        std::cerr << infileName << " not a valid file." << std::endl;
        return 1;
    }

    CsvParser parser{infile};

    std::vector<std::string> stocks{};
    std::vector<std::string> exchange{};
    std::vector<std::string> side{};
    std::vector<std::string> condition{};
    std::vector<int64_t> time{};
    std::vector<int64_t> reptime{};
    std::vector<std::string> price{};
    std::vector<int64_t> size{};

    // Since CsvParser reads the first line in on ctor, you have to do the do-while loop.
    do {
       try {
           // create column based views of the data
           stocks.push_back(parser.stringValue());
           exchange.push_back(parser.stringValue());
           side.push_back(parser.stringValue());
           condition.push_back(parser.stringValue());
           time.push_back(parser.longValue());
           reptime.push_back(parser.longValue());
           price.push_back(parser.stringValue());
           size.push_back(parser.intValue());
       } catch (std::overflow_error err) {
           std::cerr << "Problem parsing the file.   Reached EOL before reading required fields" << std::endl;
           exit(1);
       }
    } while (parser.moveToNextLine());

    FullCompress full{stocks, exchange, side, condition, time, reptime, price, size, blockSize};
    FullCompress decompress{};
    
    {
        std::ofstream outfile{outfileName};
        boost::archive::binary_oarchive oa(outfile);
        oa << full;
    }

    auto finish = std::chrono::high_resolution_clock::now();
    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(finish-start);

    // Verification that the compression worked.
    {
        std::ifstream ifs(outfileName);
        boost::archive::binary_iarchive ia(ifs);
        ia >> decompress;
        assert(full==decompress);
    }

    // Do the stats for the run
    std::ifstream in(infileName, std::ifstream::binary | std::ifstream::ate );
    std::ifstream out(outfileName, std::ifstream::binary | std::ifstream::ate );
    int cfs = out.tellg(); 
    int ifs = in.tellg(); 

    std::cout << "file compressed from " << ifs << " bytes to " << cfs << " bytes.  " << (cfs*1.0)/ifs*100 << "% compression.\n";
    std::cout << "completed in " << microseconds.count()/1000000.00 << "s" << std::endl;

};
