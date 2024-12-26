#include "ColorDetection.hpp"

int main()
{
    // Crée une instance de la classe ColorDetector
    ColorDetector cDetect;

    // Importe l'image
    cv::Mat image = cv::imread("boldt.jpg");

    // Vérifie si l'image a été correctement chargée
    if(image.empty())
    {
        return 0;
    }

    // Affiche l'image originale dans une fenêtre
    cv::namedWindow("Image Originale");
    cv::imshow("Image Originale", image);

    // Définit la couleur cible pour la détection
    cDetect.setTargetColor(230, 190, 130);

    // Applique la détection de couleur et récupère l'image résultat
    cv::Mat result = cDetect.process(image);

    // Affiche le résultat de la détection de couleur dans une fenêtre
    cv::namedWindow("Resultat");
    cv::imshow("Resultat", result);

    // Effectue une opération de remplissage par diffusion (Flood Fill) sur l'image
    cv::floodFill(image, cv::Point(100, 50), cv::Scalar(255, 255, 255), (cv::Rect*)0, cv::Scalar(35, 35, 35), cv::Scalar(35, 35, 35), cv::FLOODFILL_FIXED_RANGE);

    // Réutilise la méthode `process` pour afficher l'image après Flood Fill
    result = cDetect.process(image);

    // Affiche le résultat de l'opération Flood Fill dans une fenêtre
    cv::namedWindow("Resultat FloodFill");
    cv::imshow("Resultat FloodFill", result);

    // Crée une matrice de couleurs de démonstration
    cv::Mat colors(100, 300, CV_8UC3, cv::Scalar(100, 200, 150));
    
    // Modifie une plage de couleurs dans la matrice
    cv::Mat range = colors.colRange(0, 100); // Sélectionne une plage de couleurs
    range = range + cv::Scalar(10, 10, 10); // Augmente les valeurs des pixels
    range = colors.colRange(200, 300); // Sélectionne une autre plage de couleurs
    range = range + cv::Scalar(-10, -10, 10); // Diminue les valeurs des pixels

    // Affiche la matrice de couleurs dans une fenêtre
    cv::namedWindow("Colors");
    cv::imshow("Colors", colors);

    cv::waitKey(0);

    return 0;
}