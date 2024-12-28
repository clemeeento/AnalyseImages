#include "Integral.hpp"

using namespace std;

int main()
{
    // Charge une image 
    cv::Mat image = cv::imread("book.jpg", 0); // 0 pour charger l'image en niveaux de gris
    if (!image.data) 
    {
        return 0;
    }

    // Transpose l'image et effectue un flip vertical
    cv::transpose(image, image); // Transpose les axes de l'image
    cv::flip(image, image, 0);   // Inverse l'image sur l'axe vertical

    // Affiche l'image transformée
    cv::imshow("Original Image", image);

    // Applique un seuillage binaire fixe
    cv::Mat binaryFixed;
    cv::threshold(image, binaryFixed, 70, 255, cv::THRESH_BINARY); 
    // Pixels >= 70 deviennent blancs (255), les autres deviennent noirs (0)

    // Calcule l'image intégrale pour une analyse rapide des zones
    cv::Mat iimage;
    cv::integral(image, iimage, CV_32S); // Image intégrale (somme cumulée)

    // Définition des paramètres pour le seuillage adaptatif
    int blockSize = 21; // Taille du bloc local (doit être impair)
    int threshold = 10; // Valeur de seuil adaptatif

    int halfSize = blockSize / 2; // Moitié de la taille du bloc

    // Applique un seuillage binaire en utilisant l'image intégrale
    for (int j = halfSize; j < image.rows - halfSize; j++) // Parcourt les lignes
    {
        uchar* data = binaryFixed.ptr<uchar>(j); // Pointeur sur la ligne actuelle de l'image binaire
        int* idata1 = iimage.ptr<int>(j - halfSize); // Ligne supérieure de l'image intégrale
        int* idata2 = iimage.ptr<int>(j + halfSize + 1); // Ligne inférieure de l'image intégrale

        for (int i = halfSize; i < image.cols - halfSize; i++) // Parcourt les colonnes
        {
            // Calcule la somme moyenne des pixels dans le bloc local
            int sum = (idata2[i + halfSize + 1] - idata2[i - halfSize]
                      - idata1[i + halfSize + 1] + idata1[i - halfSize]) / (blockSize * blockSize);

            // Applique le seuil adaptatif à chaque pixel
            if (data[i] < (sum - threshold))
            {
                data[i] = 0; // Pixel noir
            }
            else
            {
                data[i] = 255; // Pixel blanc
            }
        }
    }

    // Applique un seuillage adaptatif avec OpenCV pour comparaison
    cv::Mat binaryAdaptative;

    // Mesure le temps nécessaire pour le seuillage adaptatif
    int64 time;
    time = cv::getTickCount();

    cv::adaptiveThreshold(image, binaryAdaptative, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, blockSize, threshold);

    time = cv::getTickCount() - time; // Temps écoulé

    // Applique un filtre moyen à l'image pour un seuillage basé sur le filtrage
    cv::Mat filtered;
    cv::Mat binaryFiltered;

    cv::boxFilter(image, filtered, CV_8U, cv::Size(blockSize, blockSize)); // Filtre moyen sur des blocs locaux
    binaryFiltered = image >= (filtered - threshold); // Compare chaque pixel avec le seuil

    // Affiche le temps nécessaire pour le seuillage adaptatif
    cout << "Time: " << time / cv::getTickFrequency() << endl;

    // Affiche les différentes versions de l'image
    cv::imshow("Original Image", image); // Image originale transformée
    cv::imshow("Binary Fixed Threshold", binaryFixed); // Seuillage fixe
    cv::imshow("Binary Adaptative Threshold", binaryAdaptative); // Seuillage adaptatif OpenCV
    cv::imshow("Binary Filtered", binaryFiltered); // Seuillage basé sur le filtrage


    cv::waitKey(0);

    return 0;
}