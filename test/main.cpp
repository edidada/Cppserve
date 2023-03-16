#include "../chamber.h"
#include "router.cpp"


int main()
{
    ThreadPool pool(16);
    pool.init();
    router.StartThreadPool(&pool);
    RouterManage();
    chamber c(9999);
    c.run();
    return 0;
}