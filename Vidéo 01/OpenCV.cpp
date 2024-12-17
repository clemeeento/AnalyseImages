#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


void onMouse(int event, int x, int y, int flags, void* param)
{
    cv::Mat *im = reinterpret_cast<cv::Mat*>(param);

    switch(event)
    {
        case cv::EVENT_LBUTTONDOWN:
            // Affiche la valeur du pixel au clic gauche
            std::cout << "A la position (" << x << ", " << y << ") la valeur est " << static_cast<int>(im->at<uchar>(cv::Point(x, y))) << std::endl;
        break;
    }
}


int main() 
{
    // Lecture d'une image---------------------------------
    cv::Mat image;

    image = cv::imread("baboon02.jpg", cv::IMREAD_COLOR);

    std::cout << "Cette image est de taille " << image.rows << " x " << image.cols << std::endl;

    if(image.empty())
    {
        std::cout << "Impossible de charger l'image" << std::endl;
        return 0;
    }

    // Affichage de l'image---------------------------------
    cv::namedWindow("Image");
    cv::imshow("Image", image);

    // Attente d'un clic gauche pour afficher la valeur du pixel
    cv::setMouseCallback("Image", onMouse, reinterpret_cast<void*>(&image));

    // Enregistrement de l'image---------------------------
    cv::Mat result;
    cv::flip(image, result, 1); // 1 pour flip horizontal, 0 pour flip vertical

    cv::imwrite("baboon02_flip.jpg", result);

    // Dessin d'un cercle avec un texte--------------------
    cv::circle(image, cv::Point(image.cols/2, image.rows/2), 100, cv::Scalar(0, 0, 255), 2); // Dessine un cercle rouge de rayon 100, épaisseur 2, centré au milieu de l'image

    cv::putText(image, "Cercle", cv::Point(10, 30), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(255, 0, 0), 2); // Ecrit "Cercle" en bleu en haut à gauche

    cv::imshow("Dessin d'un cercle avec un texte", image);

    // Attente d'une touche pour fermer les fenêtres-------
    cv::waitKey(0);
    cv::destroyWindow("Image");

    return 0;
}