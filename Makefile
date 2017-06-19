WARNINGS = -Wall -Wextra
STD = -ansi -pedantic -std=c++11
DEBUG =
OPTIMIZE = -O0
CXXFLAGS = $(WARNINGS) $(STD) $(DEBUG) $(OPTIMIZE)
LIBDIRS = -L/home/mvanmoer/builds/HDF5/1.8.18/lib
INCDIRS = -I/home/mvanmoer/builds/HDF5/1.8.18/include
CPPFLAGS = $(LIBDIRS) $(INCDIRS)
LIBS = -lhdf5

BIN = basic_mesh
OBJECTS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

$(BIN): $(OBJECTS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $^ $(LIBS)

.PHONY: clean
clean:
	$(RM) $(OBJECTS)
	$(RM) $(BIN)
