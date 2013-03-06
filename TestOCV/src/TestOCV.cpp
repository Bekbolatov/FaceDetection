//============================================================================
// Name        : TestOCV.cpp
// Author      : RB
// Version     :
// Copyright   : no cp notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include "opencv2\opencv.hpp"
#include <iostream>
#include "flandmark_detector.h"

using namespace std;

int g_switch_value = 0;
int filterInt = 0;
int lastfilterInt = -1;
void switch_callback( int position ){
	filterInt = position;
}
double fillDiff = 5.0;

int main(void)
{
	FLANDMARK_Model * model = flandmark_init("data/flandmark_model.dat");
	int bbox[] = {72, 72, 183, 183};
	double * landmarks = (double*)malloc(2*model->data.options.M*sizeof(double));  //used to be float *
	cout << "test" << endl;
	char key;
	int smoothing_kernel_size = 3;
	IplImage* image, *imgout;
	imgout = 0;
	CvSeq* rects;
	int nFaces;

	// Haar Cascade file, used for Face Detection.
	    char faceCascadeFilename [] = "data/haarcascade_frontalface_alt.xml";
	    // Load the HaarCascade classifier for face detection.
	    CvHaarClassifierCascade* faceCascade;
	    faceCascade = (CvHaarClassifierCascade*)cvLoad(faceCascadeFilename, 0, 0, 0);
	    if( !faceCascade )
	    {
	        cout << "Couldn't load Face detector '" << faceCascadeFilename  <<  "'"  << endl;
	        exit(1);
	    }
	    CvMemStorage* storage;
	    storage = cvCreateMemStorage(0);
	        cvClearMemStorage(storage);
	        float search_scale_factor = 1.1f;
	        CvSize minFeatureSize = cvSize(40, 40);
	            int flags =  CV_HAAR_DO_CANNY_PRUNING;

	cvNamedWindow( "Example", CV_WINDOW_AUTOSIZE );
	//cvNamedWindow( "Output", CV_WINDOW_AUTOSIZE );
	//cvCreateTrackbar( "Filters", "Output", &g_switch_value, 5, switch_callback );
	//CvPoint seed_point = cvPoint(305,195);
	//CvScalar color = CV_RGB(250,0,0);
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
		imgout = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U,1);

		cvConvertImage(image, imgout);

		rects = cvHaarDetectObjects(imgout, faceCascade, storage, search_scale_factor, 2, flags, minFeatureSize);
		nFaces = rects->total;
		for (int iface = 0; iface < (rects ? nFaces : 0); ++iface)
			{
				CvRect *r = (CvRect*)cvGetSeqElem(rects, iface);

				bbox[0] = r->x;
				bbox[1] = r->y;
				bbox[2] = r->x + r->width;
				bbox[3] = r->y + r->height;

				flandmark_detect(imgout, bbox, model, landmarks);
				cvRectangle(image, cvPoint(bbox[0], bbox[1]), cvPoint(bbox[2], bbox[3]), CV_RGB(255,0,0) );
				cvRectangle(image, cvPoint(model->bb[0], model->bb[1]), cvPoint(model->bb[2], model->bb[3]), CV_RGB(0,0,255) );
				cvCircle(image, cvPoint((int)landmarks[0], (int)landmarks[1]), 3, CV_RGB(0, 0,255), CV_FILLED);
				for (int i = 2; i < 2*model->data.options.M; i += 2)
				{
					cvCircle(image, cvPoint(int(landmarks[i]), int(landmarks[i+1])), 3, CV_RGB(255,0,0), CV_FILLED);

				}
			}

				/*
		switch( filterInt ){

					case 1:
						//cvSmooth( img, out, CV_GAUSSIAN, 7, 7 );
						cvSmooth( image, imgout, CV_GAUSSIAN, smoothing_kernel_size, smoothing_kernel_size );
						break;
					case 2:
						//cvSmooth( img, out, CV_MEDIAN, 7, 7 );
						cvSmooth( image, imgout, CV_MEDIAN, smoothing_kernel_size, smoothing_kernel_size );
						break;
					case 3:
						cvErode( image, imgout, NULL, 1);
						break;
					case 4:
						cvDilate( image, imgout, NULL, 1);
						break;
					case 5:
						cvSmooth( image, imgout, CV_GAUSSIAN, 1, 1 );
						cvFloodFill( imgout, seed_point, color, cvScalarAll(fillDiff), cvScalarAll(fillDiff), NULL, 4, NULL );
						break;
		}*/

		//cvShowImage( "Output", imgout );
		cvShowImage( "Example", image );

		////cvReleaseImage( &image );
		//cvReleaseImage( &imgout );

		key=cvWaitKey(5);

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
		} else if( key == 119 ) { //7
			fillDiff *= 1.05;
			cout << "fillDiff: " << fillDiff << endl;
		} else if( key == 115 ) { //7
			fillDiff *= 0.95;
			cout << "fillDiff: " << fillDiff << endl;
		}

	}
	if(imgout) {
		cvReleaseImage( &imgout );
	}
	cvReleaseCapture(&capture);
	cvDestroyWindow( "Example" );
	//cvDestroyWindow( "Output" );
	return 0;
}
