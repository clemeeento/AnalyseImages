#ifndef COLORDETECTION_HPP
#define COLORDETECTION_HPP

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// Classe pour effectuer une détection de couleur sur une image
class ColorDetector
{
private:
    int maxDist;               // Distance maximale pour considérer une couleur comme proche de la couleur cible
    cv::Vec3b target;          // Couleur cible
    cv::Mat converted;         // Image convertie dans un autre espace de couleurs (si nécessaire)
    bool useLab;               // Indique si l'espace de couleurs Lab est utilisé
    cv::Mat result;            // Matrice contenant le résultat du traitement

public:
    // Constructeur par défaut : initialise avec des valeurs standards
    ColorDetector() : maxDist(100), target(0, 0, 0), useLab(false) {}

    // Constructeur avec l'option d'utiliser l'espace Lab
    ColorDetector(bool useLab) : maxDist(100), target(0, 0, 0), useLab(useLab) {}

    // Constructeur avec initialisation des paramètres
    ColorDetector(uchar blue, uchar green, uchar red, float maxDist = 100.0, bool useLab = false) 
        : maxDist(maxDist), useLab(useLab)
    {
        setTargetColor(blue, green, red); // Définit la couleur cible
    }

    // Calcule la distance entre une couleur et la couleur cible
    int getDistanceToTergetColor(const cv::Vec3b &color) const
    {
        return getColorDistance(target, color); // Appelle la méthode générique
    }

    // Calcule la distance entre deux couleurs
    int getColorDistance(const cv::Vec3b &color1, const cv::Vec3b &color2) const
    {
        // Somme des distances absolues des composantes RGB
        return abs(color1[0] - color2[0]) +
               abs(color1[1] - color2[1]) +
               abs(color1[2] - color2[2]);
    }

    // Applique la détection de couleur sur une image
    cv::Mat process(const cv::Mat &image)
    {
        // Crée une matrice pour le résultat
        cv::Mat result(image.size(), CV_8U);

        // Convertit l'image dans l'espace Lab si nécessaire
        if(useLab)
        {
            cv::cvtColor(image, converted, cv::COLOR_BGR2Lab);
        }

        // Itérateurs pour parcourir les pixels de l'image
        cv::Mat_<cv::Vec3b>::const_iterator it = image.begin<cv::Vec3b>();
        cv::Mat_<cv::Vec3b>::const_iterator itend = image.end<cv::Vec3b>();
        cv::Mat_<uchar>::iterator itout = result.begin<uchar>();

        if(useLab)
        {
            it = converted.begin<cv::Vec3b>();
            itend = converted.end<cv::Vec3b>();
        }

        // Parcourt chaque pixel et applique la détection
        for(; it != itend; ++it, ++itout)
        {
            if(getDistanceToTergetColor(*it) < maxDist)
            {
                *itout = 255; // Marque les pixels proches de la couleur cible en blanc
            }
            else
            {
                *itout = 0;   // Marque les autres pixels en noir
            }
        }

        return result; // Renvoie l'image binaire résultat
    }

    // (Méthode à utiliser si vous surchargez operator() dans le futur)
    cv::Mat Operator(const cv::Mat &image)
    {
        cv::Mat input;

        // Convertit l'image en Lab si nécessaire
        if(useLab)
        {
            cv::cvtColor(image, converted, cv::COLOR_BGR2Lab);
        }
        else
        {
            input = image;
        }

        // Calcule la distance en valeurs absolues pour chaque pixel
        cv::Mat output;
        cv::absdiff(input, cv::Scalar(target), output);

        // Sépare les canaux pour effectuer une addition des distances
        std::vector<cv::Mat> images;
        cv::split(output, images);

        // Additionne les distances entre canaux et applique un seuil
        output = images[0] + images[1] + images[2];
        cv::threshold(output, output, maxDist, 255, cv::THRESH_BINARY_INV);

        return output; // Renvoie l'image seuilée
    }

    // Définit la distance maximale pour la détection
    void setColorDistanceThreshold(int distance)
    {
        if(distance < 0)
        {
            distance = 0; // Assure que la distance est toujours positive
        }

        maxDist = distance;
    }

    // Récupère la distance maximale pour la détection
    void getColorDistanceThreshold(int &distance) const
    {
        distance = maxDist;
    }

    // Définit la couleur cible à l'aide de ses composantes RGB
    void setTargetColor(uchar blue, uchar green, uchar red)
    {
        target = cv::Vec3b(blue, green, red); // Définit la couleur cible

        if(useLab)
        {
            // Convertit la couleur cible en Lab si nécessaire
            cv::Mat tmp(1, 1, CV_8UC3);
            tmp.at<cv::Vec3b>(0, 0) = cv::Vec3b(blue, green, red);
            cv::cvtColor(tmp, tmp, cv::COLOR_BGR2Lab);
            target = tmp.at<cv::Vec3b>(0, 0);
        }
    }

    // Définit la couleur cible directement avec un vecteur
    void setTargetColor(cv::Vec3b color)
    {
        target = color;
    }

    // Récupère la couleur cible
    cv::Vec3b getTargetColor() const
    {
        return target;
    }
};

#endif // COLORDETECTION_HPP