usage:
	@echo "Usage:"
	@echo " 1) dependencies: Download the EasyBMP library sources."
	@echo "	2) easyexec:	Compile and link the 'easyexec' executable."
	@echo " 3) clean:	Removes dependencies and compiled files."

#Linking replace with ex
lab5: lab5.o
	g++ -o lab5 lab5.o 

ex3: ex3.o
	g++ -o ex3 ex3.o includes/EasyBMP.o

#Compilation
lab5.o: lab5.cpp 
	g++ -c lab5.cpp 

ex3.o: ex3.cpp includes/EasyBMP.o
	g++ -c ex3.cpp -Iincludes
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
	-rm -r  *.o *.bmp *.zip *.tar.gz includes 2> /dev/null || true

.PHONY: archive
archive:
	tar -cvzf carrm_lab5.tar.gz *.cpp Makefile
