all: run_ABP run_ABP_NAK run_GBN
run_ABP:
	g++ -Wall ABP.cpp Lab2_functions.cpp -o run_ABP
run_ABP_NAK:
	g++ ABP_NAK.cpp Lab2_functions.cpp -o run_ABP_NAK
run_GBN:
	g++ GBN.cpp Lab2_functions.cpp -o run_GBN
