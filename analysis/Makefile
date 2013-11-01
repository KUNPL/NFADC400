ROOT_CFLAGS = $(shell root-config --cflags)
ROOT_LIBS = $(shell root-config --libs)

UNAME = $(shell uname -rs | cut -d. -f 1)
MAVERICKS = Darwin 13
ifeq ($(UNAME),$(MAVERICKS))
  CXX = g++ -stdlib=libstdc++
else
  CXX = g++
endif


PREFIX = FADC400
TARGET = lib$(PREFIX)
SOURCE = $(wildcard $(PREFIX)*.cc)
HEADER = $(filter-out FADC400Constants.hh,$(wildcard $(PREFIX)*.hh))
DICT = $(TARGET)Dict.cc
LINKDEF = $(TARGET)LinkDef.hh
CFLAGS = -I./ $(ROOT_CFLAGS) $(NK_CFLAGS)
LIBS = $(ROOT_LIBS)

$(TARGET): $(SOURCE:.cc=.o) $(DICT:.cc=.o)
	$(CXX) --shared -o $@.so $(LIBS) $^

$(DICT): $(HEADER) $(LINKDEF)
	rootcint -f $@ -c $^
	$(CXX) -c -fPIC $@ $(CFLAGS)

$(LINKDEF):
	@echo "" > LinkdefSpace
	@echo "#ifdef __CINT__" > LinkdefHeader
	@$(shell ls | grep ^$(PREFIX) | grep hh | awk -F. {'if ($$1 != "FADC400Constants") printf("#pragma link C++ class %s+;\n", $$1)'} > LinkdefBody)
	@echo "#endif" > LinkdefFooter
	@cat LinkdefHeader LinkdefSpace LinkdefBody LinkdefSpace LinkdefFooter > $@
	@rm -rf LinkdefSpace LinkdefHeader LinkdefBody LinkdefFooter

clean:
	@rm -rf $(DICT:.cc=.*)
	@rm -rf *.o
	@rm -rf $(LINKDEF)
	@rm -rf $(TARGET).so

%.o: %.cc
	$(CXX) -o $@ -c -fPIC $(CFLAGS) $<
