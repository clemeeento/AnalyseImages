#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// Template pour calculer une image intégrale avec des paramètres personnalisés
template<typename T, int N> 

class IntegralImage
{
private:
    cv::Mat integralImage; // Stockage de l'image intégrale
    cv::Mat image;         // Image source utilisée pour le calcul de l'image intégrale

public:
    // Constructeur par défaut
    IntegralImage() {}

    // Constructeur prenant une image source
    IntegralImage(const cv::Mat& inputImage) 
    {
        image = inputImage.clone(); // Crée une copie de l'image source pour éviter toute modification externe
        cv::integral(image, integralImage, cv::DataType<int>::type); // Calcule l'image intégrale
        // L'image intégrale permet de calculer rapidement des sommes sur des zones rectangulaires
    }

    // Surcharge de l'opérateur () pour calculer la somme sur une région rectangulaire
    cv::Vec<T, N> operator()(int xo, int yo, int width, int height)
    {
        // Calcule la somme des pixels dans une région rectangulaire donnée
        return (integralImage.at<cv::Vec<T, N>>(yo + height, xo + width) 
              - integralImage.at<cv::Vec<T, N>>(yo + height, xo) 
              - integralImage.at<cv::Vec<T, N>>(yo, xo + width) 
              + integralImage.at<cv::Vec<T, N>>(yo, xo));
        // Utilise les propriétés de l'image intégrale pour calculer la somme efficacement
    }

    // Surcharge de l'opérateur () pour calculer la somme sur une région circulaire
    cv::Vec<T, N> operator()(int x, int y, int radius)
    {
        // Calcule la somme des pixels dans un cercle défini par son centre (x, y) et un rayon
        return (integralImage.at<cv::Vec<T, N>>(y + radius + 1, x + radius + 1) 
              - integralImage.at<cv::Vec<T, N>>(y + radius + 1, x - radius) 
              - integralImage.at<cv::Vec<T, N>>(y - radius, x + radius + 1) 
              + integralImage.at<cv::Vec<T, N>>(y - radius, x - radius));
        // Similaire à la méthode précédente mais adaptée pour une zone circulaire
    }
};

// Fonction pour convertir une image en plans binaires
void convertToBinaryPlanes(const cv::Mat& input, cv::Mat& output, int nPlanes)
{
    // Calcule le nombre de bits à conserver pour les plans binaires
    int n = 8 - static_cast<int>(log(static_cast<double>(nPlanes)) / log(2.0));
    uchar mask = 0xFF << n; // Masque binaire pour réduire la précision

    std::vector<cv::Mat> planes; // Vecteur pour stocker les différents plans binaires
    cv::Mat reduced = input & mask; // Réduit les valeurs de l'image en appliquant le masque

    // Crée les nPlans plans binaires à partir de l'image réduite
    for (int i = 0; i < nPlanes; i++)
    {
        planes.push_back((reduced == (i << n)) & 0x1); 
        // Compare les pixels à un seuil décalé et garde uniquement les bits correspondants
    }

    // Combine les plans binaires en une seule image de sortie
    cv::merge(planes, output);
}