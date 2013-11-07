ROOT_CFLAGS = $(shell root-config --cflags)
ROOT_GLIBS = $(shell root-config --glibs)

#The lines below are uncommented after finishing development.
NK_CFLAGS = -DNKROOT -I$(NKHOME)/include
NK_LIBS = -L$(NKHOME)/lib -lNotice6UVME_root -lNoticeNFADC400_root

UNAME = $(shell uname -rs | cut -d. -f 1)
CXX = g++

PREFIX = NFADC400
TARGET = $(PREFIX)
SOURCE = $(wildcard $(PREFIX)*.cc)
HEADER = NFADC400.hh NFADC400Header.hh NFADC400Event.hh
DICT = $(TARGET)Dict.cc
LINKDEF = $(TARGET)LinkDef.hh
CFLAGS = $(ROOT_CFLAGS) $(NK_CFLAGS)
LIBS = $(ROOT_GLIBS) $(NK_LIBS)

SOTARGET = lib$(PREFIX)
SODICT = $(SOTARGET)Dict.cc
SOLINKDEF = $(SOTARGET)LinkDef.hh
SOHEADER = NFADC400Event.hh NFADC400Header.hh

$(TARGET): $(SOURCE:.cc=.o) $(DICT:.cc=.o)
	$(CXX) -fPIC -o $@ $(LIBS) $^

shared: $(SOHEADER:.hh=.o) $(SODICT:.cc=.o)
	$(CXX) -fPIC --shared -o $(SOTARGET).so $(ROOT_GLIBS) $^

$(DICT): $(HEADER) $(LINKDEF)
	rootcint -f $@ -c $^
	$(CXX) -c -fPIC $@ $(CFLAGS)

$(SODICT): $(SOHEADER) $(SOLINKDEF)
	rootcint -f $@ -c $^
	$(CXX) -c -fPIC $@ $(ROOT_CFLAGS)

$(SOLINKDEF): 
	@echo "" > LinkdefSpace
	@echo "#ifdef __CINT__" > LinkdefHeader
	@echo "#pragma link C++ class NFADC400Header+;" > LinkdefBody
	@echo "#endif" > LinkdefFooter
	@cat LinkdefHeader LinkdefSpace LinkdefBody LinkdefSpace LinkdefNFADC400Event LinkdefSpace LinkdefFooter > $@
	@rm -rf LinkdefSpace LinkdefHeader LinkdefBody LinkdefFooter

$(LINKDEF):
	@echo "" > LinkdefSpace
	@echo "#ifdef __CINT__" > LinkdefHeader
	@$(shell ls $(INCDIR) | grep ^$(PREFIX) | grep hh | awk -F. {'if ($$1 == "NFADC400" || $$1 == "NFADC400Header") printf("#pragma link C++ class %s+;\n", $$1)'} > LinkdefBody)
	@echo "#endif" > LinkdefFooter
	@cat LinkdefHeader LinkdefSpace LinkdefBody LinkdefSpace LinkdefNFADC400Event LinkdefSpace LinkdefFooter > $@
	@rm -rf LinkdefSpace LinkdefHeader LinkdefBody LinkdefFooter

clean:
	@rm -rf $(DICT:.cc=.*)
	@rm -rf $(SODICT:.cc=.*)
	@rm -rf *.o
	@rm -rf $(LINKDEF)
	@rm -rf $(SOLINKDEF)
	@rm -rf $(TARGET)
	@rm -rf $(SOTARGET).so

%.o: %.cc
	$(CXX) -o $@ -c -fPIC $(CFLAGS) $<
