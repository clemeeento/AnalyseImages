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
    // Charger l'image binaire en niveaux de gris
    cv::Mat image = cv::imread("binaryGroup.bmp", 0);
    if(image.empty())
    {
        return 0;
    }

    // Afficher l'image originale binaire
    cv::imshow("Original Binary Image", image);

    // Vecteur pour stocker les contours détectés
    std::vector<std::vector<cv::Point>> contours;

    // Détection des contours externes uniquement
    cv::findContours(image, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

    // Création d'une image blanche pour afficher les contours détectés
    cv::Mat result(image.size(), CV_8U, cv::Scalar(255));

    // Dessine tous les contours détectés en noir sur l'image blanche
    cv::drawContours(result, contours, -1, cv::Scalar(0), 2);

    // Affiche l'image avec les contours détectés
    cv::imshow("Contours", result);

    // Définition des tailles minimales et maximales des contours à conserver
    int cmin = 50;  // Taille minimale d'un contour
    int cmax = 500; // Taille maximale d'un contour

    // Parcours de tous les contours détectés
    std::vector<std::vector<cv::Point>>::iterator itc = contours.begin();
    while(itc != contours.end())
    {
        // Si la taille du contour est en dehors des limites, on le supprime
        if(itc->size() < cmin || itc->size() > cmax)
        {
            itc = contours.erase(itc); // Supprime et avance l'itérateur
        }
        else
        {
            ++itc; // Passe au contour suivant
        }
    }

    // Charger l'image originale en niveaux de gris
    cv::Mat original = cv::imread("group.jpg", 0);
    if(original.empty()) 
    {
        return 0;
    }

    // Dessine les contours filtrés (animaux) sur l'image originale en blanc
    cv::drawContours(original, contours, -1, cv::Scalar(255, 255, 255), 2);

    // Affiche l'image originale avec les contours dessinés
    cv::imshow("Contours on animals", original);

    // Réinitialise l'image de sortie en blanc
    result.setTo(cv::Scalar(255));

    // Dessine les contours filtrés sur une image blanche avec une épaisseur de 1 pixel
    cv::drawContours(result, contours, -1, 0, 1);

    // Recharger l'image binaire
    image = cv::imread("binaryGroup.bmp", 0);
    if(image.empty()) 
    {
        return 0;
    }

    // Détection des contours avec RETR_LIST (tous les contours sont détectés, y compris les imbriqués)
    cv::findContours(image, contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);

    // Réinitialisation de l'image de sortie en blanc
    result.setTo(cv::Scalar(255));

    // Dessine tous les contours détectés sur une image blanche avec une épaisseur de 1 pixel
    cv::drawContours(result, contours, -1, 0, 1);

    // Affiche tous les contours détectés
    cv::imshow("All contours", result);


    cv::waitKey(0);

    return 0;
}
