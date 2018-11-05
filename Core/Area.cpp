#include "Area.h"

std::vector<std::vector<Point>> Area::getWall() const
{
	return m_Wall;
}

std::vector<Point> Area::getArea() const
{
	return m_Area;
}

std::vector<Point> Area::getStart() const
{
	return m_Start;
}

int Area::getSizeX() const
{
	return m_SizeX;
}

int Area::getSizeY() const
{
	return m_SizeY;
}

std::vector<Point> Area::getEnd() const
{
	return m_End;
}

void Area::setWall(const std::vector<std::vector <Point>> & wall) {
	m_Wall = wall;
}
void Area::setArea(const std::vector <Point> & area) {
	m_Area = area;
}
void Area::setStart(const std::vector <Point> & start) {
	m_Start = start;
	// calcul center
}
void Area::setEnd(const std::vector <Point> & end) {
	m_End = end;
	// calcul center
}

void Area::setSizeX(const int & sizeX) {
	m_SizeX = sizeX;
	// calcul center
}
void Area::setSizeY(const int & sizeY) {
	m_SizeY = sizeY;
	// calcul center
}


bool Area::buildEdge(const Mat & canny) {
	// find all edge
	std::vector<std::vector<Point> > contours;
	findContours(canny.clone(), contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	std::vector<Point> approx;
	sort(contours.begin(), contours.end(), compareConvexe); // sort connection

	double max = 0;
	for (int index = 0; index < contours.size(); ++index) {
		if (fabs(contourArea(Mat(contours[index]))) <= 2500) {
			continue;
		}
		approxPolyDP(Mat(contours[index]), approx, arcLength(cv::Mat(contours[index]), true) * 0.1, true);

		if (approx.size() == 4) {
			if (fabs(contourArea(Mat(approx))) > max) {
				max = fabs(contourArea(Mat(approx)));
				m_Area = approx;
			}
		}
	}

	if (max == 0)
		return false;
	return true;
}

bool Area::buildStartEnd(const Mat & mask, const int & Xmin, const int & Xmax, const int & Ymin, const int & Ymax) {
	std::vector<std::vector<Point> > contours;
	findContours(mask.clone(), contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	std::vector<Point> approx;
	// find start and end
	for (int i = 0; i < contours.size(); i++)
	{
		if (fabs(contourArea(Mat(contours[i]))) <= 1000) {
			continue;
		}
		// Approximate contour with accuracy proportional
		// to the contour perimeter

		cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true) * 0.05, true);

		if (approx.size() == 3)
		{
			if (m_End.empty()) {

				bool isOk2 = true;
				for (int index = 0; index < approx.size(); ++index) {
					if (approx[index].x < Xmin || approx[index].x > Xmax) {
						isOk2 = false;
					}
					if (approx[index].y < Ymin || approx[index].y > Ymax) {
						isOk2 = false;
					}
				}
				if (isOk2) {
					m_End = approx;

				}
			}

		}
		else if (approx.size() == 4)
		{
			if (m_Start.empty()) {
				bool isOk = true;
				for (int index = 0; index < approx.size(); ++index) {
					if (approx[index].x < Xmin || approx[index].x > Xmax) {
						isOk = false;
					}
					if (approx[index].y < Ymin || approx[index].y > Ymax) {
						isOk = false;
					}
				}
				if (isOk) {
					m_Start = approx;
				}
			}
		}
	}

	if (m_Start.empty() || m_End.empty())
		return false;
	return true;
}

bool Area::buildWalls(const Mat & mask) {
	std::vector<Vec4i> Hugue;
	HoughLinesP(mask, Hugue, 1, CV_PI / 180, 30, 15, 10);

	std::vector<Point> square = m_Start;

	std::vector<Point> triangle = m_End;

	std::vector<std::vector<Point>> polygons;
	polygons = ConvertToPolygons(Hugue);
	polygons = FilterInside(polygons, square, m_Area);
	polygons = FilterInside(polygons, triangle, m_Area);

	std::vector<std::vector<Point>> filteredPolygons = polygons;

	for (int i = 0; i < polygons.size(); i++)
	{
		filteredPolygons = FilterInside(filteredPolygons, polygons[i], m_Area);
		filteredPolygons.push_back(polygons[i]);
	}
	m_Wall = filteredPolygons;
	if (filteredPolygons.empty() || m_Wall.size() < 3)
		return false;
	return true;
}

std::vector<std::vector<Point>> Area::ConvertToPolygons(std::vector<Vec4i> contours)
{
	std::vector<std::vector<Point>> polygons;
	for (int i = 0; i < contours.size(); i++) {
		std::vector<Point> segment(2);
		segment[0] = Point(contours[i][0], contours[i][1]);
		segment[1] = Point(contours[i][2], contours[i][3]);
		polygons.push_back(segment);
	}
	return(polygons);
}

std::vector<std::vector<Point>> Area::FilterInside(std::vector<std::vector<Point>> lines,
	std::vector<Point> polygon,
	std::vector<Point> bounds,
	int seuil /*= 50*/) {
	int xmin = INT_MAX;
	int xmax = INT_MIN;
	int ymin = INT_MAX;
	int ymax = INT_MIN;
	int upperbound = INT_MAX;
	int lowerbound = INT_MIN;
	int leftbound = INT_MAX;
	int rightbound = INT_MIN;

	std::vector<std::vector<Point>> result;

	// Getting the bounds of the map.
	for (int i = 0; i < bounds.size(); i++)
	{
		int x = bounds[i].x;
		int y = bounds[i].y;

		if (y < upperbound) {
			upperbound = y + 20;
		}
		if (y > lowerbound) {
			lowerbound = y - 20;
		}
		if (x < leftbound) {
			leftbound = x + 20;
		}
		if (x > rightbound) {
			rightbound = x - 20;
		}
	}

	// Getting a square bounding box around the polygon.
	for (int i = 0; i < polygon.size(); i++) {
		int x = polygon[i].x;
		int y = polygon[i].y;

		if (x < xmin) {
			xmin = x;
		}
		if (x > xmax) {
			xmax = x;
		}
		if (y < ymin)
		{
			ymin = y;
		}
		if (y > ymax) {
			ymax = y;
		}
	}

	// Looking for points into the bounding box or outside the bounds.
	for (int i = 0; i < lines.size(); i++)
	{
		std::vector<Point> line = lines[i];
		bool isInside = true;
		bool isOutside = false;
		for (int j = 0; j < line.size(); j++)
		{
			isInside = isInside && line[j].x >= xmin - seuil && line[j].x <= xmax + seuil;
			isInside = isInside && line[j].y >= ymin - seuil && line[j].y <= ymax + seuil;
			isOutside = isOutside || line[j].x >= rightbound || line[j].x <= leftbound;
			isOutside = isOutside || line[j].y >= lowerbound || line[j].y <= upperbound;
		}
		if (!isInside && !isOutside)
		{
			result.push_back(line);
		}
	}

	return(result);
}




bool Area::tracking(const Mat & mask, const int & Xmin, const int & Xmax, const int & Ymin, const int & Ymax) {
	std::vector<std::vector<Point> > contours;
	findContours(mask.clone(), contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	std::vector<Point> approx;
	// find start and end
	for (int i = 0; i < contours.size(); i++)
	{
		if (fabs(contourArea(Mat(contours[i]))) <= 2000) {
			continue;
		}

		cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true) * 0.1, true);



		if (approx.size() == 4)
		{
			bool isOk = true;
			for (int index = 0; index < approx.size(); ++index) {
				if ((approx[index].x < Xmin && approx[index].x > Xmin - 25)|| (approx[index].x > Xmax && approx[index].x < Xmax + 25)) {
					isOk = false;
				}
				if (approx[index].y < Ymin || approx[index].y > Ymax) {
					isOk = false;
				}
			}
			if (isOk) {
				m_Area = approx;
			}
			
		}
	}
	return true;
}



Area* create_area() {
	return new Area;
}

Point2d* get_begin_center(Area* area, TransformTracking* transformTracking) {
	vector<Point> begin = area->getStart();
	Point2d center;
	for (int i = 0; i < 4; i++) {
		center += Point2d(begin[i].x, begin[i].y);
		center /= 4;
	}

	Mat end3d(3, 1, CV_64FC1);

	end3d.at<double>(0, 0) = center.x;
	end3d.at<double>(1, 0) = center.y;
	end3d.at<double>(2, 0) = 0;

	Mat rot = transformTracking->get_init_rot();
	Mat trans = transformTracking->get_init_trans();

	Mat unprojectedPoint = trans.inv() * rot.inv() * end3d;

	return new Point2d(unprojectedPoint);
}

Point2d* get_end_center(Area* area, TransformTracking* transformTracking) {
	vector<Point> end = area->getStart();
	Point2d center;
	for (int i = 0; i < 4; i++) {
		center += Point2d(end[i].x, end[i].y);
		center /= 4;
	}

	Mat end3d(3, 1, CV_64FC1);

	end3d.at<double>(0, 0) = center.x;
	end3d.at<double>(1, 0) = center.y;
	end3d.at<double>(2, 0) = 0;

	Mat rot = transformTracking->get_init_rot();
	Mat trans = transformTracking->get_init_trans();

	Mat unprojectedPoint = trans.inv() * rot.inv() * end3d;

	return new Point2d(unprojectedPoint);
}

vector<Point2d*>* get_wall(Area* area, TransformTracking* transformTracking, int i) {

	vector<vector<Point>> walls = area->getWall();
	vector<Point> wall = walls[i];

	Mat wallMat(3,2,CV_64FC1);

	wallMat.at<double>(0, 0) = wall[0].x;
	wallMat.at<double>(1, 0) = wall[0].y;
	wallMat.at<double>(2, 0) = 0;
	wallMat.at<double>(0, 1) = wall[1].x;
	wallMat.at<double>(1, 1) = wall[1].y;
	wallMat.at<double>(2, 1) = 0;

	Mat rot = transformTracking->get_init_rot();
	Mat trans = transformTracking->get_init_trans();

	Mat unprojectedPoints = trans.inv() * rot.inv() * wallMat;

	vector<Point2d*>* result = new vector<Point2d*>;
	result->push_back(new Point2d(unprojectedPoints.at<double>(0, 0), unprojectedPoints.at<double>(1, 0)));
	result->push_back(new Point2d(unprojectedPoints.at<double>(0, 1), unprojectedPoints.at<double>(1, 1)));

	return result;

}

Point2d* get_point(int j, vector<Point2d*>* wall) {
	return (*wall)[j];
}

int nb_of_walls(Area* area) {
	return area->getWall().size();
}

int size(vector<Point*>* points) {
	return points->size();
}

double get_x(Point2d* point) {
	return point->x;
}

double get_y(Point2d* point) {
	return point->y;
}
