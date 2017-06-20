CXX = h5c++

BIN = scalar

$(BIN): $(BIN).o
	$(CXX) -o $@ $^

.PHONY: clean
clean:
	$(RM) *.o
	$(RM) $(BIN)
