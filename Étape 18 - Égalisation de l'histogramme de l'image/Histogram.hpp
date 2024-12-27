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

    // Applique une table de correspondance (LUT) à une image
    static cv::Mat applyLookUp(const cv::Mat& image, const cv::Mat& lookup)
    {
        cv::Mat result; // Image de sortie
        cv::LUT(image, lookup, result); // Utilise OpenCV pour appliquer la table de correspondance
        return result; // Retourne l'image transformée
    }

    // Applique une table de correspondance (LUT) à une image en utilisant des itérateurs
    static cv::Mat applyLookUpWhithIterator(const cv::Mat& image, const cv::Mat& lookup)
    {
        cv::Mat result(image.rows, image.cols, CV_8U); // Crée une image de sortie avec les mêmes dimensions que l'image d'entrée

        // Itérateur pour parcourir les pixels de l'image résultante
        cv::Mat_<uchar>::iterator itr = result.begin<uchar>();

        // Itérateur pour parcourir les pixels de l'image d'entrée
        cv::Mat_<uchar>::const_iterator it = image.begin<uchar>();

        // Itérateur de fin de l'image d'entrée
        cv::Mat_<uchar>::const_iterator itend = image.end<uchar>();

        // Parcourt chaque pixel de l'image d'entrée
        for (; it != itend; ++it, ++itr)
        {
            *itr = lookup.at<uchar>(*it); // Transforme la valeur de pixel en utilisant la table de correspondance
        }

        return result; // Retourne l'image transformée
    }

    // Étend les valeurs de pixels d'une image pour améliorer son contraste
    cv::Mat stretch(const cv::Mat &image, int minValue = 0) 
    {
        // Calcule l'histogramme de l'image
        cv::Mat hist = getHistogram(image);

        // Trouve l'indice du premier bin non nul au-dessus de la valeur minimale
        int imin = 0;
        for (; imin < histSize[0]; imin++)
        {
            if (hist.at<float>(imin) > minValue)
            {
                break; // Arrête la recherche une fois qu'un bin significatif est trouvé
            }
        }

        // Trouve l'indice du dernier bin non nul au-dessus de la valeur minimale
        int imax = histSize[0] - 1;
        for (; imax >= 0; imax--)
        {
            if (hist.at<float>(imax) > minValue)
            {
                break; // Arrête la recherche une fois qu'un bin significatif est trouvé
            }
        }

        // Crée une table de correspondance (LUT) pour étirer les valeurs de pixels
        int dims[1] = {256}; // Taille de la LUT (256 niveaux de gris)
        cv::Mat lookup(1, dims, CV_8U);

        // Remplit la LUT en fonction des valeurs de imin et imax
        for (int i = 0; i < 256; i++)
        {
            if (i < imin)
            {
                lookup.at<uchar>(i) = 0; // Pixels en dessous de imin deviennent 0
            }
            else if (i > imax)
            {
                lookup.at<uchar>(i) = 255; // Pixels au-dessus de imax deviennent 255
            }
            else
            {
                // Pixels dans la plage [imin, imax] sont étirés linéairement
                lookup.at<uchar>(i) = cvRound(255.0 * (i - imin) / (imax - imin));
            }
        }

        // Applique la table de correspondance pour obtenir l'image étirée
        cv::Mat result;
        result = applyLookUp(image, lookup);

        return result; // Retourne l'image avec l'histogramme étiré
    }

    // Égalise l'histogramme d'une image
    static cv::Mat equalize(const cv::Mat &image)
    {
        cv::Mat result; // Image de sortie
        cv::equalizeHist(image, result); // Utilise OpenCV pour égaliser l'histogramme
        return result; // Retourne l'image égalisée
    }
};
