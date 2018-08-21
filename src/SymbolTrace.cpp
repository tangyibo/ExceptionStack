#include "SymbolTrace.h"
#include <cxxabi.h>
#include <execinfo.h>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <boost/thread/mutex.hpp>  
#include <boost/thread/locks.hpp>

std::map<int,SymbolTrace::callback> SymbolTrace::_mapfun;
SymbolTrace* SymbolTrace::_instance=NULL;

SymbolTrace::SymbolTrace()
{
    SymbolTrace::_mapfun.clear();
    SymbolTrace::_instance=NULL;
}

SymbolTrace::~SymbolTrace()
{
    SymbolTrace::_mapfun.clear();
    
    if(NULL!=SymbolTrace::_instance)
    {
        delete SymbolTrace::_instance;
        SymbolTrace::_instance=NULL;
    }
}

SymbolTrace* SymbolTrace::instance()
{
    static boost::mutex locker;
    {
        boost::mutex::scoped_lock g(locker);
        if (NULL == _instance)
        {
            _instance = new SymbolTrace();
        }
    }
    
    return _instance;
}

void SymbolTrace::defaultSignalHandler(int signo)
{
    if (signo >= 1 && signo < 32)
    {
        SymbolTrace::instance()->fillStackTrace();

        std::map<int, callback>::const_iterator it = SymbolTrace::_mapfun.find(signo);
        if (it != SymbolTrace::_mapfun.end())
        {
            int ret = (it->second)(signo);
            exit(ret);
        }
    }
}

void SymbolTrace::addSignal(int signo,callback cb)
{
    struct sigaction sa;
    sa.sa_handler = &SymbolTrace::defaultSignalHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(signo, &sa, NULL);
    _mapfun[signo]=cb;
}

const char* SymbolTrace::stackTrace() const throw ()
{
    return stack_.c_str();
}

void SymbolTrace::fillStackTrace()
{
    const int len = 200;
    void* buffer[len];
    int nptrs = ::backtrace(buffer, len);
    char** strings = ::backtrace_symbols(buffer, nptrs);

    if (strings)
    {
        stack_.clear();
        
        for (int i = 0; i < nptrs; ++i)
        {
            std::stringstream ss;
            ss << "\n >>>[" << i << "] ";
            stack_.append(ss.str());
            stack_.append(demangle(strings[i]));
        }

        free(strings);
        strings=NULL;
    }
}

std::string SymbolTrace::demangle(const char* symbol)
{
    size_t size;
    int status;
    char temp[128];
    char* demangled;

    if (1 == sscanf(symbol, "%*[^(]%*[^_]%127[^)+]", temp))
    {
        if (NULL != (demangled = abi::__cxa_demangle(temp, NULL, &size, &status)))
        {
            std::string result(demangled);
            free(demangled);
            return result;
        }
    }

    if (1 == sscanf(symbol, "%127s", temp))
    {
        return temp;
    }

    return symbol;
}
