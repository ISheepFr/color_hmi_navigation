#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
#include <Windows.h>

using namespace cv;
using namespace std;

class pointeur_stylo
{

public:

	pointeur_stylo(Mat flux);
	cv::Point findRedPixelCoordinates();
	bool findGreenContourMidpoint();

	double dist2color(Vec3b color_pixel, Vec3b spec_color);
	void closest2color(int& res, Vec3b color);

	void start();

	void place_mouse_at(cv::Point);

	void realMouse(int event, int x, int y, int flag) {
		if (event == cv::EVENT_LBUTTONDOWN) {

			if (x >= d_gauche_buttonNOcolor.x && x <= d_droite_buttonNOcolor.x &&
				y >= d_gauche_buttonNOcolor.y && y <= d_droite_buttonNOcolor.y)
			{
				std::cout << "Les coordonn�es se situent dans le rectangle rouge." << std::endl;
				if (d_vec_color.size() == 2)
				{
					closest2color(vert_nocolor, d_vec_color[1]);
					std::cout << "vert min" << vert_nocolor << std::endl;
				}
				else {
					closest2color(rouge_nocolor, d_vec_color[0]);
					std::cout << "rouge min" << rouge_nocolor << std::endl;
				}

			}
			else if (x >= d_gauche_buttonFULLcolor.x && x <= d_droite_buttonFULLcolor.x &&
				y >= d_gauche_buttonFULLcolor.y && y <= d_droite_buttonFULLcolor.y)
			{
				std::cout << "Les coordonn�es se situent dans le rectangle vert." << std::endl;
				if (d_vec_color.size() == 2)
				{
					closest2color(vert_fullcolor, d_vec_color[1]);
					std::cout << "vert max" << vert_fullcolor << std::endl;
				}
				else {
					closest2color(rouge_fullcolor, d_vec_color[0]);
					std::cout << "rouge max" << rouge_fullcolor << std::endl;
				}
			}
			else
			{


				cv::Vec3b clickedColor = d_flux.at<cv::Vec3b>(y, x);
				
				
				d_color = clickedColor;
				d_vec_color.push_back(d_color);
				


				
				
			}
		}
	}

//stackoverflow

	// static wrapper (does NOT need "this")
	static void onMouse(int event, int x, int y, int flag, void* me) {
		// "me" is, what we fed into setMouseCallback() below:
		pointeur_stylo* that = (pointeur_stylo*)(me); // cast back to proper class
		that->realMouse(event, x, y, flag); // call class member
	}



private:
	Mat d_flux, d_flux_red, d_flux_green;

	bool isPressed = false;

	int d_width;
	int d_height;

	Mat redMask;
	Mat greenMask;

	int d_count;
	int d_red_count = 0;
	int d_green_count = 0;

	Point d_best;

	Point d_gauche_buttonNOcolor;
	Point d_droite_buttonNOcolor;
	Point d_gauche_buttonFULLcolor;
	Point d_droite_buttonFULLcolor;

	std::vector<Vec3b> d_vec_color;

	Vec3b d_color = (255, 255, 255);

	int rouge_nocolor = -1;
	int rouge_fullcolor = -2;

	int vert_nocolor = -1;
	int vert_fullcolor = -2;

    VideoCapture d_cap;





};



