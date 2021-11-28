bpt: BPT.cpp BPT.h main.cpp delet.cpp insert.cpp
	g++ -Wall BPT.cpp main.cpp delet.cpp insert.cpp -o bpt
clean: 
	-rm bpt
	