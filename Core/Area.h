#ifndef AREA_H
#define AREA_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc.hpp>
#include <numeric>
#include <iostream>

using namespace cv;

class Area {

	private:
		std::vector<std::vector <Point>> m_Wall; 
		std::vector <Point>				 m_Area;
		std::vector <Point>				 m_Start;
		std::vector <Point>				 m_End;
		Point							 m_StartCenter;
		Point							 m_EndCenter;
		struct myclass {
			bool operator() (std::vector <Point> p1, std::vector <Point> p2) { return (p1.size() < p2.size()); }
		} compareConvexe;
		std::vector<std::vector<Point>> FilterInside(std::vector<std::vector<Point>> lines, std::vector<Point> polygon, std::vector<Point> bounds, int seuil = 50);
		std::vector<std::vector<Point>> ConvertToPolygons(std::vector<Vec4i> contours);

	public:
		std::vector<std::vector <Point>> getWall() const;
		std::vector <Point> getArea() const;
		std::vector <Point> getStart() const;
		std::vector <Point> getEnd() const;

		void setWall(const std::vector<std::vector <Point>> & wall);
		void setArea(const std::vector <Point> & area);
		void setStart(const std::vector <Point> & start);
		void setEnd(const std::vector <Point> & end);

		bool buildEdge(const Mat & canny);
		bool buildStartEnd(const Mat & mask, const int & Xmin, const int & Xmax, const int & Ymin, const int & Ymax);
		bool buildWalls(const Mat & mask);
};


#endif //AREA_H