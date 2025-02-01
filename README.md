# Cours d'analyse d'images

## Pour les vidéos et les devoirs 01 & 02

Commande pour compiler sur windows
```
g++ OpenCV.cpp -o OpenCV -lopencv_core4100 -lopencv_imgcodecs4100 -lopencv_highgui4100 -lopencv_imgproc4100 -lopencv_features2d4100
```
  
Commande pour compiler sur mac
```
g++ OpenCV.cpp -o OpenCV `pkg-config --cflags --libs opencv4`
```


## Pour le devoir 03

Commande pour compiler sur windows
```
make
```
  
Commande pour compiler sur mac  
Il faut avant modifier le Makefile et remplacer à la ligne 22 :
```
-lopencv_core4100 -lopencv_imgcodecs4100 -lopencv_highgui4100 -lopencv_imgproc4100 -lopencv_features2d4100
```
par
```
`pkg-config --cflags --libs opencv4`
```
  
Puis executer la commande suivante :
```
make
```
  
Ce qui créer un fichier executable "main".