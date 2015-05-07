#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <stdio.h>

#include "serialcmd.hpp"

using namespace std;
using namespace cv;


int main(int argc, const char* argv[])
{
  //openSerial(); @check
  
  Mat frame;
  
  int width = 1280;//320;@check
  int height = 720;//180;
  
  int h = 250, s = 105;
  int tol = 120;
  
  /*VideoCapture videoCap(0);
  namedWindow( "Display window", WINDOW_AUTOSIZE );

  if (!videoCap.isOpened())
  {
    cout << "Failed to open video cam!" << endl;
    return -1;
  }

  videoCap.set(CV_CAP_PROP_FRAME_WIDTH, width);
  videoCap.set(CV_CAP_PROP_FRAME_HEIGHT, height);
  videoCap >> frame;@check */
  
  frame = imread("../../img/Picture 1.jpg", 1);
  //imshow( "Display window", frame);
  waitKey(0);

  Mat hsv_frame = frame.clone();
  cvtColor(frame, hsv_frame, CV_BGR2HSV);

  Mat mask = Mat(height, width, CV_8UC1, 1);
  inRange(hsv_frame, Scalar(h - tol, s - tol, 0), Scalar(h + tol, s + tol, 255), mask);
  
  Mat erodeKernel = getStructuringElement(MORPH_RECT, Size(6, 6));
  erode(mask, mask, erodeKernel);
  
  Mat dilateKernel = getStructuringElement(MORPH_RECT, Size(5, 5));
  dilate(mask, mask, dilateKernel);
  //imshow( "Display window", mask);
  waitKey(0);
  
  int i, j;
  Scalar intens;
  bool isWhite;
  
  int leftBorder, rightBorder;
  vector<double> centers;
  double sum, mean;
  
  
  // Parcours du masque ligne par ligne en commençant par le bas
  for (i = height - 1; i > 650; i--) // Pas à adapter : 5px, 20px, 50 px...
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
    
    if (rightBorder - leftBorder < width / 4)
      centers.push_back((leftBorder + rightBorder) / 2. );
  }
  
  sum = 0;
  for (int k = 0; k < centers.size(); k++)
    sum += centers[k];
  mean = (int) sum / centers.size();
  cout << mean << endl;
  
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
