CXX = g++
# CXXFLAGS = -g
MAIN = main.cpp
OBJS = tama_file.o tama_tag.o

main: $(MAIN) $(OBJS)
	$(CXX) $(CXXFLAGS) -std=c++20 $(MAIN) $(OBJS) -o $@

tama_file.o: tama_file.cpp
tama_new_tag.o: tama_new_tag.cpp
	$(CXX) -c $<

run: main
	./main

clean:
	rm -f $(OBJS)
