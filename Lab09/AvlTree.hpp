#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include "dsexceptions.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
using namespace std;

// AvlTree class
//
// CONSTRUCTION: zero parameter
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// void remove( x )       --> Remove x (unimplemented)
// bool contains( x )     --> Return true if x is present
// Comparable findMin( )  --> Return smallest item
// Comparable findMax( )  --> Return largest item
// boolean isEmpty( )     --> Return true if empty; else false
// void makeEmpty( )      --> Remove all items
// void printTree( )      --> Print tree in sorted order
// ******************ERRORS********************************
// Throws UnderflowException as warranted

template <typename Comparable>
class AvlTree
{
	public:
		AvlTree( ) : root{ nullptr }
		{ }

		AvlTree( const AvlTree & rhs ) : root{ nullptr }
		{
			root = clone( rhs.root );
		}

		AvlTree( AvlTree && rhs ) : root{ rhs.root }
		{
			rhs.root = nullptr;
		}

		~AvlTree( )
		{
			makeEmpty( );
		}

		/**
		 * Deep copy.
		 */
		AvlTree & operator=( const AvlTree & rhs )
		{
			AvlTree copy = rhs;
			std::swap( *this, copy );
			return *this;
		}

		/**
		 * Move.
		 */
		AvlTree & operator=( AvlTree && rhs )
		{
			std::swap( root, rhs.root );

			return *this;
		}

		/**
		 * Find the smallest item in the tree.
		 * Throw UnderflowException if empty.
		 */
		const Comparable & findMin( ) const
		{
			if( isEmpty( ) )
				throw UnderflowException{ };
			return findMin( root )->element;
		}

		/**
		 * Find the largest item in the tree.
		 * Throw UnderflowException if empty.
		 */
		const Comparable & findMax( ) const
		{
			if( isEmpty( ) )
				throw UnderflowException{ };
			return findMax( root )->element;
		}

		/**
		 * Returns true if x is found in the tree.
		 */
		bool contains( const Comparable & x , int & comp) const
		{
			return contains( x, root , comp);
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
		void printTree(int & letterCount ) const
		{
			if( isEmpty( ) )
				cout << "Empty tree" << endl;
			else
				printTree( root, letterCount );
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
		void insert( const Comparable & x, int & line, int & rotation, int & comp)
		{
			insert( x, root, line, rotation, comp);
		}

		/**
		 * Insert x into the tree; duplicates are ignored.
		 */
		void insert( Comparable && x, int & line, int & rotation, int & comp)
		{
			insert( std::move( x ), root, line, rotation, comp );
		}

		/**
		 * Remove x from the tree. Nothing is done if x is not found.
		 */
		void remove( const Comparable & x )
		{
			remove( x, root );
		}

	private:
		struct AvlNode
		{
			Comparable element;
			AvlNode   *left;
			AvlNode   *right;
			int       height;
			vector<int> lineNumber;

			AvlNode( const Comparable & ele, AvlNode *lt, AvlNode *rt, int h = 0 )
				: element{ ele }, left{ lt }, right{ rt }, height{ h } { }

			AvlNode( Comparable && ele, AvlNode *lt, AvlNode *rt, int h = 0 )
				: element{ std::move( ele ) }, left{ lt }, right{ rt }, height{ h } { }
		};

		AvlNode *root;


		/**
		 * Internal method to insert into a subtree.
		 * x is the item to insert.
		 * t is the node that roots the subtree.
		 * Set the new root of the subtree.
		 */
		void insert( const Comparable & x, AvlNode * & t, int & line, int & rotation, int & comp)
		{

			if( t == nullptr ){
				t = new AvlNode{ x, nullptr, nullptr };
				t->lineNumber.push_back(line);

			}
			else if( x < t->element ){
				comp++;
				insert( x, t->left, line, rotation, comp);

			}
			else if( t->element < x ){
				comp += 2;
				insert( x, t->right, line, rotation, comp);

			}
			else{
				comp += 2;
				t->lineNumber.push_back(line);
			}

			balance( t, rotation, comp );
		}

		/**
		 * Internal method to insert into a subtree.
		 * x is the item to insert.
		 * t is the node that roots the subtree.
		 * Set the new root of the subtree.
		 */
		void insert( Comparable && x, AvlNode * & t, int & line, int& rotation, int& comp)
		{
			if( t == nullptr )
				t = new AvlNode{ std::move( x ), nullptr, nullptr };
			else if( x < t->element ){
				comp++;
				insert( std::move( x ), t->left, line );

			}
			else if( t->element < x ){
				comp += 2;
				insert( std::move( x ), t->right, line );

			}
			else{
				comp += 2;
				t->lineNumber.push_back(line);
			}
			balance( t, rotation, comp );
		}


		static const int ALLOWED_IMBALANCE = 1;

		// Assume t is balanced or within one of being balanced
		void balance( AvlNode * & t, int & rotation, int & comp)
		{
			if( t == nullptr )
				return;
			comp++;
			if( height( t->left ) - height( t->right ) > ALLOWED_IMBALANCE ){
				comp++;
				if( height( t->left->left ) >= height( t->left->right ) ){
					rotation++;
					rotateWithLeftChild( t );

				}
				else{
					rotation += 2;
					doubleWithLeftChild( t );
				}
			}
			else{
				comp++;
				if( height( t->right ) - height( t->left ) > ALLOWED_IMBALANCE ){
					comp++;
					if( height( t->right->right ) >= height( t->right->left ) ){
						rotation++;
						rotateWithRightChild( t );
					}
					else{
						rotation += 2;
						doubleWithRightChild( t );
					}
				}
			}

			t->height = max( height( t->left ), height( t->right ) ) + 1;

		}

		/**
		 * Internal method to find the smallest item in a subtree t.
		 * Return node containing the smallest item.
		 */
		AvlNode * findMin( AvlNode *t ) const
		{
			if( t == nullptr )
				return nullptr;
			if( t->left == nullptr )
				return t;
			return findMin( t->left );
		}

		/**
		 * Internal method to find the largest item in a subtree t.
		 * Return node containing the largest item.
		 */
		AvlNode * findMax( AvlNode *t ) const
		{
			if( t != nullptr )
				while( t->right != nullptr )
					t = t->right;
			return t;
		}


		/**
		 * Internal method to test if an item is in a subtree.
		 * x is item to search for.
		 * t is the node that roots the tree.
		 */
		bool contains( const Comparable & x, AvlNode *t, int & comp ) const
		{
			if( t == nullptr )
				return false;

			else if( x < t->element ){
				comp++;
				return contains( x, t->left, comp );
			}

			else if( t->element < x ){
				comp += 2;
				return contains( x, t->right, comp );
			}
			else
				return true;    // Match
		}

		/**
		 * Internal method to make subtree empty.
		 */
		void makeEmpty( AvlNode * & t )
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
		void printTree( AvlNode *t, int & letterCount ) const
		{
			if( t != nullptr )
			{
				printTree( t->left, letterCount );
				cout << setw(letterCount) << left << t->element << " " << flush;
				for (int i = 0; i < t->lineNumber.size(); ++i)
				{
					if (i == t->lineNumber.size() - 1)
						cout << t->lineNumber[i];
					else
						cout << t->lineNumber[i] << ", ";
				}
				cout << endl;

				printTree( t->right, letterCount );
			}

		}

		/**
		 * Internal method to clone subtree.
		 */
		AvlNode * clone( AvlNode *t ) const
		{
			if( t == nullptr )
				return nullptr;
			else
				return new AvlNode{ t->element, clone( t->left ), clone( t->right ), t->height };
		}
		// Avl manipulations
		/**
		 * Return the height of node t or -1 if nullptr.
		 */
		int height( AvlNode *t ) const
		{
			return t == nullptr ? -1 : t->height;
		}

		int max( int lhs, int rhs ) const
		{
			return lhs > rhs ? lhs : rhs;
		}

		/**
		 * Rotate binary tree node with left child.
		 * For AVL trees, this is a single rotation for case 1.
		 * Update heights, then set new root.
		 */
		void rotateWithLeftChild( AvlNode * & k2 )
		{
			AvlNode *k1 = k2->left;
			k2->left = k1->right;
			k1->right = k2;
			k2->height = max( height( k2->left ), height( k2->right ) ) + 1;
			k1->height = max( height( k1->left ), k2->height ) + 1;
			k2 = k1;
		}

		/**
		 * Rotate binary tree node with right child.
		 * For AVL trees, this is a single rotation for case 4.
		 * Update heights, then set new root.
		 */
		void rotateWithRightChild( AvlNode * & k1 )
		{
			AvlNode *k2 = k1->right;
			k1->right = k2->left;
			k2->left = k1;
			k1->height = max( height( k1->left ), height( k1->right ) ) + 1;
			k2->height = max( height( k2->right ), k1->height ) + 1;
			k1 = k2;
		}

		/**
		 * Double rotate binary tree node: first left child.
		 * with its right child; then node k3 with new left child.
		 * For AVL trees, this is a double rotation for case 2.
		 * Update heights, then set new root.
		 */
		void doubleWithLeftChild( AvlNode * & k3 )
		{
			rotateWithRightChild( k3->left );
			rotateWithLeftChild( k3 );
		}

		/**
		 * Double rotate binary tree node: first right child.
		 * with its left child; then node k1 with new right child.
		 * For AVL trees, this is a double rotation for case 3.
		 * Update heights, then set new root.
		 */
		void doubleWithRightChild( AvlNode * & k1 )
		{
			rotateWithLeftChild( k1->right );
			rotateWithRightChild( k1 );
		}

		//prints the line numbers as to where a word is at
		void printLine( AvlNode *t, const Comparable & word ) const
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

	public:
		//returns the height of the BST
		int getHeight() {
			return height(root);
		}

		// returns the size of a BST
		static int size(AvlNode* t) {
			if (t == NULL) return 0;
			else
				return 1 + size(t->left) + size(t->right);
		}

		//returns the root
		AvlNode* getRoot() {
			return root;
		}

};

#endif
