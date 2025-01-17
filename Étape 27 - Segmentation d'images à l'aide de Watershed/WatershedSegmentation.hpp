#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// Classe pour effectuer une segmentation par watershed
class WatershedSegmentation
{
private: 
    cv::Mat markers; // Matrice contenant les marqueurs pour la segmentation

public:
    // Méthode pour définir les marqueurs
    void setMarkers(const cv::Mat &markerImage)
    {
        // Convertit l'image des marqueurs en une matrice 32 bits signés (CV_32S),
        // format requis par l'algorithme watershed
        markerImage.convertTo(markers, CV_32S);
    }

    // Méthode pour appliquer l'algorithme watershed
    cv::Mat process(const cv::Mat &image)
    {
        // Applique l'algorithme watershed à l'image en utilisant les marqueurs
        cv::watershed(image, markers);
        return markers; // Retourne les marqueurs mis à jour après segmentation
    }

    // Méthode pour obtenir l'image segmentée (régions)
    cv::Mat getSegmentation()
    {
        cv::Mat tmp; 
        // Convertit les marqueurs en une image 8 bits non signée (CV_8U)
        markers.convertTo(tmp, CV_8U);
        return tmp; // Retourne l'image segmentée
    }

    // Méthode pour obtenir les lignes de partage des eaux
    cv::Mat getWatersheds()
    {
        cv::Mat tmp;
        // Convertit les marqueurs en une image 8 bits (CV_8U),
        // avec un facteur d'échelle pour rendre les lignes de partage visibles
        markers.convertTo(tmp, CV_8U, 255, 255);
        return tmp; // Retourne l'image des lignes de partage des eaux
    }
};
