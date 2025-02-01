#ifndef EVALUATEURCONTOURS_HPP
#define EVALUATEURCONTOURS_HPP

#include <iostream>
#include <opencv2/opencv.hpp>

#include "DetecteurContours.hpp"

class EvaluateurContours
{
private:
    int contoursDetectes;     // Nombre total de pixels contours détectés
    int contoursReference;    // Nombre total de pixels contours dans l'image de référence
    int contoursCorrects;     // Nombre de pixels contours correctement détectés
    int fauxPositifs;         // Nombre de pixels détectés comme contours alors qu'ils ne le sont pas
    int fauxNegatifs;         // Nombre de pixels contours non détectés alors qu'ils le sont dans la référence

public:
    // Constructeur
    EvaluateurContours();

    // Fonction pour comparer deux images de contours (image test et image de référence)
    void ComparerContours(const cv::Mat &imageContours, const cv::Mat &imageReference);

    // Fonction pour calculer la performance (P)
    double CalculerPerformance();

    // Fonction pour calculer le taux de faux positifs (TFP)
    double CalculerTauxFauxPositifs();

    // Fonction pour calculer le taux de faux négatifs (TFN)
    double CalculerTauxFauxNegatifs();

    // Fonction pour afficher les résultats
    void AfficherResultats();
};

#endif // EVALUATEURCONTOURS_HPP
