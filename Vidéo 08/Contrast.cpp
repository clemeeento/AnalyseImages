#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


// Fonction pour réduire le contraste d'une image
void sharpen(const cv::Mat &image, cv::Mat &result) 
{
    result.create(image.size(), image.type()); // Crée une image de la même taille et du même type que l'image d'entrée
    int nChannels = image.channels(); // Nombre de canaux

    for(int j = 1; j < image.rows - 1; j++) // Parcours les lignes de l'image d'entrée, sauf la première et la dernière car on ne peut pas accéder à la ligne précédente ou suivante
    {
        const uchar* previous = image.ptr<const uchar>(j - 1); // Pointeur sur la ligne j - 1 de l'image d'entrée, qui est la ligne précédente
        const uchar* current = image.ptr<const uchar>(j); // Pointeur sur la ligne j de l'image d'entrée, qui est la ligne actuelle
        const uchar* next = image.ptr<const uchar>(j + 1); // Pointeur sur la ligne j + 1 de l'image d'entrée, qui est la ligne suivante
        uchar* output = result.ptr<uchar>(j); // Pointeur sur la ligne j de l'image de sortie

        for(int i = nChannels; i < nChannels * (image.cols - 1); i++) 
        {
            *output++ = cv::saturate_cast<uchar>(5 * current[i] - current[i - nChannels] - current[i + nChannels] - previous[i] - next[i]); // Applique le filtre de renforcement
            // 5 * pixel central - pixel de gauche - pixel de droite - pixel du dessus - pixel du dessous

            // Exemple : pixel central = 100, pixel de gauche = 50, pixel de droite = 50, pixel du dessus = 50, pixel du dessous = 50
            // 5 * 100 - 50 - 50 - 50 - 50 = 250
        }
    }
}

// Autre manière pour réduire le contraste d'une image
void sharpen2D(const cv::Mat &image, cv::Mat &result) 
{
    // Crée le noyau de convolution
    cv::Mat kernel(3, 3, CV_32F, cv::Scalar(0));
    kernel.at<float>(1, 1) = 5.0;
    kernel.at<float>(0, 1) = -1.0;
    kernel.at<float>(2, 1) = -1.0;
    kernel.at<float>(1, 0) = -1.0;
    kernel.at<float>(1, 2) = -1.0;

    // Applique le filtre de renforcement
    cv::filter2D(image, result, image.depth(), kernel);
}

// Contours de l'image
void outline(const cv::Mat &image, cv::Mat &result) 
{   
    result.create(image.size(), image.type()); // Crée une image de la même taille et du même type que l'image d'entrée

    result.row(0).setTo(cv::Scalar(0)); // Met la première ligne de l'image de sortie à 0
    result.row(result.rows - 1).setTo(cv::Scalar(0)); // Met la dernière ligne de l'image de sortie à 0
    result.col(0).setTo(cv::Scalar(0)); // Met la première colonne de l'image de sortie à 0
    result.col(result.cols - 1).setTo(cv::Scalar(0)); // Met la dernière colonne de l'image de sortie à 0
}

int main() 
{   
    // Importe l'image
    cv::Mat image = cv::imread("boldt.jpg");

    // Crée une image de sortie
    cv::Mat result;

    // Exemple 1 : Réduction du contraste de l'image
    // Applique le filtre
    sharpen(image, result); 
    // Affiche l'image
    cv::imshow("Image 1", image);

    // Exemple 2 : Réduction du contraste de l'image avec les contours
    // Applique le filtre
    sharpen(image, result);
    // Applique les contours
    outline(image, result);
    // Affiche l'image
    cv::imshow("Image 2", image);

    // Exemple 3 : Réduction du contraste de l'image avec un autre filtre
    // Applique le filtre
    sharpen2D(image, result);
    // Affiche l'image
    cv::imshow("Image 3", image);

    // Attend une touche
    cv::waitKey();

    return 0;
}