TAGLIB := ./include/taglib/
TAGLIB_TOOLKIT := ./include/taglib/toolkit/

main: main.cpp
	g++ -std=c++20 $@.cpp -o $@ -I $(TAGLIB) -I $(TAGLIB_TOOLKIT)