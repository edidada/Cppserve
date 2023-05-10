#include "json.h"
#include "httpserve.h"
#include "corm.h"
#include "client.h"
#include "router.cpp"


int main()
{

    
   ThreadPool pool(16);
   pool.init();

    CmysqlPool *sqlPool=CmysqlPool::AcquireCmysqlPool();
    sqlPool->InitCmysqlPool(16,"127.0.0.1", "root", "131420", "test", 3306);

    Router *r=Router::AcquireRouter();
    r->StartThreadPool(&pool);
    RouterManage();


    chamber c(9999);
    c.run();
    return 0;
}