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

    std::vector<std::string> nomsImages; // Tableau pour les noms des images
    std::vector<std::string> nomsLignesSobel = {"Taille Noyau Sobel", "Taille Noyau Gaussien", "Performance", "Taux Faux Positifs", "Taux Faux Negatifs"}; // Noms des lignes pour le fichier CSV
    std::vector<std::string> nomsLignesLaplace = {"Taille Noyau Laplace", "Taille Noyau Gaussien", "Performance", "Taux Faux Positifs", "Taux Faux Negatifs"}; // Noms des lignes pour le fichier CSV
    std::vector<std::string> nomsLignesCanny = {"Seuil Bas", "Seuil Haut", "Taille Noyau Gaussien", "Performance", "Taux Faux Positifs", "Taux Faux Negatifs"}; // Noms des lignes pour le fichier CSV

    // Récupérer les noms des images
    RecupererNomsFichiers(nomsImages, "Originales");

    // Créer les tableaux d'images
    CreerTableauImages(tableauOriginales, "Originales");
    CreerTableauImages(tableauContours, "Contours");

    // Variables pour les meilleurs paramètres et résultats
    int meilleursParametresGenerauxSobel[tableauOriginales.size()][2];
    double resultatsMeilleursParametresGenerauxSobel[tableauOriginales.size()][3];

    int meilleursParametresGenerauxLaplace[tableauOriginales.size()][2];
    double resultatsMeilleursParametresGenerauxLaplace[tableauOriginales.size()][3];

    int meilleursParametresGenerauxCanny[tableauOriginales.size()][3];
    double resultatsMeilleursParametresGenerauxCanny[tableauOriginales.size()][3];

    double moyennesSobel[5];
    double moyennesLaplace[5];
    double moyennesCanny[6];

    // Trouver les meilleurs paramètres globaux pour Sobel
    MeilleursParametresGenerauxSobel(tableauOriginales, tableauContours, meilleursParametresGenerauxSobel, resultatsMeilleursParametresGenerauxSobel, moyennesSobel);

    // Trouver les meilleurs paramètres globaux pour Laplace
    MeilleursParametresGenerauxLaplace(tableauOriginales, tableauContours, meilleursParametresGenerauxLaplace, resultatsMeilleursParametresGenerauxLaplace, moyennesLaplace);

    // Trouver les meilleurs paramètres globaux pour Canny
    MeilleursParametresGenerauxCanny(tableauOriginales, tableauContours, meilleursParametresGenerauxCanny, resultatsMeilleursParametresGenerauxCanny, moyennesCanny);

    // Exporter les résultats des meilleurs paramètres globaux pour Sobel dans un fichier CSV
    ExportCSV("MeilleursParametresSobel.csv", meilleursParametresGenerauxSobel, resultatsMeilleursParametresGenerauxSobel, nomsLignesSobel, nomsImages, tableauOriginales.size());

    // Exporter les résultats des meilleurs paramètres globaux pour Laplace dans un fichier CSV
    ExportCSV("MeilleursParametresLaplace.csv", meilleursParametresGenerauxLaplace, resultatsMeilleursParametresGenerauxLaplace, nomsLignesLaplace, nomsImages, tableauOriginales.size());

    // Exporter les résultats des meilleurs paramètres globaux pour Canny dans un fichier CSV
    ExportCSVCanny("MeilleursParametresCanny.csv", meilleursParametresGenerauxCanny, resultatsMeilleursParametresGenerauxCanny, nomsLignesCanny, nomsImages, tableauOriginales.size());

    return 0;
}
