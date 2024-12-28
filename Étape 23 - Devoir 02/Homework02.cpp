#include "Histogramme.hpp"

int main() 
{
    // Déclaration des variables
    cv::Mat imageOriginale;
    cv::Mat imageEtendue;
    cv::Mat histogrammeImageOriginale;
    cv::Mat histogrammeImageEtendue;

    // Créer un objet Histogramme
    Histogramme h;

    // Charger l'image
    imageOriginale = cv::imread("AI_totoro.png", cv::IMREAD_GRAYSCALE);
    if (imageOriginale.empty()) 
    {
        return 0;
    }

    // Étendre le contraste de l'image
    imageEtendue = h.etendreContraste(imageOriginale);


    // Générer les histogrammes
    histogrammeImageOriginale = h.genererImageHistogramme(imageOriginale, 2);
    histogrammeImageEtendue = h.genererImageHistogramme(imageEtendue, 2);

    // Afficher les images
    cv::imshow("Image originale", imageOriginale);
    cv::imshow("Image Etendue", imageEtendue);

    // Afficher les histogrammes
    cv::imshow("Histogramme Image Originale", histogrammeImageOriginale);
    cv::imshow("Histogramme Image Etendue", histogrammeImageEtendue);

    // Enregistrer l'image étendue
    cv::imwrite("AI_totoro_etendu.png", imageEtendue);

    cv::waitKey(0);

    return 0;
}