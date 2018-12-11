#ifndef CORE_H
#define CORE_H

#include "Area.h"
#include "Homography\Homography\TransformTracking.h"
#include "VideoStream\CameraCV.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/tracking/tracking.hpp>
#include <opencv2/tracking/tracker.hpp>
#include <opencv2/imgproc.hpp>
#include <numeric>
#include <iostream>
#include <string>
#include <vector>
 
using namespace cv;

class Core {
	private:
		CameraCV *		m_CameraCV;
		Mat				m_MaskTracker;
		Area			m_Area;
		bool			m_IsBuilt;
		Rect2d			m_TrackBox;
		int				m_TreshholdCanny1 = 50;
		int				m_TreshholdCanny2 = 200;
	public:
		Core();
		Core(CameraCV cameraCV);

		void Start();
		void Video();
		void BuildMaze();
		void TrackingArea();

		bool get_isBuild();
		bool capture_read();
		CameraCV * get_camera();
		void set_camera(CameraCV * camera);
		Area* getArea();

		void setTreshholdCanny1(int & newValue);
		void setTreshholdCanny2(int & newValue);
};

#endif //CORE_H