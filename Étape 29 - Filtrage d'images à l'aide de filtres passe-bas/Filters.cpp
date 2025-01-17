#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>

int main()
{
    // Chargement de l'image en niveaux de gris
    cv::Mat image = cv::imread("boldt.jpg", 0);
    if (image.empty()) return 0;

    // Affichage de l'image originale
    cv::imshow("Original Image", image);

    // Application d'un filtre moyen (blur)
    cv::Mat result;
    cv::blur(image, result, cv::Size(5, 5));
    cv::imshow("Mean filtered Image", result);

    // Application d'un filtre gaussien
    cv::GaussianBlur(image, result, cv::Size(5, 5), 1.5);
    cv::imshow("Gaussian filtered Image", result);

    // Applique un flou gaussien directement sur l'image d'origine
    cv::GaussianBlur(image, image, cv::Size(11, 11), 1.75);

    // Affichage de l'image floutée
    cv::imshow("Gaussian filtered Image", image);

    cv::waitKey(0);

    return 0;
}
