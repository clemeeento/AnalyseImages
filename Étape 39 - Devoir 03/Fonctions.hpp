#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>



// Fonction pour importer une image
// nomFichier : nom du fichier à importer
// image : image importée
void ImporterImage(const std::string& nomFichier, cv::Mat& image)
{
    // Importer l'image
    image = cv::imread(nomFichier, 0);
    // Vérifier si l'image a été importée
    if(image.empty())
    {
        std::cerr << "Impossible de charger l'image " << nomFichier << std::endl;
        exit(0);
    }
}

// Fonction pour créer un tableau d'images à partir d'un dossier
// tableau : tableau d'images
// nomDossier : nom du dossier contenant les images
void CreerTableauImages(std::vector<cv::Mat>& tableau, const std::string& nomDossier)
{
    cv::String chemin(nomDossier); // Convertir le nom du dossier en cv::String
    std::vector<cv::String> nomsFichiers; // Vecteur pour stocker les noms des fichiers
    cv::glob(chemin, nomsFichiers); // Lire les noms des fichiers dans le dossier
    cv::Mat image; // Image à importer

    for(size_t i = 0; i < nomsFichiers.size(); ++i)
    {
        ImporterImage(nomsFichiers[i], image); // Importer l'image
        tableau.push_back(image); // Ajouter l'image au tableau
    }
}



// Fonction pour afficher une image
// nomFenetre : nom de la fenêtre d'affichage
// image : image à afficher
void AfficherImage(const std::string& nomFenetre, const cv::Mat& image)
{
    // Afficher l'image
    cv::imshow(nomFenetre, image);
}

