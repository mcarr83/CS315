//Modified by: Michael Carr
//Lab08 HPP

#ifndef BINARYSEARCH_HPP
#define BINARYSEARCH_HPP

#include <iostream>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <fstream>
#include "dsexceptions.h"

using namespace std;

template <typename Comparable>
class BinarySearchTree
{
	public:
		//Default constructor
		BinarySearchTree( ) : root{ nullptr }
		{
		}


		/**
		 * Move constructor
		 */
		BinarySearchTree( BinarySearchTree && rhs ) : root{ rhs.root }
		{
			rhs.root = nullptr;
		}

		/**
		 * Destructor for the tree
		 */
		~BinarySearchTree( )
		{
			makeEmpty( );
		}

		/**
		 * Copy assignment
		 */
		BinarySearchTree & operator=( const BinarySearchTree & rhs )
		{
			BinarySearchTree copy = rhs;
			swap( *this, copy );
			return *this;
		}

		/**
		 * Move assignment
		 */
		BinarySearchTree & operator=( BinarySearchTree && rhs )
		{
			swap( root, rhs.root );       
			return *this;
		}

		/**
		 * Returns true if x is found in the tree.
		 */
		bool contains( const Comparable & x ) const
		{
			return contains( x, root );
		}

		/**
		 * Test if the tree is logically empty.
		 * Return true if empty, false otherwise.
		 */
		bool isEmpty( ) const
		{
			return root == nullptr;
		}

		/**
		 * Print the tree contents in sorted order.
		 */

		void printTree( ostream & out = cout ) const
		{
			if( isEmpty( ) )
				out << "Empty tree" << endl;
			else
				printTree( root, out );
		}

		//Prints only the line number assoicated with the word

		void printLine ( const Comparable & word) const
		{
			if( isEmpty())
				cout << "Empty tree" << endl;
			else
				printLine(root, word);
		}

		/**
		 * Make the tree logically empty.
		 */
		void makeEmpty( )
		{
			makeEmpty( root );
		}

		/**
		 * Insert x into the tree; duplicates are ignored.
		 */
		void insert( const Comparable & x, int line)
		{
			insert( x, root, line);
		}


	private:
		struct BinaryNode
		{
			Comparable element;
			BinaryNode *left;
			BinaryNode *right;
			vector<int> lineNumber;

			BinaryNode( const Comparable & theElement, BinaryNode *lt, BinaryNode *rt )
				: element{ theElement }, left{ lt }, right{ rt } { }

		};

		BinaryNode *root;


		/**
		 * Internal method to insert into a subtree.
		 * x is the item to insert.
		 * t is the node that roots the subtree.
		 * Set the new root of the subtree.
		 */
		void insert( const Comparable & x, BinaryNode * & t, int line)
		{
			if( t == nullptr ){
				t = new BinaryNode{ x, nullptr, nullptr };
				t->lineNumber.push_back(line);
			}
			else if( x < t->element )
				insert( x, t->left, line );
			else if( t->element < x )
				insert( x, t->right, line );
			else
				t->lineNumber.push_back(line);
		}
		/**
		 * Internal method to test if an item is in a subtree.
		 * x is item to search for.
		 * t is the node that roots the subtree.
		 */
		bool contains( const Comparable & x, BinaryNode *t ) const
		{
			if( t == nullptr )
				return false;
			else if( x < t->element )
				return contains( x, t->left );
			else if( t->element < x )
				return contains( x, t->right );
			else
				return true;    // Match
		}

		//deallocate memory
		void makeEmpty( BinaryNode * & t )
		{
			if( t != nullptr )
			{
				makeEmpty( t->left );
				makeEmpty( t->right );
				delete t;
			}
			t = nullptr;
		}

		/**
		 * Internal method to print a subtree rooted at t in sorted order.
		 */
		void printTree( BinaryNode *t, ostream & out ) const
		{
			if( t != nullptr )
			{
				printTree( t->left, out );
				out << setw(20) << left << t->element << " " << flush;
				for (int i = 0; i < t->lineNumber.size(); ++i)
				{
					if (i == t->lineNumber.size() - 1)
						out << t->lineNumber[i];
					else
						out << t->lineNumber[i] << ", ";
				}
				out << endl;

				printTree( t->right, out );
			}
		}

		//prints the line numbers as to where a word is at
		void printLine( BinaryNode *t, const Comparable & word ) const
		{
			if (word < t->element)
				return printLine(t->left, word);

			else if (t->element < word )
				return printLine(t->right, word);

			else
			{
				cout << "Line Numbers<BST>: ";
				for(int i = 0; i < t->lineNumber.size(); ++i)
				{
					if (i == t->lineNumber.size() - 1)
						cout << t->lineNumber[i];
					else
						cout << t->lineNumber[i] << ", ";
				}
				cout << endl;
			}
		}

		//recursively finds the height of a BST
		static int height(BinaryNode* t) {
			// returns the height of the tree t
			if (t == NULL) return -1;
			else {
				int p = height(t->left);
				int q = height(t->right);
				if (p < q)
					return 1+q;
				else return 1 + p;

			}
		}
	public:

		//returns the height of the BST
		int getHeight() {
			return height(root);
		}

		// returns the size of a BST
		static int size(BinaryNode* t) {
			if (t == NULL) return 0;
			else
				return 1 + size(t->left) + size(t->right);
		}

		//returns the root
		BinaryNode* getRoot() {
			return root;
		}
};



#endif
