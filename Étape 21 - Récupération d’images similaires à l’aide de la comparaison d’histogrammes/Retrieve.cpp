#include "ImageComparator.hpp"
using namespace std;

int main()
{
    // Charge l'image de référence
    cv::Mat image = cv::imread("waves.jpg");
    if (!image.data)
    {
        return 0;
    }

    // Affiche l'image de référence
    cv::imshow("Query Image", image);

    // Crée un comparateur d'images
    ImageComparator c;

    // Définit l'image de référence pour la comparaison
    c.setReferenceImage(image);

    // Charge et compare la première image ("dog.jpg") avec l'image de référence
    cv::Mat input = cv::imread("dog.jpg");
    cout << "Waves vs dog : " << c.compare(input) << endl; // Affiche la similarité

    // Charge et compare la deuxième image ("marais.jpg") avec l'image de référence
    input = cv::imread("marais.jpg");
    cout << "Waves vs marais : " << c.compare(input) << endl; // Affiche la similarité

    // Charge et compare la troisième image ("bear.jpg") avec l'image de référence
    input = cv::imread("bear.jpg");
    cout << "Waves vs bear : " << c.compare(input) << endl; // Affiche la similarité


    cv::waitKey();

    return 0;
}