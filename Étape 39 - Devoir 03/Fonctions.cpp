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

// Fonction pour extraire le nom du fichier sans l'extension
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

// Fonction pour récupérer les noms de fichiers d'un dossier sans extension
void RecupererNomsFichiers(std::vector<std::string>& nomsImages, const std::string& nomDossier)
{
    cv::String chemin(nomDossier);  // Convertir le nom du dossier en cv::String pour OpenCV
    std::vector<cv::String> nomsFichiers; // Stocker les noms des fichiers du dossier
    cv::glob(chemin, nomsFichiers); // Lire les fichiers présents

    nomsImages.clear(); // Nettoyer le vecteur avant ajout

    // Parcours des fichiers pour extraire leur nom sans extension
    for (const auto& fichier : nomsFichiers)
    {
        size_t dernierSlash = fichier.find_last_of("/\\"); // Trouver le dernier '/' ou '\'
        size_t dernierPoint = fichier.find_last_of(".");   // Trouver le dernier '.'

        std::string nomSansExtension;
        if (dernierPoint == std::string::npos || dernierPoint < dernierSlash)
        {
            // Si aucun point trouvé ou qu'il est avant le dernier slash, garder tout le nom
            nomSansExtension = fichier.substr(dernierSlash + 1);
        }
        else
        {
            // Extraire uniquement le nom sans l'extension
            nomSansExtension = fichier.substr(dernierSlash + 1, dernierPoint - dernierSlash - 1);
        }

        nomsImages.push_back(nomSansExtension); // Ajouter au tableau
    }
}



// Fonction pour afficher une image
void AfficherImage(const std::string& nomFenetre, const cv::Mat& image)
{
    // Afficher l'image avec OpenCV
    cv::imshow(nomFenetre, image);
}

void ExportCSV(const std::string &nomFichier,
               int meilleursParametresGeneraux[][2], // Tableau de paramètres (ex: tailleNoyau, tailleNoyauGaussien)
               double resultatsMeilleursParametresGeneraux[][3], // Tableau des résultats (ex: P, TFP, TFN)
               const std::vector<std::string> &nomsLignes, // Tableau contenant les noms des lignes
               const std::vector<std::string> &nomsImages, // Tableau contenant les noms des images
               int nombreImages) // Nombre d'images dans le tableau
{
    // Création du fichier avant ouverture (écrase s'il existe)
    std::ofstream fichier(nomFichier, std::ios::out);

    // Vérifier si le fichier est bien ouvert
    if (!fichier)
    {
        std::cerr << "Erreur : Impossible de creer le fichier " << nomFichier << std::endl;
        return;
    }

    // Écriture de la première ligne (nom des images)
    fichier << ";"; // Première case vide
    for (int i = 0; i < nombreImages; i = i + 1)
    {
        fichier << nomsImages[i] << ";";
    }
    fichier << "\n";

    // Écriture des meilleurs paramètres généraux (ex: tailleNoyau, tailleNoyauGaussien)
    for (int i = 0; i < 2; i = i + 1) // Nombre de paramètres (ex: 2 = tailleNoyau, tailleNoyauGaussien)
    {
        fichier << nomsLignes[i] << ";";
        for (int j = 0; j < nombreImages; j = j + 1)
        {
            fichier << meilleursParametresGeneraux[j][i] << ";";
        }
        fichier << "\n";
    }

    // Écriture des résultats des meilleurs paramètres (ex: Performance, TFP, TFN)
    for (int i = 0; i < 3; i = i + 1) // Nombre de résultats (ex: 3 = P, TFP, TFN)
    {
        fichier << nomsLignes[i + 2] << ";"; // Décalage pour accéder aux bons noms
        for (int j = 0; j < nombreImages; j = j + 1)
        {
            fichier << resultatsMeilleursParametresGeneraux[j][i] << ";";
        }
        fichier << "\n";
    }

    // Fermer le fichier
    fichier.close();
    std::cout << "Fichier CSV genere avec succes : " << nomFichier << std::endl;
}

void ExportCSVCanny(const std::string &nomFichier,
               int meilleursParametresGeneraux[][3], // Tableau de paramètres (ex: tailleNoyau, tailleNoyauGaussien)
               double resultatsMeilleursParametresGeneraux[][3], // Tableau des résultats (ex: P, TFP, TFN)
               const std::vector<std::string> &nomsLignes, // Tableau contenant les noms des lignes
               const std::vector<std::string> &nomsImages, // Tableau contenant les noms des images
               int nombreImages) // Nombre d'images dans le tableau
{
    // Création du fichier avant ouverture (écrase s'il existe)
    std::ofstream fichier(nomFichier, std::ios::out);

    // Vérifier si le fichier est bien ouvert
    if (!fichier)
    {
        std::cerr << "Erreur : Impossible de creer le fichier " << nomFichier << std::endl;
        return;
    }

    // Écriture de la première ligne (nom des images)
    fichier << ";"; // Première case vide
    for (int i = 0; i < nombreImages; i = i + 1)
    {
        fichier << nomsImages[i] << ";";
    }
    fichier << "\n";

    // Écriture des meilleurs paramètres généraux (ex: tailleNoyau, tailleNoyauGaussien)
    for (int i = 0; i < 3; i = i + 1) // Nombre de paramètres (ex: 2 = tailleNoyau, tailleNoyauGaussien)
    {
        fichier << nomsLignes[i] << ";";
        for (int j = 0; j < nombreImages; j = j + 1)
        {
            fichier << meilleursParametresGeneraux[j][i] << ";";
        }
        fichier << "\n";
    }

    // Écriture des résultats des meilleurs paramètres (ex: Performance, TFP, TFN)
    for (int i = 0; i < 3; i = i + 1) // Nombre de résultats (ex: 3 = P, TFP, TFN)
    {
        fichier << nomsLignes[i + 3] << ";"; // Décalage pour accéder aux bons noms
        for (int j = 0; j < nombreImages; j = j + 1)
        {
            fichier << resultatsMeilleursParametresGeneraux[j][i] << ";";
        }
        fichier << "\n";
    }

    // Fermer le fichier
    fichier.close();
    std::cout << "Fichier CSV genere avec succes : " << nomFichier << std::endl;
}

// Fonction pour exporter les moyennes dans un fichier CSV en colonnes avec espaces entre les méthodes
void ExportCSVMoyenne(const std::string &nomFichier,
                      const double moyennesSobel[5],
                      const double moyennesLaplace[5],
                      const double moyennesCanny[6],
                      const std::vector<std::string> &nomsLignesSobel,
                      const std::vector<std::string> &nomsLignesLaplace,
                      const std::vector<std::string> &nomsLignesCanny)
{
    // Création du fichier CSV (écrase s'il existe déjà)
    std::ofstream fichier(nomFichier, std::ios::out);

    // Vérifier si le fichier est bien ouvert
    if (!fichier)
    {
        std::cerr << "Erreur : Impossible de créer le fichier " << nomFichier << std::endl;
        return;
    }

    // Écriture de l'en-tête (nom des méthodes)
    fichier << "Description;Sobel;;Description;Laplace;;Description;Canny\n";

    // Écriture des lignes avec espace entre chaque méthode
    for (size_t i = 0; i < 5; i = i + 1)
    {
        fichier << nomsLignesSobel[i] << ";" << moyennesSobel[i] << ";;"  // Sobel et colonne vide
                << nomsLignesLaplace[i] << ";" << moyennesLaplace[i] << ";;" // Laplace et colonne vide
                << nomsLignesCanny[i] << ";" << moyennesCanny[i] << "\n"; // Canny
    }

    // Écriture de la dernière ligne spécifique à Canny (qui a une valeur supplémentaire)
    fichier << ";;;;;;" << nomsLignesCanny[5] << ";" << moyennesCanny[5] << "\n";

    // Fermeture du fichier
    fichier.close();
    std::cout << "Fichier CSV genere avec succes : " << nomFichier << std::endl;
}
