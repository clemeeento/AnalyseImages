#include "ContentFinder.hpp"

int main()
{
    // Détection basée sur des niveaux de gris ------------
    // Charge une image en niveaux de gris
    cv::Mat image = cv::imread("waves.jpg", 0); 
    if (!image.data) 
    {
        return 0; 
    }

    // Définition d'une région d'intérêt (ROI) dans l'image
    cv::Mat imageROI;
    imageROI = image(cv::Rect(216, 33, 24, 30)); // Sélectionne une ROI de 24x30 pixels à partir du point (216, 33)

    // Calcule l'histogramme de la ROI
    Histogram1D h; // Crée un objet Histogram1D pour gérer les histogrammes
    cv::Mat hist = h.getHistogram(imageROI); // Calcule l'histogramme de la ROI

    // Configure un ContentFinder pour utiliser l'histogramme calculé
    ContentFinder finder;
    finder.setHistogram(hist); // Définit l'histogramme cible
    finder.setThreshold(-1.0f); // Désactive le seuil (aucune filtration basée sur la probabilité)

    // Applique la rétroprojection pour trouver les zones correspondant à l'histogramme dans l'image complète
    cv::Mat result1;
    result1 = finder.find(image); // Rétroprojection de l'image avec l'histogramme cible

    // Convertit le résultat en une image affichable
    cv::Mat tmp;
    result1.convertTo(tmp, CV_8U, -1.0, 255.0); // Échelle des valeurs pour l'affichage
    cv::imshow("Backprojection result", tmp); // Affiche la rétroprojection brute

    // Applique un seuil pour filtrer les résultats de la rétroprojection
    finder.setThreshold(0.12f); // Définit un seuil de 12 %
    result1 = finder.find(image); // Recalcule la rétroprojection avec le seuil

    // Dessine un rectangle noir autour de la ROI d'origine dans l'image
    cv::rectangle(image, cv::Rect(216, 33, 24, 30), cv::Scalar(0, 0, 0));

    // Affiche l'image originale avec le rectangle
    cv::imshow("Image", image);

    // Affiche les résultats de la détection après l'application du seuil
    cv::imshow("Detection result", result1);

    // Détection basée sur des couleurs -------------------
    // Crée un objet ColorHistogram pour analyser les canaux couleur
    ColorHistogram hc;
    cv::Mat color = cv::imread("waves.jpg"); // Charge l'image "waves.jpg" en couleur

    // Définit une nouvelle ROI dans l'image couleur
    imageROI = color(cv::Rect(0, 0, 100, 45)); // Sélectionne une ROI de 100x45 pixels à partir du coin supérieur gauche

    // Calcule un histogramme pour la ROI en utilisant 8 bins par canal
    hc.setSize(8); // Définit 8 bins pour chaque canal (B, G, R)
    cv::Mat shist = hc.getHistogram3D(imageROI); // Calcule l'histogramme couleur de la ROI

    // Configure le ContentFinder pour utiliser cet histogramme couleur
    finder.setHistogram(shist); // Définit l'histogramme cible (couleur)
    finder.setThreshold(0.05f); // Définit un seuil de 5 %

    // Applique la rétroprojection à l'image couleur
    result1 = finder.find(color); // Rétroprojection basée sur l'histogramme couleur

    // Affiche les résultats de la détection basée sur les couleurs
    cv::imshow("Color Detection result", result1);

    // Attend qu'une touche soit pressée avant de quitter
    cv::waitKey();

    return 0;
}