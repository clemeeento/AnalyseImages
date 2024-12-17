#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// Exemple 1 : colorReduce
// - Modifie directement l'image originale.
// - Réduit les couleurs en divisant les valeurs des pixels par "div",
//   en les multipliant à nouveau par "div", et en ajoutant div/2 pour centrer la couleur.
// - Simple mais non optimisé pour les grandes images ou images continues.
void colorReduce(cv::Mat image, int div = 64) 
{
    int nl = image.rows; // Nombre de lignes
    int nc = image.cols * image.channels(); // Nombre de colonnes * nombre de canaux, pour avoir le nombre de colonnes total

    for (int j = 0; j < nl; j++) 
    {
        uchar* data = image.ptr<uchar>(j); // Pointeur sur la ligne j, pour accéder aux valeurs des pixels

        for (int i = 0; i < nc; i++) 
        {
            data[i] = data[i] / div * div + div / 2; // Réduction de couleur : divise par div, multiplie par div et ajoute div/2

            // Exemple : div = 64, data[i] = 100
            // 100 / 64 = 1
            // 1 * 64 = 64
            // 64 + 32 = 96

            // Autre manière de faire
            //*data = *data / div * div + div / 2; 
        }
    }
}

// Exemple 2 : colorReduceIO
// - Non destructive : utilise une image d'entrée et une image de sortie.
// - Double accès mémoire : lecture de l'image source, écriture dans l'image de sortie.
// - Convient pour des pipelines où l'image d'origine doit rester intacte.
void colorReduceIO(const cv::Mat &image, cv::Mat &result, int div = 64) 
{
    int nl = image.rows; // Nombre de lignes
    int nc = image.cols; // Nombre de colonnes
    int nChannels = image.channels(); // Nombre de canaux

    result.create(image.rows, image.cols, image.type()); // Crée une image de la même taille et du même type que l'image d'entrée

    for (int j = 0; j < nl; j++) 
    {
        const uchar* data_in = image.ptr<uchar>(j); // Pointeur sur la ligne j de l'image d'entrée
        uchar* data_out = result.ptr<uchar>(j); // Pointeur sur la ligne j de l'image de sortie

        for (int i = 0; i < nc * nChannels; i++) 
        {
            data_out[i] = data_in[i] / div * div + div / 2; // Réduction de couleur, divise par div, multiplie par div et ajoute div/2
        }
    }
}

// Exemple 3 : colorReduce6
// - Optimisé pour les images continues.
// - Utilise un masque binaire pour remplacer les divisions par des opérations logiques plus rapides.
// - Traite les images continues comme un tableau 1D pour plus d'efficacité.
void colorReduce6(cv::Mat image, int div = 64) 
{
    int nl = image.rows; // Nombre de lignes
    int nc = image.cols * image.channels(); // Nombre de colonnes * nombre de canaux, pour avoir le nombre de colonnes total

    if(image.isContinuous()) // Si l'image est stockée en continu
    {
        // Traite toute l'image comme une seule ligne
        nc = nc * nl; // Nombre de colonnes * nombre de lignes
        nl = 1; // Nombre de lignes = 1
    }

    int n = static_cast<int>(log(static_cast<double>(div)) / log(2.0) + 0.5); // Logarithme en base 2 de div

    uchar mask = 0xFF << n; // Crée un masque de bits
    uchar div2 = div >> 1; // div 2 = div / 2

    for (int j = 0; j < nl; j++) 
    {
        uchar* data = image.ptr<uchar>(j); // Pointeur sur la ligne j, pour accéder aux valeurs des pixels

        for (int i = 0; i < nc; i++) 
        {
            *data &= mask; // Réduction par masque binaire (optimisation)
            *data++ += div2; // Ajoute div/2 pour centrer la couleur
        }
    }
}

// Exemple 4 : colorReduce7
// - Version la plus optimisée avec reshape pour traiter l'image comme un tableau 1D.
// - Combine `isContinuous` et `reshape` pour éviter les itérations sur les lignes.
// - Utilise un masque binaire pour réduire les couleurs.
void colorReduce7(cv::Mat image, int div = 64)
{
    if(image.isContinuous()) // Si l'image est stockée en continu
    {
        image.reshape(1, 1); // Redimensionne l'image pour la traiter comme un tableau 1D
    }

    int nl = image.rows; // Nombre de lignes (1 car reshaped)
    int nc = image.cols * image.channels(); // Nombre de colonnes * nombre de canaux, pour avoir le nombre de colonnes total

    int n = static_cast<int>(log(static_cast<double>(div)) / log(2.0) + 0.5); // Logarithme en base 2 de div

    uchar mask = 0xFF << n; // Crée un masque de bits
    uchar div2 = div >> 1; // div 2 = div / 2

    for (int j = 0; j < nl; j++) 
    {
        uchar* data = image.ptr<uchar>(j); // Pointeur sur la ligne j, pour accéder aux valeurs des pixels

        for (int i = 0; i < nc; i++) 
        {
            *data &= mask; // Réduction par masque binaire (optimisation)
            *data++ += div2; // Ajoute div/2 pour centrer la couleur
        }
    }
}

// Exemple 5 : colorReduce10
// - Utilise des itérateurs pour parcourir l'image.
// - Réduit les couleurs en utilisant un masque binaire et l'ajout de div/2.
// - Convient pour une approche plus orientée "C++ moderne" avec des itérateurs.
void colorReduce10(cv::Mat image, int div = 64)
{
    int n = static_cast<int>(log(static_cast<double>(div)) / log(2.0) + 0.5); // Logarithme en base 2 de div

    uchar mask = 0xFF << n; // Crée un masque de bits
    uchar div2 = div >> 1; // div 2 = div / 2

    cv::Mat_<cv::Vec3b>::iterator it = image.begin<cv::Vec3b>(); // Itérateur sur le début de l'image
    cv::Mat_<cv::Vec3b>::iterator itend = image.end<cv::Vec3b>(); // Itérateur sur la fin de l'image

    // Autre manière de faire
    // cv::Mat_<cv::Vec3b> cimage = image; // Convertit l'image en Mat_<cv::Vec3b>
    // cv::Mat_<cv::Vec3b>::iterator itend = cimage.begin(); // Itérateur sur le début de l'image
    // cv::Mat_<cv::Vec3b>::iterator it = cimage.end(); // Itérateur sur la fin de l'image

    for(; it != itend; ++it)
    {   
        // (*it) est un vecteur de 3 canaux
        // (*it)[0] est le premier canal
        (*it)[0] &= mask; // Réduction par masque binaire (optimisation)
        (*it)[0] += div2; // Ajoute div/2 pour centrer la couleur

        // (*it)[1] est le deuxième canal
        (*it)[1] &= mask; // Réduction par masque binaire (optimisation)
        (*it)[1] += div2; // Ajoute div/2 pour centrer la couleur
        
        // (*it)[2] est le troisième canal
        (*it)[2] &= mask; // Réduction par masque binaire (optimisation)
        (*it)[2] += div2; // Ajoute div/2 pour centrer la couleur

        // Autre manière de faire
        // (*it)[0] = (*it)[0] / div * div + div / 2; // Réduction de couleur
        // (*it)[1] = (*it)[1] / div * div + div / 2; // Réduction de couleur
        // (*it)[2] = (*it)[2] / div * div + div / 2; // Réduction de couleur
    }
}

// Exemple 6 : colorReduce12
// - Utilise la méthode `at` pour accéder aux pixels directement.
// - Réduit les couleurs en divisant, multipliant, et ajoutant div/2.
// - Moins performante pour les grandes images mais très lisible.
void colorReduce12(cv::Mat image, int div = 64)
{
    int nl = image.rows; // Nombre de lignes
    int nc = image.cols; // Nombre de colonnes  

    uchar div2 = div >> 1; // div 2 = div / 2

    for(int j = 0; j < nl; j++)
    {
       for(int i = 0; i < nc; i++)
       {    
            // Utilise l'opérateur at pour accéder aux valeurs des pixels
            image.at<cv::Vec3b>(j, i)[0] = image.at<cv::Vec3b>(j, i)[0] / div * div + div2; // Réduction de couleur
            image.at<cv::Vec3b>(j, i)[1] = image.at<cv::Vec3b>(j, i)[1] / div * div + div2; // Réduction de couleur
            image.at<cv::Vec3b>(j, i)[2] = image.at<cv::Vec3b>(j, i)[2] / div * div + div2; // Réduction de couleur
       }
    }
}

// Version la plus optimisée avec reshape pour traiter l'image comme un tableau 1D.
void colorReduceOptimized(cv::Mat image, int div = 64)
{
    if (image.isContinuous()) 
    {
        image = image.reshape(1, 1); // Traite comme un tableau 1D
    }

    int total = image.rows * image.cols * image.channels(); // Nombre total de pixels
    uchar div2 = div >> 1; // div / 2

    uchar* data = image.ptr<uchar>(0);
    for (int i = 0; i < total; i++) 
    {
        data[i] = data[i] / div * div + div2;
    }
}


// Exemple pour mesurer le temps d'exécution
// const int64 start = cv::getTickCount(); // Démarre le compteur
// colorReduce10(image); // Appelle la fonction
// const duration = (cv::getTickCount() - start) / cv::getTickFrequency(); // Calcule la durée

int main() 
{   
    // Importe l'image
    cv::Mat image = cv::imread("boldt.jpg");

    // Inistalise le compteur
    int64 start;

    // Exemple 1
    // Démarre le compteur
    start = cv::getTickCount();
    // Copie de l'image
    cv::Mat imageClone = image.clone(); 
    // Réduction de couleur
    colorReduce(imageClone);
    // Calcule la durée
    double duration = (cv::getTickCount() - start) / cv::getTickFrequency();
    // Affiche la durée
    std::cout << "Temps d'execution image 1 : " << duration << " s" << std::endl;
    // Affiche l'image
    cv::imshow("Image 1", imageClone);
    
    // Exemple 2
    // Démarre le compteur
    start = cv::getTickCount();
    // Crée une image de sortie
    cv::Mat result;
    // Réduction de couleur
    colorReduceIO(image, result);
    // Calcule la durée
    duration = (cv::getTickCount() - start) / cv::getTickFrequency();
    // Affiche la durée
    std::cout << "Temps d'execution image 2 : " << duration << " s" << std::endl;
    // Affiche l'image
    cv::imshow("Image 2", result);

    // Exemple 3
    // Démarre le compteur
    start = cv::getTickCount();
    // Copie de l'image
    imageClone = image.clone();
    // Réduction de couleur
    colorReduce6(imageClone);
    // Calcule la durée
    duration = (cv::getTickCount() - start) / cv::getTickFrequency();
    // Affiche la durée
    std::cout << "Temps d'execution image 3 : " << duration << " s" << std::endl;
    // Affiche l'image
    cv::imshow("Image 3", imageClone);

    // Exemple 4
    // Démarre le compteur
    start = cv::getTickCount();
    // Copie de l'image
    imageClone = image.clone();
    // Réduction de couleur
    colorReduce7(imageClone);
    // Calcule la durée
    duration = (cv::getTickCount() - start) / cv::getTickFrequency();
    // Affiche la durée
    std::cout << "Temps d'execution image 4 : " << duration << " s" << std::endl;
    // Affiche l'image
    cv::imshow("Image 4", imageClone);  

    // Exemple 5
    // Démarre le compteur
    start = cv::getTickCount();
    // Copie de l'image
    imageClone = image.clone();
    // Réduction de couleur
    colorReduce10(imageClone);
    // Calcule la durée
    duration = (cv::getTickCount() - start) / cv::getTickFrequency();
    // Affiche la durée
    std::cout << "Temps d'execution image 5 : " << duration << " s" << std::endl;
    // Affiche l'image
    cv::imshow("Image 5", imageClone);  

    // Exemple 6
    // Démarre le compteur
    start = cv::getTickCount();
    // Copie de l'image
    imageClone = image.clone();
    // Réduction de couleur
    colorReduce12(imageClone);
    // Calcule la durée
    duration = (cv::getTickCount() - start) / cv::getTickFrequency();
    // Affiche la durée
    std::cout << "Temps d'execution image 6 : " << duration << " s" << std::endl;
    // Affiche l'image
    cv::imshow("Image 6", imageClone);

    // Version la plus optimisée
    // Démarre le compteur
    start = cv::getTickCount();
    // Copie de l'image
    imageClone = image.clone();
    // Réduction de couleur
    colorReduceOptimized(imageClone);
    // Calcule la durée
    duration = (cv::getTickCount() - start) / cv::getTickFrequency();
    // Affiche la durée
    std::cout << "Temps d'execution image optimisee : " << duration << " s" << std::endl;
    // Affiche l'image
    cv::imshow("Image optimisee", imageClone);

    // Attend une touche
    cv::waitKey();

    return 0;
}

// Resultats :
// Temps d'execution image 1 : 0.0004061 s
// Temps d'execution image 2 : 0.0004306 s
// Temps d'execution image 3 : 0.0005392 s
// Temps d'execution image 4 : 0.0005351 s
// Temps d'execution image 5 : 0.0013053 s
// Temps d'execution image 6 : 0.0017975 s
// Temps d'execution image optimisee : 0.0003738 s