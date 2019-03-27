//Michael Carr
//Lab 7

#include "EasyBMP.h"
#include <iostream>

using namespace std;

const int MSIZE = 1000;

class point {
private:
  int x; int y;

public:

  point(int p, int q) {
    x = p; y = q;
  }

  int getx() {
    return x;
  }

  int gety() {
    return y;
  }
};

class queue {

private:
  point* Q[MSIZE];

  int front, rear, size;

public:
  queue() {
    // initialize an empty queue
    front = 0; rear = 0; size = 0;
    for (int j=0; j < MSIZE; ++j)
      Q[j] = 0;
  }

  void insert(point * x) {
    if (size != MSIZE) {
      front++; size++;
      if (front == MSIZE) front = 0;
      Q[front] = x;
    }
  }

  point del() {
    if (size != 0) {
      rear++; if (rear == MSIZE) rear = 0;
      point temp(Q[rear]->getx(), Q[rear]->gety());
      size--;
      return temp;
    };
  }

  void print() {
    for (int j = 1; j <= size; ++j) {
      int i = front - j + 1;
      cout << "x = " << Q[i]->getx() << " y = " << Q[i]->gety() << endl;
    }
    cout << "end of queue" << endl;
  }

  bool isEmpty() {
    return (size == 0);
  }
};

/********************************************************************/
/*Function used to create a black and white image from a color image*/
/*This function makes it easy to change the pixels and make boxes.  */
/********************************************************************/

void Convert(BMP &Input){

  // convert each pixel to grayscale using RGB->YUV
 
  for( int j=0 ; j < Input.TellHeight() ; j++)
    {
      for( int i=0 ; i < Input.TellWidth() ; i++)
	{
	  int Temp = (int) floor( 0.299*Input(i,j)->Red +
				  0.587*Input(i,j)->Green +
				  0.114*Input(i,j)->Blue );
	  ebmpBYTE TempBYTE = (ebmpBYTE) Temp;
	  Input(i,j)->Red = TempBYTE;
	  Input(i,j)->Green = TempBYTE;
	  Input(i,j)->Blue = TempBYTE;
	}
    }
}

/********************************************************************/
/*Function used to draw a red box around each letter.               */
/*                                                                  */
/********************************************************************/

void DrawBow(BMP &Output, int lowx, int lowy, int highx, int highy){

  //Object created to be used to change the certain pixel to red
  RGBApixel c;
  c.Red = 255; c.Green = 0; c.Blue = 0; c.Alpha = 0;

  //makes the left and right edges of the box
  for (int LandR = lowy-1; LandR < highy+2; LandR++) {
    Output(lowx-1, LandR)->Red = c.Red; 
    Output(lowx-1, LandR)->Green = c.Green;
    Output(lowx-1, LandR)->Blue = c.Blue;
	  
    Output(highx+1, LandR)->Red = c.Red;
    Output(highx+1, LandR)->Green = c.Green;
    Output(highx+1, LandR)->Blue = c.Blue;
  }

  //make the top and bottom edges of the box
  for (int topAndBot = lowx-1; topAndBot < highx+2; topAndBot++) {
    Output(topAndBot, lowy-1)->Red = c.Red;
    Output(topAndBot, lowy-1)->Green = c.Green;
    Output(topAndBot, lowy-1)->Blue = c.Blue;
  
    Output(topAndBot, highy+1)->Red = c.Red;
    Output(topAndBot, highy+1)->Green = c.Green;
    Output(topAndBot, highy+1)->Blue = c.Blue;
  }

}

/********************************************************************/
/*Function used to draw a green in a certain location               */
/*                                                                  */
/********************************************************************/

void DrawGreenPixel(BMP &Output, int x1, int y1){

  //object initialized to color green
  RGBApixel c;
  c.Red = 0; c.Green = 255; c.Blue = 0; c.Alpha = 0;

  //pixel drawn with an x and y coordinate passed in
  Output(x1,y1)->Red = c.Red;
  Output(x1,y1)->Green = c.Green;
  Output(x1,y1)->Blue = c.Blue;
}

/********************************************************************/
/*Function used to calculate the number of letter and lines as well */
/*as directing as to what pixels need to be green or red.           */
/*                                                                  */
/********************************************************************/

int BFS(BMP & Output, int xpixels, int ypixels, int xlow, int xhigh, int ylow, int yhigh){

  //object initialized to color black
  RGBApixel c;
  c.Red = 0; c.Green = 0; c.Blue = 0; c.Alpha = 0;

  //w and h initialized and set to the height and width of the object
  int w = Output.TellWidth(); int h = Output.TellHeight();

  //count used to be the counter not just for letters but lines depending
  //on what is passed in the variables xpixel and ypixel
  int count = 0;

  //set visited [j][k] false for all j, k
  bool visited[h][w];
  for(int j = 0; j < h; j++){
    for (int k = 0; k < w; k++){
      visited[j][k] = false;
    }
  }
  
  //initialize Q
  queue Q;

  //initialized to store the x and y coordinates that are deleted and put into
  //pointer p (below)
  int x1, y1;

  //initialized to find the highest and lowest pixels
  int lowx, highx, lowy, highy;

  //for loops used to go through the whole picture, probably could have used
  //height and width of the image itself
  
  for (int j = xlow; j <= xhigh; j++){
    for (int k = ylow; k <= yhigh; k++){

      //if pixel is black
      if (Output(j,k)->Red == c.Red && Output(j,k)->Blue == c.Blue && Output(j,k)->Green == c.Green){
	//iterate count
	count++;
	//set the low and high x and ys
	lowx = j; lowy = k; highx = j; highy = k;
	//initialize a new point p with j and k
	point * p = new point(j,k);
	//insert the point into the queue
	Q.insert(p);
	//set the visited array to true at that point
	visited[k][j] = true;

	//while the queue is not empty
	while (!Q.isEmpty()){
	  //delete a point from the queue and set it to pointer p
	  *p = Q.del(); 

	  //get the integers in the the pointer
	  x1 = p->getx(); y1 = p->gety();

	  //Draw a single green pixel and x1 and y1 location
	  DrawGreenPixel(Output, x1, y1);

	  //if any x1 or y2 is the lowest or highest set it
	  if (x1 < lowx)
	    lowx = x1;
	  if (y1 < lowy)
	    lowy = y1; 
	  if (x1 > highx)
	    highx = x1;
	  if (y1 > highy)
	    highy = y1;

	  //check the pixels around x1 and y1
	  for (int x = x1-xpixels; x <= x1+xpixels; x++){
	    for (int y = y1-ypixels; y <= y1+ypixels; y++){
	      //if the pixel hasnt been visited and ..
	      if (!visited[y][x]){
		visited[y][x] = true;

		//if the pixel is black
		if (Output(x,y)->Red == c.Red && Output(x,y)->Blue == c.Blue && Output(x,y)->Green == c.Green){
		  //new pointer n is initialized with x and y
		  point * n = new point(x, y);
		  //insert the point into the queue
		  Q.insert(n);
		}
	      }
	    }
	  }
  
	}

	//call draw box with low and high x and y
	DrawBow(Output, lowx, lowy, highx, highy);
	      
      }      
      //if pixel is white set it to true and move on
      visited[k][j] = true; 
      
    }
  }
  return count;
}


int main() {

  //two chars initialized to be used to pass the opening and closing files
  char infile[15]; char outfile[15];

  //Asks user to a file to input and output
  cout << "Please enter an input file no more than 15 characters: " << flush;
  cin >> infile;

  cout << "Please enter an output file no more than 15 characters: " << flush;
  cin >> outfile;
  cout << endl;

  //BMP object initialized and read
  BMP Input;
  Input.ReadFromFile(infile);

  //variables used to pass to two functions
  int xlow = 0; int ylow = 0; int xhigh = 0; int yhigh = 0;

  //The most lower right pixel back off by one pixel in the height and width
  xhigh = Input.TellWidth() -1; yhigh = Input.TellHeight() - 1;

  //Converts a color image to color
  Convert(Input);

  //Two new objects initialized
  BMP numWordCount = Input; BMP numLetterCount = Input;

  //Object of numLetterCount with the lowest pixel and high pixel. This will
  //return the number of letters in the image
  int numletters = BFS(numLetterCount, 1, 6, xlow, xhigh, ylow, yhigh);
  numLetterCount.WriteToFile(outfile);

  //Object of numWordCount with the lowest pixel and high pixel. This will
  //return the number of lines in the image
  int numlines = BFS(numWordCount, 6, 6, xlow, xhigh, ylow, yhigh);

  //Output of numletters and numlines
  cout << "The number of letters are: " << numletters << endl;
  cout << "The number of lines are: " << numlines <<  endl;
   
  return 0;
}
