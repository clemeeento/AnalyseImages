#include "Histogram.hpp"

int main()
{
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

    // Étend la plage de l'histogramme (égalisation linéaire)
    cv::Mat str = h.stretch(image);
    cv::imshow("Stretched Image", str); // Affiche l'image avec l'histogramme étiré
    cv::imshow("Stretched Histogram", h.getHistogramImage(str)); // Affiche l'histogramme de l'image étirée

    // Crée une table de correspondance (LUT) pour générer un négatif de l'image
    cv::Mat lut(1, 256, CV_8U); // LUT de 256 valeurs (1 ligne, 256 colonnes)
    for (int i = 0; i < 256; i++) 
    {
        lut.at<uchar>(i) = 255 - i; // Inverse les niveaux de gris (0 devient 255, 255 devient 0)
    }

    // Applique la LUT pour obtenir l'image négative
    cv::imshow("Negative Image", h.applyLookUp(image, lut)); // Affiche l'image négative


    // Égalise l'histogramme de l'image
    cv::Mat eq = h.equalize(image);
    cv::imshow("Equalized Image", eq); // Affiche l'image égalisée
    cv::imshow("Equalized Histogram", h.getHistogramImage(eq)); // Affiche l'histogramme de l'image égalisée

    cv::waitKey(0);

    return 0;
}