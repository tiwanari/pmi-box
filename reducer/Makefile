CXX := g++
CXXFLAGS = $(opts)
INCLUDES = -I.
INCLUDES += -I../
INCLUDES += -I$(TEST_INCLUDE_DIR)
TEST_LIBS = $(LIBS)
TEST_LIBS += $(TEST_STATIC_LIBRARY)

OBJDIR := ./obj
LIBRARY_DIR := ../lib/gen

GTEST_DIR := ../googletest/googletest
TEST_INCLUDE_DIR := $(GTEST_DIR)/include
TEST_STATIC_LIBRARY := $(LIBRARY_DIR)/libgtest.a


h_files  := $(wildcard *.h)
cc_files := $(wildcard *.cc)
objs = $(addprefix $(OBJDIR)/, $(cc_files:.cc=.o))
opts :=
opts += -O3
opts += -std=c++11
opts += -Wall
opts += -Werror
opts += -Wno-sign-compare

ifeq (${shell uname},Linux)
opts += -lpthread
endif

debug := -g

# test options
test_h_files  := $(h_files)
test_h_files  += $(wildcard unittest/*.h)
test_cc_files := $(filter-out main.cc, $(cc_files))
test_cc_files += $(wildcard unittest/*.cc)
test_objs = $(addprefix $(OBJDIR)/, $(test_cc_files:.cc=.o))
test_opts := $(debug)


main : $(LIBS) $(objs) $(h_files)
	$(CXX) $(CXXFLAGS) $(LIBS) -o $@ $(objs)

$(OBJDIR)/%.o : %.cc
	@mkdir -p $(OBJDIR)/unittest
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<


.PHONY: run
run:
	./main

.PHONY: test
test: $(TEST_LIBS) $(test_objs) $(test_h_files)
	$(CXX) $(CXXFLAGS) $(test_opts) $(TEST_LIBS) -o $@ $(test_objs)
	./$@

$(TEST_STATIC_LIBRARY):
	cd ..; git submodule init; git submodule update
	$(CXX) -c -I$(TEST_INCLUDE_DIR) -I$(GTEST_DIR) $(GTEST_DIR)/src/gtest-all.cc -o $(GTEST_DIR)/gtest-all.o
	mkdir -p $(LIBRARY_DIR)
	ar -rv $(TEST_STATIC_LIBRARY) $(GTEST_DIR)/gtest-all.o

debug: $(LIBS) $(objs) $(h_files)
	$(CXX) $(debug) $(CXXFLAGS)  $(LIBS) -o $@ $(objs)
	gdb ./$@

.PHONY: clean
clean :
	-rm -rf obj
	-rm -f main
	-rm -f test
	-rm -f debug
	-rm -rf unittest/out/*
	-rm -r *.dSYM
