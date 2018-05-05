# Convex Polygon Detection using Linear Hough Transform
Polygon detection framework for bitmap images written in C that implements Otsu's thresholding and a Deriche edge detector for blurring, calculation of magnitude and gradient direction, non-maximum suppression and hysteresis thresholding. As a final step, it performs a Hough transform to detect the number and orientation of each polygon edge in the source image.

For a detailed overview, be sure to checkout the guide at the following link:
[Deriche-Edges-HoughTransform](https://arjun372.github.io/Deriche-Edges-HoughTransform/).

## The Problem
Given a bitmap image containing at-most one [convex polygon](https://en.wikipedia.org/wiki/Convex_polygon), determine the shape, orientation and side-lengths of said polygon. The polygon could be solid or wireframe, hand-drawn or computer generated, and the input image may contain significant background noise.

Some example images may look like:

image1.bmp | diamond.bmp | hexagon.bmp
------------ | ------------- | -------
![image1.bmp](examples/image1.bmp)|![diamond.bmp](examples/diamond.bmp)|![hexagon.bmp](examples/hexagon.bmp)
## Our Approach

### System Overview

#### Stage 1: Read Bitmap as Matrix

#### Stage 2: Deriche Blur Filter

#### Stage 3: Otsu's Thresholder

#### Stage 4: Deriche Gradient Filter

#### Stage 5: Hysteresis Thresholding

#### Stage 6: Hough Transform

#### Stage 7: Edge Count & Orientation

### Examples
STAGE|image1.bmp | diamond.bmp | hexagon.bmp
----|------------ | ------------- | -------
**READ**  |![image1.bmp](sample_outputs/image1/image1.gray.png)|![diamond.bmp](sample_outputs/diamond/diamond.gray.png)|![hexagon.bmp](sample_outputs/hexagon/hexagon.gray.png)
**BLUR**  |![image1.bmp](sample_outputs/image1/image1.blur.png)|![diamond.bmp](sample_outputs/diamond/diamond.blur.png)|![hexagon.bmp](sample_outputs/hexagon/hexagon.blur.png)
**OTSU**  |![image1.bmp](sample_outputs/image1/image1.otsu.png)|![diamond.bmp](sample_outputs/diamond/diamond.otsu.png)|![hexagon.bmp](sample_outputs/hexagon/hexagon.otsu.png)
**XGrad** |![image1.bmp](sample_outputs/image1/image1.xgrad.png)|![diamond.bmp](sample_outputs/diamond/diamond.xgrad.png)|![hexagon.bmp](sample_outputs/hexagon/hexagon.xgrad.png)
**YGrad** |![image1.bmp](sample_outputs/image1/image1.ygrad.png)|![diamond.bmp](sample_outputs/diamond/diamond.ygrad.png)|![hexagon.bmp](sample_outputs/hexagon/hexagon.ygrad.png)
**Grad**  |![image1.bmp](sample_outputs/image1/image1.gradOnly.png)|![diamond.bmp](sample_outputs/diamond/diamond.gradOnly.png)|![hexagon.bmp](sample_outputs/hexagon/hexagon.gradOnly.png)
**Hough** |![image1.bmp](sample_outputs/image1/image1.hough.png)|![diamond.bmp](sample_outputs/diamond/diamond.hough.png)|![hexagon.bmp](sample_outputs/hexagon/hexagon.hough.png)

## Results

## Build Instructions
