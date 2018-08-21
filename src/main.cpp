#include "Exception.h"
#include "SymbolTrace.h"
#include <stdio.h>
//#include <boost/bind.hpp>

using namespace std;

class Bar
{
public:

    void test()
    {
        throw Exception("oops");
    }
};

void foo()
{
    Bar b;
    b.test();
}

void test_case_1()
{
    try
    {
        foo();
    }
    catch (const Exception& ex)
    {
        printf("reason: %s\n", ex.what());
        printf("stack trace: %s\n", ex.stackTrace());
    }
    catch (const std::exception& ex)
    {
        fprintf(stderr, "reason: %s\n", ex.what());
    }
    catch (...)
    {
        fprintf(stderr, "unknown exception caught \n");
    }
}

class Test
{
public:
    Test(){}

    int handler(int signo)
    {
        const char *stack_trace = SymbolTrace::instance()->stackTrace();
        printf("stack trace [signo=%d]: %s\n", signo, stack_trace);

        return 1;
    }
};

void test_case_2()
{
    Test obj;
    boost::function<int (int) > fun = boost::bind(&Test::handler, &obj, _1);
    SymbolTrace::instance()->addSignal(SIGINT, fun);
    SymbolTrace::instance()->addSignal(SIGTERM, fun);
    SymbolTrace::instance()->addSignal(SIGHUP, fun);
    SymbolTrace::instance()->addSignal(SIGQUIT, fun);
    SymbolTrace::instance()->addSignal(SIGABRT, fun);
    SymbolTrace::instance()->addSignal(SIGSEGV, fun);
    SymbolTrace::instance()->addSignal(SIGILL, fun);
    SymbolTrace::instance()->addSignal(SIGFPE, fun);
    SymbolTrace::instance()->addSignal(SIGPIPE, fun);

    {
        int arr[10];
        for (int i = 0; i <= 11; i++)
            arr[i] = i + 1;
    }

    {
        int *ptr = NULL;
        *ptr = 2;
    }
}

int main()
{
    test_case_2();
}
