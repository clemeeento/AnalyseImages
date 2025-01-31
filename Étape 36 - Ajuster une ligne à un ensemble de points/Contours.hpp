#include <iostream>
#include <vector>
#include <iomanip>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>

// Classe pour détecter les contours d'une image à l'aide du filtre de Sobel
class EdgeDetector
{
private:
    cv::Mat sobelX, sobelY;         // Matrices pour stocker les gradients Sobel en X et Y
    cv::Mat sobelMagnitude;         // Matrice pour stocker la magnitude du gradient
    cv::Mat sobelImage;             // Image finale de Sobel normalisée
    cv::Mat sobelDirection;        // Matrice pour stocker l'orientation des gradients
    cv::Mat sobelNormalized;       // Matrice pour stocker l'orientation des gradients normalisée

public:
    // Constructeur par défaut
    EdgeDetector() {}

    // Méthode pour calculer le gradient de Sobel sur une image en niveaux de gris
    void computeSobel(const cv::Mat &image)
    {
        // Calcul des dérivées en 16 bits pour éviter la saturation des valeurs
        cv::Sobel(image, sobelX, CV_16S, 1, 0); // Sobel X en 16 bits
        cv::Sobel(image, sobelY, CV_16S, 0, 1); // Sobel Y en 16 bits

        // Combine les gradients horizontaux et verticaux pour obtenir la magnitude du gradient
        sobelMagnitude = abs(sobelX) + abs(sobelY);

        // Normalisation pour l'affichage : mise à l'échelle entre 0 et 255
        double sobMin, sobMax;
        cv::minMaxLoc(sobelMagnitude, &sobMin, &sobMax); // Trouve les valeurs min/max
        sobelMagnitude.convertTo(sobelImage, CV_8U, -255.0 / sobMax, 255); // Convertit en image affichable

        // Calcule les gradients en virgule flottante pour obtenir l'orientation des gradients
        cv::Sobel(image, sobelX, CV_32F, 1,0);
        cv::Sobel(image, sobelY, CV_32F, 0,1);

        // Convertit les gradients en coordonnées polaires (magnitude et direction)
        cv::cartToPolar(sobelX, sobelY, sobelNormalized, sobelDirection);
    }

    // Retourne l'image du gradient Sobel normalisée
    cv::Mat getSobelImage() const
    {
        return sobelImage;
    }

    // Retourne l'image de magnitude Sobel brute
    cv::Mat getSobelMagnitude() const
    {
        return sobelMagnitude;
    }

    // Génère une image binaire des contours en appliquant un seuillage
    cv::Mat getBinaryMap(int seuil) const
    {
        cv::Mat binaryMap;
        cv::threshold(sobelImage, binaryMap, seuil, 255, cv::THRESH_BINARY); // Seuillage binaire
        return binaryMap;
    }

    // Calcule et retourne l'image de l'orientation des gradients de Sobel
    cv::Mat getSobelOrientationImage() const
    {
        return sobelDirection;
    }
};