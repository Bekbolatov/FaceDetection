//============================================================================
// Name        : TestOCV.cpp
// Author      : RB
// Version     :
// Copyright   : no cp notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include "opencv2\opencv.hpp"
#include <iostream>

using namespace std;
int main(void)
{
	cout << "test" << endl;
	char key;
	int smoothing_kernel_size = 3;
	IplImage* image, *imgout;
	imgout = 0;
	cvNamedWindow( "Example", CV_WINDOW_AUTOSIZE );
	cvNamedWindow( "Output", CV_WINDOW_AUTOSIZE );
	//cvMoveWindow( "Output", 300, 100 );

	    CvCapture* capture;
	if((capture = cvCreateCameraCapture ( 0 ))==0){
		cout<<"ERROR: NO CAMERAS DETECTED."<<endl;
		return 0;
	}
	while(1){
		if((image = cvQueryFrame( capture ))==0){
			cout<<"ERROR: BAD FRAME!"<<endl;
			return 0;
		}
		if(imgout) {
			cvReleaseImage( &imgout );
		}
		imgout = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U,3);
		cvSmooth( image, imgout, CV_GAUSSIAN, smoothing_kernel_size, smoothing_kernel_size );
		cvShowImage( "Output", imgout );
		cvShowImage( "Example", image );

		//cvReleaseImage( &image );
		//cvReleaseImage( &imgout );

		key=cvWaitKey(10);

		if( key == 27 ) {
			cout << "Exiting on KEY: " << key << endl;
			break;
		} else if( key == 32 ) {
			cout << "Writing info on Space: " << image->width << "x" << image->height << endl;
			cout << "nChannels: " << image->nChannels << ", data[0]: " << image->imageData[0] <<  endl;
		} else if( key == 51 ) { //3
			smoothing_kernel_size = 3;
			cout << "smoothing: " << smoothing_kernel_size << endl;
		} else if( key == 52 ) {
			smoothing_kernel_size = 5;
			cout << "smoothing: " << smoothing_kernel_size << endl;
		} else if( key == 53 ) {
			smoothing_kernel_size = 11;
			cout << "smoothing: " << smoothing_kernel_size << endl;
		} else if( key == 54 ) {
			smoothing_kernel_size = 21;
			cout << "smoothing: " << smoothing_kernel_size << endl;
		} else if( key == 55 ) { //7
			smoothing_kernel_size = 31;
			cout << "smoothing: " << smoothing_kernel_size << endl;
		}
	}
	cvReleaseImage( &imgout );
	cvReleaseCapture(&capture);
	cvDestroyWindow( "Example" );
	cvDestroyWindow( "Output" );
	return 0;
}
