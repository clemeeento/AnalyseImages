#include "DetecteurContours.hpp"

// Détection des contours avec Sobel
cv::Mat DetecteurContours::DetecterSobel(const cv::Mat &image, int tailleNoyauSobel, int tailleNoyauGaussien)
{
    // Vérifier si l'image est valide
    if (image.empty())
    {
        std::cerr << "Erreur : Image introuvable !" << std::endl;
        return cv::Mat();
    }

    // Appliquer un flou gaussien pour réduire le bruit
    cv::Mat imageFloue;
    cv::GaussianBlur(image, imageFloue, cv::Size(tailleNoyauGaussien, tailleNoyauGaussien), 1.0);

    // Calcul des gradients de Sobel en X et Y
    cv::Mat gradientX, gradientY, gradientTotal;
    cv::Sobel(imageFloue, gradientX, CV_32F, 1, 0, tailleNoyauSobel);
    cv::Sobel(imageFloue, gradientY, CV_32F, 0, 1, tailleNoyauSobel);

    // Magnitude des gradients pour obtenir les contours
    cv::Mat magnitude;
    cv::magnitude(gradientX, gradientY, magnitude);

    // Conversion en format affichable
    cv::Mat contours;
    magnitude.convertTo(contours, CV_8U);

    // Seuillage pour obtenir uniquement les contours
    cv::threshold(contours, contours, 50, 255, cv::THRESH_BINARY_INV);

    return contours;
}

// Détection des contours avec Laplace
cv::Mat DetecteurContours::DetecterLaplace(const cv::Mat &image, int tailleNoyauLaplace, int tailleNoyauGaussien)
{
    // Vérifier si l'image est valide
    if (image.empty())
    {
        std::cerr << "Erreur : Image introuvable !" << std::endl;
        return cv::Mat();
    }

    // Appliquer un flou gaussien pour réduire le bruit
    cv::Mat imageFloue;
    cv::GaussianBlur(image, imageFloue, cv::Size(tailleNoyauGaussien, tailleNoyauGaussien), 1.0);

    // Appliquer le filtre Laplacien
    cv::Mat laplace;
    cv::Laplacian(imageFloue, laplace, CV_16S, tailleNoyauLaplace);

    // Conversion en format affichable
    cv::Mat contours;
    laplace.convertTo(contours, CV_8U);

    // Seuillage pour obtenir uniquement les contours
    cv::threshold(contours, contours, 50, 255, cv::THRESH_BINARY_INV);

    return contours;
}

// Détection des contours avec Canny
cv::Mat DetecteurContours::DetecterCanny(const cv::Mat &image, int seuilBas, int seuilHaut, int tailleNoyauGaussien)
{
    // Vérifier si l'image est valide
    if (image.empty())
    {
        std::cerr << "Erreur : Image introuvable !" << std::endl;
        return cv::Mat();
    }

    // Appliquer un flou gaussien pour réduire le bruit
    cv::Mat imageFloue;
    cv::GaussianBlur(image, imageFloue, cv::Size(tailleNoyauGaussien, tailleNoyauGaussien), 1.0);

    // Appliquer le filtre de Canny
    cv::Mat contours;
    cv::Canny(imageFloue, contours, seuilBas, seuilHaut);

    // Inverser les couleurs
    contours = 255 - contours;

    return contours;
}