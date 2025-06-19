LIBTAG.a := ${CURDIR}/include/libtag.a

main: main.cpp
	g++ -std=c++20 $@.cpp tama_file.cpp tama_tag.cpp -o $@ $(LIBTAG.a)

clean:
	rm -f main