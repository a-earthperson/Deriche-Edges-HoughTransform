# Convex Polygon Detection using Linear Hough Transform
Polygon detection framework for bitmap images written in C that implements Otsu's thresholding and a Deriche edge detector for blurring, calculation of magnitude and gradient direction, non-maximum suppression and hysteresis thresholding. As a final step, it performs a Hough transform to detect the number and orientation of each polygon edge in the source image.

For a detailed overview, be sure to checkout the guide at the following link:
[Deriche-Edges-HoughTransform](https://arjun372.github.io/Deriche-Edges-HoughTransform/).

## The Problem
Given a bitmap image containing at-most one [convex polygon](https://en.wikipedia.org/wiki/Convex_polygon), determine the shape and orientation of said polygon. The polygon could be solid or wireframe, hand-drawn or computer generated, and the input image may contain significant background noise.

Some example images may look like:

image1.bmp | diamond.bmp | hexagon.bmp
------------ | ------------- | -------
![image1.bmp](examples/image1.bmp)|![diamond.bmp](examples/diamond.bmp)|![hexagon.bmp](examples/hexagon.bmp)
## Our Solution

### System Overview

#### Stage 1: Read Bitmap as Matrix

#### Stage 2: Deriche Blur Filter

#### Stage 3: Otsu's Thresholder

#### Stage 4: Deriche Gradient Filter

#### Stage 5: Hysteresis Thresholding

#### Stage 6: Hough Transform

#### Stage 7: Edge Count & Orientation

### Examples

## Results

## Build Instructions
