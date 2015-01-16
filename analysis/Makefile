ROOT_CFLAGS = $(shell root-config --cflags)
ROOT_GLIBS = $(shell root-config --glibs)

UNAME = $(shell uname -rs | cut -d. -f 1)
CXX = g++

PREFIX = NFADC400
CFLAGS = $(ROOT_CFLAGS)
LIBS = $(ROOT_GLIBS)

SOTARGET = lib$(PREFIX)
SODICT = $(SOTARGET)Dict.cc
SOLINKDEF = $(SOTARGET)LinkDef.hh
SOHEADER = NFADC400Event.hh NFADC400Header.hh

all: $(SOTARGET) 
	$(CXX) -o zeroSuppression zeroSuppression.cc $(CFLAGS) $(LIBS) $(SOHEADER:.hh=.o) $(SODICT:.cc=.o)
	@rm -rf *.o

$(SOTARGET): $(SOHEADER:.hh=.o) $(SODICT:.cc=.o)
	$(CXX) -fPIC --shared -o $(SOTARGET).so $(ROOT_GLIBS) $^

$(SODICT): $(SOHEADER) $(SOLINKDEF)
	rootcint -f $@ -c $^
	$(CXX) -c -fPIC $@ $(ROOT_CFLAGS)

clean:
	@rm -rf zeroSuppression
	@rm -rf $(SODICT:.cc=.*)
	@rm -rf *.o
	@rm -rf $(TARGET)
	@rm -rf $(SOTARGET).so

%.o: %.cc
	$(CXX) -o $@ -c -fPIC $(CFLAGS) $<
