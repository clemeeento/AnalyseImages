# Cours d'analyse d'images

Commande pour compiler sur windows
```
g++ OpenCV.cpp -o OpenCV -lopencv_core4100 -lopencv_imgcodecs4100 -lopencv_highgui4100 -lopencv_imgproc4100 -lopencv_features2d4100
```
  
Commande pour compiler sur mac
```
g++ OpenCV.cpp -o OpenCV `pkg-config --cflags --libs opencv4`
```