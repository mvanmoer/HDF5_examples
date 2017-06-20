CXX = h5c++

BIN = basic_mesh
OBJECTS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

$(BIN): $(OBJECTS)
	$(CXX) -o $@ $^

.PHONY: clean
clean:
	$(RM) $(OBJECTS)
	$(RM) $(BIN)
