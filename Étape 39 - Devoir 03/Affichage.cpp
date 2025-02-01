#include "Affichage.hpp"

// Constructeur
Affichage::Affichage()
{
    InitialiserTableaux();
}

// Fonctions pour initialiser les tableaux
void Affichage::InitialiserTableaux()
{
    nomsLignesSobel = {"Taille Noyau Sobel", "Taille Noyau Gaussien", "Performance", "Taux Faux Positifs", "Taux Faux Negatifs"}; // Noms des lignes pour le fichier CSV
    nomsLignesLaplace = {"Taille Noyau Laplace", "Taille Noyau Gaussien", "Performance", "Taux Faux Positifs", "Taux Faux Negatifs"}; // Noms des lignes pour le fichier CSV
    nomsLignesCanny = {"Seuil Bas", "Seuil Haut", "Taille Noyau Gaussien", "Performance", "Taux Faux Positifs", "Taux Faux Negatifs"}; // Noms des lignes pour le fichier CSV

    // Récupérer les noms des images
    RecupererNomsFichiers(nomsImages, "Originales");

    // Créer les tableaux d'images
    CreerTableauImages(tableauOriginales, "Originales");
    CreerTableauImages(tableauContours, "Contours");
}

// Fonction pour afficher les contours avec la méthode de Sobel (ChatGPT)

void Affichage::AfficherContoursSobel()
{
    std::cout << "Pour naviguer, appuyez sur \"q\" pour afficher l'image precedente et sur \"d\" pour l'image suivante (assurez-vous d'etre dans la bonne fenetre).\n";

    if (tableauOriginales.empty()){
        std::cerr << "Erreur : Aucune image originale disponible pour afficher les contours Sobel !" << std::endl;
        return;
    }

    indexImage = 0;
    cv::Mat imageContours;

    int tailleNoyauSobel = 3;
    int tailleNoyauGaussien = 5;

    cv::namedWindow("Contours Sobel", cv::WINDOW_AUTOSIZE);

    auto MiseAJourSobel = [](int, void *userData)
    {
        auto *params = static_cast<std::tuple<Affichage *, int *, int *, cv::Mat *> *>(userData);
        Affichage *affichage = std::get<0>(*params);
        int *tailleNoyauSobel = std::get<1>(*params);
        int *tailleNoyauGaussien = std::get<2>(*params);
        cv::Mat *imageContours = std::get<3>(*params);

        if (*tailleNoyauSobel % 2 == 0) (*tailleNoyauSobel)++;
        if (*tailleNoyauGaussien % 2 == 0) (*tailleNoyauGaussien)++;

        *imageContours = DetecteurContours::DetecterSobel(affichage->tableauOriginales[affichage->indexImage], *tailleNoyauSobel, *tailleNoyauGaussien);

        cv::imshow("Contours Sobel", *imageContours);
    };

    cv::createTrackbar("Noyau Sobel", "Contours Sobel", &tailleNoyauSobel, 7, MiseAJourSobel,
                        new std::tuple<Affichage *, int *, int *, cv::Mat *>(this, &tailleNoyauSobel, &tailleNoyauGaussien, &imageContours));

    cv::createTrackbar("Noyau Gaussien", "Contours Sobel", &tailleNoyauGaussien, 11, MiseAJourSobel,
                        new std::tuple<Affichage *, int *, int *, cv::Mat *>(this, &tailleNoyauSobel, &tailleNoyauGaussien, &imageContours));

    MiseAJourSobel(0, new std::tuple<Affichage *, int *, int *, cv::Mat *>(this, &tailleNoyauSobel, &tailleNoyauGaussien, &imageContours));

    while (true){
        int touche = cv::waitKey(0);

        if (touche == 27) // Touche ESC pour quitter
            break;
        else if (touche == 81 || touche == 'q') // Flèche gauche ← (ou 'q')
            indexImage = (indexImage - 1 + tableauOriginales.size()) % tableauOriginales.size();
        else if (touche == 83 || touche == 'd') // Flèche droite → (ou 'd')
            indexImage = (indexImage + 1) % tableauOriginales.size();

        MiseAJourSobel(0, new std::tuple<Affichage *, int *, int *, cv::Mat *>(this, &tailleNoyauSobel, &tailleNoyauGaussien, &imageContours));
    }

    cv::destroyWindow("Contours Sobel");
}



// Fonction pour afficher les contours avec la méthode de Laplace (ChatGPT)
void Affichage::AfficherContoursLaplace()
{
    std::cout << "Pour naviguer, appuyez sur \"q\" pour afficher l'image precedente et sur \"d\" pour l'image suivante (assurez-vous d'etre dans la bonne fenetre).\n";

    if (tableauOriginales.empty()){
        std::cerr << "Erreur : Aucune image originale disponible pour afficher les contours Laplace !" << std::endl;
        return;
    }

    indexImage = 0;
    cv::Mat imageContours;

    int tailleNoyauLaplace = 3;
    int tailleNoyauGaussien = 5;

    cv::namedWindow("Contours Laplace", cv::WINDOW_AUTOSIZE);

    auto MiseAJourLaplace = [](int, void *userData)
    {
        auto *params = static_cast<std::tuple<Affichage *, int *, int *, cv::Mat *> *>(userData);
        Affichage *affichage = std::get<0>(*params);
        int *tailleNoyauLaplace = std::get<1>(*params);
        int *tailleNoyauGaussien = std::get<2>(*params);
        cv::Mat *imageContours = std::get<3>(*params);

        if (*tailleNoyauLaplace % 2 == 0) (*tailleNoyauLaplace)++;
        if (*tailleNoyauGaussien % 2 == 0) (*tailleNoyauGaussien)++;

        *imageContours = DetecteurContours::DetecterLaplace(affichage->tableauOriginales[affichage->indexImage], *tailleNoyauLaplace, *tailleNoyauGaussien);

        cv::imshow("Contours Laplace", *imageContours);
    };

    cv::createTrackbar("Noyau Laplace", "Contours Laplace", &tailleNoyauLaplace, 7, MiseAJourLaplace,
                        new std::tuple<Affichage *, int *, int *, cv::Mat *>(this, &tailleNoyauLaplace, &tailleNoyauGaussien, &imageContours));

    cv::createTrackbar("Noyau Gaussien", "Contours Laplace", &tailleNoyauGaussien, 11, MiseAJourLaplace,
                        new std::tuple<Affichage *, int *, int *, cv::Mat *>(this, &tailleNoyauLaplace, &tailleNoyauGaussien, &imageContours));

    MiseAJourLaplace(0, new std::tuple<Affichage *, int *, int *, cv::Mat *>(this, &tailleNoyauLaplace, &tailleNoyauGaussien, &imageContours));

    while (true){
        int touche = cv::waitKey(0);

        if (touche == 27) // Touche ESC pour quitter
            break;
        else if (touche == 81 || touche == 'q') // Flèche gauche ← (ou 'q')
            indexImage = (indexImage - 1 + tableauOriginales.size()) % tableauOriginales.size();
        else if (touche == 83 || touche == 'd') // Flèche droite → (ou 'd')
            indexImage = (indexImage + 1) % tableauOriginales.size();

        MiseAJourLaplace(0, new std::tuple<Affichage *, int *, int *, cv::Mat *>(this, &tailleNoyauLaplace, &tailleNoyauGaussien, &imageContours));
    }

    cv::destroyWindow("Contours Laplace");
}


// Fonction pour afficher les contours avec la méthode de Canny (ChatGPT)
void Affichage::AfficherContoursCanny()
{
    std::cout << "Pour naviguer, appuyez sur \"q\" pour afficher l'image precedente et sur \"d\" pour l'image suivante (assurez-vous d'etre dans la bonne fenetre).\n";

    if (tableauOriginales.empty()){
        std::cerr << "Erreur : Aucune image originale disponible pour afficher les contours Canny !" << std::endl;
        return;
    }

    indexImage = 0;
    cv::Mat imageContours;

    int seuilBas = 100;
    int seuilHaut = 200;
    int tailleNoyauGaussien = 5;

    cv::namedWindow("Contours Canny", cv::WINDOW_AUTOSIZE);

    auto MiseAJourCanny = [](int, void *userData)
    {
        auto *params = static_cast<std::tuple<Affichage *, int *, int *, int *, cv::Mat *> *>(userData);
        Affichage *affichage = std::get<0>(*params);
        int *seuilBas = std::get<1>(*params);
        int *seuilHaut = std::get<2>(*params);
        int *tailleNoyauGaussien = std::get<3>(*params);
        cv::Mat *imageContours = std::get<4>(*params);

        if (*tailleNoyauGaussien % 2 == 0) (*tailleNoyauGaussien)++;

        *imageContours = DetecteurContours::DetecterCanny(affichage->tableauOriginales[affichage->indexImage], *seuilBas, *seuilHaut, *tailleNoyauGaussien);

        cv::imshow("Contours Canny", *imageContours);
    };

    cv::createTrackbar("Seuil Bas", "Contours Canny", &seuilBas, 255, MiseAJourCanny,
                        new std::tuple<Affichage *, int *, int *, int *, cv::Mat *>(this, &seuilBas, &seuilHaut, &tailleNoyauGaussien, &imageContours));

    cv::createTrackbar("Seuil Haut", "Contours Canny", &seuilHaut, 255, MiseAJourCanny,
                        new std::tuple<Affichage *, int *, int *, int *, cv::Mat *>(this, &seuilBas, &seuilHaut, &tailleNoyauGaussien, &imageContours));

    cv::createTrackbar("Noyau Gaussien", "Contours Canny", &tailleNoyauGaussien, 11, MiseAJourCanny,
                        new std::tuple<Affichage *, int *, int *, int *, cv::Mat *>(this, &seuilBas, &seuilHaut, &tailleNoyauGaussien, &imageContours));

    MiseAJourCanny(0, new std::tuple<Affichage *, int *, int *, int *, cv::Mat *>(this, &seuilBas, &seuilHaut, &tailleNoyauGaussien, &imageContours));

    while (true){
        int touche = cv::waitKey(0);

        if (touche == 27) // Touche ESC pour quitter
            break;
        else if (touche == 81 || touche == 'q') // Flèche gauche ← (ou 'q')
            indexImage = (indexImage - 1 + tableauOriginales.size()) % tableauOriginales.size();
        else if (touche == 83 || touche == 'd') // Flèche droite → (ou 'd')
            indexImage = (indexImage + 1) % tableauOriginales.size();

        // Mise à jour de l'image affichée
        MiseAJourCanny(0, new std::tuple<Affichage *, int *, int *, int *, cv::Mat *>(this, &seuilBas, &seuilHaut, &tailleNoyauGaussien, &imageContours));
    }

    // Fermer la fenêtre après sortie de la boucle
    cv::destroyWindow("Contours Canny");
}


// Fonction pour calculer les meilleurs paramètres des trois détecteurs de contours et les exporter dans un fichier CSV
void Affichage::CalculerMeilleursParametres()
{
    // Variables pour les meilleurs paramètres et résultats
    int meilleursParametresGenerauxSobel[tableauOriginales.size()][2];
    double resultatsMeilleursParametresGenerauxSobel[tableauOriginales.size()][3];

    int meilleursParametresGenerauxLaplace[tableauOriginales.size()][2];
    double resultatsMeilleursParametresGenerauxLaplace[tableauOriginales.size()][3];

    int meilleursParametresGenerauxCanny[tableauOriginales.size()][3];
    double resultatsMeilleursParametresGenerauxCanny[tableauOriginales.size()][3];

    double moyennesSobel[5];
    double moyennesLaplace[5];
    double moyennesCanny[6];

    // Trouver les meilleurs paramètres globaux pour Sobel
    MeilleursParametresGenerauxSobel(tableauOriginales, tableauContours, meilleursParametresGenerauxSobel, resultatsMeilleursParametresGenerauxSobel, moyennesSobel);

    // Trouver les meilleurs paramètres globaux pour Laplace
    MeilleursParametresGenerauxLaplace(tableauOriginales, tableauContours, meilleursParametresGenerauxLaplace, resultatsMeilleursParametresGenerauxLaplace, moyennesLaplace);

    // Trouver les meilleurs paramètres globaux pour Canny
    MeilleursParametresGenerauxCanny(tableauOriginales, tableauContours, meilleursParametresGenerauxCanny, resultatsMeilleursParametresGenerauxCanny, moyennesCanny);

    // Exporter les résultats des meilleurs paramètres globaux pour Sobel dans un fichier CSV
    ExportCSV("MeilleursParametresSobel.csv", meilleursParametresGenerauxSobel, resultatsMeilleursParametresGenerauxSobel, nomsLignesSobel, nomsImages, tableauOriginales.size());

    // Exporter les résultats des meilleurs paramètres globaux pour Laplace dans un fichier CSV
    ExportCSV("MeilleursParametresLaplace.csv", meilleursParametresGenerauxLaplace, resultatsMeilleursParametresGenerauxLaplace, nomsLignesLaplace, nomsImages, tableauOriginales.size());

    // Exporter les résultats des meilleurs paramètres globaux pour Canny dans un fichier CSV
    ExportCSVCanny("MeilleursParametresCanny.csv", meilleursParametresGenerauxCanny, resultatsMeilleursParametresGenerauxCanny, nomsLignesCanny, nomsImages, tableauOriginales.size());

    // Exporter les moyennes des résultats dans un fichier CSV
    ExportCSVMoyenne("Moyennes.csv", moyennesSobel, moyennesLaplace, moyennesCanny, nomsLignesSobel, nomsLignesLaplace, nomsLignesCanny);

    std::cout << "\n";
}

// Fonction pour afficher et gerer le menu
void Affichage::Menu()
{
    int choix = 0;        // Variable pour stocker le choix de l'utilisateur

    while (true)
    {
        // Affichage du menu dans le terminal
        std::cout << "\n================= MENU =================\n";
        std::cout << "1 - Afficher contours Sobel\n";
        std::cout << "2 - Afficher contours Laplace\n";
        std::cout << "3 - Afficher contours Canny\n";
        std::cout << "4 - Calculer les meilleurs parametres\n";
        std::cout << "5 - Quitter\n";
        std::cout << "========================================\n";
        std::cout << "Votre choix : ";
        std::cin >> choix;

        // Vérification de l'entrée utilisateur
        if (std::cin.fail()) 
        {
            std::cin.clear(); // Réinitialiser cin en cas d'erreur
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignorer l'entrée incorrecte
            std::cout << "\nEntree invalide, veuillez entrer un chiffre entre 1 et 5.\n\n";
            continue;
        }

        // Exécuter l'action choisie
        switch (choix)
        {
            case 1:
                std::cout << "\nAffichage des contours avec Sobel...\n\n";
                AfficherContoursSobel();
                break;

            case 2:
                std::cout << "\nAffichage des contours avec Laplace...\n\n";
                AfficherContoursLaplace();
                break;

            case 3:
                std::cout << "\nAffichage des contours avec Canny...\n\n";
                AfficherContoursCanny();
                break;

            case 4:
                std::cout << "\nCalcul des meilleurs parametres...\n\n";
                CalculerMeilleursParametres();
                break;

            case 5:
                std::cout << "\nFermeture du programme...\n";
                return; // Quitter le programme proprement

            default:
                std::cout << "\nChoix invalide, veuillez entrer un chiffre entre 1 et 5.\n\n";
                break;
        }
    }
}