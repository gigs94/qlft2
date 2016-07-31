#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>


// _getLine is a utility function that takes a ifstream and returns the string until the next comma
std::istringstream _getLine(std::ifstream& infile) { 
    std::string line;
    if (!std::getline( infile, line )) throw std::overflow_error("EOF");
    std::istringstream ss( line );
    return std::move(ss);
}

// getString is a utility function that takes a ifstream and returns the string until the next comma
std::string _getString(std::ifstream& infile) { 
    std::string d;
    if (!getline( _getLine(infile), d, ',' )) throw std::overflow_error("EOF");
    return std::move(d);
};

// _getLong is a utility function that takes a ifstream and returns the integer until the next comma
int _getLong(std::ifstream& infile) { 
    return std::stol(_getString(infile));
};

// _getInteger is a utility function that takes a ifstream and returns the integer until the next comma
int _getInteger(std::ifstream& infile) { 
    return std::stoi(_getString(infile));
};

// _getFloat is a utility function that takes a ifstream and returns the float until the next comma
float _getFloat(std::ifstream& infile) { 
    return std::stof(_getString(infile));
};


enum FieldType { stringFT, intFT, longFT, floatFT };

class Field {
    /** Field is an interface that allows each configuration for different
     *  field types (columns) from a csv file stream -- contructed on a
     *  stream to read and reads until the next delimiter
     */

  public:

    std::string stringValue() {
        return std::move(_getString(_stream));
    };

    // ctor/dtor/etc
    Field(std::ifstream& mystream, FieldType ft) :
        _stream(mystream), _ft(ft) {};
    ~Field() {};

  private:
    std::ifstream& _stream; 
    FieldType _ft; 
};
