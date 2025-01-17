#include "WatershedSegmentation.hpp"

int main()
{
    // Charger l'image originale
    cv::Mat image = cv::imread("group.jpg");
    if (!image.data)
    {
        return 0;
    }

    // Afficher l'image originale
    cv::imshow("Original Image", image);

    // Charger l'image binaire
    cv::Mat binary = cv::imread("binary.bmp", 0);
    if (!binary.data)
    {
        return 0;
    }

    // Afficher l'image binaire
    cv::imshow("Binary Image", binary);

    // Réduire le bruit et isoler l'avant-plan en appliquant une érosion
    cv::Mat fg;
    cv::erode(binary, fg, cv::Mat(), cv::Point(-1, -1), 4);
    cv::imshow("Foreground Image", fg);

    // Identifier l'arrière-plan en dilatant les objets et en inversant l'image
    cv::Mat bg;
    cv::dilate(binary, bg, cv::Mat(), cv::Point(-1, -1), 4);
    cv::threshold(bg, bg, 1, 128, cv::THRESH_BINARY_INV);
    cv::imshow("Background Image", bg);

    // Combiner l'avant-plan et l'arrière-plan pour créer les marqueurs
    cv::Mat markers(binary.size(), CV_8U, cv::Scalar(0));
    markers = fg + bg;
    cv::imshow("Markers", markers);

    // Créer un objet pour la segmentation par Watershed
    WatershedSegmentation segmenter;

    // Définir les marqueurs et effectuer la segmentation
    segmenter.setMarkers(markers);
    segmenter.process(image);

    // Récupérer et afficher les résultats de la segmentation
    cv::Mat segmentation = segmenter.getSegmentation();
    cv::imshow("Segmentation", segmentation);

    // Récupérer et afficher les lignes de partage des eaux
    cv::Mat watersheds = segmenter.getWatersheds();
    cv::imshow("Watersheds", watersheds);

    // Charger une autre image pour un exemple supplémentaire
    image = cv::imread("tower.jpg");
    if (!image.data)
    {
        return 0;
    }

    // Créer un masque pour les marqueurs sur cette nouvelle image
    cv::Mat imageMask(image.size(), CV_8U, cv::Scalar(0));
    cv::rectangle(imageMask, cv::Point(5, 5), cv::Point(image.cols - 5, image.rows - 5), cv::Scalar(255), -1); // Bordures
    cv::rectangle(imageMask, cv::Point(image.cols / 2 - 10, image.rows / 2 - 10), cv::Point(image.cols / 2 + 10, image.rows / 2 + 10), cv::Scalar(1), 10); // Centre

    // Appliquer les marqueurs et segmenter l'image
    segmenter.setMarkers(imageMask);
    segmenter.process(image);

    // Dessiner les rectangles sur l'image pour visualiser les marqueurs
    cv::rectangle(image, cv::Point(5, 5), cv::Point(image.cols - 5, image.rows - 5), cv::Scalar(255, 255, 255), 2); // Bordure blanche
    cv::rectangle(image, cv::Point(image.cols / 2 - 10, image.rows / 2 - 10), cv::Point(image.cols / 2 + 10, image.rows / 2 + 10), cv::Scalar(1, 1, 1), 10); // Centre

    // Afficher l'image avec les marqueurs
    cv::imshow("Image avec le marker", image);

    // Afficher le résultat du Watershed sur cette nouvelle image
    cv::imshow("Watershed", segmenter.getWatersheds());

    cv::waitKey(0);

    return 0;
}
