#include <iostream>
#include <opencv2/opencv.hpp>
#include "pointeur_stylo.h"

using namespace cv;
using namespace std;

int main()
{
	Mat flux;
	pointeur_stylo p = pointeur_stylo(flux);
	p.start();
	return 0;
}
