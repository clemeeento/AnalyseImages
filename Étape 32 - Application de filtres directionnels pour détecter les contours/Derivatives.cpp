#include <iostream>
#include <vector>
#include <iomanip>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>

int main()
{
    // Chargement de l'image en niveaux de gris
    cv::Mat image = cv::imread("boldt.jpg", 0);
    if (image.empty())
    {
        return 0;
    }

    // Calcul du filtre Sobel pour détecter les contours horizontaux
    cv::Mat sobelX;
    cv::Sobel(image, sobelX, CV_8U, 1, 0, 3, 0.4, 128);
    cv::imshow("Sobel X Image", sobelX);

    // Calcul du filtre Sobel pour détecter les contours verticaux
    cv::Mat sobelY;
    cv::Sobel(image, sobelY, CV_8U, 0, 1, 3, 0.4, 128);
    cv::imshow("Sobel Y Image", sobelY);

    // Recalcul des gradients Sobel avec une précision 16 bits pour éviter les saturations
    cv::Sobel(image, sobelX, CV_16S, 1, 0);
    cv::Sobel(image, sobelY, CV_16S, 0, 1);

    // Combine les gradients horizontaux et verticaux en calculant leur magnitude approximative
    cv::Mat sobel = abs(sobelX) + abs(sobelY);

    // Normalisation des valeurs du gradient Sobel pour une visualisation correcte
    double sobmin, sobmax;
    cv::minMaxLoc(sobel, &sobmin, &sobmax);

    // Affiche le filtre Sobel X avec un noyau plus grand (7x7)
    cv::Sobel(image, sobelX, CV_8U, 1, 0, 7, 0.001, 128);
    cv::imshow("Sobel X Image (7x7)", sobelX);

    // Convertit les valeurs de gradient Sobel pour les adapter à une plage de 8 bits
    cv::Mat sobelImage;
    sobel.convertTo(sobelImage, CV_8U, -255.0 / sobmax, 255);
    cv::imshow("Sobel Image", sobelImage);

    // Application d'un seuillage bas sur l'image de gradient Sobel
    cv::Mat sobelThresholded;
    cv::threshold(sobelImage, sobelThresholded, 225, 255, cv::THRESH_BINARY);
    cv::imshow("Binary Sobel Image (low)", sobelThresholded);

    // Application d'un seuillage haut sur l'image de gradient Sobel
    cv::threshold(sobelImage, sobelThresholded, 190, 255, cv::THRESH_BINARY);
    cv::imshow("Binary Sobel Image (high)", sobelThresholded);

    cv::waitKey(0);

    return 0;
}
