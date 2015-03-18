ALL: run_ABP run_ABP_NAK run_GBN
run_ABP: ABP.h ABP.cpp ABP_main.cpp Lab2_functions.h Lab2_functions.cpp
	g++ -Wall ABP.cpp ABP_main.cpp Lab2_functions.cpp -o run_ABP
run_ABP_NAK: ABP.h ABP.cpp ABP_NAK.h ABP_NAK.cpp ABP_NAK_main.cpp Lab2_functions.h Lab2_functions.cpp
	g++ -Wall ABP.cpp ABP_NAK.cpp ABP_NAK_main.cpp Lab2_functions.cpp -o run_ABP_NAK
run_GBN: ABP.h ABP.cpp GBN.h GBN.cpp Lab2_functions.h Lab2_functions.cpp GBN_main.cpp
	g++ -Wall ABP.cpp GBN.cpp GBN_main.cpp Lab2_functions.cpp -o run_GBN
