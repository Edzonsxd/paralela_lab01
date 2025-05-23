.PHONY: all
%:
	g++ -std=c++17 -static-libstdc++ -fopenacc -o $@ $@.cpp