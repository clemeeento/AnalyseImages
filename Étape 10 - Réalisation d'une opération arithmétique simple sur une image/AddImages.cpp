#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

int main()
{
    // Importe les images
    cv::Mat image1 = cv::imread("boldt.jpg", cv::IMREAD_COLOR);
    cv::Mat image2 = cv::imread("rain.jpg", cv::IMREAD_COLOR);

    // Vérifie si les images sont valides
    if(!image1.data)
    {
        return 0;
    }
    if(!image2.data)
    {
        return 0;
    }

    // Crée une image pour le résultat
    cv::Mat result;

    // Masque pour l'addition
    cv::Mat mask;

    // Differentes facon d'ajouter les images
    cv::addWeighted(image1, 0.7, image2, 0.9, 0, result); // Avec un poids
    //result = 0.7 * image1 + 0.9 * image2; // Autre manière de faire avec des poids
    //cv::add(image1, image2, result); // Additionne les images
    //result = image1 + image2; // Autre manière de faire
    //cv::add(image1, cv::Scalar(0, 0, 255), result); // Ajoute une couleur à l'image
    //cv::scaleAdd(image1, 0.5, image2, result); // Ajoute une image à une autre avec un facteur
    //cv::add(image1, image2, result, mask); // Ajoute les images avec un masque

    // Affiche le résultat
    cv::namedWindow("Resultat");
    cv::imshow("Resultat", result);

    
    // Importe l'image en niveaux de gris pour n'avoir qu'un canal
    image2 = cv::imread("rain.jpg", 0);

    // Initialise planes pour la fonction split
    std::vector<cv::Mat> planes;

    // Split l'image en 3 canaux
    cv::split(image1, planes);

    // Ajoute l'image 2 au canal bleu
    planes[0] = planes[0] + image2;

    // Merge les canaux
    cv::merge(planes, result);

    // Affiche le résultat
    cv::namedWindow("Resultat 2");
    cv::imshow("Resultat 2", result);

    // Attend une touche
    cv::waitKey(0);

    return 0;
}