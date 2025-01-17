#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>

int main()
{
    // Charger l'image en niveaux de gris
    cv::Mat image = cv::imread("building.jpg", 0); // 0 pour charger en niveaux de gris
    if (image.empty()) 
    {
        return 0; // Vérifie si l'image a été chargée correctement
    }

    // Affiche l'image originale
    cv::imshow("Image", image);

    // Initialiser l'objet MSER (Maximally Stable Extremal Regions)
    cv::Ptr<cv::MSER> ptrMSER = cv::MSER::create(5, 200, 2000); 
    // 5 : Delta pour MSER, 200 : Aire min des régions, 2000 : Aire max des régions

    // Conteneurs pour les points et rectangles détectés
    std::vector<std::vector<cv::Point>> points;
    std::vector<cv::Rect> rects;

    // Détecte les régions MSER
    ptrMSER->detectRegions(image, points, rects);

    // Crée une image blanche pour afficher les points MSER détectés
    cv::Mat output(image.size(), CV_8UC3, cv::Scalar(255, 255, 255));

    // Générateur de couleurs aléatoires
    cv::RNG rng;

    // Parcourt les points détectés et les dessine avec des couleurs aléatoires
    for (std::vector<std::vector<cv::Point>>::reverse_iterator it = points.rbegin(); it != points.rend(); ++it)
    {
        // Génère une couleur aléatoire
        cv::Vec3b c(rng.uniform(0, 254), rng.uniform(0, 254), rng.uniform(0, 254));

        // Colorie les points de la région si non déjà colorés
        for (std::vector<cv::Point>::iterator itPts = it->begin(); itPts != it->end(); ++itPts)
        {
            if (output.at<cv::Vec3b>(*itPts)[0] == 255)
            {
                // Si le point n'est pas déjà coloré, le colorie
                output.at<cv::Vec3b>(*itPts) = c;
            }
        }
    }

    // Affiche les ensembles de points MSER colorés
    cv::imshow("MSER point sets", output);
    cv::imwrite("mser.bmp", output); // Sauvegarde le résultat

    // Parcourt les rectangles associés aux régions MSER et les dessine
    std::vector<cv::Rect>::iterator itr = rects.begin();
    std::vector<std::vector<cv::Point>>::iterator itp = points.begin();

    for (; itr != rects.end(); ++itr, ++itp)
    {
        // Condition : Si la densité de points MSER dans le rectangle est suffisante
        if (static_cast<double>(itp->size()) / itr->area() > 0.6)
        {
            cv::rectangle(image, *itr, cv::Scalar(255), 2); // Dessine un rectangle blanc
        }
    }

    // Affiche les rectangles des régions MSER
    cv::imshow("Rectangular MSERs", image);

    // Recharge l'image pour travailler avec les ellipses
    image = cv::imread("building.jpg", 0);
    if (image.empty()) 
    {
        return 0;
    }

    // Dessine les ellipses minimales autour des points MSER
    for (std::vector<std::vector<cv::Point>>::iterator it = points.begin(); it != points.end(); ++it)
    {
        // Calcule le rectangle rotatif minimal englobant les points
        cv::RotatedRect rr = cv::minAreaRect(*it);

        // Filtre sur les proportions de l'ellipse
        if (rr.size.height / rr.size.width > 0.6 && rr.size.height / rr.size.width < 1.6)
        {
            cv::ellipse(image, rr, cv::Scalar(255), 2); // Dessine l'ellipse en blanc
        }
    }

    // Affiche les ellipses correspondant aux régions MSER
    cv::imshow("MSERs ellipses", image);

    // Attend une interaction utilisateur pour fermer les fenêtres
    cv::waitKey(0);

    return 0;
}
