#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

int main()
{
    // Importe l'image
    cv::Mat image = cv::imread("boldt.jpg");

    // Vérifie si l'image a été correctement chargée
    if(image.empty())
    {
        return 0;
    }

    // Définit un rectangle initial autour de la région d'intérêt (ROI)
    // Ce rectangle délimite approximativement la zone de premier plan à extraire
    cv::Rect rectangle(50, 25, 210, 180);

    // Matrices utilisées pour l'algorithme GrabCut
    cv::Mat result;         // Masque qui contient les étiquettes de chaque pixel (arrière-plan/premier plan)
    cv::Mat bgModel, fgModel; // Modèles pour les distributions des couleurs de l'arrière-plan et du premier plan

    // Applique l'algorithme GrabCut
    // L'algorithme segmente l'image en arrière-plan et premier plan à partir du rectangle initial
    cv::grabCut(image,         // Image source
                result,        // Masque contenant les informations de segmentation
                rectangle,     // Rectangle de départ pour la segmentation
                bgModel,       // Modèle de l'arrière-plan (mis à jour par GrabCut)
                fgModel,       // Modèle du premier plan (mis à jour par GrabCut)
                5,             // Nombre d'itérations pour l'optimisation
                cv::GC_INIT_WITH_RECT); // Mode : initialise avec le rectangle fourni

    // Modifie le masque pour conserver uniquement les pixels marqués comme premier plan probable ou certain
    cv::compare(result, cv::GC_PR_FGD, result, cv::CMP_EQ); // Ou bien : result = result & 1;

    // Crée une image blanche de la même taille que l'image source pour représenter le premier plan extrait
    cv::Mat foreground(image.size(), CV_8UC3, cv::Scalar(255, 255, 255));

    // Copie les pixels de l'image source correspondant au premier plan probable ou certain dans l'image de premier plan
    // Les pixels d'arrière-plan ne sont pas copiés
    image.copyTo(foreground, result);

    // Dessine un rectangle sur l'image
    // cv::rectangle(image, rectangle, cv::Scalar(255, 255, 255), 1);

    // Affiche l'image originale
    cv::imshow("Image", image);

    // Affiche le masque résultant après l'exécution de GrabCut (utilisé pour le debug)
    cv::imshow("Result", result);

    // Affiche l'image contenant uniquement le premier plan extrait
    cv::imshow("Foreground", foreground);

    cv::waitKey(0);

    return 0;
}