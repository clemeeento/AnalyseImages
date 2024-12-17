#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

int main() 
{
    cv::namedWindow("Image");

    // Image avec le logo normal---------------------------------
    cv::Mat image = cv::imread("puppy.bmp");
    cv::Mat logo = cv::imread("smalllogo.png");

    // cv::Rect est un rectangle qui est défini par les coordonnées de son coin supérieur gauche et de son coin inférieur droit
    // imageROI est une image qui pointe sur une région de l'image image
    cv::Mat imageROI(image, cv::Rect(image.cols - logo.cols, image.rows - logo.rows, logo.cols, logo.rows)); //

    // Copie le logo dans l'image
    logo.copyTo(imageROI);

    cv::imshow("Image", image);
    cv::waitKey(0);

    // Image avec le logo masqué---------------------------------
    image = cv::imread("puppy.bmp");

    imageROI = image(cv::Rect(image.cols - logo.cols, image.rows - logo.rows, logo.cols, logo.rows)); 

    // Autre manière de définir imageROI
    // imageROI = image(cv::Range(image.rows - logo.rows, image.rows), cv::Range(image.cols - logo.cols, image.cols)); 

    // Autres manières de définir imageROI
    // cv::Mat imageROI = image.rowRange(start, end); 
    // cv::Mat imageROI = image.colRange(start, end);

    // Crée un masque en niveaux de gris
    cv::Mat mask(logo);

    logo.copyTo(imageROI, mask);

    cv::imshow("Image", image);
    cv::waitKey(0);

    return 0;
}
