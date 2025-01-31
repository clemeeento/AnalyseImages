#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

#include "Fonctions.hpp"
#include "DetecteurContours.hpp"

int main()
{
    std::vector<cv::Mat> tableauOriginales; // Tableau pour les images originales
    std::vector<cv::Mat> tableauContours; // Tableau pour le contour des images
    std::vector<cv::Mat> tableauContoursSobel; // Tableau pour le contour des images avec Sobel
    std::vector<cv::Mat> tableauContoursLaplace; // Tableau pour le contour des images avec Laplace
    std::vector<cv::Mat> tableauContoursCanny; // Tableau pour le contour des images avec Canny

    // Créer un tableau pour les images originales
    CreerTableauImages(tableauOriginales, "Originales");

    //Créer un tableau pour les contours des images
    CreerTableauImages(tableauContours, "Contours");

    // Contour des images avec Sobel
    for (size_t i = 0; i < tableauOriginales.size(); i++)
    {
        // Détection des contours avec Sobel
        tableauContoursSobel.push_back(DetecteurContours::DetecterSobel(tableauOriginales[i]));
        // Afficher les images
        cv::imshow("Image originale", tableauOriginales[i]);
        cv::imshow("Contours Sobel", tableauContoursSobel[i]);
        cv::waitKey(0);
    }

    // Contour des images avec Laplace
    for (size_t i = 0; i < tableauOriginales.size(); i++)
    {
        // Détection des contours avec Laplace
        tableauContoursLaplace.push_back(DetecteurContours::DetecterLaplace(tableauOriginales[i]));
        // Afficher les images
        cv::imshow("Image originale", tableauOriginales[i]);
        cv::imshow("Contours Laplace", tableauContoursLaplace[i]);
        cv::waitKey(0);
    }

    // Contour des images avec Canny
    for (size_t i = 0; i < tableauOriginales.size(); i++)
    {
        // Détection des contours avec Canny
        tableauContoursCanny.push_back(DetecteurContours::DetecterCanny(tableauOriginales[i]));
        // Afficher les images
        cv::imshow("Image originale", tableauOriginales[i]);
        cv::imshow("Contours Canny", tableauContoursCanny[i]);
        cv::waitKey(0);
    }

    cv::waitKey(0);
    return 0;
}