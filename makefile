CXX = g++ -std=c++20
CXXFLAGS = -fsanitize=address,leak
MAIN = main.cpp
OBJS = tama_file.o tama_tag.o tama_user.o

main: $(MAIN) $(OBJS)
	$(CXX) $(CXXFLAGS) $(MAIN) $(OBJS) -o $@

tama_file.o: tama_file.cpp
	$(CXX) -c $<

tama_tag.o: tama_tag.cpp
	$(CXX) -c $<

tama_user.o: tama_user.cpp
	$(CXX) -c $<

clean:
	rm -f main $(OBJS)