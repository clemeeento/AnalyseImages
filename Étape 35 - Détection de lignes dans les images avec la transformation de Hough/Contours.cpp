#include "Contours.hpp"
#include "LineFinder.hpp"

int main()
{
    // Chargement de l'image en niveaux de gris
    cv::Mat image = cv::imread("road.jpg", 0);
    if(image.empty())
    {
        return 0;
    }

    // Affichage de l'image originale
    cv::imshow("Original Image", image);

    // Création d'un détecteur de contours
    EdgeDetector ed;

    // Calcul des gradients de Sobel pour détecter les contours
    ed.computeSobel(image);

    // Sauvegarde de l'orientation des gradients dans une image
    cv::imwrite("ori.bmp", ed.getSobelOrientationImage());

    // Affichage des contours détectés avec Sobel, seuil bas et élevé
    cv::imshow("Sobel (low threshold)", ed.getBinaryMap(125));
    cv::imshow("Sobel (high threshold)", ed.getBinaryMap(350));

    // Détection des contours avec l'algorithme de Canny
    cv::Mat contours;
    cv::Canny(image, contours, 125, 350);

    // Affichage des contours détectés par Canny (inversé pour meilleure visibilité)
    cv::imshow("Canny Contours", 255 - contours);

    // Rechargement de l'image originale
    cv::Mat result = cv::imread("road.jpg", 0);
    if(result.empty())
    {
        return 0;
    }

    // Détection des lignes avec la transformée de Hough classique (HoughLines)
    std::vector<cv::Vec2f> lines;
    cv::HoughLines(contours, lines, 1, M_PI / 180, 60); // Résolution : 1 pixel, 1 degré, 60 votes min.

    // Parcours des lignes détectées
    std::vector<cv::Vec2f>::const_iterator it = lines.begin();
    while(it != lines.end())
    {
        float rho = (*it)[0];   // Distance par rapport à l'origine
        float theta = (*it)[1]; // Angle de la ligne

        // Vérifie si la ligne est principalement verticale
        if(theta < M_PI / 4 || theta > 3.0 * M_PI / 4.0)
        {
            // Calcule deux points pour tracer la ligne verticale
            cv::Point pt1(rho / cos(theta), 0); // Point de départ
            cv::Point pt2((rho - image.rows * sin(theta)) / cos(theta), result.rows); // Point d'arrivée
            cv::line(result, pt1, pt2, cv::Scalar(255), 1); // Dessine la ligne
        }
        else // La ligne est principalement horizontale
        {
            // Calcule deux points pour tracer la ligne horizontale
            cv::Point pt1(0, rho / sin(theta)); // Point de départ
            cv::Point pt2(result.cols, (rho - result.cols * cos(theta)) / sin(theta)); // Point d'arrivée
            cv::line(result, pt1, pt2, cv::Scalar(255), 1); // Dessine la ligne
        }
        ++it;
    }

    // Affichage des lignes détectées avec la transformée de Hough classique
    cv::imshow("Lines with Hough", result);

    // Utilisation de la transformée de Hough probabiliste avec la classe LineFinder
    LineFinder ld;
    ld.setLineLengthAndGap(100, 20); // Longueur min 100 pixels, espace max 20 pixels
    ld.setMinVote(60); // Nombre minimal de votes

    // Détection des lignes avec la méthode probabiliste
    std::vector<cv::Vec4i> li = ld.findLines(contours);
    ld.drawDetectedLines(image); // Dessine les lignes détectées

    // Affichage des lignes détectées avec Hough probabiliste
    cv::imshow("Lines with HoughP", image);

    // Création et affichage de l'accumulateur de Hough
    cv::Mat acc(200, 180, CV_8U, cv::Scalar(0));
    int x = 50, y = 30;

    // Génération de l'accumulateur de Hough pour une ligne donnée
    for(int i = 0; i < 180; i++)
    {
        double theta = i * M_PI / 180.0;       // Convertit l'angle en radians
        double rho = x * std::cos(theta) + y * std::sin(theta); // Calcule la valeur de rho
        int j = static_cast<int>(rho + 100.5); // Décalage pour éviter les valeurs négatives
        std::cout << i << "," << j << std::endl; // Affiche les coordonnées
        acc.at<uchar>(j, i)++;                 // Incrémente le compteur pour cet angle et cette distance
    }

    // Tracé des axes de l'accumulateur
    cv::line(acc, cv::Point(0, 0), cv::Point(0, acc.rows - 1), 255);
    cv::line(acc, cv::Point(acc.cols - 1, acc.rows - 1), cv::Point(0, acc.rows - 1), 255);

    // Sauvegarde de l'accumulateur de Hough
    cv::imwrite("hough1.bmp", 255 - (acc * 100));

    // Ajout d'une deuxième ligne au même accumulateur
    x = 30, y = 10;
    for(int i = 0; i < 180; i++)
    {
         double theta = i * M_PI / 180.0;       // Convertit l'angle en radians
        double rho = x * cos(theta) + y * sin(theta); // Calcule la valeur de rho
        int j = static_cast<int>(rho + 100.5); // Décalage pour éviter les valeurs négatives
        acc.at<uchar>(j, i)++;                 // Incrémente le compteur pour cet angle et cette distance
    }

    // Affichage de l'accumulateur de Hough
    cv::imshow("Hough Accumulator", acc * 100);

    // Détection de cercles avec la transformée de Hough circulaire
    image = cv::imread("chariot.jpg", 0);
    if(image.empty())
    {
        return 0;
    }

    // Applique un flou gaussien pour améliorer la détection des cercles
    cv::GaussianBlur(image, image, cv::Size(5, 5), 1.5);

    // Détection des cercles avec HoughCircles
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(image, circles, cv::HOUGH_GRADIENT, 2, 20, 200, 60, 15, 50);

    // Recharge l'image originale pour éviter toute modification accidentelle
    image = cv::imread("chariot.jpg", 0);
    if(image.empty())
    {
        return 0;
    }

    // Dessin des cercles détectés sur l'image
    std::vector<cv::Vec3f>::const_iterator itc = circles.begin();
    while(itc != circles.end())
    {
        // Dessine un cercle à la position détectée avec le rayon trouvé
        cv::circle(image, cv::Point((*itc)[0], (*itc)[1]), (*itc)[2], cv::Scalar(255), 2);
        ++itc;
    }

    // Affichage de l'image avec les cercles détectés
    cv::imshow("Detected Circles", image);


    cv::waitKey();

    return 0;
}
