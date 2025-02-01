#ifndef MEILLEURSPARAMETRES_HPP
#define MEILLEURSPARAMETRES_HPP

#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>

#include "EvaluateurContours.hpp"
#include "DetecteurContours.hpp"

// Fonction pour trouver les meilleurs paramètres pour la détection de contours avec Sobel sur une image
// imageOriginale : image originale en niveaux de gris
// imageContours : image des contours de référence
// meilleursParametres : tableau pour stocker les meilleurs paramètres (tailleNoyauSobel, tailleNoyauGaussien)
// resultatMeillieursParametres : tableau pour stocker les résultats des meilleurs paramètres (performance, tauxFauxPositifs, tauxFauxNegatifs)
void MeilleursParametresSobel(const cv::Mat &imageOriginale, const cv::Mat &imageContours, int meilleursParametres[2], double resultatMeillieursParametres[3]);

// Fonction pour trouver les meilleurs paramètres pour la détection de contours avec Laplace sur une image
// imageOriginale : image originale en niveaux de gris
// imageContours : image des contours de référence
// meilleursParametres : tableau pour stocker les meilleurs paramètres (tailleNoyauLaplace, tailleNoyauGaussien)
// resultatMeillieursParametres : tableau pour stocker les résultats des meilleurs paramètres (performance, tauxFauxPositifs, tauxFauxNegatifs)
void MeilleursParametresLaplace(const cv::Mat &imageOriginale, const cv::Mat &imageContours, int meilleursParametres[2], double resultatMeillieursParametres[3]);

// Fonction pour trouver les meilleurs paramètres pour la détection de contours avec Canny sur une image
// imageOriginale : image originale en niveaux de gris
// imageContours : image des contours de référence
// meilleursParametres : tableau pour stocker les meilleurs paramètres (seuilBas, seuilHaut, tailleNoyauGaussien)
// resultatMeillieursParametres : tableau pour stocker les résultats des meilleurs paramètres (performance, tauxFauxPositifs, tauxFauxNegatifs)
void MeilleursParametresCanny(const cv::Mat &imageOriginale, const cv::Mat &imageContours, int meilleursParametres[3], double resultatMeillieursParametres[3]);


// Fonction pour trouver les meilleurs paramètres globaux pour Sobel sur un ensemble d'images
// tableauOriginales : tableau d'images originales en niveaux de gris
// tableauContours : tableau d'images des contours de référence
// meilleursParametresGeneraux : tableau pour stocker les meilleurs paramètres globaux (tailleNoyauSobel, tailleNoyauGaussien)
// resultatsMeilleursParametresGeneraux : tableau pour stocker les résultats des meilleurs paramètres globaux (performance, tauxFauxPositifs, tauxFauxNegatifs)
// moyennesSobel : tableau pour stocker les moyennes des performances, taux de faux positifs et taux de faux négatifs
void MeilleursParametresGenerauxSobel(const std::vector<cv::Mat> &tableauOriginales,
                                      const std::vector<cv::Mat> &tableauContours,
                                      int meilleursParametresGeneraux[][2],
                                      double resultatsMeilleursParametresGeneraux[][3],
                                      double moyennesSobel[5]);

// Fonction pour trouver les meilleurs paramètres globaux pour Laplace sur un ensemble d'images
// tableauOriginales : tableau d'images originales en niveaux de gris
// tableauContours : tableau d'images des contours de référence
// meilleursParametresGeneraux : tableau pour stocker les meilleurs paramètres globaux (tailleNoyauLaplace, tailleNoyauGaussien)
// resultatsMeilleursParametresGeneraux : tableau pour stocker les résultats des meilleurs paramètres globaux (performance, tauxFauxPositifs, tauxFauxNegatifs)
// moyennesLaplace : tableau pour stocker les moyennes des performances, taux de faux positifs et taux de faux négatifs
void MeilleursParametresGenerauxLaplace(const std::vector<cv::Mat> &tableauOriginales,
                                        const std::vector<cv::Mat> &tableauContours,
                                        int meilleursParametresGeneraux[][2],
                                        double resultatsMeilleursParametresGeneraux[][3],
                                        double moyennesLaplace[5]);

// Fonction pour trouver les meilleurs paramètres globaux pour Canny sur un ensemble d'images
// tableauOriginales : tableau d'images originales en niveaux de gris
// tableauContours : tableau d'images des contours de référence
// meilleursParametresGeneraux : tableau pour stocker les meilleurs paramètres globaux (seuilBas, seuilHaut, tailleNoyauGaussien)
// resultatsMeilleursParametresGeneraux : tableau pour stocker les résultats des meilleurs paramètres globaux (performance, tauxFauxPositifs, tauxFauxNegatifs)
// moyennesCanny : tableau pour stocker les moyennes des performances, taux de faux positifs et taux de faux négatifs
void MeilleursParametresGenerauxCanny(const std::vector<cv::Mat> &tableauOriginales,
                                      const std::vector<cv::Mat> &tableauContours,
                                      int meilleursParametresGeneraux[][3],
                                      double resultatsMeilleursParametresGeneraux[][3],
                                      double moyennesCanny[6]);


#endif // MEILLEURSPARAMETRES_HPP

