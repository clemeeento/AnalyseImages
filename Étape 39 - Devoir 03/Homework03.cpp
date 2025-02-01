#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

#include "Fonctions.hpp"
#include "MeilleursParametres.hpp"

int main()
{
    std::vector<cv::Mat> tableauOriginales; // Tableau pour les images originales
    std::vector<cv::Mat> tableauContours; // Tableau pour le contour des images
    std::vector<cv::Mat> tableauContoursSobel; // Tableau pour le contour des images avec Sobel
    std::vector<cv::Mat> tableauContoursLaplace; // Tableau pour le contour des images avec Laplace
    std::vector<cv::Mat> tableauContoursCanny; // Tableau pour le contour des images avec Canny

    // // Créer un tableau pour les images originales
    // CreerTableauImages(tableauOriginales, "Originales");

    // //Créer un tableau pour les contours des images
    // CreerTableauImages(tableauContours, "Contours");

    // // Tests sur une image
    // // Variables pour stocker les meilleurs paramètres pour Sobel, Laplace et Canny
    // int meilleursParametresSobel[2];
    // double resultatsMeilleursParametresSobel[3];

    // int meilleursParametresLaplace[2];
    // double resultatsMeilleursParametresLaplace[3];

    // int meilleursParametresCanny[3];
    // double resultatsMeilleursParametresCanny[3];

    // // Trouver les meilleurs paramètres pour Sobel
    // MeilleursParametresSobel(tableauOriginales[0], tableauContours[0], meilleursParametresSobel, resultatsMeilleursParametresSobel);

    // // Trouver les meilleurs paramètres pour Laplace
    // MeilleursParametresLaplace(tableauOriginales[0], tableauContours[0], meilleursParametresLaplace, resultatsMeilleursParametresLaplace);

    // // Trouver les meilleurs paramètres pour Canny
    // MeilleursParametresCanny(tableauOriginales[0], tableauContours[0], meilleursParametresCanny, resultatsMeilleursParametresCanny);

    // // Afficher les résultats
    // std::cout << "Meilleurs parametres pour Sobel : " << std::endl;
    // std::cout << "Taille du noyau de Sobel : " << meilleursParametresSobel[0] << std::endl;
    // std::cout << "Taille du noyau de Gaussien : " << meilleursParametresSobel[1] << std::endl;
    // std::cout << "Performance : " << resultatsMeilleursParametresSobel[0] << std::endl;
    // std::cout << "Taux de faux positifs : " << resultatsMeilleursParametresSobel[1] << std::endl;
    // std::cout << "Taux de faux negatifs : " << resultatsMeilleursParametresSobel[2] << "\n" << std::endl;

    // std::cout << "Meilleurs parametres pour Laplace : " << std::endl;
    // std::cout << "Taille du noyau Laplacien : " << meilleursParametresLaplace[0] << std::endl;
    // std::cout << "Taille du noyau de Gaussien : " << meilleursParametresLaplace[1] << std::endl;
    // std::cout << "Performance : " << resultatsMeilleursParametresLaplace[0] << std::endl;
    // std::cout << "Taux de faux positifs : " << resultatsMeilleursParametresLaplace[1] << std::endl;
    // std::cout << "Taux de faux negatifs : " << resultatsMeilleursParametresLaplace[2] << "\n" << std::endl;

    // std::cout << "Meilleurs parametres pour Canny : " << std::endl;
    // std::cout << "Seuil bas : " << meilleursParametresCanny[0] << std::endl;
    // std::cout << "Seuil haut : " << meilleursParametresCanny[1] << std::endl;
    // std::cout << "Taille du noyau de Gaussien : " << meilleursParametresCanny[2] << std::endl;
    // std::cout << "Performance : " << resultatsMeilleursParametresCanny[0] << std::endl;
    // std::cout << "Taux de faux positifs : " << resultatsMeilleursParametresCanny[1] << std::endl;
    // std::cout << "Taux de faux negatifs : " << resultatsMeilleursParametresCanny[2] << std::endl;

    // // Afficher la première image originale
    // AfficherImage("Image Originale", tableauOriginales[0]);

    // // Afficher la première image de contours
    // AfficherImage("Contours de référence", tableauContours[0]);

    // // Afficher le contour de la première image avec Sobel
    // AfficherImage("Contour Sobel", DetecteurContours::DetecterSobel(tableauOriginales[0], meilleursParametresSobel[0], meilleursParametresSobel[1]));

    // // Afficher le contour de la première image avec Laplace
    // AfficherImage("Contour Laplace", DetecteurContours::DetecterLaplace(tableauOriginales[0], meilleursParametresLaplace[0], meilleursParametresLaplace[1]));

    // // Afficher le contour de la première image avec Canny
    // AfficherImage("Contour Canny", DetecteurContours::DetecterCanny(tableauOriginales[0], meilleursParametresCanny[0], meilleursParametresCanny[1], meilleursParametresCanny[2]));


    // Tests sur un ensemble d'images
    // Variables pour stocker les meilleurs paramètres globaux et les moyennes
    int meilleursParametresGenerauxSobel[tableauOriginales.size()][2];
    double resultatsMeilleursParametresGenerauxSobel[tableauOriginales.size()][3];

    int meilleursParametresGenerauxLaplace[tableauOriginales.size()][2];
    double resultatsMeilleursParametresGenerauxLaplace[tableauOriginales.size()][3];

    int meilleursParametresGenerauxCanny[tableauOriginales.size()][3];
    double resultatsMeilleursParametresGenerauxCanny[tableauOriginales.size()][3];

    // Variables pour les moyennes
    double moyennesSobel[5] = {0};
    double moyennesLaplace[5] = {0};
    double moyennesCanny[6] = {0};

    // Trouver les meilleurs paramètres globaux pour Sobel
    MeilleursParametresGenerauxSobel(tableauOriginales, tableauContours, meilleursParametresGenerauxSobel, resultatsMeilleursParametresGenerauxSobel, moyennesSobel);

    // Trouver les meilleurs paramètres globaux pour Laplace
    MeilleursParametresGenerauxLaplace(tableauOriginales, tableauContours, meilleursParametresGenerauxLaplace, resultatsMeilleursParametresGenerauxLaplace, moyennesLaplace);

    // Trouver les meilleurs paramètres globaux pour Canny
    MeilleursParametresGenerauxCanny(tableauOriginales, tableauContours, meilleursParametresGenerauxCanny, resultatsMeilleursParametresGenerauxCanny, moyennesCanny);


    // Afficher les résultats
    std::cout << "Meilleurs parametres globaux pour Sobel : " << std::endl;
    std::cout << "Taille du noyau de Sobel : " << moyennesSobel[0] << std::endl;
    std::cout << "Taille du noyau de Gaussien : " << moyennesSobel[1] << std::endl;
    std::cout << "Performance moyenne : " << moyennesSobel[2] << std::endl;
    std::cout << "Taux de faux positifs moyen : " << moyennesSobel[3] << std::endl;
    std::cout << "Taux de faux negatifs moyen : " << moyennesSobel[4] << "\n" << std::endl;

    std::cout << "Meilleurs parametres globaux pour Laplace : " << std::endl;
    std::cout << "Taille du noyau de Laplace : " << moyennesLaplace[0] << std::endl;
    std::cout << "Taille du noyau de Gaussien : " << moyennesLaplace[1] << std::endl;
    std::cout << "Performance moyenne : " << moyennesLaplace[2] << std::endl;
    std::cout << "Taux de faux positifs moyen : " << moyennesLaplace[3] << std::endl;
    std::cout << "Taux de faux negatifs moyen : " << moyennesLaplace[4] << "\n" << std::endl;

    std::cout << "Meilleurs parametres globaux pour Canny : " << std::endl;
    std::cout << "Seuil bas : " << moyennesCanny[0] << std::endl;
    std::cout << "Seuil haut : " << moyennesCanny[1] << std::endl;
    std::cout << "Taille du noyau de Gaussien : " << moyennesCanny[2] << std::endl;
    std::cout << "Performance moyenne : " << moyennesCanny[3] << std::endl;
    std::cout << "Taux de faux positifs moyen : " << moyennesCanny[4] << std::endl;
    std::cout << "Taux de faux negatifs moyen : " << moyennesCanny[5] << std::endl;

    // Afficher la première image originale
    AfficherImage("Image Originale", tableauOriginales[0]);

    // Afficher la première image de contours
    AfficherImage("Contours de référence", tableauContours[0]);

    // Afficher le contour de la première image avec Sobel (avec les meilleurs paramètres globaux)
    AfficherImage("Contour Sobel", DetecteurContours::DetecterSobel(tableauOriginales[0], static_cast<int>(moyennesSobel[0]), static_cast<int>(moyennesSobel[1])));

    // Afficher le contour de la première image avec Laplace (avec les meilleurs paramètres globaux)
    AfficherImage("Contour Laplace", DetecteurContours::DetecterLaplace(tableauOriginales[0], static_cast<int>(moyennesLaplace[0]), static_cast<int>(moyennesLaplace[1])));

    // Afficher le contour de la première image avec Canny (avec les meilleurs paramètres globaux)
    AfficherImage("Contour Canny", DetecteurContours::DetecterCanny(tableauOriginales[0], static_cast<int>(moyennesCanny[0]), static_cast<int>(moyennesCanny[1]), static_cast<int>(moyennesCanny[2])));

    // Attendre une touche pour fermer les fenêtres
    cv::waitKey(0);

    return 0;
}