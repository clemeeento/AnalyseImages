#ifndef AFFICHAGE_HPP
#define AFFICHAGE_HPP

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

#include "Fonctions.hpp"
#include "EvaluateurContours.hpp"
#include "MeilleursParametres.hpp"
#include "DetecteurContours.hpp"

class Affichage
{
private :
    std::vector<cv::Mat> tableauOriginales; // Tableau pour les images originales
    std::vector<cv::Mat> tableauContours; // Tableau pour le contour des images

    std::vector<std::string> nomsImages; // Tableau pour les noms des images
    std::vector<std::string> nomsLignesSobel;
    std::vector<std::string> nomsLignesLaplace;
    std::vector<std::string> nomsLignesCanny;

    int indexImage; // Index de l'image actuelle

public:
    // Constructeur
    Affichage();

    // Fonctions pour initialiser les tableaux
    void InitialiserTableaux();

    // Fonction pour afficher les contours avec la méthode de Sobel
    void AfficherContoursSobel();

    // Fonction pour afficher les contours avec la méthode de Laplace
    void AfficherContoursLaplace();

    // Fonction pour afficher les contours avec la méthode de Canny
    void AfficherContoursCanny();

    // Fonction pour calculer les meilleurs paramètres des trois détecteurs de contours et les exporter dans un fichier CSV
    void CalculerMeilleursParametres();

    // Fonction pour afficher et gerer le menu
    void Menu();
};

#endif // AFFICHAGE_HPP