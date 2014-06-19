CXX = clang++
CXXFLAGS = -Wall -pedantic -std=c++11 -pthread
LFLAGS = -lboost_system -lboost_filesystem -lboost_program_options -lboost_serialization

SRCDIR = src
OUTDIR = bin

INIT = locate
FORMAT = updatedb

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:%.cpp=$(OUTDIR)/%.o)

all: ADDDIR $(INIT) $(FORMAT)

init: $(INIT)
format: $(FORMAT)

ADDDIR:
	mkdir -p $(OUTDIR)/$(SRCDIR)

$(INIT): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) ./src/mains/locate_main.cpp -o $(INIT) $(LFLAGS)

$(FORMAT): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) ./src/mains/update_main.cpp -o $(FORMAT) $(LFLAGS)

$(OBJECTS): $(OUTDIR)/%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

clean:
	rm -rf $(OUTDIR)
	rm -rf $(INIT)
	rm -rf $(FORMAT)

.PHONY: clean
