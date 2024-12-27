#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "Histogram.hpp"

// Classe ImageComparator : compare deux images en utilisant des histogrammes
class ImageComparator
{
private:
    cv::Mat refH;      // Histogramme 3D de l'image de référence
    cv::Mat inputH;    // Histogramme 3D de l'image à comparer
    ColorHistogram hist; // Objet pour calculer les histogrammes couleur
    int nBins;         // Nombre de bins pour l'histogramme (résolution)

public:
    // Constructeur par défaut : initialise avec 8 bins
    ImageComparator() : nBins(8) {}

    // Définit le nombre de bins pour les histogrammes
    void setNumberOfBins(int bins)
    {
        nBins = bins; // Met à jour la résolution de l'histogramme
    }

    // Récupère le nombre de bins actuellement configuré
    int getNumberOfBins() const
    {
        return nBins;
    }

    // Définit l'image de référence pour la comparaison
    void setReferenceImage(const cv::Mat &image)
    {
        hist.setSize(nBins); // Configure le nombre de bins pour l'histogramme
        refH = hist.getHistogram3D(image); // Calcule l'histogramme 3D de l'image de référence
    }

    // Compare l'image d'entrée à l'image de référence
    double compare(const cv::Mat &image)
    {
        inputH = hist.getHistogram3D(image); // Calcule l'histogramme 3D de l'image à comparer

        // Compare les deux histogrammes en utilisant la méthode d'intersection
        return cv::compareHist(refH, inputH, cv::HISTCMP_INTERSECT);
    }
};