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

    // **Création d'un élément structurant**
    // L'élément structurant est une matrice 5x5 remplie de 1.
    // Cet élément structurant sera utilisé pour les transformations morphologiques.
    cv::Mat element5(5, 5, CV_8U, cv::Scalar(1));

    // **Fermeture morphologique**
    // La fermeture est une combinaison de dilatation suivie d'érosion.
    cv::Mat closed;
    cv::morphologyEx(image, closed, cv::MORPH_CLOSE, element5);
    cv::imshow("Closed image", closed); // Affiche l'image après fermeture

    // **Ouverture morphologique**
    // L'ouverture est une combinaison d'érosion suivie de dilatation.
    cv::Mat opened;
    cv::morphologyEx(image, opened, cv::MORPH_OPEN, element5);
    cv::imshow("Opened image", opened); // Affiche l'image après ouverture

    // **Combinaison dilatation + érosion**
    // Dilate l'image en premier pour agrandir les régions blanches.
    cv::Mat result;
    cv::dilate(image, result, element5); // Applique une dilatation
    cv::erode(result, result, element5); // Suit d'une érosion pour réduire les régions étendues
    cv::imshow("Dilate/Erode image", result); // Affiche le résultat de la combinaison

    // Applique une fermeture pour combler les trous
    cv::morphologyEx(image, image, cv::MORPH_CLOSE, element5);
    // Ensuite, applique une ouverture pour nettoyer le bruit
    cv::morphologyEx(image, image, cv::MORPH_OPEN, element5);
    cv::imshow("Close/Open image", image); // Affiche le résultat combiné

    // Attend une interaction utilisateur avant de fermer les fenêtres
    cv::waitKey(0);
    
    return 0;
}
