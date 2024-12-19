#include <iostream>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void wave(const cv::Mat &image, cv::Mat &result)
{
    // Crée les matrices srcX et srcY pour le remap de la taille de l'image
    cv::Mat srcX(image.rows, image.cols, CV_32F);
    cv::Mat srcY(image.rows, image.cols, CV_32F);

    for(int i = 0; i < image.rows; i++)
    {
        for(int j = 0; j < image.cols; j++)
        {
            srcX.at<float>(i, j) = j; // L'axe des x reste le même
            srcY.at<float>(i, j) = i + 5 * sin(j / 6.0); // L'axe des y est décalé de 5 pixels en fonction de la sinusoïde

            //srcX.at<float>(i, j) = image.cols - j - 1; // Inverse l'image horizontalement
            //srcY.at<float>(i, j) = i; // L'axe des y reste le même
        }
    }

    // Remap l'image
    cv::remap(image, result, srcX, srcY, cv::INTER_LINEAR);
}

int main()
{
    // Importe l'image
    cv::Mat image = cv::imread("boldt.jpg", cv::IMREAD_COLOR);

    // Vérifie si l'image est valide
    if(!image.data)
    {
        return 0;
    }

    // Crée une image pour le résultat
    cv::Mat result(image.rows, image.cols, image.type());

    // Applique l'effet de vague
    wave(image, result);

    // Affiche le résultat
    cv::namedWindow("Resultat");
    cv::imshow("Resultat", result);

    cv::waitKey(0);

    return 0;
}