CXX = g++
CXXFLAGS = -g
FILES = main.cpp tama_file.cpp tama_tag.cpp

main: $(FILES)
	$(CXX) $(CXXFLAGS) -std=c++20 $(FILES) -o $@

run: main
	./main

clean:
	rm -f main
