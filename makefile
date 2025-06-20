main: main.cpp
	g++ -std=c++20 $@.cpp tama_file.cpp tama_tag.cpp -o $@

clean:
	rm main
