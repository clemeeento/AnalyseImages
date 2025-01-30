#include <iostream>
#include <vector>
#include <iomanip>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>

// Classe permettant de détecter des lignes dans une image en utilisant la transformée de Hough
class LineFinder
{
    private:
        cv::Mat img;  // Image source
        std::vector<cv::Vec4i> lines; // Liste des lignes détectées sous forme de vecteurs (x1, y1, x2, y2)

        // Paramètres pour la transformée de Hough probabiliste (HoughLinesP)
        double deltaRho;    // Résolution en pixels de la distance rho
        double deltaTheta;  // Résolution en radians de l'angle theta
        int minVote;        // Nombre minimal de votes pour qu'une ligne soit validée

        // Paramètres supplémentaires pour filtrer les lignes détectées
        double minLength;   // Longueur minimale des segments de ligne détectés
        double maxGap;      // Espace maximum entre deux segments pour être considérés comme une seule ligne

    public:
        // Constructeur par défaut avec initialisation des paramètres
        LineFinder() 
            : deltaRho(1),             // Résolution spatiale de la distance rho (1 pixel)
              deltaTheta(M_PI / 180),   // Résolution angulaire de l'angle theta (1 degré converti en radians)
              minVote(10),              // Nombre minimum de votes pour valider une ligne (10 par défaut)
              minLength(0.0),           // Longueur minimale des segments de ligne (non filtré par défaut)
              maxGap(0.0) {}            // Distance maximale entre les segments de ligne (non filtré par défaut)

        // Définit la résolution de l'accumulateur Hough (rho et theta)
        void setAccResolution(double dRho, double dTheta)
        {
            deltaRho = dRho;      // Définit la résolution en pixels de rho
            deltaTheta = dTheta;  // Définit la résolution en radians de theta
        }

        // Définit le nombre minimum de votes requis pour qu'une ligne soit détectée
        void setMinVote(int minv)
        {
            minVote = minv;
        }

        // Définit la longueur minimale d'une ligne et l'écart maximal entre les segments de ligne connectés
        void setLineLengthAndGap(double length, double gap)
        {
            minLength = length;
            maxGap = gap;
        }

        // Détecte les lignes dans une image binaire en utilisant la transformée de Hough probabiliste (HoughLinesP)
        std::vector<cv::Vec4i> findLines(cv::Mat& binary)
        {
            lines.clear(); // Efface les lignes précédemment détectées

            // Applique la transformée de Hough probabiliste
            cv::HoughLinesP(binary,       // Image binaire en entrée
                            lines,        // Liste des lignes détectées en sortie
                            deltaRho,     // Résolution de rho (distance entre deux lignes)
                            deltaTheta,   // Résolution de theta (angle entre deux lignes)
                            minVote,      // Nombre minimum de votes pour qu'une ligne soit validée
                            minLength,    // Longueur minimale d'une ligne détectée
                            maxGap);      // Espace maximal entre deux segments pour être fusionnés

            return lines; // Retourne la liste des lignes détectées
        }

        // Dessine les lignes détectées sur l'image fournie
        void drawDetectedLines(cv::Mat &image, cv::Scalar color = cv::Scalar(255, 255, 255))
        {
            // Itérateur pour parcourir toutes les lignes détectées
            std::vector<cv::Vec4i>::const_iterator it2 = lines.begin();

            // Boucle sur toutes les lignes détectées
            while(it2 != lines.end())
            {
                // Récupère les points de début et de fin de la ligne
                cv::Point pt1((*it2)[0], (*it2)[1]); // Point de départ (x1, y1)
                cv::Point pt2((*it2)[2], (*it2)[3]); // Point d'arrivée (x2, y2)

                // Dessine la ligne sur l'image
                cv::line(image, pt1, pt2, color);

                ++it2; // Passe à la ligne suivante
            }
        }
};
