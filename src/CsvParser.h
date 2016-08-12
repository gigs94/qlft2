
#ifndef __CSV_PARSER_H__
#define __CSV_PARSER_H__

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>


// _getLine is a utility function that takes a ifstream and returns the istringstream
std::istringstream _getLine(std::ifstream& infile) { 
    std::string line;
    if (!std::getline( infile, line )) throw std::overflow_error("EOF");
    std::istringstream ss( line );
    return std::move(ss);
}

// getString is a utility function that takes a istringstream and returns the string until the next comma
std::string _getString(std::istringstream& line) { 
    std::string d;
    if (!getline( line, d, ',' )) throw std::overflow_error("EOL");
    return std::move(d);
};

// _getLong is a utility function that takes a istringstream and returns the integer until the next comma
long _getLong(std::istringstream& line) { 
    return std::stol(_getString(line));
};

// _getInteger is a utility function that takes a istringstream and returns the integer until the next comma
int _getInteger(std::istringstream& line) { 
    return std::stoi(_getString(line));
};

// _getFloat is a utility function that takes a istringstream and returns the float until the next comma
float _getFloat(std::istringstream& line) { 
    float x = std::stof(_getString(line));
    return x;
};


class CsvParser {
    /** CsvParser is a class that allows each configuration for different
     *  field types (columns) from a csv file stream -- contructed on a
     *  stream to read and reads until the next delimiter
     */

    public:

        std::string stringValue() {
            return std::move(_getString(_line));
        };

        int intValue() {
            return _getInteger(_line);
        };

        long longValue() {
            return _getLong(_line);
        };

        float floatValue() {
            return _getFloat(_line);
        };

        bool moveToNextLine() {
            try {
                _line = _getLine(_infile);
            } catch (const std::overflow_error & err) {
                return false;
            }
            return true;
        };

        // ctor/dtor/move
        CsvParser(std::ifstream& mystream) : 
            _infile(std::move(mystream)),
            _line(_getLine(mystream)) {};
        virtual ~CsvParser() {};
        CsvParser(CsvParser&& rhs) noexcept : _line(std::move(rhs._line)), _infile(std::move(rhs._infile)) {};

    private:
        std::istringstream _line;
        std::ifstream _infile;
};

#endif // __CSV_PARSER_H__
