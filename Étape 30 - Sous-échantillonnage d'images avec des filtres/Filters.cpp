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

    // Réduction naïve de l'image (échantillonnage)
    cv::Mat reduced(image.rows / 4, image.cols / 4, CV_8U);
    cv::Mat reduced2(image.rows / 4, image.cols / 4, CV_8U);
    for (int i = 0; i < reduced.rows; i++)
    {
        for (int j = 0; j < reduced.cols; j++)
        {
            reduced.at<uchar>(i, j) = image.at<uchar>(i * 4, j * 4);
            reduced2.at<uchar>(i, j) = image.at<uchar>(i * 4, j * 4);
        }
    }

    // Sauvegarde de l'image réduite
    cv::imwrite("reducedimage.bmp", reduced);

    // Affichage de l'image réduite
    cv::imshow("Badly Reduced Image", reduced);

    // Redimensionnement avec interpolation au plus proche voisin
    cv::Mat newImage;
    cv::resize(reduced, newImage, cv::Size(), 4, 4, cv::INTER_NEAREST);
    cv::imshow("Reduced Image", newImage);

    // Redimensionnement avec interpolation bilinéaire
    cv::resize(reduced, newImage, cv::Size(), 4, 4, cv::INTER_LINEAR);
    cv::imshow("Bilinear resizing", newImage);

    // Création d'une pyramide d'images
    cv::Mat pyramid(image.rows, image.cols + image.cols / 2 + image.cols / 4 + image.cols / 8, CV_8U, cv::Scalar(255));
    image.copyTo(pyramid(cv::Rect(0, 0, image.cols, image.rows)));

    // Réduction pyramidale progressive
    cv::pyrDown(image, reduced); // Réduction de moitié
    reduced.copyTo(pyramid(cv::Rect(image.cols, image.rows / 2, image.cols / 2, image.rows / 2)));

    cv::pyrDown(reduced, reduced2); // Nouvelle réduction de moitié
    reduced2.copyTo(pyramid(cv::Rect(image.cols + image.cols / 2, image.rows - image.rows / 4, image.cols / 4, image.rows / 4)));

    cv::pyrDown(reduced2, reduced); // Encore une réduction
    reduced.copyTo(pyramid(cv::Rect(image.cols + image.cols / 2 + image.cols / 4, image.rows - image.rows / 8, image.cols / 8, image.rows / 8)));

    // Affichage de la pyramide d'images
    cv::imshow("Pyramid of images", pyramid);

    // Redimensionnement avec interpolation bilinéaire pour agrandir
    cv::resize(reduced2, newImage, cv::Size(), 4, 4, cv::INTER_LINEAR);
    cv::imshow("Bilinear resizing", newImage);

    // Application d'un filtre médian
    cv::medianBlur(image, result, 7);
    cv::imshow("Median filtered Image", result);

    // Application d'un filtre moyen après le filtre médian
    cv::blur(result, result, cv::Size(5, 5));
    cv::imshow("Median filtered S&P Image", result);

    cv::waitKey(0);

    return 0;
}
