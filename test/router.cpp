#include "./controller/test.cpp"


void RouterManage(){
    Router *r=Router::AcquireRouter();
    r->Get("index",test);
    r->Get("login",login);
    r->Get("fin",fin);
}

