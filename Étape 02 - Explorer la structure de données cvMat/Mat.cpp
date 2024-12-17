#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// Fonction qui retourne une image de 500x500 pixels en niveau de gris
cv::Mat function() 
{
    cv::Mat ima(500, 500, CV_8U, 50);
    return ima;
}

int main() 
{
    // Création des images---------------------------------
    cv::Mat image1(240, 320, CV_8U, 100); // Image de 240 lignes, 320 colonnes, en niveaux de gris, initialisée à 100

    image1.create(200, 200, CV_8U); // Crée une image de 200x200 pixels en niveaux de gris
    image1 = 200; // Tous les pixels de l'image sont à 200

    cv::Mat image2(240, 320, CV_8UC3, cv::Scalar(0, 0, 255)); // Image de 240 lignes, 320 colonnes, en couleur (3 canaux), initialisée en rouge
    //cv::Mat image2(cv:Size(320, 240), CV_8UC3, cv::Scalar(0, 0, 255)); // Autre manière de créer une image

    cv::Mat image3 = cv::imread("baboon02.jpg"); // Charge une image couleur

    cv::Mat image4(image3); // Crée une image qui partage les données de image3
    image1 = image3; // Crée une image qui partage les données de image3 d'une autre manière

    image3.copyTo(image2); // Copie les données de image3 dans image2
    cv::Mat image5 = image3.clone(); // Crée une image qui contient une copie des données de image3

    cv::flip(image3, image3, 1); // Flip horizontal de l'image3

    // Affichage des images--------------------------------
    cv::imshow("Image 3", image3); 
    cv::imshow("Image 1", image1);
    cv::imshow("Image 2", image2);
    cv::imshow("Image 4", image4);
    cv::imshow("Image 5", image5);

    cv::waitKey(0);

    cv::Mat gray = function(); // Appel de la fonction function

    image1 = cv::imread("baboon02.jpg", cv::IMREAD_GRAYSCALE); // Charge une image en niveau de gris
    image1.convertTo(image2, CV_32F, 1/255.0, 0.0); // Convertit l'image en float, normalise les valeurs entre 0 et 1

    // Création d'une image 3 canaux en float
    cv::Matx33d matrix(3.0, 2.0, 1.0,
                    2.0, 1.0, 3.0,
                    1.0, 2.0, 3.0);

    // Multiplication d'une matrice par un vecteur
    cv::Matx31d vector(5.0, 1.0, 3.0);
    cv::Matx31d result = matrix * vector;

    cv::waitKey(0);

    return 0;
}


