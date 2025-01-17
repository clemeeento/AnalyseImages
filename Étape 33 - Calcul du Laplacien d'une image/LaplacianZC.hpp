#include <iostream>
#include <vector>
#include <iomanip>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>


class LaplacianZC
{
private:
    cv::Mat laplace; // Image du Laplacien calculé
    int aperture;    // Taille de l'ouverture du noyau Laplacien (doit être impair)

public:

    // Constructeur par défaut, initialisant l'ouverture à 3
    LaplacianZC() : aperture(3) {}

    // Définit la taille de l'ouverture (noyau) pour le calcul du Laplacien
    void setAperture(int a)
    {
        aperture = a;
    }

    // Récupère la taille de l'ouverture actuelle
    int getAperture() const
    {
        return aperture;
    }

    // Calcule le Laplacien de l'image d'entrée et le stocke
    cv::Mat computeLaplacian(const cv::Mat& image)
    {
        cv::Laplacian(image, laplace, CV_32F, aperture); // Calcul du Laplacien en précision flottante
        return laplace; // Retourne l'image Laplacien calculée
    }

    // Convertit le Laplacien en une image affichable
    cv::Mat getLaplacianImage(double scale = -1.0)
    {
        if(scale < 0) // Si l'échelle n'est pas définie
        {
            double lapmin, lapmax;
            cv::minMaxLoc(laplace, &lapmin, &lapmax); // Trouve les valeurs minimale et maximale du Laplacien
            scale = 127 / std::max(-lapmin, lapmax); // Définit une échelle pour adapter les valeurs au format 8 bits
        }

        cv::Mat laplaceImage;
        laplace.convertTo(laplaceImage, CV_8U, scale, 128); // Convertit les valeurs en 8 bits pour affichage
        return laplaceImage;
    }

    // Détecte les points de zéro-croisement dans le Laplacien
    cv::Mat getZeroCrossings(cv::Mat laplace)
    {
        cv::Mat signImage;
        cv::threshold(laplace, signImage, 0, 255, cv::THRESH_BINARY); // Convertit le Laplacien en une image binaire pour détecter les signes

        cv::Mat binary;
        signImage.convertTo(binary, CV_8U); // Conversion en format 8 bits pour traitement

        cv::Mat dilated;
        cv::dilate(binary, dilated, cv::Mat()); // Dilate les bords pour identifier les changements de signe

        return dilated - binary; // Les différences entre l'image dilatée et l'image originale représentent les zéro-croisements
    }
};
