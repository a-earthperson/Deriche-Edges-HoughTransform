# Convex Polygon Detection using Linear Hough Transform
Polygon detection framework for bitmap images written in C that implements Otsu's thresholding and a Deriche edge detector for blurring, calculation of magnitude and gradient direction, non-maximum suppression and hysteresis thresholding. As a final step, it performs a Hough transform to detect the number and orientation of each polygon edge in the source image.

For a detailed overview, be sure to checkout the guide at the following link:
[Deriche-Edges-HoughTransform](https://arjun372.github.io/Deriche-Edges-HoughTransform/).

## Building
Clone the repo and run the following commands in shell, which should output an executable named ```edgedetect``` :
```
./configure && make
```

## Running Instructions
Once built, you can execute ```edgedetect``` like so:

```
./edgedetect examples/image1.bmp
```

### Optional arguments
```
--verbose            :
--write-csv          : Writes a CSV file for every intermediate step
--set-blur-alpha     : Blur alpha, greater than 0.0
--set-gradient-alpha : Gradient alpha, greater than 0.0
```

### Example usage

```
./edgedetect examples/diamond.bmp --set-blur-alpha 1.0 --set-gradient-alpha 10.0
```
