#include "Histogram.hpp"

int main()
{
    // Histogram1D-----------------------------------------
    // Charge une image en niveaux de gris
    cv::Mat image = cv::imread("group.jpg", 0); // 0 pour charger en niveaux de gris
    if (!image.data)
    {
        return 0; 
    }

    // Crée un objet Histogram1D pour calculer l'histogramme en niveaux de gris
    Histogram1D h;
    cv::Mat histo = h.getHistogram(image); // Calcule l'histogramme de l'image

    // Affiche les valeurs de l'histogramme dans la console
    for (int i = 0; i < 256; i++)
    {
        std::cout << "Value " << i << " = " << histo.at<float>(i) << std::endl;
    }

    // Affiche une visualisation de l'histogramme dans une fenêtre
    cv::namedWindow("Histogram");
    cv::imshow("Histogram", h.getHistogramImage(image));

    // Dessine une ligne sur l'histogramme pour marquer une valeur seuil
    cv::Mat hi = h.getHistogramImage(image); // Copie l'image de l'histogramme
    cv::line(hi, cv::Point(70, 0), cv::Point(70, 255), cv::Scalar(128)); // Dessine une ligne verticale à 70
    cv::imshow("Histogram with threshold value", hi); // Affiche l'histogramme modifié

    // Applique un seuillage binaire sur l'image en niveaux de gris
    cv::Mat thresholded;
    cv::threshold(image, thresholded, 70, 255, cv::THRESH_BINARY); // Pixels >= 70 deviennent blancs

    // Affiche l'image binaire
    cv::imshow("Binary Image", thresholded);

    // Inverse l'image binaire (noir devient blanc, blanc devient noir)
    thresholded = 255 - thresholded;
    cv::imwrite("binary.bmp", thresholded); // Sauvegarde l'image inversée dans un fichier

    // ColorHistogram--------------------------------------
    // Charge une image en couleur
    cv::Mat colorImage = cv::imread("group.jpg");
    if (!colorImage.data)
    {
        return 0;
    }

    // Crée un objet ColorHistogram pour analyser les canaux de couleur
    ColorHistogram ch;

    // Récupère et affiche les histogrammes individuels des canaux B, G, et R
    std::vector<cv::Mat> histImages = ch.getHistogramImages(colorImage, 2);
    cv::imshow("Blue Histogram", histImages[0]);  // Histogramme du canal bleu
    cv::imshow("Green Histogram", histImages[1]); // Histogramme du canal vert
    cv::imshow("Red Histogram", histImages[2]);   // Histogramme du canal rouge

    // Calcule l'histogramme 3D des couleurs
    cv::Mat hist3D = ch.getHistogram3D(colorImage); // Calcule l'histogramme 3D
    cv::Mat histImage3D = ch.get3DHistogramImage(hist3D);  // Génère une image de l'histogramme 3D (2D pour visualisation)
    cv::imshow("3D Histogram Image", histImage3D); // Histogramme 3D


    cv::waitKey(0);

    return 0;
}