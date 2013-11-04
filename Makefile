ROOT_CFLAGS = $(shell root-config --cflags)
ROOT_GLIBS = $(shell root-config --glibs)

#The lines below are uncommented after finishing development.
NK_CFLAGS = -DNKROOT -I$(NKHOME)/include
NK_LIBS = -L$(NKHOME)/lib -lNotice6UVME_root -lNoticeFADC400_root

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

$(TARGET): $(SOURCE:.cc=.o) $(DICT:.cc=.o)
	$(CXX) -fPIC -o $@ $(LIBS) $^

$(DICT): $(HEADER) $(LINKDEF)
	rootcint -f $@ -c $^
	$(CXX) -c -fPIC $@ $(CFLAGS)

$(LINKDEF):
	@echo "" > LinkdefSpace
	@echo "#ifdef __CINT__" > LinkdefHeader
	@$(shell ls $(INCDIR) | grep ^$(PREFIX) | grep hh | awk -F. {'if ($$1 == "NFADC400" || $$1 == "NFADC400Header") printf("#pragma link C++ class %s+;\n", $$1)'} > LinkdefBody)
	@echo "#endif" > LinkdefFooter
	@cat LinkdefHeader LinkdefSpace LinkdefBody LinkdefSpace LinkdefFADC400Event LinkdefSpace LinkdefFooter > $@
	@rm -rf LinkdefSpace LinkdefHeader LinkdefBody LinkdefFooter

clean:
	@rm -rf $(DICT:.cc=.*)
	@rm -rf *.o
	@rm -rf $(LINKDEF)
	@rm -rf $(TARGET)

%.o: %.cc
	$(CXX) -o $@ -c -fPIC $(CFLAGS) $<
