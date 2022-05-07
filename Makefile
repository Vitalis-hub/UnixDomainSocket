# Author: Vitalis Ibe
# E-Mail: vibe@uco.edu
# Date: January 28, 2022

obj	=	p1.o

p1:	${obj}
	g++ -o p1 ${obj}

p1.o:	p1.cpp server.cpp
	g++ -g -c p1.cpp
#----------------------------------------------------------------------
# File p01.cpp processes command line arguments
#----------------------------------------------------------------------
obj2	=	p2.o

p2:	${obj2}
	g++ -o p2 ${obj2}

p2.o:	p2.cpp server.cpp
	g++ -g -c p2.cpp
