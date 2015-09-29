
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <unistd.h>
#include <stdio.h>

#include "serialcmd.hpp"

using namespace std;
using namespace cv;


VideoCapture videoCap;

Mat frame, hsv_frame, mask;
Mat erodeKernel, dilateKernel;
int width = 320, height = 180, roi_height = 70;

int h = 250, s = 105;
int tol = 120;


bool setup(VideoCapture &videoCap);
bool loop();


int main(int argc, const char* argv[])
{
  setup(videoCap);
  
  while(1)
  {
    loop();
  }
}

bool setup(VideoCapture &videoCap)
{
  bool is_OK;
  
  videoCap = VideoCapture(0);
  if (!videoCap.isOpened())
  {
    cout << "Failed to open video cam!" << endl;
    is_OK = false;
  }
  
  videoCap.set(CV_CAP_PROP_FRAME_WIDTH, width);
  videoCap.set(CV_CAP_PROP_FRAME_HEIGHT, height);
  
  is_OK &= openSerial();
  
  return is_OK;
}

bool loop()
{
  videoCap >> frame;

  hsv_frame = frame.clone();
  cvtColor(frame, hsv_frame, CV_BGR2HSV);

  mask = Mat(height, width, CV_8UC1, 1);
  inRange(hsv_frame, Scalar(h - tol, s - tol, 0), Scalar(h + tol, s + tol, 255), mask);
  
  erodeKernel = getStructuringElement(MORPH_RECT, Size(6, 6));
  erode(mask, mask, erodeKernel);
  
  dilateKernel = getStructuringElement(MORPH_RECT, Size(5, 5));
  dilate(mask, mask, dilateKernel);
  //imshow( "Display window", mask);
  waitKey(100);
  
  int i, j;
  Scalar intens;
  bool isWhite;
  
  int leftBorder, rightBorder;
  vector<double> centers;
  double sum, mean;
  
  
  // Parcours du masque ligne par ligne en commençant par le bas
  for (i = height - 1; i > height - roi_height; i--) // Pas à adapter : 5px, 20px, 50 px...
  {
    // Parcours de gauche à droite
    isWhite = false;
    j = 0;
    while(j < width && !isWhite)
    {
      intens = mask.at<uchar>(i,j);
      isWhite = (intens.val[0] == 255);
      j++;
    }
    leftBorder = j;
    
    // Parcours de gauche à droite
    isWhite = false;
    j = width - 1;
    while(j > -1 && !isWhite)
    {
      intens = mask.at<uchar>(i,j);
      isWhite = (intens.val[0] == 255);
      j--;
    }
    rightBorder = j;
    
    if ( (rightBorder - leftBorder < width / 4) && (i > 0) && (j > 0) )
      centers.push_back((leftBorder + rightBorder) / 2. );
  }
  
  if (centers.size() > 5)
  {
    sum = 0;
    for (int k = 0; k < centers.size(); k++)
      sum += centers[k];
    mean = (int) sum / centers.size();
    
    // Si la position horizontale moyenne est trop à droite
    if (mean > width * 2 /3)
      commandRight();
    // Si la position horizontale moyenne est trop à gauche
    else if (mean < width /3)
      commandLeft();
    // Si la bande est située suffisamment au centre
    else
      commandForward();
  }
  else
  {
    commandLeft();
  }
  
}