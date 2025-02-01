#ifndef DETECTEURCONTOURS_HPP
#define DETECTEURCONTOURS_HPP

#include <iostream>
#include <opencv2/opencv.hpp>

class DetecteurContours
{
public:
    // Détection des contours avec Sobel
    // image : image en niveaux de gris
    // tailleNoyau : taille du noyau de Sobel (3 par défaut), doit être impair, et generalement 1, 3, 5 ou 7
    // tailleNoyauGaussien : taille du noyau de Gaussien (5 par défaut), doit être impair, plage recommandée : 1 à 31 (en restant généralement dans 3 à 11).
    static cv::Mat DetecterSobel(const cv::Mat &image, int tailleNoyauSobel = 3, int tailleNoyauGaussien = 5);

    // Détection des contours avec Laplace
    // image : image en niveaux de gris
    // tailleNoyauLaplace : taille du noyau Laplacien (3 par défaut), doit être impair, et generalement 1, 3, 5 ou 7
    // tailleNoyauGaussien : taille du noyau de Gaussien (5 par défaut), doit être impair, plage recommandée : 1 à 31 (en restant généralement dans 3 à 11).
    static cv::Mat DetecterLaplace(const cv::Mat &image, int tailleNoyauLaplace = 3, int tailleNoyauGaussien = 5);

    // Détection des contours avec Canny
    // image : image en niveaux de gris
    // seuilBas et seuilHaut : doivent être dans la plage de 0 à 255 (Car l'image est en 8 bits)
    // seuilBas : seuil bas pour la détection des contours (100 par défaut), doit être positif et inférieur à seuilHaut
    // seuilHaut : seuil haut pour la détection des contours (200 par défaut), doit être au moins 1,5 à 2 fois supérieur à seuilBas pour de bons résultats.
    // tailleNoyauGaussien : taille du noyau de Gaussien (5 par défaut), doit être impair, plage recommandée : 1 à 31 (en restant généralement dans 3 à 11).
    static cv::Mat DetecterCanny(const cv::Mat &image, int seuilBas = 100, int seuilHaut = 200, int tailleNoyauGaussien = 5);
};

#endif // DETECTEURCONTOURS_HPP
