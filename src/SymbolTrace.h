#ifndef _EXEPTTRACE_HEADER_H_
#define _EXEPTTRACE_HEADER_H_
#include <string>
#include <map>
#include <signal.h>  //SIGINT/SIGTERM/SIGHUP/SIGQUIT/SIGABRT
#include <boost/function.hpp>
#include <boost/bind.hpp>

class SymbolTrace
{
public:
    typedef boost::function<int (int) > callback;

    SymbolTrace ( );
    virtual ~SymbolTrace ( );

    void addSignal ( int signo,callback cb );
    const char* stackTrace ( ) const throw ( );
    
    static SymbolTrace* instance ( );

private:
    static void defaultSignalHandler ( int signo );

    void fillStackTrace ( ); 
    std::string demangle ( const char* symbol );

    std::string stack_; 

    static std::map<int,callback> _mapfun;
    static SymbolTrace* _instance;
};

#endif /* EXEPTTRACE_H */

