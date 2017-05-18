all:main PathGen cost
main:main.cpp
	g++ -g -o main main.cpp
PathGen:PathGen.cpp
	g++ -g -o PathGen PathGen.cpp
cost:cost.cpp
	g++ -g -o cost cost.cpp
