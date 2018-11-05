#pragma once

#include "MazeTransform.h"

class TransformTracking
{

public:
	TransformTracking();
	~TransformTracking();

	void init_from_maze(MazeTransform maze);
	void update_from_maze(MazeTransform maze);

	void init(Mat trans, Mat rot);
	void update(Mat trans, Mat rot);

	Mat get_init_rot();
	Mat get_current_rot();
	Mat get_delta_rot();

	Mat get_init_trans();
	Mat get_current_trans();
	Mat get_delta_trans();

	static Mat affect_tranform(Mat point, Mat trans, Mat rot);

private:
	Mat m_initRot;
	Mat m_currentRot;
	Mat m_deltaRot;

	Mat m_initTrans;
	Mat m_currentTrans;
	Mat m_deltaTrans;

};

#ifdef _WIN32
#define DllExport extern "C" __declspec(dllexport)
#else
#define DllExport extern "C"
#endif

DllExport TransformTracking* create_transform_tracking();
DllExport Mat* get_init_rot(TransformTracking*);
DllExport Mat* get_delta_rot(TransformTracking*);
DllExport Mat* get_init_trans(TransformTracking*);
DllExport Mat* get_delta_trans(TransformTracking*);
DllExport double at(int, int, Mat*);