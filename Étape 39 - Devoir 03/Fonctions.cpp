#include "Fonctions.hpp"

// Fonction pour importer une image
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
void CreerTableauImages(std::vector<cv::Mat>& tableau, const std::string& nomDossier)
{
    cv::String chemin(nomDossier); // Convertir le nom du dossier en cv::String
    std::vector<cv::String> nomsFichiers; // Vecteur pour stocker les noms des fichiers
    cv::glob(chemin, nomsFichiers); // Lire les noms des fichiers dans le dossier
    cv::Mat image; // Image à importer

    // Boucle pour importer les images
    for(size_t i = 0; i < nomsFichiers.size(); ++i)
    {
        ImporterImage(nomsFichiers[i], image); // Importer l'image
        tableau.push_back(image); // Ajouter l'image au tableau
    }
}

// Fonction pour afficher une image
void AfficherImage(const std::string& nomFenetre, const cv::Mat& image)
{
    // Afficher l'image avec OpenCV
    cv::imshow(nomFenetre, image);
}
