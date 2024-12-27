#include "Histogram.hpp"
#include "ContentFinder.hpp"

using namespace std;

int main()
{
    // Charge l'image
    cv::Mat image = cv::imread("baboon01.jpg");
    if (!image.data) 
    {
        return 0; 
    }

    // Définition d'une région d'intérêt (ROI)
    cv::Rect rect(110, 45, 35, 45); // Délimite une ROI de 35x45 à partir du point (110, 45)
    cv::rectangle(image, rect, cv::Scalar(0, 0, 255)); // Dessine un rectangle rouge autour de la ROI

    // Extrait la ROI de l'image
    cv::Mat imageROI = image(rect);

    // Affiche l'image originale avec le rectangle rouge
    cv::imshow("Image 1", image);

    // Définition d'une saturation minimale pour filtrer les pixels faibles
    int minSat = 65;

    // Calcule l'histogramme de la teinte (Hue) pour la ROI
    ColorHistogram hc; // Crée un objet ColorHistogram
    cv::Mat colorhist = hc.getHueHistogram(imageROI, minSat); // Calcule l'histogramme en filtrant les pixels avec une saturation < 65

    // Configure le ContentFinder avec l'histogramme calculé
    ContentFinder finder;
    finder.setHistogram(colorhist); // Définit l'histogramme cible
    finder.setThreshold(0.2f); // Définit un seuil de 20 %

    // Conversion de l'image originale en espace HSV (Teinte, Saturation, Valeur)
    cv::Mat hsv;
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);

    // Sépare les canaux HSV pour appliquer un masque basé sur la saturation
    vector<cv::Mat> v;
    cv::split(hsv, v); // Sépare les canaux Hue, Saturation et Value

    // Applique un seuil à la saturation pour conserver uniquement les pixels suffisamment saturés
    cv::threshold(v[1], v[1], minSat, 255, cv::THRESH_BINARY);

    // Charge une seconde image pour le suivi (frame suivante ou scène suivante)
    image = cv::imread("baboon02.jpg");

    // Convertit la seconde image en espace HSV
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);

    // Définit les canaux à analyser pour la recherche
    int ch[1] = {0}; // Canal 0 (Teinte)

    // Désactive le seuil pour la rétroprojection
    finder.setThreshold(-1.0f);

    // Applique la rétroprojection pour trouver les zones correspondant à l'histogramme dans la seconde image
    cv::Mat result = finder.find(hsv, 0.0f, 180.0f, ch); // Recherche dans la plage complète de teintes [0, 180]

    // Dessine le rectangle rouge autour de la position initiale sur la seconde image
    cv::rectangle(image, rect, cv::Scalar(0, 0, 255));

    // Configure les critères pour l'algorithme de Mean Shift
    cv::TermCriteria criteria(cv::TermCriteria::MAX_ITER | cv::TermCriteria::EPS, 10, 1); // Maximum 10 itérations ou convergence avec une précision de 1

    // Applique l'algorithme de Mean Shift pour ajuster la position du rectangle
    cout << "Mean shift = " << cv::meanShift(result, rect, criteria) << endl;

    // Dessine le rectangle vert autour de la position ajustée après Mean Shift
    cv::rectangle(image, rect, cv::Scalar(0, 255, 0));

    // Affiche la seconde image avec les rectangles (rouge = initial, vert = ajusté)
    cv::imshow("Image 2", image);


    cv::waitKey(0);

    return 0;
}