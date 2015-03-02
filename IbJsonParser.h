#ifndef ibjsonparser_h__INCLUDED
#define ibjsonparser_h__INCLUDED

#include <string>
#include <cstring>
#include <node.h>

using namespace v8;

class JSONNode;

class IbJsonParser {
public:
    IbJsonParser();
    ~IbJsonParser();

    Handle<Object> parse(const JSONNode& n);
private:
    bool filterAsString( const std::string& node_name );
    bool filterAsNumber( const std::string& node_name );
    bool filterAsInteger( const std::string& node_name );
    bool filterAsBoolean( const std::string& node_name );
};

#endif
