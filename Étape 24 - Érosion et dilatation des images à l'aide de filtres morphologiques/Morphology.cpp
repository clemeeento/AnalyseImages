#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

int main()
{   
    // Charger l'image
    cv::Mat image = cv::imread("binary.bmp");
    if (image.empty()) 
    {
        return 0;
    }
    // Affiche l'image originale
    cv::imshow("Original", image);

    // Erosion simple de l'image avec un élément structurant par défaut (3x3 carré rempli de 1)
    cv::Mat eroded;
    cv::erode(image, eroded, cv::Mat()); 
    // Affiche l'image érodée
    cv::imshow("Eroded image", eroded);

    // Dilatation simple de l'image avec un élément structurant par défaut (3x3 carré rempli de 1)
    cv::Mat dilated;
    cv::dilate(image, dilated, cv::Mat());
    // Affiche l'image dilatée
    cv::imshow("Dilated image", dilated);

    // Création d'un élément structurant personnalisé de taille 7x7 rempli de 1
    cv::Mat element(7, 7, CV_8U, cv::Scalar(1));
    // Applique l'érosion avec l'élément structurant personnalisé
    cv::erode(image, eroded, element);
    // Affiche l'image érodée avec un élément structurant 7x7
    cv::imshow("Eroded image (7x7)", eroded);

    // Applique l'érosion 3 fois avec l'élément structurant par défaut (3x3 carré rempli de 1)
    // cv::Point(-1, -1) indique que le point d'ancrage est au centre de l'élément structurant
    cv::erode(image, eroded, cv::Mat(), cv::Point(-1, -1), 3);
    // Affiche l'image érodée après 3 itérations
    cv::imshow("Eroded image (3 times)", eroded);

    // Attend une interaction utilisateur avant de fermer les fenêtres
    cv::waitKey(0);
    
    return 0;
}
