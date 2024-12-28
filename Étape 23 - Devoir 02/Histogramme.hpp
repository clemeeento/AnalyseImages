#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class Histogramme
{
private:
    int tailleHistogramme[1];      // Nombre de bins pour l'histogramme (par défaut, 256 bins)
    float plageValeurs[2];         // Plage des valeurs (min et max)
    const float* plages[1];        // Pointeur vers les plages de valeurs
    int canaux[1];                 // Indique le canal à utiliser pour le calcul de l'histogramme

public:
    // Constructeur par défaut
    Histogramme()
    {
        tailleHistogramme[0] = 256;      // Initialise à 256 bins
        plageValeurs[0] = 0.0;           // Valeur minimale des pixels
        plageValeurs[1] = 255.0;         // Valeur maximale des pixels
        plages[0] = plageValeurs;        // Associe les plages au pointeur
        canaux[0] = 0;                   // Par défaut, utilise le canal 0 (par exemple, le bleu pour une image BGR)
    }

    // Calcule l'histogramme pour une image donnée
    cv::Mat calculerHistogramme(const cv::Mat &image)
    {
        cv::Mat histogramme; // Matrice de sortie pour stocker l'histogramme

        // Appelle la fonction calcHist pour calculer l'histogramme
        cv::calcHist(
            &image,             // Image source (par référence)
            1,                  // Nombre d'images (ici, une seule)
            canaux,             // Canal(s) à utiliser
            cv::Mat(),          // Aucun masque (considère toute l'image)
            histogramme,        // Histogramme de sortie
            1,                  // Histogramme 1D
            tailleHistogramme,  // Nombre de bins
            plages              // Plage des valeurs
        );

        return histogramme; // Retourne l'histogramme calculé
    }

    // Méthode statique pour convertir un histogramme en une image affichable
    static cv::Mat dessinerHistogramme(const cv::Mat &histogramme, int zoom)
    {
        double valeurMax = 0;
        double valeurMin = 0;

        // Récupère les valeurs maximale et minimale de l'histogramme
        cv::minMaxLoc(histogramme, &valeurMin, &valeurMax, nullptr, nullptr);

        int tailleHist = histogramme.rows; // Taille de l'histogramme (nombre de bins)

        // Crée une image blanche pour afficher l'histogramme
        cv::Mat imageHistogramme(tailleHist * zoom, tailleHist * zoom, CV_8U, cv::Scalar(255));

        // Hauteur maximale des barres (90 % de l'image)
        int hauteurMax = static_cast<int>(0.9 * tailleHist);

        // Parcourt chaque bin de l'histogramme
        for (int i = 0; i < tailleHist; i++)
        {
            float valeurBin = histogramme.at<float>(i); // Valeur du bin actuel
            if (valeurBin > 0)
            {
                // Calcule la hauteur de la barre en fonction de la valeur maximale
                int hauteur = static_cast<int>(valeurBin * hauteurMax / valeurMax);

                // Dessine une ligne verticale pour représenter la barre du bin
                cv::line(imageHistogramme, cv::Point(i * zoom, tailleHist * zoom), cv::Point(i * zoom, (tailleHist - hauteur) * zoom), cv::Scalar(0), zoom);
            }
        }

        return imageHistogramme; // Retourne l'image générée
    }

    // Génère une image visuelle de l'histogramme
    cv::Mat genererImageHistogramme(const cv::Mat &image, int zoom = 1)
    {
        cv::Mat histogramme = calculerHistogramme(image); // Calcule l'histogramme
        return Histogramme::dessinerHistogramme(histogramme, zoom); // Génère une image à partir de l'histogramme
    }

    // Applique une table de correspondance (LUT) à une image
    static cv::Mat appliquerTableCorrespondance(const cv::Mat& image, const cv::Mat& table)
    {
        cv::Mat resultat; // Image de sortie
        cv::LUT(image, table, resultat); // Utilise OpenCV pour appliquer la table de correspondance
        return resultat; // Retourne l'image transformée
    }

    
    // Étend les valeurs de pixels d'une image pour améliorer son contraste
    cv::Mat etendreContraste(const cv::Mat &image, int valeurMin = 0)
    {
        // Calcule l'histogramme de l'image
        cv::Mat histogramme = calculerHistogramme(image);

        // Détermine les indices de début et de fin pour l'étirement
        int indiceDebut, indiceFin;

        for (indiceDebut = 0; indiceDebut < tailleHistogramme[0]; indiceDebut = indiceDebut + 1)
        {
            if (histogramme.at<float>(indiceDebut) > valeurMin) 
            {
                break; // Arrête la recherche une fois qu'un bin significatif est trouvé
            }
        }

        for (indiceFin = tailleHistogramme[0] - 1; indiceFin >= 0; indiceFin = indiceFin - 1)
        {
            if (histogramme.at<float>(indiceFin) > valeurMin) 
            {
                break; // Arrête la recherche une fois qu'un bin significatif est trouvé
            }
        }

        // Crée une table de correspondance
        cv::Mat table(1, 256, CV_8U);
        for (int i = 0; i < 256; i++)
        {
            if (i < indiceDebut) 
            {
                table.at<uchar>(i) = 0;
            }
            else if (i > indiceFin) 
            {
                table.at<uchar>(i) = 255;
            }
            else 
            {
                table.at<uchar>(i) = cvRound(255.0 * (i - indiceDebut) / (indiceFin - indiceDebut));
            }
        }

        return appliquerTableCorrespondance(image, table); // Retourne l'image étirée
    }
};