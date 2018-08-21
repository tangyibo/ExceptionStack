##
##
##
BINDIR=bin
EXES = $(BINDIR)/except_trace
SRCS=$(wildcard src/*.cpp src/*.c)
OBJS=$(patsubst %.cpp,%.o,$(SRCS) )
RM :=rm -f 

CFLAGS= -g -Wall  -rdynamic -O2 -D_GLIBCXX_USE_CXX11_ABI=0
CXXFLAGS = -g -Wall -rdynamic -O2 -D_GLIBCXX_USE_CXX11_ABI=0
CPPFLAGS = 
LIBS = 

all: dir $(OBJ) $(EXES)

show:
	@echo "EXES=$(EXES)"
	@echo "SRCS=$(SRCS)"
	@echo "OBJS=$(OBJS)"

dir:
	if [ ! -d $(BINDIR) ]; then mkdir $(BINDIR) ; fi;

$(EXES): $(OBJS)
	g++ -o $@ $^ $(CXXFLAGS) $(CPPFLAGS) $(LIBS)

clean:
	$(RM) $(EXES) $(OBJS)
	
.c:
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $<

.cpp:
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

#
#

