#include "EvaluateurContours.hpp"

// Constructeur
EvaluateurContours::EvaluateurContours()
{
    contoursDetectes = 0;
    contoursReference = 0;
    contoursCorrects = 0;
    fauxPositifs = 0;
    fauxNegatifs = 0;
}

// Fonction pour comparer deux images de contours (image test et image de référence)
void EvaluateurContours::ComparerContours(const cv::Mat &imageContours, const cv::Mat &imageReference)
{
    // Vérifier que les images sont bien en niveaux de gris et de même taille
    if (imageContours.size() != imageReference.size() || imageContours.channels() != 1 || imageReference.channels() != 1)
    {
        std::cerr << "Erreur : les images doivent être en niveaux de gris et de même taille." << std::endl;
        return;
    }

    // Compter le nombre total de pixels contours détectés
    contoursDetectes = cv::countNonZero(imageContours);

    // Compter le nombre total de pixels contours dans l'image de référence
    contoursReference = cv::countNonZero(imageReference);

    // Création d'une copie dilatée de l'image de référence pour prendre en compte le voisinage 3x3
    cv::Mat referenceDilatee;
    cv::dilate(imageReference, referenceDilatee, cv::Mat::ones(3, 3, CV_8U));

    // Trouver les pixels contours correctement détectés en tenant compte du voisinage
    cv::Mat intersection;
    cv::bitwise_and(imageContours, referenceDilatee, intersection);
    contoursCorrects = cv::countNonZero(intersection);

    // Calculer les faux positifs (pixels détectés comme contours mais absents dans la référence)
    fauxPositifs = contoursDetectes - contoursCorrects;

    // Calculer les faux négatifs (pixels contours manquants dans l'image détectée)
    fauxNegatifs = contoursReference - contoursCorrects;
}

// Fonction pour calculer la performance (P)
double EvaluateurContours::CalculerPerformance()
{
    int totalPixels = contoursCorrects + fauxPositifs + fauxNegatifs;
    if (totalPixels == 0)
    {
        return 0.0;
    }
    return static_cast<double>(contoursCorrects) / totalPixels;
}

// Fonction pour calculer le taux de faux positifs (TFP)
double EvaluateurContours::CalculerTauxFauxPositifs()
{
    int totalPixels = contoursCorrects + fauxPositifs + fauxNegatifs;
    if (totalPixels == 0)
    {
        return 0.0;
    }
    return static_cast<double>(fauxPositifs) / totalPixels;
}

// Fonction pour calculer le taux de faux négatifs (TFN)
double EvaluateurContours::CalculerTauxFauxNegatifs()
{
    int totalPixels = contoursCorrects + fauxPositifs + fauxNegatifs;
    if (totalPixels == 0)
    {
        return 0.0;
    }
    return static_cast<double>(fauxNegatifs) / totalPixels;
}

// Fonction pour afficher les résultats
void EvaluateurContours::AfficherResultats()
{
    std::cout << "Contours détectés     : " << contoursDetectes << std::endl;
    std::cout << "Contours référence    : " << contoursReference << std::endl;
    std::cout << "Contours corrects     : " << contoursCorrects << std::endl;
    std::cout << "Faux positifs         : " << fauxPositifs << std::endl;
    std::cout << "Faux négatifs         : " << fauxNegatifs << std::endl;
    std::cout << "Performance (P)       : " << CalculerPerformance() << std::endl;
    std::cout << "Taux de faux positifs : " << CalculerTauxFauxPositifs() << std::endl;
    std::cout << "Taux de faux négatifs : " << CalculerTauxFauxNegatifs() << std::endl;
}

