#ifndef DETECTEURCONTOURS_HPP
#define DETECTEURCONTOURS_HPP

#include <iostream>
#include <opencv2/opencv.hpp>

class DetecteurContours
{
public:
    // Détection des contours avec Sobel
    static cv::Mat DetecterSobel(const cv::Mat &image, int tailleNoyau = 3, int tailleNoyauGaussien = 5);

    // Détection des contours avec Laplace
    static cv::Mat DetecterLaplace(const cv::Mat &image, int tailleNoyauLaplace = 3, int tailleNoyauGaussien = 5);

    // Détection des contours avec Canny
    static cv::Mat DetecterCanny(const cv::Mat &image, int seuilBas = 100, int seuilHaut = 200, int tailleNoyauGaussien = 5);
};

#endif // DETECTEURCONTOURS_HPP
