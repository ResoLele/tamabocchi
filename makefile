LIBTAG.a := /Library/Developer/CommandLineTools/usr/bin/lib/libtag.a

main: main.cpp
	g++ -std=c++20 $@.cpp tama_file.cpp tama_tag.cpp -o $@ $(LIBTAG.a) -I $(DIR_INCLUDE_TABLIB)