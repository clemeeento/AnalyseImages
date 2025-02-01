#include "MeilleursParametres.hpp"

void MeilleursParametresSobel(const cv::Mat &imageOriginale, const cv::Mat &imageContours, int meilleursParametres[2], double resultatsMeilleursParametres[3])
{
    // Vérification si les images sont valides
    if (imageOriginale.empty() || imageContours.empty())
    {
        std::cerr << "Erreur : une des images est vide !" << std::endl;
        return;
    }

    // Création de l'objet EvaluateurContours
    EvaluateurContours evaluateur;

    // Déclaration des variables
    double meilleurePerformance = 0.0;
    double performance;
    double tauxFauxPositifs;
    double tauxFauxNegatifs;
    cv::Mat contoursDetectes;

    // Parcours des différentes tailles de noyau pour Sobel
    for (int tailleNoyauSobel : {1, 3, 5, 7})
    {
        // Parcours des différentes tailles de noyau pour le flou gaussien
        for (int tailleNoyauGaussien : {1, 3, 5, 7, 9, 11})
        {
            // Appliquer la détection de contours avec Sobel
            contoursDetectes = DetecteurContours::DetecterSobel(imageOriginale, tailleNoyauSobel, tailleNoyauGaussien);

            // Comparer les contours détectés avec la référence
            evaluateur.ComparerContours(contoursDetectes, imageContours);

            // Récupérer les métriques de performance
            performance = evaluateur.CalculerPerformance();
            tauxFauxPositifs = evaluateur.CalculerTauxFauxPositifs();
            tauxFauxNegatifs = evaluateur.CalculerTauxFauxNegatifs();

            // Mise à jour des meilleurs paramètres si une meilleure performance est trouvée
            if (performance > meilleurePerformance)
            {
                meilleurePerformance = performance;
                meilleursParametres[0] = tailleNoyauSobel;
                meilleursParametres[1] = tailleNoyauGaussien;
                resultatsMeilleursParametres[0] = performance;
                resultatsMeilleursParametres[1] = tauxFauxPositifs;
                resultatsMeilleursParametres[2] = tauxFauxNegatifs;
            }
        }
    }
}

void MeilleursParametresLaplace(const cv::Mat &imageOriginale, const cv::Mat &imageContours, int meilleursParametres[2], double resultatsMeilleursParametres[3])
{
    // Vérification si les images sont valides
    if (imageOriginale.empty() || imageContours.empty())
    {
        std::cerr << "Erreur : une des images est vide !" << std::endl;
        return;
    }

    // Création de l'objet EvaluateurContours
    EvaluateurContours evaluateur;

    // Déclaration des variables
    double meilleurePerformance = 0.0;
    double performance;
    double tauxFauxPositifs;
    double tauxFauxNegatifs;
    cv::Mat contoursDetectes;

    // Parcours des différentes tailles de noyau pour Laplace
    for (int tailleNoyauLaplace : {1, 3, 5, 7})
    {
        // Parcours des différentes tailles de noyau pour le flou gaussien
        for (int tailleNoyauGaussien : {1, 3, 5, 7, 9, 11})
        {
            // Appliquer la détection de contours avec Laplace
            contoursDetectes = DetecteurContours::DetecterLaplace(imageOriginale, tailleNoyauLaplace, tailleNoyauGaussien);

            // Comparer les contours détectés avec la référence
            evaluateur.ComparerContours(contoursDetectes, imageContours);

            // Récupérer les métriques de performance
            performance = evaluateur.CalculerPerformance();
            tauxFauxPositifs = evaluateur.CalculerTauxFauxPositifs();
            tauxFauxNegatifs = evaluateur.CalculerTauxFauxNegatifs();

            // Mise à jour des meilleurs paramètres si une meilleure performance est trouvée
            if (performance > meilleurePerformance)
            {
                meilleurePerformance = performance;
                meilleursParametres[0] = tailleNoyauLaplace;
                meilleursParametres[1] = tailleNoyauGaussien;
                resultatsMeilleursParametres[0] = performance;
                resultatsMeilleursParametres[1] = tauxFauxPositifs;
                resultatsMeilleursParametres[2] = tauxFauxNegatifs;
            }
        }
    }
}

void MeilleursParametresCanny(const cv::Mat &imageOriginale, const cv::Mat &imageContours, int meilleursParametres[3], double resultatsMeilleursParametres[3])
{
    // Vérification si les images sont valides
    if (imageOriginale.empty() || imageContours.empty())
    {
        std::cerr << "Erreur : une des images est vide !" << std::endl;
        return;
    }

    // Création de l'objet EvaluateurContours
    EvaluateurContours evaluateur;

    // Déclaration des variables
    double meilleurePerformance = 0.0;
    double performance;
    double tauxFauxPositifs;
    double tauxFauxNegatifs;
    cv::Mat contoursDetectes;

    // Parcours des différentes seuils bas pour Canny
    for (int seuilBas = 0; seuilBas <= 225; seuilBas = seuilBas + 25)
    {
        // Parcours des différentes seuils haut pour Canny
        for (int seuilHaut = 25; seuilHaut <= 250; seuilHaut = seuilHaut + 25)
        {
            // Parcours des différentes tailles de noyau pour le flou gaussien
            for (int tailleNoyauGaussien : {1, 3, 5, 7, 9, 11})
            {
                // Appliquer la détection de contours avec Canny
                contoursDetectes = DetecteurContours::DetecterCanny(imageOriginale, seuilBas, seuilHaut, tailleNoyauGaussien);

                // Comparer les contours détectés avec la référence
                evaluateur.ComparerContours(contoursDetectes, imageContours);

                // Récupérer les métriques de performance
                performance = evaluateur.CalculerPerformance();
                tauxFauxPositifs = evaluateur.CalculerTauxFauxPositifs();
                tauxFauxNegatifs = evaluateur.CalculerTauxFauxNegatifs();

                // Mise à jour des meilleurs paramètres si une meilleure performance est trouvée
                if (performance > meilleurePerformance)
                {
                    meilleurePerformance = performance;
                    meilleursParametres[0] = seuilBas;
                    meilleursParametres[1] = seuilHaut;
                    meilleursParametres[2] = tailleNoyauGaussien;
                    resultatsMeilleursParametres[0] = performance;
                    resultatsMeilleursParametres[1] = tauxFauxPositifs;
                    resultatsMeilleursParametres[2] = tauxFauxNegatifs;
                }
            }
        }
    }
}


// Fonction pour trouver les meilleurs paramètres globaux pour Sobel sur un ensemble d'images
void MeilleursParametresGenerauxSobel(const std::vector<cv::Mat> &tableauOriginales,
                                      const std::vector<cv::Mat> &tableauContours,
                                      int meilleursParametresGeneraux[][2],
                                      double resultatsMeilleursParametresGeneraux[][3],
                                      double moyennesSobel[5])
{
    // Vérification que les tableaux ont la même taille
    if (tableauOriginales.size() != tableauContours.size() || tableauOriginales.empty())
    {
        std::cerr << "Erreur : les tableaux d'images doivent être de même taille et non vides !" << std::endl;
        return;
    }

    int nombreImages = tableauOriginales.size();

    // Variables pour accumuler les résultats globaux
    double sommeTailleNoyauSobel = 0;
    double sommeTailleNoyauGaussien = 0;
    double sommePerformance = 0;
    double sommeTauxFauxPositifs = 0;
    double sommeTauxFauxNegatifs = 0;
    int meilleursParametres[2];
    double resultatsMeilleursParametres[3];

    // Variables pour les calculs d'arrondi
    double valeur;
    double arrondi;

    // Parcours de chaque image pour trouver les meilleurs paramètres
    for (size_t i = 0; i < tableauOriginales.size(); i = i + 1)
    {
        meilleursParametres[0] = 0;
        meilleursParametres[1] = 0;
        resultatsMeilleursParametres[0] = 0.0;
        resultatsMeilleursParametres[1] = 0.0;
        resultatsMeilleursParametres[2] = 0.0;

        // Trouver les meilleurs paramètres pour l'image actuelle
        MeilleursParametresSobel(tableauOriginales[i], tableauContours[i], meilleursParametres, resultatsMeilleursParametres);

        // Stocker les résultats dans les tableaux passés en paramètre
        meilleursParametresGeneraux[i][0] = meilleursParametres[0];
        meilleursParametresGeneraux[i][1] = meilleursParametres[1];
        resultatsMeilleursParametresGeneraux[i][0] = resultatsMeilleursParametres[0];
        resultatsMeilleursParametresGeneraux[i][1] = resultatsMeilleursParametres[1];
        resultatsMeilleursParametresGeneraux[i][2] = resultatsMeilleursParametres[2];

        // Ajouter aux sommes pour le calcul des moyennes
        sommeTailleNoyauSobel = sommeTailleNoyauSobel + meilleursParametres[0];
        sommeTailleNoyauGaussien = sommeTailleNoyauGaussien + meilleursParametres[1];
        sommePerformance = sommePerformance + resultatsMeilleursParametres[0];
        sommeTauxFauxPositifs = sommeTauxFauxPositifs + resultatsMeilleursParametres[1];
        sommeTauxFauxNegatifs = sommeTauxFauxNegatifs + resultatsMeilleursParametres[2];
    }

    // Calcul des moyennes des paramètres et des résultats
    // Paramètres : tailleNoyauSobel, tailleNoyauGaussien
    // Pour la taille du noyau de Sobel, on arrondit à l'entier impair le plus proche
    valeur = sommeTailleNoyauSobel / nombreImages;
    arrondi = std::round(valeur);
    // Si l'arrondi est pair, on ajuste en fonction de la valeur originale
    if (std::fmod(arrondi, 2.0) == 0.0) 
    {
        if (arrondi > valeur) 
        {
            arrondi = arrondi - 1.0; // Prendre l'impair inférieur
        }
        else
        {
            arrondi = arrondi + 1.0; // Prendre l'impair supérieur
        }
    }
    moyennesSobel[0] = arrondi;

    // Pour la taille du noyau de Gaussien, on arrondit à l'entier impair le plus proche
    valeur = sommeTailleNoyauGaussien / nombreImages;
    arrondi = std::round(valeur);
    // Si l'arrondi est pair, on ajuste en fonction de la valeur originale
    if (std::fmod(arrondi, 2.0) == 0.0) 
    {
        if (arrondi > valeur) 
        {
            arrondi = arrondi - 1.0; // Prendre l'impair inférieur
        }
        else
        {
            arrondi = arrondi + 1.0; // Prendre l'impair supérieur
        }
    }
    moyennesSobel[1] = arrondi;

    // Résultats : performance, tauxFauxPositifs, tauxFauxNegatifs
    moyennesSobel[2] = sommePerformance / nombreImages;
    moyennesSobel[3] = sommeTauxFauxPositifs / nombreImages;
    moyennesSobel[4] = sommeTauxFauxNegatifs / nombreImages;
}

// Fonction pour trouver les meilleurs paramètres globaux pour Laplace sur un ensemble d'images
void MeilleursParametresGenerauxLaplace(const std::vector<cv::Mat> &tableauOriginales,
                                      const std::vector<cv::Mat> &tableauContours,
                                      int meilleursParametresGeneraux[][2],
                                      double resultatsMeilleursParametresGeneraux[][3],
                                      double moyennesLaplace[5])
{
    // Vérification que les tableaux ont la même taille
    if (tableauOriginales.size() != tableauContours.size() || tableauOriginales.empty())
    {
        std::cerr << "Erreur : les tableaux d'images doivent être de même taille et non vides !" << std::endl;
        return;
    }

    int nombreImages = tableauOriginales.size();

    // Variables pour accumuler les résultats globaux
    double sommeTailleNoyauLaplace = 0;
    double sommeTailleNoyauGaussien = 0;
    double sommePerformance = 0;
    double sommeTauxFauxPositifs = 0;
    double sommeTauxFauxNegatifs = 0;
    int meilleursParametres[2];
    double resultatsMeilleursParametres[3];

    // Variables pour les calculs d'arrondi
    double valeur;
    double arrondi;

    // Parcours de chaque image pour trouver les meilleurs paramètres
    for (size_t i = 0; i < tableauOriginales.size(); i = i+1)
    {
        meilleursParametres[0] = 0;
        meilleursParametres[1] = 0;
        resultatsMeilleursParametres[0] = 0.0;
        resultatsMeilleursParametres[1] = 0.0;
        resultatsMeilleursParametres[2] = 0.0;

        // Trouver les meilleurs paramètres pour l'image actuelle
        MeilleursParametresLaplace(tableauOriginales[i], tableauContours[i], meilleursParametres, resultatsMeilleursParametres);

        // Stocker les résultats dans les tableaux passés en paramètre
        meilleursParametresGeneraux[i][0] = meilleursParametres[0];
        meilleursParametresGeneraux[i][1] = meilleursParametres[1];
        resultatsMeilleursParametresGeneraux[i][0] = resultatsMeilleursParametres[0];
        resultatsMeilleursParametresGeneraux[i][1] = resultatsMeilleursParametres[1];
        resultatsMeilleursParametresGeneraux[i][2] = resultatsMeilleursParametres[2];

        // Ajouter aux sommes pour le calcul des moyennes
        sommeTailleNoyauLaplace = sommeTailleNoyauLaplace + meilleursParametres[0];
        sommeTailleNoyauGaussien = sommeTailleNoyauGaussien + meilleursParametres[1];
        sommePerformance = sommePerformance + resultatsMeilleursParametres[0];
        sommeTauxFauxPositifs = sommeTauxFauxPositifs + resultatsMeilleursParametres[1];
        sommeTauxFauxNegatifs = sommeTauxFauxNegatifs + resultatsMeilleursParametres[2];
    }

    // Calcul des moyennes des paramètres et des résultats
    // Paramètres : tailleNoyauLaplace, tailleNoyauGaussien
    // Pour la taille du noyau de Lapalce, on arrondit à l'entier impair le plus proche
    valeur = sommeTailleNoyauLaplace / nombreImages;
    arrondi = std::round(valeur);
    // Si l'arrondi est pair, on ajuste en fonction de la valeur originale
    if (std::fmod(arrondi, 2.0) == 0.0) 
    {
        if (arrondi > valeur) 
        {
            arrondi = arrondi - 1.0; // Prendre l'impair inférieur
        }
        else
        {
            arrondi = arrondi + 1.0; // Prendre l'impair supérieur
        }
    }
    moyennesLaplace[0] = arrondi;

    // Pour la taille du noyau de Gaussien, on arrondit à l'entier impair le plus proche
    valeur = sommeTailleNoyauGaussien / nombreImages;
    arrondi = std::round(valeur);
    // Si l'arrondi est pair, on ajuste en fonction de la valeur originale
    if (std::fmod(arrondi, 2.0) == 0.0) 
    {
        if (arrondi > valeur) 
        {
            arrondi = arrondi - 1.0; // Prendre l'impair inférieur
        }
        else
        {
            arrondi = arrondi + 1.0; // Prendre l'impair supérieur
        }
    }
    moyennesLaplace[1] = arrondi;

    // Résultats : performance, tauxFauxPositifs, tauxFauxNegatifs
    moyennesLaplace[2] = sommePerformance / nombreImages;
    moyennesLaplace[3] = sommeTauxFauxPositifs / nombreImages;
    moyennesLaplace[4] = sommeTauxFauxNegatifs / nombreImages;
}

// Fonction pour trouver les meilleurs paramètres globaux pour Canny sur un ensemble d'images
void MeilleursParametresGenerauxCanny(const std::vector<cv::Mat> &tableauOriginales,
                                      const std::vector<cv::Mat> &tableauContours,
                                      int meilleursParametresGeneraux[][3],
                                      double resultatsMeilleursParametresGeneraux[][3],
                                      double moyennesCanny[6])
{
    // Vérification que les tableaux ont la même taille
    if (tableauOriginales.size() != tableauContours.size() || tableauOriginales.empty())
    {
        std::cerr << "Erreur : les tableaux d'images doivent être de même taille et non vides !" << std::endl;
        return;
    }

    int nombreImages = tableauOriginales.size();

    // Variables pour accumuler les résultats globaux
    double sommeTailleSeuilBas = 0;
    double sommeTailleSeuilHaut = 0;
    double sommeTailleNoyauGaussien = 0;
    double sommePerformance = 0;
    double sommeTauxFauxPositifs = 0;
    double sommeTauxFauxNegatifs = 0;
    int meilleursParametres[3];
    double resultatsMeilleursParametres[3];

    // Variables pour les calculs d'arrondi
    double valeur;
    double arrondi;

    // Parcours de chaque image pour trouver les meilleurs paramètres
    for (size_t i = 0; i < tableauOriginales.size(); i = i+1)
    {
        meilleursParametres[0] = 0;
        meilleursParametres[1] = 0;
        meilleursParametres[2] = 0;
        resultatsMeilleursParametres[0] = 0.0;
        resultatsMeilleursParametres[1] = 0.0;
        resultatsMeilleursParametres[2] = 0.0;

        // Trouver les meilleurs paramètres pour l'image actuelle
        MeilleursParametresCanny(tableauOriginales[i], tableauContours[i], meilleursParametres, resultatsMeilleursParametres);

        // Stocker les résultats dans les tableaux passés en paramètre
        meilleursParametresGeneraux[i][0] = meilleursParametres[0];
        meilleursParametresGeneraux[i][1] = meilleursParametres[1];
        meilleursParametresGeneraux[i][2] = meilleursParametres[2];
        resultatsMeilleursParametresGeneraux[i][0] = resultatsMeilleursParametres[0];
        resultatsMeilleursParametresGeneraux[i][1] = resultatsMeilleursParametres[1];
        resultatsMeilleursParametresGeneraux[i][2] = resultatsMeilleursParametres[2];

        // Ajouter aux sommes pour le calcul des moyennes
        sommeTailleSeuilBas = sommeTailleSeuilBas + meilleursParametres[0];
        sommeTailleSeuilHaut = sommeTailleSeuilHaut + meilleursParametres[1];
        sommeTailleNoyauGaussien = sommeTailleNoyauGaussien + meilleursParametres[2];
        sommePerformance = sommePerformance + resultatsMeilleursParametres[0];
        sommeTauxFauxPositifs = sommeTauxFauxPositifs + resultatsMeilleursParametres[1];
        sommeTauxFauxNegatifs = sommeTauxFauxNegatifs + resultatsMeilleursParametres[2];
    }

    // Calcul des moyennes des paramètres et des résultats
    // Paramètres : seuilBas, seuilHaut, tailleNoyauGaussien
    moyennesCanny[0] = sommeTailleSeuilBas / nombreImages;
    moyennesCanny[1] = sommeTailleSeuilHaut / nombreImages;
    // Pour la taille du noyau de Gaussien, on arrondit à l'entier impair le plus proche
    valeur = sommeTailleNoyauGaussien / nombreImages;
    arrondi = std::round(valeur);
    // Si l'arrondi est pair, on ajuste en fonction de la valeur originale
    if (std::fmod(arrondi, 2.0) == 0.0) 
    {
        if (arrondi > valeur) 
        {
            arrondi = arrondi - 1.0; // Prendre l'impair inférieur
        }
        else
        {
            arrondi = arrondi + 1.0; // Prendre l'impair supérieur
        }
    }
    moyennesCanny[2] = arrondi;
    std::cout << "moyennesCanny[2] : " << moyennesCanny[2] << std::endl;

    // Résultats : performance, tauxFauxPositifs, tauxFauxNegatifs
    moyennesCanny[3] = sommePerformance / nombreImages;
    moyennesCanny[4] = sommeTauxFauxPositifs / nombreImages;
    moyennesCanny[5] = sommeTauxFauxNegatifs / nombreImages;
}