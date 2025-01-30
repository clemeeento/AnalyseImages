#include "Contours.hpp"  

int main()
{
    // Chargement de l'image en niveaux de gris
    cv::Mat image = cv::imread("road.jpg", 0);
    if(image.empty())
    {
        return 0; 
    }

    // Création d'une fenêtre et affichage de l'image originale
    cv::imshow("Original Image", image);

    // Création d'un détecteur de contours
    EdgeDetector ed;

    // Calcul du gradient de Sobel sur l'image pour détecter les contours
    ed.computeSobel(image);

    // Sauvegarde de l'orientation des gradients de Sobel dans un fichier image
    cv::imwrite("ori.bmp", ed.getSobelOrientationImage());

    // Affichage de l'image binaire des contours détectés avec un seuil bas (125)
    cv::imshow("Sobel (low threshold)", ed.getBinaryMap(125));

    // Affichage de l'image binaire des contours détectés avec un seuil élevé (350)
    cv::imshow("Sobel (high threshold)", ed.getBinaryMap(350));

    // Application du détecteur de contours de Canny avec des seuils de 125 et 350
    cv::Mat contours;
    cv::Canny(image, contours, 125, 350);

    // Affichage du résultat de l'algorithme de Canny, en inversant l'image pour une meilleure visibilité
    cv::imshow("Canny Contours", 255 - contours);

    cv::waitKey();

    return 0;
}
