#ifndef AREA_H
#define AREA_H

#include "Homography\Homography\TransformTracking.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc.hpp>
#include <numeric>
#include <iostream>
#include <vector>

using namespace cv;

class Area {

	private:
		std::vector<std::vector <Point>> m_Wall; 
		std::vector <Point>				 m_Area;
		int								 m_SizeX;
		int								 m_SizeY;

		std::vector <Point>				 m_Start;
		std::vector <Point>				 m_End;
		Point							 m_StartCenter;
		Point							 m_EndCenter;
		struct myclass {
			bool operator() (std::vector <Point> p1, std::vector <Point> p2) { return (p1.size() < p2.size()); }
		} compareConvexe;
		std::vector<std::vector<Point>> FilterInside(std::vector<std::vector<Point>> lines, std::vector<Point> polygon, std::vector<Point> bounds, int seuil = 20);
		std::vector<std::vector<Point>> ConvertToPolygons(std::vector<Vec4i> contours);

	public:
		std::vector<std::vector <Point>> getWall() const;
		std::vector <Point> getArea() const;
		std::vector <Point> getStart() const;
		std::vector <Point> getEnd() const;
		int getSizeX() const;
		int getSizeY() const;

		void setWall(const std::vector<std::vector <Point>> & wall);
		void setArea(const std::vector <Point> & area);
		void setStart(const std::vector <Point> & start);
		void setEnd(const std::vector <Point> & end);
		void setSizeX(const int & sizex);
		void setSizeY(const int & sizey);

		bool buildEdge(const Mat & canny);
		bool buildStartEnd(const Mat & mask, const int & Xmin, const int & Xmax, const int & Ymin, const int & Ymax);
		bool buildWalls(const Mat & mask);
		bool tracking(const Mat & mask, const int & Xmin, const int & Xmax, const int & Ymin, const int & Ymax);
};

#ifdef _WIN32
#define DllExport extern "C" __declspec(dllexport)
#else
#define DllExport extern "C"
#endif

DllExport Area* create_area();

DllExport Point2d* get_begin_center(Area*, TransformTracking*);
DllExport Point2d* get_end_center(Area*, TransformTracking*);

DllExport vector<Point2d*>* get_wall(Area*, TransformTracking*, int i);
DllExport Point2d* get_point(int j, vector<Point*>*);
DllExport int nb_of_walls(Area*);
DllExport int size(vector<Point*>*);

DllExport double get_x(Point2d*);
DllExport double get_y(Point2d*);

#endif //AREA_H