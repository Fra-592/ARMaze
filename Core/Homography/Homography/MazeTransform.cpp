#include "MazeTransform.h"

Mat getIntrinsicMatrix() {

	Mat K = Mat(3, 3, CV_64FC1);

	K.at<double>(0, 0) = FX;
	K.at<double>(0, 1) = 0.0f;
	K.at<double>(0, 2) = X0;

	K.at<double>(1, 0) = 0.0f;
	K.at<double>(1, 1) = FY;
	K.at<double>(1, 2) = Y0;

	K.at<double>(2, 0) = 0.0f;
	K.at<double>(2, 1) = 0.0f;
	K.at<double>(2, 2) = 1.0f;

	return K;
}

bool sortByY(Point2d p1, Point2d p2) {
	return p1.y < p2.y;
}

vector<Point2d> sortPoints(vector<Point2d> coord) {

	sort(coord.begin(), coord.end(), sortByY);

	if (coord[0].x > coord[1].x) swap(coord[0], coord[1]);
	if (coord[3].x > coord[2].x) swap(coord[2], coord[3]);

	return coord;
}

MazeTransform::MazeTransform() {
	K = getIntrinsicMatrix();
}
MazeTransform::MazeTransform(Mat intrinsic_matrix) {
	K = intrinsic_matrix;
}

MazeTransform::~MazeTransform() {

}

void MazeTransform::compute_transform(vector<Point2d> corners) {

	for(int i = 0; i < corners.size(); i++)
		cout << corners[i] << endl;

	corners = sortPoints(corners);

	vector<Point3f> a4points;
	a4points.push_back(Point3f(0, 0, 0));
	a4points.push_back(Point3f(dimX, 0, 0));
	a4points.push_back(Point3f(dimX, dimY, 0));
	a4points.push_back(Point3f(0, dimY, 0));

	H = findHomography(a4points, corners);

	vector<float> vectNull;
	solvePnP(a4points, corners, getIntrinsicMatrix(), vectNull , rot, trans);

	cout << "homographie décomposée" << endl;

}

Mat MazeTransform::get_H() {
	return H;
}

Mat MazeTransform::get_rots() {
	return rot;
}

Mat MazeTransform::get_trans() {
	return trans;
}
