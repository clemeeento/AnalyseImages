#include "LaplacianZC.hpp"

int main()
{
    // Chargement de l'image en niveaux de gris
    cv::Mat image = cv::imread("boldt.jpg", 0);
    if (image.empty()) 
    {
        return 0; // Quitte si l'image n'est pas chargée
    }

    // Calcul et affichage des gradients Sobel dans les directions X et Y
    cv::Mat sobelX, sobelY;
    cv::Sobel(image, sobelX, CV_8U, 1, 0, 3, 0.4, 128); // Gradient horizontal
    cv::imshow("Sobel X Image", sobelX);

    cv::Sobel(image, sobelY, CV_8U, 0, 1, 3, 0.4, 128); // Gradient vertical
    cv::imshow("Sobel Y Image", sobelY);

    // Calcul des gradients Sobel avec précision 16 bits pour éviter la saturation
    cv::Sobel(image, sobelX, CV_16S, 1, 0); 
    cv::Sobel(image, sobelY, CV_16S, 0, 1);

    // Combine les gradients X et Y pour obtenir la magnitude approximative
    cv::Mat sobel = abs(sobelX) + abs(sobelY);

    // Normalisation pour adapter les valeurs du gradient à une plage de visualisation
    double sobmin, sobmax;
    cv::minMaxLoc(sobel, &sobmin, &sobmax);

    cv::Sobel(image, sobelX, CV_8U, 1, 0, 7, 0.001, 128); // Gradient horizontal avec noyau 7x7
    cv::imshow("Sobel X Image (7x7)", sobelX);

    // Conversion du gradient en une image 8 bits
    cv::Mat sobelImage;
    sobel.convertTo(sobelImage, CV_8U, -255.0 / sobmax, 255);
    cv::imshow("Sobel Image", sobelImage);

    // Application de seuils bas et élevés pour extraire les contours
    cv::Mat sobelThresholded;
    cv::threshold(sobelImage, sobelThresholded, 225, 255, cv::THRESH_BINARY); // Seuillage bas
    cv::imshow("Binary Sobel Image (low)", sobelThresholded);

    cv::threshold(sobelImage, sobelThresholded, 190, 255, cv::THRESH_BINARY); // Seuillage élevé
    cv::imshow("Binary Sobel Image (high)", sobelThresholded);

    // Calcul des contours avec Laplacian
    LaplacianZC laplacian;
    laplacian.setAperture(7); // Définition de l'ouverture pour le calcul de Laplacian

    cv::Mat flap = laplacian.computeLaplacian(image); // Calcul du Laplacian
    double lapmin, lapmax;
    cv::minMaxLoc(flap, &lapmin, &lapmax); // Récupère les valeurs min et max du Laplacian

    cv::Mat laplace = laplacian.getLaplacianImage(); // Convertit en une image visualisable
    cv::imshow("Laplacian Image (7x7)", laplace);

    // Affiche les valeurs Laplacian dans une région spécifique
    int cx = 238, cy = 90, dx = 12, dy = 12;
    std::cout << "Image values:\n";
    for (int i = 0; i < dx; i++)
    {
        for (int j = 0; j < dy; j++)
        {
            std::cout << std::setw(5) << static_cast<int>(image.at<uchar>(i + cy, j + cx)) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "Laplacian value range = [" << lapmin << ", " << lapmax << "]\n";

    // Calcul et affichage des points de zéro-croisement
    cv::Mat zeros = laplacian.getZeroCrossings(flap);
    cv::imshow("Zero-crossings", 255 - zeros);

    // Difference of Gaussian (DoG) pour extraire les contours à différentes échelles
    cv::Mat gauss20, gauss22, dog;
    cv::GaussianBlur(image, gauss20, cv::Size(), 2.0); // Flou avec sigma 2.0
    cv::GaussianBlur(image, gauss22, cv::Size(), 2.2); // Flou avec sigma 2.2

    cv::subtract(gauss22, gauss20, dog, cv::Mat(), CV_32F); // Calcul du DoG
    zeros = laplacian.getZeroCrossings(dog); // Points de zéro-croisement sur DoG
    cv::imshow("Zero-crossings of DoG", 255 - zeros);

    // Dessine un rectangle autour de la région étudiée
    cv::rectangle(image, cv::Rect(cx, cy, dx, dy), cv::Scalar(255, 255, 255));
    cv::imshow("Original Image with window", image);


    cv::waitKey(0);

    return 0;
}
