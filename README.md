# Cours d'analyse d'images

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
  
Ce qui créer un fichier executable "main".