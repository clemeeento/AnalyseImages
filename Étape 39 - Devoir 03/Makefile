all: main Homework03.o EvaluateurContours.o Fonctions.o DetecteurContours.o MeilleursParametres.o Affichage.o

Homework03.o: Homework03.cpp
	g++ -c Homework03.cpp -Wall -Wextra

EvaluateurContours.o: EvaluateurContours.cpp EvaluateurContours.hpp
	g++ -c EvaluateurContours.cpp -Wall -Wextra

Fonctions.o: Fonctions.cpp Fonctions.hpp
	g++ -c Fonctions.cpp -Wall -Wextra

DetecteurContours.o: DetecteurContours.cpp DetecteurContours.hpp
	g++ -c DetecteurContours.cpp -Wall -Wextra

MeilleursParametres.o: MeilleursParametres.cpp MeilleursParametres.hpp
	g++ -c MeilleursParametres.cpp -Wall -Wextra

Affichage.o: Affichage.cpp Affichage.hpp
	g++ -c Affichage.cpp -Wall -Wextra

main: Homework03.o EvaluateurContours.o Fonctions.o DetecteurContours.o MeilleursParametres.o Affichage.o
	g++ -g -o main Homework03.o EvaluateurContours.o Fonctions.o DetecteurContours.o MeilleursParametres.o Affichage.o -lopencv_core4100 -lopencv_imgcodecs4100 -lopencv_highgui4100 -lopencv_imgproc4100 -lopencv_features2d4100 -Wall -Wextra

clean:
	rm -f main *.o

# -Werror
