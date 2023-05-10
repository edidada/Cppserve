#include "../Cmdoule.h"

class Srpc{
public:
    Srpc(int port,std::string add):port(port),address_ip(add){};
    bool InitRpcRun();
private:
    int port;
    std::string address_ip="0.0.0.0";
};