#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "Histogram.hpp"

// Classe ContentFinder : utilisée pour trouver des zones spécifiques dans une image
// en fonction d'un histogramme donné.
class ContentFinder
{
private:
    int channels[3];        // Canaux à analyser (B, G, R ou autres)
    float hranges[2];       // Plage des valeurs de pixel (min et max)
    const float *ranges[3]; // Pointeur vers les plages de valeurs
    bool isSparse;          // Indique si l'histogramme est dense ou épars
    float threhold;         // Seuil pour filtrer les résultats de la rétroprojection
    cv::Mat histogram;      // Histogramme dense
    cv::SparseMat shistogram; // Histogramme épars

public:
    // Constructeur : initialise les plages et le seuil par défaut
    ContentFinder() : threhold(0.1f), isSparse(false)
    {
        // Associe les plages à leur pointeur pour les trois canaux
        ranges[0] = hranges;
        ranges[1] = hranges;
        ranges[2] = hranges;
    }

    // Définit un seuil pour la rétroprojection
    void setThreshold(float t)
    {
        threhold = t;
    }

    // Récupère le seuil actuel
    float getThreshold()
    {
        return threhold;
    }

    // Définit un histogramme dense et le normalise
    void setHistogram(const cv::Mat &h)
    {
        isSparse = false; // Indique que l'histogramme est dense
        cv::normalize(h, histogram, 1.0); // Normalise les valeurs pour qu'elles soient dans [0, 1]
    }

    // Définit un histogramme épars et le normalise
    void setHistogram(const cv::SparseMat &h)
    {
        isSparse = true; // Indique que l'histogramme est épars
        cv::normalize(h, shistogram, 1.0, cv::NORM_L2); // Normalise l'histogramme épars
    }

    // Trouve les zones correspondantes dans une image en utilisant la plage [0, 256]
    cv::Mat find(const cv::Mat &image)
    {
        cv::Mat result;

        // Définit les plages de valeurs pour la rétroprojection
        hranges[0] = 0.0;
        hranges[1] = 256.0;

        // Définit les canaux à analyser (par défaut, BGR)
        channels[0] = 0; // Bleu
        channels[1] = 1; // Vert
        channels[2] = 2; // Rouge

        // Appelle la méthode surchargée pour effectuer la recherche
        return find(image, hranges[0], hranges[1], channels);
    }

    // Méthode surchargée pour rechercher des zones spécifiques dans une image
    // Permet de définir des plages de valeurs personnalisées
    cv::Mat find(const cv::Mat &image, float minValue, float maxValue, int *channels)
    {
        cv::Mat result;

        // Définit les plages de valeurs pour la rétroprojection
        hranges[0] = minValue;
        hranges[1] = maxValue;

        // Effectue la rétroprojection avec un histogramme dense ou épars
        if (isSparse)
        {
            // Si l'histogramme est épars, configure les canaux pour la rétroprojection
            for (int i = 0; i < shistogram.dims(); i++)
            {
                this->channels[i] = channels[i];
            }

            // Calcule la rétroprojection en utilisant l'histogramme épars
            cv::calcBackProject(&image, 1, channels, shistogram, result, ranges, 255.0);
        }
        else
        {
            // Si l'histogramme est dense, configure les canaux pour la rétroprojection
            for (int i = 0; i < histogram.dims; i++)
            {
                this->channels[i] = channels[i];
            }

            // Calcule la rétroprojection en utilisant l'histogramme dense
            cv::calcBackProject(&image, 1, channels, histogram, result, ranges, 255.0);
        }

        // Applique un seuil si configuré (valeurs en dessous du seuil sont ignorées)
        if (threhold >= 0.0)
        {
            cv::threshold(result, result, 255.0 * threhold, 255.0, cv::THRESH_BINARY);
        }

        // Retourne le résultat de la rétroprojection
        return result;
    }
};