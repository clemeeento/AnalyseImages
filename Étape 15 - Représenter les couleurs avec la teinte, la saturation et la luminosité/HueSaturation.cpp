#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// Fonction pour détecter les pixels dans une plage de teinte (Hue) et saturation (Saturation)
void detectHScolor(const cv::Mat &image, double minHue, double maxHue, double minSat, double maxSat, cv::Mat &mask)
{
    // Convertit l'image de l'espace BGR à HSV
    cv::Mat hsv;
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);

    // Sépare les trois canaux HSV (teinte, saturation, valeur)
    std::vector<cv::Mat> channels;
    cv::split(hsv, channels);

    // Crée un masque pour la teinte (Hue)
    cv::Mat mask1, mask2;
    cv::threshold(channels[0], mask1, maxHue, 255, cv::THRESH_BINARY_INV); // Teintes inférieures à maxHue
    cv::threshold(channels[0], mask2, minHue, 255, cv::THRESH_BINARY);     // Teintes supérieures à minHue

    cv::Mat hueMask;
    if (minHue < maxHue)
    {
        hueMask = mask1 & mask2; // Intersection des plages pour une teinte continue
    }
    else
    {
        hueMask = mask1 | mask2; // Union des plages pour une teinte "enroulée" (ex : rouge)
    }

    // Crée un masque pour la saturation (Saturation)
    cv::threshold(channels[1], mask1, maxSat, 255, cv::THRESH_BINARY_INV); // Saturation inférieure à maxSat
    cv::threshold(channels[1], mask2, minSat, 255, cv::THRESH_BINARY);     // Saturation supérieure à minSat

    cv::Mat satMask = mask1 & mask2; // Intersection des plages de saturation

    // Combine les masques de teinte et saturation
    mask = hueMask & satMask;
}

int main()
{
    // Charge une première image
    cv::Mat image = cv::imread("boldt.jpg");
    if (image.empty())
    {
        return 0;
    }

    // Conversion de l'image en HSV
    cv::Mat hsv;
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);

    // Sépare les canaux HSV (teinte, saturation, valeur)
    std::vector<cv::Mat> channels;
    cv::split(hsv, channels);

    // Affiche les canaux HSV séparés
    cv::imshow("Value", channels[2]);        // Valeur (luminosité)
    cv::imshow("Saturation", channels[1]);  // Saturation
    cv::imshow("Hue", channels[0]);         // Teinte

    // Modifie les canaux pour des démonstrations
    cv::Mat newImage;
    cv::Mat tmp(channels[2].clone());       // Sauvegarde temporaire de la valeur originale
    channels[2] = 255;                      // Fixe la valeur (luminosité) au maximum
    cv::merge(channels, hsv);               // Fusionne les canaux modifiés
    cv::cvtColor(hsv, newImage, cv::COLOR_HSV2BGR); // Reconvertit en BGR
    cv::imshow("Fixed value", newImage);    // Affiche l'image avec la luminosité maximale

    channels[1] = 255;                      // Fixe la saturation au maximum
    channels[2] = tmp;                      // Restaure la luminosité originale
    cv::merge(channels, hsv);
    cv::cvtColor(hsv, newImage, cv::COLOR_HSV2BGR);
    cv::imshow("Fixed saturation", newImage); // Affiche l'image avec la saturation maximale

    channels[1] = 255;                      // Fixe la saturation au maximum
    channels[2] = 255;                      // Fixe la luminosité au maximum
    cv::merge(channels, hsv);
    cv::cvtColor(hsv, newImage, cv::COLOR_HSV2BGR);
    cv::imshow("Fixed saturation/value", newImage); // Affiche l'image avec saturation et luminosité maximales

    // Crée une matrice visualisant les teintes et saturations
    cv::Mat hs(128, 360, CV_8UC3);
    for (int h = 0; h < 360; h++) // Parcourt les teintes (0 à 360°)
    {
        for (int s = 0; s < 128; s++) // Parcourt les saturations (0 à 255, mais échantillonné ici)
        {
            hs.at<cv::Vec3b>(s, h)[0] = h / 2;         // Teinte
            hs.at<cv::Vec3b>(s, h)[1] = 255 - s * 2;   // Saturation inversée
            hs.at<cv::Vec3b>(s, h)[2] = 255;          // Valeur maximale
        }
    }
    cv::cvtColor(hs, newImage, cv::COLOR_HSV2BGR);    // Conversion en BGR
    cv::imshow("Hue/Saturation", newImage);           // Affiche la visualisation

    // Charge une deuxième image pour la détection de couleurs
    image = cv::imread("girl.jpg");
    if (image.empty())
    {
        return 0;
    }

    // Affiche l'image originale
    cv::imshow("Original image", image);

    // Détection de couleurs dans une plage de teinte et saturation
    cv::Mat mask;
    detectHScolor(image, 160, 10, 25, 166, mask); // Plages de teinte (rouge) et saturation

    // Crée une image contenant uniquement les pixels détectés
    cv::Mat detected(image.size(), CV_8UC3, cv::Scalar(0, 0, 0)); // Fond noir
    image.copyTo(detected, mask); // Copie les pixels détectés depuis l'image originale
    cv::imshow("Detection result", detected); // Affiche l'image avec la détection

    // Génère une image dégradée linéaire pour visualiser les valeurs
    cv::Mat linear(100, 256, CV_8U);
    for (int i = 0; i < 256; i++)
    {
        linear.col(i) = i; // Chaque colonne a une intensité croissante de 0 à 255
    }
    cv::imshow("Linear", linear); // Affiche le dégradé

    cv::waitKey(0);

    return 0;
}