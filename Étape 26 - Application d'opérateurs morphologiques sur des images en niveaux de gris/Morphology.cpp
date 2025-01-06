#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

int main()
{   
    // Charger l'image en niveaux de gris
    cv::Mat image = cv::imread("boldt.jpg", 0);
    if (image.empty())
    {
        return 0;
    }

    // Affiche l'image originale
    cv::imshow("Original", image);

    // Gradient morphologique pour extraire les contours
    cv::Mat result;
    cv::morphologyEx(image, result, cv::MORPH_GRADIENT, cv::Mat());

    // On inverse les couleurs en utilisant "255 - result" pour rendre les contours visibles en blanc.
    cv::imshow("Edge Image", 255 - result);

    // Charger l'image en niveaux de gris
    image = cv::imread("book.jpg", 0); 
    if (image.empty())
    {
        return 0;
    }

    // Effectue une transposition de l'image (rotation diagonale : échange lignes/colonnes)
    cv::transpose(image, image);
    // Effectue un retournement vertical de l'image
    cv::flip(image, image, 0);

    // L'élément structurant est une matrice de taille 7x7 remplie de 1.
    // Cet élément structurant sera utilisé pour la transformation morphologique Blackhat.
    cv::Mat element7(7, 7, CV_8U, cv::Scalar(1));

    // La transformation Blackhat met en évidence les pixels sombres sur un fond clair.
    // Elle est obtenue en soustrayant une fermeture morphologique de l'image d'origine.
    cv::morphologyEx(image, result, cv::MORPH_BLACKHAT, element7);

    // Affiche le résultat de la transformation Blackhat
    // On inverse les couleurs pour mieux visualiser les détails sombres.
    cv::namedWindow("7x7 Black Top-hat Image");
    cv::imshow("7x7 Black Top-hat Image", 255 - result);


    cv::waitKey();

    return 0;
}
