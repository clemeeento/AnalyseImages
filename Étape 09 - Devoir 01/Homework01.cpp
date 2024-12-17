#include <opencv2/opencv.hpp>
#include <iostream>

// Variables globales
cv::Mat imageOriginale; // Image originale
cv::Mat imageAffichee; // Image affichée avec la ligne

// Dessine la ligne rouge dans l'image
void dessinerLigne(int y)
{
    // Réinitialise l'image affichée
    imageAffichee = imageOriginale.clone();

    // Convertit l'image en niveaux de gris vers BGR pour permettre les couleurs
    cv::cvtColor(imageOriginale, imageAffichee, cv::COLOR_GRAY2BGR);

    // Dessine la ligne rouge
    cv::line(imageAffichee, cv::Point(0, y), cv::Point(imageAffichee.cols - 1, y), cv::Scalar(0, 0, 255), 1);

    // Affiche l'image avec la ligne
    cv::imshow("Image avec ligne", imageAffichee); 
}

void afficherProfilIntensite(int y)
{
    // Intensité maximale possible d'un pixel
    int intensiteMax = 255;

    // Largeur de la fenêtre du profil, pour avoir le même nombre de pixels que l'image
    int largeur = imageOriginale.cols; 
    // Hauteur de la fenêtre du profil
    int hauteur = 300;

    // Crée une image noire pour le profil d'intensité
    cv::Mat profilImage = cv::Mat::zeros(hauteur, largeur, CV_8UC3);
    
    // Crée un vecteur contenant les intensités de l'image à la ligne y
    int profilIntensite[largeur];
    for (int i = 0; i < imageOriginale.cols; i= i + 1)
    {
        profilIntensite[i] = imageOriginale.at<uchar>(y, i);
    }

    // Dessine le profil dans la fenêtre
    for (int i = 0; i < largeur - 1; i= i + 1)
    {   
        // Calcule les coordonnées des deux points
        int y1 = hauteur - (profilIntensite[i] * hauteur / intensiteMax);
        int y2 = hauteur - (profilIntensite[i + 1] * hauteur / intensiteMax);

        // Dessine une ligne entre les deux points
        cv::line(profilImage, cv::Point(i, y1), cv::Point(i + 1, y2), cv::Scalar(255, 255, 255), 1);
    }

    // Affiche la fenêtre du profil d'intensité
    cv::imshow("Profil d'intensite", profilImage);
}

// Fonction de callback pour la souris
void clicSouris(int evenement, int x, int y, int flags, void* donnees)
{
    if (evenement == cv::EVENT_LBUTTONDOWN) // Clic gauche
    {
        // Affiche l'image avec la ligne
        dessinerLigne(y);

        // Affiche le profil d'intensité
        afficherProfilIntensite(y);
    }
}

int main()
{
    // Importe l'image en niveaux de gris
    imageOriginale = cv::imread("tower.jpg", cv::IMREAD_GRAYSCALE);

    // Crée une copie pour affichage
    imageAffichee = imageOriginale.clone();

    // Affiche l'image
    cv::namedWindow("Image avec ligne", cv::WINDOW_AUTOSIZE);
    cv::setMouseCallback("Image avec ligne", clicSouris, nullptr);
    cv::imshow("Image avec ligne", imageAffichee);

    // Attend une touche
    cv::waitKey(0);

    return 0;
}
