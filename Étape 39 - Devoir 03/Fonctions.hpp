#ifndef FONCTIONS_HPP
#define FONCTIONS_HPP

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

// Fonction pour importer une image
// nomFichier : nom du fichier à importer
// image : image importée
void ImporterImage(const std::string& nomFichier, cv::Mat& image);

// Fonction pour créer un tableau d'images à partir d'un dossier
// tableau : tableau d'images
// nomDossier : nom du dossier contenant les images
void CreerTableauImages(std::vector<cv::Mat>& tableau, const std::string& nomDossier);

// Fonction pour afficher une image
// nomFenetre : nom de la fenêtre d'affichage
// image : image à afficher
void AfficherImage(const std::string& nomFenetre, const cv::Mat& image);

#endif // FONCTIONS_HPP
