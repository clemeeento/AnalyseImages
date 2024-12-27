#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class Histogram1D 
{
private:
    int histSize[1];        // Nombre de bins pour l'histogramme (par défaut, 256 bins)
    float hranges[2];       // Plage des valeurs (min et max)
    const float* ranges[1]; // Pointeur vers les plages de valeurs
    int channels[1];        // Indique le canal à utiliser pour le calcul de l'histogramme

public:
    // Constructeur par défaut
    Histogram1D() 
    {
        histSize[0] = 256;   // Initialise à 256 bins
        hranges[0] = 0.0;    // Valeur minimale des pixels
        hranges[1] = 255.0;  // Valeur maximale des pixels
        ranges[0] = hranges; // Associe les plages au pointeur
        channels[0] = 0;     // Par défaut, utilise le canal 0 (par exemple, le canal bleu pour une image BGR)
    }

    // Définit le canal pour lequel l'histogramme sera calculé
    void setChannel(int c) 
    {
        channels[0] = c; // Canal (0: Bleu, 1: Vert, 2: Rouge pour BGR)
    }

    // Récupère le canal actuellement configuré
    int getChannel() 
    {
        return channels[0];
    }

    // Définit les plages de valeurs (min et max) pour l'histogramme
    void setRanges(float minValue, float maxValue) 
    {
        hranges[0] = minValue; // Nouvelle valeur minimale
        hranges[1] = maxValue; // Nouvelle valeur maximale
    }

    // Récupère la valeur minimale actuellement configurée
    float getMinValue() 
    {
        return hranges[0];
    }

    // Récupère la valeur maximale actuellement configurée
    float getMaxValue() 
    {
        return hranges[1];
    }

    // Définit le nombre de bins pour l'histogramme
    void setNBins(int nbins) 
    {
        histSize[0] = nbins; // Définit le nombre de bins
    }

    // Récupère le nombre de bins actuellement configuré
    int getNBins() 
    {
        return histSize[0];
    }

    // Calcule l'histogramme pour une image donnée
    cv::Mat getHistogram(const cv::Mat &image) 
    {
        cv::Mat hist; // Matrice de sortie pour stocker l'histogramme

        // Appelle la fonction calcHist pour calculer l'histogramme
        cv::calcHist(
            &image,       // Image source (par référence)
            1,            // Nombre d'images (ici, une seule)
            channels,     // Canal(s) à utiliser
            cv::Mat(),    // Aucun masque (considère toute l'image)
            hist,         // Histogramme de sortie
            1,            // Histogramme 1D
            histSize,     // Nombre de bins
            ranges        // Plage des valeurs
        );

        return hist; // Retourne l'histogramme calculé
    }

    // Génère une image visuelle de l'histogramme
    cv::Mat getHistogramImage(const cv::Mat &image, int zoom = 1) 
    {
        cv::Mat hist = getHistogram(image); // Calcule l'histogramme
        return Histogram1D::getImageOfHistogram(hist, zoom); // Génère une image à partir de l'histogramme
    }

    // Méthode statique pour convertir un histogramme en une image affichable
    static cv::Mat getImageOfHistogram(const cv::Mat &hist, int zoom) 
    {
        double maxVal = 0;
        double minVal = 0;

        // Récupère les valeurs maximale et minimale de l'histogramme
        cv::minMaxLoc(hist, &minVal, &maxVal, 0, 0);

        int histSize = hist.rows; // Taille de l'histogramme (nombre de bins)

        // Crée une image blanche pour afficher l'histogramme
        cv::Mat histImg(histSize * zoom, histSize * zoom, CV_8U, cv::Scalar(255));

        // Hauteur maximale des barres (90 % de l'image)
        int hpt = static_cast<int>(0.9 * histSize);

        // Parcourt chaque bin de l'histogramme
        for (int h = 0; h < histSize; h++)
        {
            float binVal = hist.at<float>(h); // Valeur du bin actuel
            if (binVal > 0)
            {
                // Calcule la hauteur de la barre en fonction de la valeur maximale
                int intensity = static_cast<int>(binVal * hpt / maxVal);

                // Dessine une ligne verticale pour représenter la barre du bin
                cv::line(histImg, cv::Point(h * zoom, histSize * zoom), cv::Point(h * zoom, (histSize - intensity) * zoom), cv::Scalar(0), zoom);
            }
        }

        return histImg; // Retourne l'image générée
    }
};

class ColorHistogram
{
private:
    int histSize[3];        // Taille des histogrammes pour chaque canal (B, G, R)
    float hranges[2];       // Plage des valeurs de pixel
    const float* ranges[3]; // Plages pour chaque canal
    int channels[3];        // Index des canaux (B, G, R)

public:
    // Constructeur par défaut
    ColorHistogram()
    {
        histSize[0] = histSize[1] = histSize[2] = 256; // Par défaut, 256 bins pour chaque canal
        hranges[0] = 0.0; // Plage des valeurs : 0-255
        hranges[1] = 255.0;
        ranges[0] = hranges; // Même plage pour tous les canaux
        ranges[1] = hranges;
        ranges[2] = hranges;
        channels[0] = 0; // Canal bleu
        channels[1] = 1; // Canal vert
        channels[2] = 2; // Canal rouge
    }

    // Définit la taille des bins pour chaque canal
    void setNBins(int nbins)
    {
        histSize[0] = histSize[1] = histSize[2] = nbins;
    }

    // Récupère la taille des bins pour chaque canal
    int getNBins() const
    {
        return histSize[0]; // Les trois canaux ont la même taille
    }

    // Définit les plages des valeurs de pixel
    void setRanges(float minValue, float maxValue)
    {
        hranges[0] = minValue;
        hranges[1] = maxValue;
    }

    // Calcule l'histogramme pour une image donnée et un canal donné
    cv::Mat getHistogram(const cv::Mat &image, int channel)
    {
        cv::Mat hist; // Matrice de sortie pour stocker l'histogramme

        // Appelle la fonction calcHist pour calculer l'histogramme
        cv::calcHist(
            &image,       // Image source (par référence)
            1,            // Nombre d'images (ici, une seule)
            &channels[channel], // Canal à utiliser
            cv::Mat(),    // Aucun masque (considère toute l'image)
            hist,         // Histogramme de sortie
            1,            // Histogramme 1D
            histSize,     // Nombre de bins
            ranges        // Plage des valeurs
        );

        return hist; // Retourne l'histogramme calculé
    }

    // Calcule l'histogramme 3D pour une image couleur
    cv::Mat getHistogram3D(const cv::Mat& image)
    {
        cv::Mat hist;

        // Calcule l'histogramme en 3 dimensions pour les canaux B, G, R
        cv::calcHist(&image,
                     1,          // Une seule image
                     channels,   // Canaux à considérer
                     cv::Mat(),  // Pas de masque
                     hist,       // Histogramme de sortie
                     3,          // Histogramme 3D
                     histSize,   // Nombre de bins pour chaque canal
                     ranges);    // Plage des valeurs pour chaque canal

        return hist;
    }

    // Visualise les histogrammes individuels des canaux (B, G, R)
    std::vector<cv::Mat> getHistogramImages(const cv::Mat& image, int zoom = 1)
    {
        std::vector<cv::Mat> histImages;

        for (int i = 0; i < 3; ++i)
        {
            cv::Mat hist = getHistogram(image, i); // Calcule l'histogramme

            histImages.push_back(ColorHistogram::getImageOfHistogram(hist, zoom)); // Génère une image de l'histogramme
        }

        return histImages;
    }

    // Méthode statique pour convertir un histogramme en une image affichable
    static cv::Mat getImageOfHistogram(const cv::Mat &hist, int zoom) 
    {
        double maxVal = 0;
        double minVal = 0;

        // Récupère les valeurs maximale et minimale de l'histogramme
        cv::minMaxLoc(hist, &minVal, &maxVal, 0, 0);

        int histSize = hist.rows; // Taille de l'histogramme (nombre de bins)

        // Crée une image blanche pour afficher l'histogramme
        cv::Mat histImg(histSize * zoom, histSize * zoom, CV_8U, cv::Scalar(255));

        // Hauteur maximale des barres (90 % de l'image)
        int hpt = static_cast<int>(0.9 * histSize);

        // Parcourt chaque bin de l'histogramme
        for (int h = 0; h < histSize; h++)
        {
            float binVal = hist.at<float>(h); // Valeur du bin actuel
            if (binVal > 0)
            {
                // Calcule la hauteur de la barre en fonction de la valeur maximale
                int intensity = static_cast<int>(binVal * hpt / maxVal);

                // Dessine une ligne verticale pour représenter la barre du bin
                cv::line(histImg, cv::Point(h * zoom, histSize * zoom), cv::Point(h * zoom, (histSize - intensity) * zoom), cv::Scalar(0), zoom);
            }
        }

        return histImg; // Retourne l'image générée
    }

    // Normalise et retourne une image 3D de l'histogramme (en 2D pour visualisation)
    cv::Mat get3DHistogramImage(const cv::Mat& hist)
    {
        // Normalise l'histogramme pour une meilleure visualisation
        cv::Mat normalizedHist;
        cv::normalize(hist, normalizedHist, 0, 255, cv::NORM_MINMAX);

        // Dimension de l'image de sortie
        int histDim = histSize[0]; // Hypothèse : les trois dimensions ont la même taille
        cv::Mat histImage(histDim, histDim, CV_8UC3, cv::Scalar(0, 0, 0));

        // Parcourt les bins et dessine une représentation simplifiée
        for (int r = 0; r < histDim; ++r)
        {
            for (int g = 0; g < histDim; ++g)
            {
                float binVal = 0.0;
                for (int b = 0; b < histDim; ++b)
                {
                    binVal += normalizedHist.at<float>(b, g, r); // Accumule les valeurs
                }
                int intensity = static_cast<int>(binVal);
                histImage.at<cv::Vec3b>(g, r) = cv::Vec3b(intensity, intensity, intensity);
            }
        }

        return histImage;
    }
};
