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


    // Recharge l'image originale en niveaux de gris
    image = cv::imread("road.jpg", 0);
    if (image.empty())
    {
        return 0;
    }

    // Détection des contours avec l'algorithme de Canny
    cv::Canny(image, contours, 125, 350);

    int n = 0; // Index de la ligne à traiter

    // Création d'une image noire de même taille que l'image originale
    cv::Mat oneline(image.size(), CV_8U, cv::Scalar(0));

    // Dessine une ligne blanche sur l'image noire à partir des coordonnées de la ligne détectée
    cv::line(oneline, 
            cv::Point(li[n][0], li[n][1]),  // Premier point de la ligne
            cv::Point(li[n][2], li[n][3]),  // Deuxième point de la ligne
            cv::Scalar(255));

    // Applique un "ET bit-à-bit" entre les contours détectés et l'image contenant une seule ligne
    cv::bitwise_and(contours, oneline, oneline);

    // Affiche l'image résultante avec une seule ligne détectée en inversant les couleurs (255 - oneline)
    cv::imshow("One line", 255 - oneline);

    // Vecteur pour stocker les points appartenant à la ligne détectée
    std::vector<cv::Point> points;

    // Parcours de tous les pixels de l'image "oneline"
    for (int y = 0; y < oneline.rows; y++)
    {
        uchar* rowPtr = oneline.ptr<uchar>(y); // Pointeur vers la ligne actuelle de l'image
        for (int x = 0; x < oneline.cols; x++)
        {
            if (rowPtr[x]) // Vérifie si le pixel est blanc (fait partie de la ligne)
            {
                points.push_back(cv::Point(x, y)); // Ajoute le point à la liste des points de la ligne
            }
        }
    }

    // Vecteur pour stocker les paramètres de la ligne ajustée
    cv::Vec4f line;

    // Ajuste une ligne aux points détectés en utilisant la méthode des moindres carrés
    cv::fitLine(points, line, cv::DIST_L2, 0, 0.01, 0.01);

    // Coordonnées d'un point central sur la ligne ajustée
    int x0 = line[2]; // Coordonnée X du point central
    int y0 = line[3]; // Coordonnée Y du point central

    // Calcul des coordonnées d'un deuxième point à 100 pixels du premier selon la direction du vecteur directeur
    int x1 = x0 + 100 * line[0]; // Nouvelle coordonnée X
    int y1 = y0 + 100 * line[1]; // Nouvelle coordonnée Y

    // Recharge l'image originale en niveaux de gris
    image = cv::imread("road.jpg", 0);
    if (image.empty())
    {
        return 0;
    }

    // Dessine la ligne ajustée sur l'image originale
    cv::line(image, cv::Point(x0, y0), cv::Point(x1, y1), 0, 2); // Dessin en noir, épaisseur = 2 pixels

    // Affiche l'image finale avec la ligne ajustée
    cv::imshow("Fitted line", image);


    cv::waitKey();

    return 0;
}
