#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

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

    // Calcule l'histogramme de la teinte (Hue) pour une image en couleur
    cv::Mat getHueHistogram(const cv::Mat &image, int minSaturation = 0)
    {
        cv::Mat hist; // Matrice pour stocker l'histogramme de sortie

        // Convertit l'image d'entrée de l'espace BGR à HSV (Teinte, Saturation, Valeur)
        cv::Mat hsv;
        cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);

        // Crée un masque pour ignorer les pixels avec une saturation inférieure au seuil
        cv::Mat mask;
        if (minSaturation > 0) // Si un seuil de saturation est défini
        {
            std::vector<cv::Mat> v; // Vecteur pour stocker les canaux HSV
            cv::split(hsv, v); // Sépare les canaux Hue, Saturation et Value

            // Applique un seuil à la saturation (canal 1)
            cv::threshold(v[1], mask, minSaturation, 255, cv::THRESH_BINARY);
            // Résultat : masque binaire où les pixels avec saturation >= minSaturation sont conservés
        }

        // Définit les plages pour l'histogramme de teinte
        hranges[0] = 0.0;   // Plage minimale : 0
        hranges[1] = 180.0; // Plage maximale : 180 (valeurs de Hue en HSV)
        channels[0] = 0;    // Canal 0 : Teinte (Hue)

        // Calcule l'histogramme de la teinte
        cv::calcHist(
            &hsv,          // Pointeur vers l'image d'entrée (HSV)
            1,             // Nombre d'images
            channels,      // Canal à utiliser (Hue)
            mask,          // Masque pour inclure uniquement les pixels avec saturation >= minSaturation
            hist,          // Histogramme de sortie
            1,             // Histogramme 1D (seulement pour Hue)
            histSize,      // Taille de l'histogramme (nombre de bins)
            ranges         // Plages de l'histogramme (0 à 180 pour Hue)
        );

        return hist; // Retourne l'histogramme calculé
    }

    // Définit la taille des bins pour chaque canal (B, G, R)
    void setSize(int nbins)
    {
        histSize[0] = histSize[1] = histSize[2] = nbins; // Applique la même taille de bin à tous les canaux
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
        cv::Mat hist = getHistogram3D(image);

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