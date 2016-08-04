#ifndef __ROWS_H__
#define __ROWS_H__

#include <string>
#include <vector>
#include <CsvParser.h>

using string_type = std::string;


enum FieldType { stringT, longT, floatT, intT };

class FieldMetadata {
    public:
        FieldMetadata() : _ft(stringT) {};
        FieldMetadata(std::string& name, FieldType ft) : _name(name), _ft(ft) {};
        FieldMetadata(FieldMetadata&& rhs) : _name(std::move(rhs._name)), _ft(rhs._ft) {};
        FieldMetadata(const FieldMetadata& rhs) : _ft(rhs._ft), _name(rhs._name) {};

        FieldType type() { return _ft; };
        std::string& name() { return _name; };

    private:
        FieldType _ft;
        std::string _name;
};

class Field {
    public:
        union {
            int i;
            double d;
            long l;
            float f;
            std::string s;
        };

        Field(FieldMetadata& fmd) : i{0}, _fmd{fmd} {};
        Field(const Field& rhs) : _fmd{rhs._fmd} {
                switch(_fmd.type()) {
                    case stringT:
                        s = rhs.s;
                        break;
                    case longT:
                        l = rhs.l;
                        break;
                    case floatT:
                        f = rhs.f;
                        break;
                    case intT:
                        i = rhs.i;
                        break;
                }
        };
        virtual ~Field() { if (_fmd.type() == stringT) s.~string_type();};

        FieldType type() { return _fmd.type(); };
        std::string& name() { return _fmd.name(); };
        
    private:
        FieldMetadata _fmd;
};

class Row {
    /**
     * Row is a class that encapsulates a set of FieldMetadatas and provides a utility
     * method to take a CsvParser line and return the vector of fields.
     *
     * Note: Row doesn't seem like the right name for this class... but RowMetadata or
     * FieldMetadatas just didn't seem right either
     */

    public:
        Row() {};

        void addField(std::string name, FieldType ft) {
            FieldMetadata field{name,ft};
            _fields.push_back(field);
        }

        std::vector<Field> parseLine(CsvParser& line) {
            std::vector<Field> rtn;


            for(FieldMetadata fmd : _fields) {
                Field f{fmd};
                switch(fmd.type()) {
                    case stringT:
                        f.s = line.stringValue();
                        break;
                    case longT:
                        f.l = line.longValue();
                        break;
                    case floatT:
                        f.f = line.floatValue();
                        break;
                    case intT:
                        f.i = line.intValue();
                        break;
                }
                rtn.push_back(f);
            }
            return rtn;
        }
   
    private:
        std::vector<FieldMetadata> _fields;
        
};

#endif // __ROWS_H__
