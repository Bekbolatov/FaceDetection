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

int main(void)
{
	FLANDMARK_Model * model = flandmark_init("data/flandmark_model.dat");
	int bbox[] = {72, 72, 183, 183};
	double * landmarks = (double*)malloc(2*model->data.options.M*sizeof(double));  //used to be float *
	cout << "test" << endl;
	char key;
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

		cvShowImage( "Example", image );


		key=cvWaitKey(5);

		if( key == 27 ) {
			cout << "Exiting on KEY: " << key << endl;
			break;
		}
	}
	if(imgout) {
		cvReleaseImage( &imgout );
	}
	cvReleaseCapture(&capture);
	cvDestroyWindow( "Example" );
	return 0;
}
