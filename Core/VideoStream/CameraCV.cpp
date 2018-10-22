#include "CameraCV.h"

CameraCV::CameraCV() {
	this->capture = cv::VideoCapture();
}

CameraCV::~CameraCV() {
}

void CameraCV::openStream(int inputId /* = 0 */) {
	this->capture.open(inputId);
	if(!this->capture.isOpened()) {
		printf("--(!)Error opening video capture\n");
		return;
	}
}

cv::Mat CameraCV::getFrame() {
	this->capture.read(this->frame);
	return this->frame;
}

int CameraCV::getWidth() {
	return this->frame.cols;
}

int CameraCV::getHeigth() {
	return this->frame.rows;
}

void CameraCV::displayStream() {
	cv::namedWindow("Debug CameraCV frame");

	int c = cvWaitKey(10);
	while((char)c != 'q') {
		cv::imshow("Debug CameraCV frame", this->getFrame());
		int c = cvWaitKey(10);
	}
}

uchar * CameraCV::getLiveFrame(int * sizeofMat) {
	this->capture.read(this->frame);
	cv::cvtColor(this->frame, this->frame, CV_BGR2BGRA, 4);
	*sizeofMat = this->frame.rows * this->frame.cols * 4; //this->frame.total() * this->frame.elemSize();

	return this->frame.data;
}