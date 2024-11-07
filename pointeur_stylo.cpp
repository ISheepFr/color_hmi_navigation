#include "pointeur_stylo.h"

pointeur_stylo::pointeur_stylo(Mat flux):d_flux{flux}
{

}

bool pointeur_stylo::findGreenContourMidpoint()
{
    cv::Mat hsvImage;
    cv::cvtColor(d_flux, hsvImage, cv::COLOR_BGR2HSV);
    Point plusHautPixelCouleur(d_width + 1, d_height + 1);

    for (int y = 0; y < d_flux.rows; y++)
    {
        for (int x = 0; x < d_flux.cols; x++)
        {
            Vec3b currentColor = d_flux.at<cv::Vec3b>(y, x);
            if (dist2color(currentColor, d_vec_color[0]) < 3)
            {
                return true;
                
            }
        }
    }
    
    return false;
}


cv::Point pointeur_stylo::findRedPixelCoordinates()
{
    cv::Mat hsvImage;
    cv::cvtColor(d_flux, hsvImage, cv::COLOR_BGR2HSV);
    Point plusHautPixelCouleur(d_width +1 ,d_height+1);

    for (int y = 0; y < d_flux.rows; y++)
    {
        for (int x = 0; x < d_flux.cols; x++)
        {
            Vec3b currentColor = d_flux.at<cv::Vec3b>(y, x);
            if (dist2color(currentColor, d_color) < 20)
            {
                if (y < plusHautPixelCouleur.y && x < plusHautPixelCouleur.x)
                {
                    plusHautPixelCouleur.y = y;
                    plusHautPixelCouleur.x = x;
                }
            }
        }
    }


    return plusHautPixelCouleur;
}

double pointeur_stylo::dist2color(Vec3b color_pixel, Vec3b spec_color)
{
    int r_diff = color_pixel[2] - spec_color[2];
    int g_diff = color_pixel[1] - spec_color[1];
    int b_diff = color_pixel[0] - spec_color[0];

    return std::sqrt(static_cast<double>(r_diff * r_diff + g_diff * g_diff + b_diff * b_diff));
}

void pointeur_stylo::closest2color(int& res, Vec3b color)
{
    double count = 0;
    double minDistance = 500000000000000000000.0;

    for (int y = 0; y < d_flux.rows; ++y) {
        for (int x = 0; x < d_flux.cols; ++x) {

            cv::Vec3b currentColor = d_flux.at<cv::Vec3b>(y, x);
            double distance = dist2color(currentColor, color);
           
            if (distance < minDistance) {
                minDistance = distance;
                d_best.x = x;
                d_best.y = y;
            }

            if (distance < 20)
            {
                count++;
            }
        }
    }

    res = count;
    std::cout << res << std::endl;

}




void pointeur_stylo::place_mouse_at(cv::Point p)
{

    if (!(p.x == -1 && p.y == -1))
    {
        SetCursorPos(p.x, p.y);
    }
}


void pressMouseButton()
{
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
}

void releaseMouseButton()
{
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}


void pointeur_stylo::start()
{
    if (d_cap.open(0))
    {
        cv::namedWindow("Flux original", cv::WINDOW_AUTOSIZE);
        d_width = d_cap.get(cv::CAP_PROP_FRAME_WIDTH);
        d_height = d_cap.get(cv::CAP_PROP_FRAME_HEIGHT);
        setMouseCallback("Flux original", pointeur_stylo::onMouse, this);
    }


    while (true)
    {
        Mat eq;
        d_cap >> d_flux;
        cv::flip(d_flux, d_flux, 1);

        d_flux_green = d_flux.clone();
        d_flux_red = d_flux.clone();

        //equalizeHistRGB(d_flux);
        //cv::bitwise_not(d_flux, d_flux);
        if (d_vec_color.size() == 2)
        {
            cv::Point redPixel = findRedPixelCoordinates();
            //std::cout << redPixel;
            Point p{ d_width + 1,d_height + 1 };
            if (redPixel != p)
            {

                place_mouse_at(redPixel);
                std::cout << "Coordonn�es pixel : " << redPixel.x << std::endl;


                if (findGreenContourMidpoint())
                {
                    if (!isPressed)
                    {
                    pressMouseButton();
                    std::cout << "PRESSED" << std::endl;
                    }
                    else {
                        std::cout << "still PRESSED" << std::endl;
                    }
                    
                }

                else {
                    isPressed = false;
                    releaseMouseButton();
                    std::cout << "nopressed" << std::endl;
                }

            }

            //releaseMouseButton();


        }

    imshow("Flux original", d_flux);

	if (waitKey(30) == 27)
		break;

}

d_cap.release();
destroyAllWindows();
}

