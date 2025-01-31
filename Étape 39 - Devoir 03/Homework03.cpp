#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

#include "Fonctions.hpp"
#include "EvaluateurContours.hpp"

int main()
{
    std::vector<cv::Mat> tableauOriginales; // Tableau pour les images originales
    std::vector<cv::Mat> tableauContours; // Tableau pour le contour des images

    // Créer un tableau pour les images originales
    CreerTableauImages(tableauOriginales, "Originales");

    //Créer un tableau pour les contours des images
    CreerTableauImages(tableauContours, "Contours");

    // Afficher les images originales
    for(size_t i = 0; i < tableauOriginales.size(); ++i)
    {
        AfficherImage("Image originale", tableauOriginales[i]);
        cv::waitKey(0);
    }

    // Afficher les contours des images
    for(size_t i = 0; i < tableauContours.size(); ++i)
    {
        AfficherImage("Contours", tableauContours[i]);
        cv::waitKey(0);
    }

    cv::waitKey(0);
    return 0;
}