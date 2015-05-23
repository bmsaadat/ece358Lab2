ALL: ABP_exec ABP_NAK_exec GBN_exec
ABP_exec: ABP.h ABP.cpp ABP_main.cpp Lab2_functions.h Lab2_functions.cpp
	g++ -Wall ABP.cpp ABP_main.cpp Lab2_functions.cpp -o ABP_exec
ABP_NAK_exec: ABP.h ABP.cpp ABP_NAK.h ABP_NAK.cpp ABP_NAK_main.cpp Lab2_functions.h Lab2_functions.cpp
	g++ -Wall ABP.cpp ABP_NAK.cpp ABP_NAK_main.cpp Lab2_functions.cpp -o ABP_NAK_exec
GBN_exec: ABP.h ABP.cpp GBN.h GBN.cpp Lab2_functions.h Lab2_functions.cpp GBN_main.cpp
	g++ -Wall ABP.cpp GBN.cpp GBN_main.cpp Lab2_functions.cpp -o GBN_exec
