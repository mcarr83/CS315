//Michael Carr
//Lab08 main

#include "BinarySearch.hpp"
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <time.h>
#include <locale>
#include <iomanip>

using namespace std;

//struct used as the main data bucket inside a vector
struct wordStruct{
	string key;
	int lineCount;
};


int main(){
	//variables to be used
	ifstream input; string filename; string word; int choice = 0; int lineNumber = 1; char c; int it = 0; string searchWord; vector<wordStruct>words;

	cout << "******************************************************" << endl;
	cout << " Please make sure the end of the text has a space or  " << endl;
	cout << "                   end line {Enter}                   " << endl;
	cout << "******************************************************" << endl;

	//User inputs file name and opens it
	cout<<"Enter the file name: ";
	cin>>filename;
	input.open(filename.c_str());

	//keep asking for a valid file name until one is given
	while(input.fail())
	{
		input.clear();
		cout<<"Incorrect filename, please enter again: " << flush;
		cin>>filename;
		input.open(filename.c_str());
	}

	//character by character are read in and proccessed
	while (input.get(c)){
		//if a character is a letter or number, concatenate it with other letters
		if (isalnum(c)){
			word += c;
		}

		//if character is a given symbol, do nothing
		else if (c == ',' || c == '.' || c == ':' || c == ';' || c == '?'){
		}

		//if a character is a space or end of line
		//intialize a new struct inside a vector
		//put the word in key of struct and put lineNumber in lineCount
		//iterate it
		//make word equal empty string
		else if (c == ' ' || c == '\n'){

			words.push_back(wordStruct());
			words[it].key = word;
			words[it].lineCount = lineNumber;
			it++;
			word = "";

			//if end of the line, iterate lineNumber by one
			if (c == '\n')
				lineNumber++;
		}

	}
	//trim the last element in the vector
	words.pop_back();

	//make an object to use throughout the program
	BinarySearchTree<string>BST;

	//set up time to calculate time it takes to build a tree
	double finish; double start = clock(); double time; //get it ... run!

	//take the vector of struct and pass words and line numbers to build
	//a BST
	for (int i = 0; i < words.size(); i++){

		BST.insert(words[i].key, words[i].lineCount);
	}

	//calculate time
	finish = clock();
	time = (double)(finish - start) / CLOCKS_PER_SEC;

	//print 
	cout << "Total number of words: " <<  words.size() << endl;
	cout << "Total number of distinct words: " << BST.size(BST.getRoot()) << endl;
	cout << "Time taken to build the index using BST is: " << time << endl;
	cout << "Height of BST is: " << BST.getHeight() << endl;

	//close the file that was open
	input.close();


	do{
		cout << "Options: (1) display index, (2) search, (3) save index and exit (4) quit" << endl;
		cin >> choice;


		//print the indexes in order
		if (choice == 1){
			cout << "Binary Search Tree Index:" << endl;
			cout << "-------------------------" << endl;

			BST.printTree();
			cout << endl;
		}

		//search for a word
		else if (choice == 2){

			cout << "Which word to search for? " << flush;
			cin >> searchWord;

			if (BST.contains(searchWord))
				BST.printLine(searchWord);

			else
				cout << "The word is not in the document" << endl;
		}

		//save the indexes to a file the user specifies
		//instead of cout going to the screen, it is redirected to a file
		//only way to get out of a hang is to break out of the program.
		else if (choice == 3){
			char outfile [15];
			cout << "What file do you want to save to: " << flush;
			cin >> outfile;

			freopen(outfile, "w", stdout);

			cout << "Binary Search Tree:" << endl;
			cout << "------------------" << endl;

			BST.printTree();
			cout << endl;

			fclose(stdout);
			freopen("CON", "w", stdout);
			break;
		}

	}while (choice <= 4);

	//Deallocate memory
	BST.makeEmpty();
	return 0;
}
