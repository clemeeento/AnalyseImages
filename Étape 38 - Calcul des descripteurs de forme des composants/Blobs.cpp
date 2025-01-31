#include <iostream>
#include <vector>
#include <iomanip>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>

int main()
{
    // Charger l'image binaire en niveaux de gris
    cv::Mat image = cv::imread("binaryGroup.bmp", 0);
    if(image.empty())
    {
        return 0;
    }

    // Afficher l'image originale binaire
    cv::imshow("Original Binary Image", image);

    // Vecteur pour stocker les contours détectés
    std::vector<std::vector<cv::Point>> contours;

    // Détection des contours externes uniquement
    cv::findContours(image, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

    // Création d'une image blanche pour afficher les contours détectés
    cv::Mat result(image.size(), CV_8U, cv::Scalar(255));

    // Dessine tous les contours détectés en noir sur l'image blanche
    cv::drawContours(result, contours, -1, cv::Scalar(0), 2);

    // Affiche l'image avec les contours détectés
    cv::imshow("Contours", result);

    // Définition des tailles minimales et maximales des contours à conserver
    int cmin = 50;  // Taille minimale d'un contour
    int cmax = 500; // Taille maximale d'un contour

    // Parcours de tous les contours détectés
    std::vector<std::vector<cv::Point>>::iterator itc = contours.begin();
    while(itc != contours.end())
    {
        // Si la taille du contour est en dehors des limites, on le supprime
        if(itc->size() < cmin || itc->size() > cmax)
        {
            itc = contours.erase(itc); // Supprime et avance l'itérateur
        }
        else
        {
            ++itc; // Passe au contour suivant
        }
    }

    // Charger l'image originale en niveaux de gris
    cv::Mat original = cv::imread("group.jpg", 0);
    if(original.empty()) 
    {
        return 0;
    }

    // Dessine les contours filtrés (animaux) sur l'image originale en blanc
    cv::drawContours(original, contours, -1, cv::Scalar(255, 255, 255), 2);

    // Affiche l'image originale avec les contours dessinés
    cv::imshow("Contours on animals", original);

    // Réinitialise l'image de sortie en blanc
    result.setTo(cv::Scalar(255));

    // Dessine les contours filtrés sur une image blanche avec une épaisseur de 1 pixel
    cv::drawContours(result, contours, -1, 0, 1);

    // Recharger l'image binaire
    image = cv::imread("binaryGroup.bmp", 0);
    if(image.empty()) 
    {
        return 0;
    }

    // Détection des contours avec RETR_LIST (tous les contours sont détectés, y compris les imbriqués)
    cv::findContours(image, contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);

    // Réinitialisation de l'image de sortie en blanc
    result.setTo(cv::Scalar(255));

    // Dessine tous les contours détectés sur une image blanche avec une épaisseur de 1 pixel
    cv::drawContours(result, contours, -1, 0, 1);

    // Affiche tous les contours détectés
    cv::imshow("All contours", result);


    // Recharger l'image binaire pour poursuivre l'analyse des formes
    image = cv::imread("binaryGroup.bmp", 0);
    if(image.empty())
    {
        return 0;
    }

    // Encadrer le premier contour détecté avec un rectangle englobant
    cv::Rect r0 = cv::boundingRect(contours[0]);
    cv::rectangle(result, r0, 0, 2); // Dessine le rectangle en noir

    // Trouver le plus petit cercle englobant pour le deuxième contour
    float radius;
    cv::Point2f center;
    cv::minEnclosingCircle(contours[1], center, radius);
    cv::circle(result, center, static_cast<int>(radius), 0, 2); // Dessine le cercle en noir

    // Approximations polygonales des contours
    std::vector<cv::Point> poly;
    cv::approxPolyDP(contours[2], poly, 5, true);
    cv::polylines(result, poly, true, 0, 2); // Dessine un polygone

    // Calcul de l'enveloppe convexe du quatrième contour
    std::vector<cv::Point> hull;
    cv::convexHull(contours[3], hull);
    cv::polylines(result, hull, true, 0, 2); // Dessine l'enveloppe convexe

    // Parcours de chaque contour pour calculer ses moments géométriques
    itc = contours.begin();
    while(itc != contours.end())
    {
        cv::Moments mom = cv::moments(*itc++);
        cv::circle(result, cv::Point(mom.m10 / mom.m00, mom.m01 / mom.m00), 2, cv::Scalar(0), 2); // Centre de gravité
    }

    // Affichage des descripteurs de forme appliqués aux contours
    cv::imshow("Some Shape Descriptors", result);

    // Chargement de l'image contenant des composants connectés (MSER)
    cv::Mat components = cv::imread("mser.bmp", 0);
    if(components.empty())
    {
        return 0;
    }

    // Seuil pour convertir en binaire
    components = components == 255;

    // Appliquer une ouverture morphologique pour éliminer le bruit
    cv::morphologyEx(components, components, cv::MORPH_OPEN, cv::Mat(), cv::Point(-1, -1), 3);

    // Affichage de l'image après transformation morphologique
    cv::imshow("MSER image", components);

    // Nettoyage des anciens contours et calcul des contours sur l’image inversée
    contours.clear();
    cv::Mat componentsInv = 255 - components;
    cv::findContours(componentsInv, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

    // Création d'une image blanche pour afficher les quadrilatères détectés
    cv::Mat quadri(components.size(), CV_8U, 255);
    std::vector<std::vector<cv::Point>>::iterator it = contours.begin();

    // Parcours des contours détectés pour identifier les quadrilatères
    while(it != contours.end())
    {
        poly.clear();
        cv::approxPolyDP(*it, poly, 5, true); // Approximation polygonale du contour

        // Si le contour a 4 sommets, il est considéré comme un quadrilatère
        if(poly.size() == 4)
        {
            cv::polylines(quadri, poly, true, 0, 2); // Dessine le quadrilatère
        }
        ++it;
    }

    // Affichage des quadrilatères détectés
    cv::imshow("MSER quadrilateral", quadri);


    cv::waitKey(0);

    return 0;
}
