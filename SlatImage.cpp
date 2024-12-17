#include <iostream>
#include <random>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void salt(cv::Mat image, int n)
{   
    // Générateur de nombres aléatoires
    std::default_random_engine generator;
    std::uniform_int_distribution<int> randomRow(0, image.rows - 1);
    std::uniform_int_distribution<int> randomCol(0, image.cols - 1);

    int i, j;
    for (int k = 0; k < n; k++)
    {
        i = randomRow(generator);
        j = randomCol(generator);

        if(image.type() == CV_8UC1) // Si l'image est en niveaux de gris
        {
            // 255 est la valeur maximale pour un pixel en niveaux de gris
            image.at<uchar>(i, j) = 255; 
        }
        else if(image.type() == CV_8UC3) // Si l'image est en couleur
        {   
            // 255 est la valeur maximale pour un pixel en couleur
            image.at<cv::Vec3b>(i, j)[0] = 255;
            image.at<cv::Vec3b>(i, j)[1] = 255;
            image.at<cv::Vec3b>(i, j)[2] = 255;

            // Autre manière de définir les valeurs des pixels
            //image.at<cv::Vec3b>(j, i) = cv::Vec3b(255, 255, 255);
        }
    }
}

int main() 
{   
    // Importe l'image
    cv::Mat image = cv::imread("boldt.jpg", 1);

    // Ajoute du bruit
    salt(image, 3000);

    // Affiche l'image
    cv::imshow("Image", image);

    // Attend une touche
    cv::waitKey();

    return 0;
}