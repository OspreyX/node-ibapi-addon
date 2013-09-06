#ifndef NODEIBAPI_H
#define NODEIBAPI_H

#define IB_USE_STD_STRING

#include <node.h>
#include "import/IbPosixClient.h"

class NodeIbapi : public node::ObjectWrap {
public:
    static void Init(v8::Handle<v8::Object> exports);

private:
    NodeIbapi();
    ~NodeIbapi();

    static v8::Handle<v8::Value> New(const v8::Arguments& args);
    static v8::Handle<v8::Value> Connect(const v8::Arguments& args);
    static v8::Handle<v8::Value> Disconnect(const v8::Arguments& args);
    static v8::Handle<v8::Value> IsConnected(const v8::Arguments& args);
    static v8::Handle<v8::Value> ProcessMsg(const v8::Arguments& args);

    // requests
    static v8::Handle<v8::Value> ReqCurrentTime(const v8::Arguments& args);
    static v8::Handle<v8::Value> ReqMktData(const v8::Arguments& args);
    static v8::Handle<v8::Value> PlaceOrder(const v8::Arguments& args);
    static v8::Handle<v8::Value> CancelOrder(const v8::Arguments& args);
    static v8::Handle<v8::Value> GetNextOrderId(const v8::Arguments& args);

    // events
    static v8::Handle<v8::Value> CurrentTime(const v8::Arguments& args);
    static v8::Handle<v8::Value> TickPrice(const v8::Arguments& args);

private:
    static char *getChar(v8::Local<v8::Value> value, const char *fallback = "");

    IbPosixClient m_client;

};

#endif