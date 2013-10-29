ROOT_CFLAGS = $(shell root-config --cflags)
ROOT_GLIBS = $(shell root-config --glibs)

# The lines below are uncommented after finishing development.
# NK_CFLAGS = -DNKROOT -I/$(NKHOME)/include
# NK_LIBS = -L/$(NKHOME)/lib  -lNotice6UVME_root -lNoticeTDC64M_root

UNAME = $(shell uname -rs | cut -d. -f 1)
MAVERICKS = Darwin 13
ifeq ($(UNAME),$(MAVERICKS))
  CXX = g++ -stdlib=libstdc++
else
  CXX = g++
endif


PREFIX = FADC400
TARGET = $(PREFIX)
SOURCE = $(wildcard $(PREFIX)*.cc)
HEADER = FADC400.hh
DICT = $(TARGET)Dict.cc
LINKDEF = $(TARGET)LinkDef.hh
CFLAGS = -I./ $(ROOT_CFLAGS) $(NK_CFLAGS)
LIBS = $(ROOT_GLIBS) $(NK_LIBS)

$(TARGET): $(SOURCE:.cc=.o) $(DICT:.cc=.o)
	$(CXX) -fPIC -o $@ $(LIBS) $^

$(DICT): $(HEADER) $(LINKDEF)
	rootcint -f $@ -c $^
	$(CXX) -c -fPIC $@ $(CFLAGS)

$(LINKDEF):
	@echo "" > LinkdefSpace
	@echo "#ifdef __CINT__" > LinkdefHeader
	@$(shell ls | grep ^$(PREFIX) | grep hh | awk -F. {'if ($$1 != "FADC400Settings.hh") printf("#pragma link C++ class %s+;\n", $$1)'} > LinkdefBody)
	@echo "#endif" > LinkdefFooter
	@cat LinkdefHeader LinkdefSpace LinkdefBody LinkdefSpace LinkdefFooter > $@
	@rm -rf LinkdefSpace LinkdefHeader LinkdefBody LinkdefFooter

clean:
	@rm -rf $(DICT:.cc=.*)
	@rm -rf *.o
	@rm -rf $(LINKDEF)
	@rm -rf $(TARGET)

%.o: %.cc
	$(CXX) -o $@ -c -fPIC $(CFLAGS) $<
