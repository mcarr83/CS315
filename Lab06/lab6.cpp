//Michael Carr
//Lab 6

#include "EasyBMP.h"
#include <iostream>

using namespace std;

const int MSIZE = 1000;

int row[] = {1, 0, 0, -1}; 
int col[] = {0, 1, -1, 0}; 

void Copy(BMP &Image, BMP &Output);
bool PixelCheck(BMP &Input, int row, int col);

// point class is used by queue class
class point{
   private:
        int x; int y;

   public:
	point(int p, int q){
	   x = p; y = q;
    	}

  	int getx(){
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
      
       void insert(int n,int m){
	 point* temp = new point(n,m);
	 insert(temp);
       }
 
       void insert(point* x) {
         if (size != MSIZE) {
	   front++; size++;
           if (front == MSIZE) front = 0;
	      Q[front] = x;
         }
       }


       point* del() {
          if (size != 0) {
	      rear++; if (rear == MSIZE) rear = 0;

	      size--;
	      
	      return Q[rear];
          }
	 else return new point(-1,-1);
       }

       void print() {
 	   for (int j = 1; j <= size; ++j) {
   	   	int i = front - j + 1;
           	cout << "x = " << Q[i]->getx() << " y = " 
					<< Q[i]->gety() << endl;
           	}
 	   cout << "-----------------------" << endl;
       }

       bool isEmpty() {
  	   return (size == 0);
       }

  int getfront(){
    return front;
  }
};



/*******************************************************************/
/*                                                                 */
/*This function converts color image to gray scale just in case    */ 
/*                                                                 */
/*                                                                 */
/*******************************************************************/


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


/*******************************************************************/
/*                                                                 */
/*This function checks to see if the start pixel is white. Returns */ 
/*bool                                                             */
/*                                                                 */
/*******************************************************************/


bool CheckStartEnd(BMP &Input, int startx, int starty){
 
  if ((int) Input (startx, starty)->Red != 255){
    return false;
    if ((int) Input (startx, starty)->Green != 255){
      return false;
	if ((int) Input (startx, starty)->Blue != 255){
	  return false;
	  if ((int) Input (startx, starty)->Alpha != 0){
	     return false;
	  }
	}
      }
  }
  else
    return true;
}

/*******************************************************************/
/*                                                                 */
/*This function checks to see if a pixel is white. Returns         */ 
/*bool                                                             */
/*                                                                 */
/*******************************************************************/


bool PixelCheck(BMP &Input, int row, int col){
  
  int width = Input.TellWidth();
  int height = Input.TellHeight();

  //checks if pixel is within range
  if (0 > row || 0 > col)
    return false;

  if (row > width || col > height)
    return false;

  //checks if pixel is white

  if ((int) Input (row, col)->Red != 255){
    return false;
    if ((int) Input (row, col)->Green != 255){
      return false;
      if ((int) Input (row, col)->Blue != 255){
	return false;
	if ((int) Input (row, col)->Alpha != 0){
	    return false;
	}
      }
    }
  }

  else
    return true;
  
}


/*******************************************************************/
/*                                                                 */
/*This function does a Breadth First search, then traces the       */ 
/*shortest path. Passes in two Image objects as passby references, */
/*an x and y starting point and an x and y end point.*/
/*                                                                 */
/*******************************************************************/

int BFS(BMP &Input, BMP &Output, int startx, int starty, int endx, int endy, int count){

  //Initializes a queue called Q
  queue Q;

  //initializes a new pointer of p with start x and y values 
  point* p1 = new point (startx,starty);

  //inserts point into queue
  Q.insert(p1);

  //iterate count 
  count++;

  //Copy Input to Output(not actually needed)
  Copy(Input, Output);

  //initialize two values for height and width of an image
  int width = Input.TellWidth(); int height = Input.TellHeight();

  //bool array used to hold what points have been evaluated. Set to flase for all
  bool visited [width][height] = {0};

  //visited start x and y point set to true.
  visited[startx][starty] = true;

  //array used to hold distance
  int d [width][height];

  //pointer array used to hold the previous point
  point* p [width][height];
  
  //set to 0 or NULL
  p[startx][starty] = 0;

  //set to 0, no distance away from starting point
  d[startx][starty] = 0;

  //u initialized to hold the point coming out of the queue
  point* u;

  //while the queue is not empty and the end point is not reached
  while (!Q.isEmpty() && !visited[endx][endy]){
    //grab a point from the queue
    u = Q.del();

    //get x and y from it
    int x =  u->getx();int y = u->gety();

    //test north, south, east and west of a given point
    for (int i = 0; i < 4; i++){
      
      // if the point has not been visited and pixel is white
      if ((!visited[x+row[i]][y+col[i]]) && (PixelCheck(Input, x+row[i], y+col[i]))){
	//mark the pixel as true (visited)
	     visited[x+row[i]][y+col[i]] = true;

	     //insert the point into the queue   
	     Q.insert(x+row[i],y+col[i]);

	     //inser the distance into d 
	     d[x+row[i]][y+col[i]] = d[x][y]+1;

	     //p (pointer) points to point u...OMG!!!
	     p[x+row[i]][y+col[i]] = u;

	     //iterates count up 
	     count++;
       }
    }
    
  }

  //Print time!!!
  if (visited[endx][endy]){
    //initializes new pointer v and point to end x and y 
    point *v = new point(endx, endy);

    //initialize w and h (width and height) get the x and y from v using
    //getx and get y function
    int w = v->getx(); int h = v->gety();

    //while w is not equal to starting x or starting y is not h
    while (w != p1->getx() || p1->gety() != h){

      //color pixel w and h to red
      RGBApixel Temp = Output.GetPixel(w,h);
	Temp.Red = 255; Temp.Green = 0; Temp.Blue = 0; Temp.Alpha = 0;
	Output.SetPixel(w,h,Temp);

	//set the parent pointer of w and h to the pointer v
	v = p[w][h];

	//reset w and h to the next point (previous)
	w = v->getx();
	h = v->gety();
      }

    //write Output to test.bmp
    Output.WriteToFile("out.bmp");
    cout << "The count is " << count << endl;
    cout << "The distance is " << d[endx][endy] << endl;
  }

  
  else{
    cout << "There is no path" << endl;
    return 0;
  }
  
  return count;
}    


/*******************************************************************/
/*                                                                 */
/*This function copies the input image to the output. First the    */ 
/*size of the image is calculated, then a new canvas is made using */
/*the size of image (height and width). Once that is done the pixels*/
/*are copied pixel by pixel from Image to Output.*/
/*                                                                 */
/*******************************************************************/

void Copy(BMP &Image, BMP &Output){

  int width = Image.TellWidth();
  int height = Image.TellHeight();

  Output.SetSize(width, height);
  
  for (int i = 0; i < Image.TellWidth(); i++){
    for (int j = 0; j < Image.TellHeight(); j++){
      //SetEasyBMPwarningsOn();
      PixelToPixelCopy(Image, i, j, Output, i, j);
    }
  }
  //Output.WriteToFile("second.bmp");
 
  
}
  
int main() 
{
 
  SetEasyBMPwarningsOn();

  //initializes two BMP objects
  BMP Input; BMP Output;

  //read in file to Input
  Input.ReadFromFile("in.bmp");

  //Convert from color to grayscale image
  Convert(Input);

  //initializing variables
  int startx = 0; int starty = 0;
  int endx = 0; int endy = 0;
  bool check1 = true; bool check2 = true;


  //Asking the starting x and y and checking if they are valid and white pixels
  cout << "What is your starting x? " << flush;
  cin >> startx;

  if(0 > startx){
    cout << "Error, where are you going. Out of range." << endl;
    return 0;
  }
  cout << "What is your starting y? " << flush;
  cin >> starty;

  if(0 > starty){
    cout << "Error, where are you going. Out of range." << endl;
    return 0;
  }

  check1 = CheckStartEnd(Input, startx, starty);

  if (check1 == false){
    cout << "Pixel is not white" << endl;
    return 0;  
  }
  //------------------------------------------

  //Asking the ending x and y and checking if they are valid and white pixels
  cout << "What is your ending x? " << flush;
  cin >> endx;

  if(endx > Input.TellWidth()){
     cout << "Error, where are you going. Out of range." << endl;
     return 0;
  }

  cout << "What is your ending y? " << flush;
  cin >> endy;

  if(endy > Input.TellHeight()){
    cout << "Error, where are you going. Out of range." << endl;
    return 0;
  }
  check2 = CheckStartEnd(Input, endx, endy);

  if(check2 == false){
    cout << "Pixel is not white" << endl;
    return 0;
  }

 //------------------------------------------

  int count = 0;

  //BFS TIME!!!
  count = BFS(Input, Output, startx, starty, endx, endy, count);
  
  
  return 0;
}
 
