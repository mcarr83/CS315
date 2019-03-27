#include "EasyBMP.h"
#include <math.h>
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

int label(BMP & Output, int xpixels, int ypixels, int xlow, int xhigh, int ylow, int yhigh){

  RGBApixel c1;
  RGBApixel c2;
  RGBApixel c3;
  RGBApixel c4;
  c1.Red = 255; c1.Green = 255; c1.Blue = 255; c1.Alpha = 0;
  c2.Red = 0; c2.Green = 0; c2.Blue = 0; c2.Alpha = 0;
  c3.Red = 0; c3.Green = 255; c3.Blue = 0; c3.Alpha = 0;
  c4.Red = 255; c4.Green = 0; c4.Blue = 0; c4.Alpha = 0;

  int w = Output.TellWidth();
  int h = Output.TellHeight();
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
  int x1, y1;
  int lxlow, lxhigh, lylow, lyhigh;

  for (int j = xlow; j <= xhigh; j++){
    for (int k = ylow; k <= yhigh; k++){
      
      if (Output(j,k)->Red == c2.Red && Output(j,k)->Blue == c2.Blue && Output(j,k)->Green == c2.Green){
	count++;
	lxlow = j; lylow = k; lxhigh = j; lyhigh = k;
	point * p = new point(j,k);
	Q.insert(p);
	visited[k][j] = true;

	while (!Q.isEmpty()){
	  *p = Q.del(); 
	  x1 = p->getx();
	  y1 = p->gety(); 
  
	  Output(x1,y1)->Red = c3.Red;
	  Output(x1,y1)->Green = c3.Green;
	  Output(x1,y1)->Blue = c3.Blue;

	  if (x1 < lxlow) lxlow = x1;
	  if (y1 < lylow) lylow = y1; 
	  if (x1 > lxhigh) lxhigh = x1;
	  if (y1 > lyhigh) lyhigh = y1;
  
	  for (int x = x1-xpixels; x <= x1+xpixels; x++){
	    for (int y = y1-ypixels; y <= y1+ypixels; y++){
	      if (!visited[y][x]){
		visited[y][x] = true;
		if (Output(x,y)->Red == c2.Red && Output(x,y)->Blue == c2.Blue && Output(x,y)->Green == c2.Green){
		  point * n = new point(x, y);
		  Q.insert(n);
		}
	      }
	    }
	  }
  
	}// end while

	for (int sides = lylow-1; sides < lyhigh+2; sides++) {
	  Output(lxlow-1, sides)->Red = c4.Red; 
	  Output(lxlow-1, sides)->Green = c4.Green;
	  Output(lxlow-1, sides)->Blue = c4.Blue;
	  
	  Output(lxhigh+1, sides)->Red = c4.Red;
	  Output(lxhigh+1, sides)->Green = c4.Green;
	  Output(lxhigh+1, sides)->Blue = c4.Blue;
	}

	for (int topnbot = lxlow-1; topnbot < lxhigh+2; topnbot++) {
	  Output(topnbot, lylow-1)->Red = c4.Red;
	  Output(topnbot, lylow-1)->Green = c4.Green;
	  Output(topnbot, lylow-1)->Blue = c4.Blue;
  
	  Output(topnbot, lyhigh+1)->Red = c4.Red;
	  Output(topnbot, lyhigh+1)->Green = c4.Green;
	  Output(topnbot, lyhigh+1)->Blue = c4.Blue;
	}

       
      } // end if
      
      visited[k][j] = true; 
      
    }// end for k;
  }// end for j;
  return count;
}


int main() {
  BMP Input;
  Input.ReadFromFile("Input2.bmp");




  int xlow = 0;
  int ylow = 0;
  int xhigh = 0;
  int yhigh = 0;

  xhigh = Input.TellWidth() -1;
  yhigh = Input.TellHeight() - 1;

  
  
  Convert(Input);
  BMP numWordCount = Input;
  BMP numLetterCount = Input;
  BMP numLineCount = Input;

  int numletters = label(numLetterCount, 1, 6, xlow, xhigh, ylow, yhigh);
  numLetterCount.WriteToFile("out.bmp");

  int numlines = label(numLineCount, 30, 6, xlow, xhigh, ylow, yhigh);
 


  int numwords = label(numWordCount, 6, 6, xlow, xhigh, ylow, yhigh);



  cout << "The number of letters are: " << numletters << endl;
  cout << "The number of lines are: " << numlines <<  endl;
  cout << "The number of words are " << numwords << endl;
  
  
  return 0;
}
