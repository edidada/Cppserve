#include "../Cmdoule.h"



class Crpc{
public:
    Crpc(int port,std::string add):port(port),address_ip(add){};
    bool RpcRun(std::string);
    
private:
    int port;
    std::string address_ip;
};

