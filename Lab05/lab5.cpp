//Michael Carr
//Lab 5
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>
#include <set>
#include <algorithm>

//#include "scan.cpp"

using namespace std;

class Stack;

class Node {

public:
Node(int key1){
	key = key1;
	tag = 0;
	down = NULL;
	next = NULL;
}

Node(Node* dptr, Node* nptr) {
	tag = 1;
	down = dptr;
	next = nptr;
}

void setKey(int k) {
	key = k;
}

void setDown(Node* ptr) {
	down = ptr;
}

  void setTag (int k){
    tag = k;//sets tag. Why not included?
  }
  
void setNext(Node* ptr) {
	next = ptr;
}

int getKey() {
	return key;
}

  int getTag(){
    return tag;//getter for tag. No other way to access it. Why not included?
  }

Node* getDown() {
	return down;
}

Node* getNext() {
	return next;
}

int size() {
	if (this == NULL)
	   return 0;
	else if (this->tag == 0) return 1;
	else
	   return down->size() + next->size();
}

void update(int j, int k) {

  //If list is empty print error and return
  if(this->isEmpty())
    {
        cout << "Error: Invalid to update empty list" << endl;
        return;
    }
  //If list is a leaf change key to k and return
    if(this->getTag() == 0)
    {
        this->setKey(k);
        return;
    }

    //set list down size to int s
    else
    {
        int s = this->down->size();

	//if s is greater than or less than j go down until value is found
	//recursive call
	if(s >= j)
            this->down->update(j,k);

	//else go next until value is found, recursive call
	else
            this->next->update(j - s, k);
    }

  } 

bool isEmpty() {
	  return this == NULL;
  }

void print(int depth) {
	  if (this == NULL) return;
	  if (tag == 0) {
	      cout << "<" << key << ":" << depth << ">  " ;
	      }
	  else {
	     down->print(depth+1);
	     next->print(depth);
	    } // end else
	 } // end print

private:
   int tag;
   int key;
   Node* down;
   Node* next;
};

class Stack {

private:
  int maxTop;// max stack size = size - 1
  int top;// current top of stack
  Node* values[1000];// element array

public:

  bool IsEmpty() { return top == -1; }
  bool IsFull() { return top == maxTop; }

 Stack() {
    top=-1;
    maxTop= 999;
  }

  void Push(Node* x) {
    if (IsFull())  // if stack is full, print error
      cout << "Error: the stack is full." << endl;
    else {
        values[++top]= x;
        }
  }

  Node*  Pop() {
    if (IsEmpty()) { //if stack is empty, print error
      cout << "Error: the stack is empty." << endl;
      return NULL;
    }
    else {
      return values[top--];
    }
  }

  Node* Top() {
    if (IsEmpty()) {
      cout << "Error: the stack is empty." << endl;
      return NULL;
    }
    else
      return values[top];
  }
};

int convert(string x) {
 // converts a string rep of decimal integer to an int
  return atoi(x.c_str());
}

/*************************************************************************/
/*                                                                       */
/*                         Build Function                                */
/*                                                                       */
/*************************************************************************/

Node* build (vector<string> T) {

  //Stack S initialized
  Stack S;
  cout << "Stack called" << endl;

  //For loop used to go through entire vector of T
     for(int i = 0; i < T.size(); i++)
     {
       //if T[i] is a [, Node pointer ListPtr set to new Node with down point set to
       //NULL and next pointer set to NULL

       if(T[i] == "[")
	 {
            Node* ListPtr = new Node(NULL, NULL);

	    //ListPtr Pushed onto the stack
	    S.Push(ListPtr);
	    }

       //if T[i] is a ] 
       else if(T[i] == "]")
         {
	   

	   //Create NULL pointer N and set it to call Pop on S (stack)
	   Node* N = S.Pop();
	  

	   //while top is not [
	   while(S.Top()->size() != 0)
             {
	       //Create NULL pointer k and set it to call Pop on S (stack)
	       Node* k = S.Pop();
	     

	       //k->set to next
	       k->setNext(N);
	       N = k;
             }

	   //Create NULL pointer List and set it to call Pop on S (stack)
	   Node* List = S.Pop();
	   

	   //List pointer set 
	   List->setDown(N);
	   S.Push(List);
	 
         }
         else
         {
	   Node* ListPtr = new Node(new Node(convert(T[i])), NULL);
	   S.Push(ListPtr);	   
         }
     }
     
     return S.Pop();

}

/*************************************************************************/
/*                                                                       */
/*                         Create Function                               */
/*                                                                       */
/*************************************************************************/

vector<string>create(string s){
  vector<string>v;

  for(int i = 0; i < s.size(); i++)
    {
      //if char [ found push string version to vector v
        if (s[i] == '[')
        {
            v.push_back("[");
        }

	//for negative numbers. 
        if (s[i] == '-')
	  {
	    //used to hold a negative number in its entirety. Will be concatanted  with
	    //negSign and negNum at end.
	    string num;

	    //used to hold negative sign
            string negSign(1, s[i]);

	    //iterate up one, used for determining if next char is a digit or not
	    i++;

	    //checks to see if next char is a digit or not (should be)
	    if (isdigit(s[i]))
            {
	      //while the characters are digits...
                while(isdigit(s[i]))
                {
		  //but if the character is a '[', ']', ',', ' '
                    if (s[i] == '[' || s[i] == ']' || s[i] == ',' || s[i] == ' ')
		      //break outta the loop
		      break;
		    //if not save character in negNum
		    string negNum (1, s[i]);

		    //concatante negSign and negNum ie "-" + "1" would be "-1"
                    num  = negSign + negNum;

		    //iterate up one, for next character
		    i++;
                }
            }
	    //Take the negative string number and push it into v
	    v.push_back(num);
        }

	//for positive numbers
        if (isdigit(s[i]))
	  {
	    //very similar to negative numbers minus the '-'
	    //used to store the first number 
            string posNum(1, s[i]);

	    //next used to be used with i to see what the next character is in the string
	    //could not fine a peek() function
	    int next = 1;

	    //looks to see if next character is a digit if true while loop it
	    while(isdigit(s[i+next]))
            {
	      //if character is any thing other than a digit break out of the loop
                if (s[i+next] == '[' || s[i+next] == ']' || s[i+next] == ',' || s[i+next] == ' ')
                    break;

		//new string variable to hold the next character
                string posNum2(1, s[i+next]);

		//concatanate all together
                posNum = posNum+ posNum2;

		//iterate by 1
                i++;
            }

	    //push the number into the vector v
            v.push_back(posNum);
        }

	//if the character is a ']', push back a string ] into vector v.
        if (s[i] == ']')
        {
            v.push_back("]");
        }
	}

  //return the vector to main
  return v;
  

}
//-----------------------------------------------
int main(int argc, char* argv[]) {
  
  cout << "Input a generalized list: " << flush;
  //inputs a whole line
  string list = " ";
  getline(cin, list);

  //tokenVector initialized, create called passing string list
  vector<string> tokenVector = create(list);

  //
  Node* List = build(tokenVector);
  cout << "The list that was input: " << endl;

  //Prints the list
  List->print(-1);
  cout << endl;

  //option initialized for do while loop
  char option;
  do {
    cout << "What do you want to do?" << endl;
    cout << endl;
    cout << "'u' : Update an element" << endl;
    cout << "'q' : Quit" << endl;

    //used to put cursor between What do you want to do and Update an element. Weird side effects when exiting program but looks great!!!!
    cout << "\x1b[A";
    cout << "\x1b[A";
    cout << "\x1b[A";
    
    //spot and newValue initialized to pass into update function
    int spot = 0;
    int newValue = 0;
    cin >> option;
    cout << endl;

    if (option == 'u')
        {
	  cout << "Which element would you like to update? ";
	  cin >> spot;
	  cout << "What would you like to update it to? ";
	  cin >> newValue;
	  cout << endl;

	  //Update called by List pointers
	  List->update(spot,newValue);
	  cout << "The updated list is:" << endl;

	  //print
	  List->print(-1);
	  cout << endl;
	  cout << endl;}
    
  }while (option != 'q');
  //do while loop end
 

  return 0;
}

