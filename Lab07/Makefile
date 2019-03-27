usage:
	@echo "Usage:"
	@echo " 1) dependencies: Download the EasyBMP library sources."
	@echo "	2) easyexec:	Compile and link the 'easyexec' executable."
	@echo " 3) clean:	Removes dependencies and compiled files."

#Linking replace with ex
lab07: lab07.o
	g++ -o lab07 lab07.o includes/EasyBMP.o

test: test.o
	g++ -o test test.o includes/EasyBMP.o

#Compilation

lab07.o: lab07.cpp includes/EasyBMP.o
	g++ -c lab07.cpp -Iincludes

test.o: test.cpp includes/EasyBMP.o
	g++ -c test.cpp -Iincludes
#Easy BMP library compilation
includes/EasyBMP.o:
	g++ -c includes/EasyBMP.cpp -o includes/EasyBMP.o

#Download and unzip the EasyBMP library
.PHONY: dependencies
dependencies: includes
	wget http://prdownloads.sourceforge.net/easybmp/EasyBMP_1.06.zip
	unzip EasyBMP_1.06.zip -d includes && rm EasyBMP_1.06.zip

#Create the includes subdirectory if it does not exist
includes:
	mkdir -p includes

.PHONY: clean
clean:
	-rm -r *.bmp *.o *.zip *.tar.gz includes 2> /dev/null || true

.PHONY: archive
archive:
	tar -cvzf carrm_lab7.tar.gz *.bmp *.cpp Makefile
