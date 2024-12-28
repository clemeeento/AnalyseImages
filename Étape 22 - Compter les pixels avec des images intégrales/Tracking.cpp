#include "Integral.hpp" // Inclusion de la classe ou fonctions liées à Integral
#include "Histogram.hpp" // Inclusion de la classe Histogram1D

using namespace std;

int main()
{
    // Charge une image
    cv::Mat image = cv::imread("bike55.bmp", 0); // 0 pour charger en niveaux de gris
    if (!image.data) 
    {
        return 0;
    }

    // Définition des coordonnées et dimensions de la région d'intérêt (ROI)
    int xo = 97, yo = 112; // Coordonnées du coin supérieur gauche
    int width = 25, height = 30; // Dimensions de la ROI
    cv::Mat roi(image, cv::Rect(xo, yo, width, height)); // Extraction de la ROI

    // Calcule la somme des pixels dans la ROI
    cv::Scalar sum = cv::sum(roi);

    // Calcul de l'image intégrale
    cv::Mat integralImage;
    cv::integral(image, integralImage, CV_32S); // Image intégrale avec des valeurs 32 bits entières

    // Calcule la somme des pixels dans la ROI en utilisant l'image intégrale
    int sumInt = integralImage.at<int>(yo + height, xo + width) 
               - integralImage.at<int>(yo + height, xo) 
               - integralImage.at<int>(yo, xo + width) 
               + integralImage.at<int>(yo, xo);

    // Initialisation de l'histogramme
    Histogram1D h;
    h.setNBins(16); // Définit 16 bins pour l'histogramme
    cv::Mat refHistogram = h.getHistogram(roi); // Histogramme de référence pour la ROI
    cv::Mat histogram = h.getHistogram(roi); // Histogramme temporaire pour la comparaison

    // Charge une seconde image pour effectuer la recherche
    cv::Mat secondImage = cv::imread("bike65.bmp", 0);
    if (!secondImage.data)
    {
        return 0;
    }

    // Conversion de l'image en plans binaires
    cv::Mat planes;
    convertToBinaryPlanes(secondImage, planes, 16); // Décompose l'image en 16 plans binaires
    IntegralImage<float, 16> intHistogram(planes); // Initialisation de l'image intégrale pour les histogrammes

    // Initialisation des variables pour la recherche
    double maxSimilarity = 0.0; // Meilleure similarité trouvée
    double distance = 0.0; // Distance calculée pour l'histogramme courant
    int xbest = 0, ybest = 0; // Coordonnées du meilleur match

    // Parcourt une partie de la deuxième image pour rechercher la meilleure correspondance
    for (int y = 110; y < 120; y++) // Parcourt les lignes
    {
        for (int x = 0; x < secondImage.cols - width; x++) // Parcourt les colonnes
        {   
            // Calcule l'histogramme pour la région courante dans la deuxième image
            histogram = intHistogram(x, y, width, height);

            // Compare l'histogramme courant avec l'histogramme de référence
            distance = cv::compareHist(refHistogram, histogram, cv::HISTCMP_INTERSECT);

            // Met à jour les coordonnées si une meilleure similarité est trouvée
            if (distance > maxSimilarity)
            {
                xbest = x; // Meilleure position en x
                ybest = y; // Meilleure position en y
                maxSimilarity = distance; // Met à jour la similarité maximale
            }
        }
    }

    // Dessine un rectangle autour de la ROI dans la première image
    cv::rectangle(image, cv::Rect(xo, yo, width, height), 0);

    // Dessine un rectangle autour de la meilleure correspondance trouvée dans la deuxième image
    cv::rectangle(secondImage, cv::Rect(xbest, ybest, width, height), 0);

    // Dessine un rectangle supplémentaire autour de la zone explorée
    cv::rectangle(secondImage, cv::Rect(0, 110, secondImage.cols, height + 10), 255);

    // Affiche les images avec les résultats
    cv::imshow("Original Image", image); // Image originale avec la ROI
    cv::imshow("Second Image", secondImage); // Image avec la correspondance trouvée

    cv::waitKey(0);

    return 0;
}