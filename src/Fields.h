#include <string>
#include <vector>

enum FieldType { stringT, longT, floatT, intT };

class Field {
    public:
        Field(std::string& name, FieldType ft) : _name(name), _ft(ft) {};

        FieldType getType() { return _ft; };
        std::string& getName() { return _name; };

    private:
        FieldType _ft;
        std::string _name;
        
};

class Fields {

    public:
        Fields() {};

        void addField(std::string name, FieldType ft) {
            Field* field = new Field{name,ft};
            _fields.push_back(field);
        }

        std::vector<basic> getLine() {
            std:vector<basic> rtn;
            for(basic b : _fields) rtn.push_back<b>;
            return rtn;
        }
   
    private:
        std::vector<Field*> _fields;

};
