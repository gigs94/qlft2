#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <FullCompress.h>
#include <CsvParser.h>

int main()
{
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream infile{"ebat.csv"};
    CsvParser parser{infile};

    /* Row line_definition;
    line_definition.addField("stockName", FieldType::stringT);
    line_definition.addField("exchange", FieldType::stringT);
    line_definition.addField("side", FieldType::stringT);
    line_definition.addField("condition", FieldType::stringT);
    line_definition.addField("time", FieldType::longT);
    line_definition.addField("reptime", FieldType::longT);
    line_definition.addField("price", FieldType::floatT);
    line_definition.addField("size", FieldType::intT); */

    std::vector<std::string> stocks{};
    std::vector<std::string> exchange{};
    std::vector<std::string> side{};
    std::vector<std::string> condition{};
    std::vector<uint64_t> time{};
    std::vector<uint64_t> reptime{};
    std::vector<float> price{};
    std::vector<uint64_t> size{};

    // Since CsvParser reads the first line in on ctor, you have to do the do-while loop.
    do {
       try {
           stocks.push_back(parser.stringValue());
           exchange.push_back(parser.stringValue());
           side.push_back(parser.stringValue());
           condition.push_back(parser.stringValue());
           time.push_back(parser.longValue());
           reptime.push_back(parser.longValue());
           price.push_back(parser.floatValue());
           size.push_back(parser.intValue());
       } catch (std::overflow_error err) {
           std::cerr << "Problem parsing the file.   Reached EOL before reading required fields" << std::endl;
           exit(1);
       }
    } while (parser.moveToNextLine());

    FullCompress full{stocks, exchange, side, condition, time, reptime, price, size};
    FullCompress decompress{};

    
    {
        std::ofstream outfile{"ebat.compressed"};
        boost::archive::binary_oarchive oa(outfile);
        oa << full;
    }

    auto finish = std::chrono::high_resolution_clock::now();
    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(finish-start);

    
    {
        std::ifstream ifs("ebat.compressed");
        boost::archive::binary_iarchive ia(ifs);
        ia >> decompress;
        assert(full==decompress);
   }

   // TODO calculate statistics for the compression... need to start a stopwatch, get the input filesize(ifs), and the compressed file size(cfs).
   // compression ratio = cfs/ifs.
    std::ifstream in("ebat.csv", std::ifstream::binary | std::ifstream::ate );
    std::ifstream out("ebat.compressed", std::ifstream::binary | std::ifstream::ate );
    int cfs = out.tellg(); 
    int ifs = in.tellg(); 

    std::cout << "file compressed from " << ifs << " bytes to " << cfs << " bytes.  " << cfs/ifs*100 << "% compression.\n";
    std::cout << "completed in " << microseconds.count()/1000000.00 << "s" << std::endl;

};
