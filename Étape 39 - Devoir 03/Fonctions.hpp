#ifndef FONCTIONS_HPP
#define FONCTIONS_HPP

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <opencv2/opencv.hpp>

// Fonction pour importer une image
// nomFichier : nom du fichier à importer
// image : image importée
void ImporterImage(const std::string& nomFichier, cv::Mat& image);

// Fonction pour récupérer les noms de fichiers d'un dossier sans extension
// nomsImages : noms des images
// nomDossier : nom du dossier contenant les images
void RecupererNomsFichiers(std::vector<std::string>& nomsImages, const std::string& nomDossier);

// Fonction pour créer un tableau d'images à partir d'un dossier
// tableau : tableau d'images
// nomDossier : nom du dossier contenant les images
void CreerTableauImages(std::vector<cv::Mat>& tableau, const std::string& nomDossier);

// Fonction pour afficher une image
// nomFenetre : nom de la fenêtre d'affichage
// image : image à afficher
void AfficherImage(const std::string& nomFenetre, const cv::Mat& image);

// Fonction pour exporter les résultats dans un fichier CSV
// nomFichier : nom du fichier CSV
// meilleursParametresGeneraux : tableau des meilleurs paramètres
// resultatsMeilleursParametresGeneraux : tableau des résultats
// nomsLignes : noms des lignes du fichier CSV
// nomsImages : noms des images
// nombreImages : nombre d'images dans le tableau
void ExportCSV(const std::string& nomFichier,
               int meilleursParametresGeneraux[][2],
               double resultatsMeilleursParametresGeneraux[][3],
                const std::vector<std::string> &nomsLignes,
               const std::vector<std::string> &nomsImages,
               int nombreImages);

// Fonction pour exporter les résultats dans un fichier CSV pour Canny
// nomFichier : nom du fichier CSV
// meilleursParametresGeneraux : tableau des meilleurs paramètres
// resultatsMeilleursParametresGeneraux : tableau des résultats
// nomsLignes : noms des lignes du fichier CSV
// nomsImages : noms des images
// nombreImages : nombre d'images dans le tableau
void ExportCSVCanny(const std::string& nomFichier,
               int meilleursParametresGeneraux[][3],
               double resultatsMeilleursParametresGeneraux[][3],
                const std::vector<std::string> &nomsLignes,
               const std::vector<std::string> &nomsImages,
               int nombreImages);

// Fonction pour exporter les moyennes dans un fichier CSV en colonnes avec espaces entre les méthodes
// nomFichier : nom du fichier CSV
// moyennesSobel : moyennes des résultats pour Sobel
// moyennesLaplace : moyennes des résultats pour Laplace
// moyennesCanny : moyennes des résultats pour Canny
// nomsLignesSobel : noms des lignes pour Sobel
// nomsLignesLaplace : noms des lignes pour Laplace
// nomsLignesCanny : noms des lignes pour Canny
void ExportCSVMoyenne(const std::string& nomFichier,
                      const double moyennesSobel[5],
                      const double moyennesLaplace[5],
                      const double moyennesCanny[6],
                      const std::vector<std::string>& nomsLignesSobel,
                      const std::vector<std::string>& nomsLignesLaplace,
                      const std::vector<std::string>& nomsLignesCanny);

#endif // FONCTIONS_HPP
