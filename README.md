# EdgeDetection
Edge detection using C.
## Convolution Process:

* In image processing, convolution is a mathematical operation that is used to modify the intensity values of pixels in an image. It involves multiplying 
the intensity values of the pixels in the image by a set of coefficients, or "weights," and summing the results to produce a new intensity value for each pixel.

* Convolution is often used to apply various types of filters to an image, such as edge detection filters, blur filters, and sharpening filters. It is also used in
image compression, image recognition, and other applications.

* To perform convolution on an image, the following steps are typically followed:

  1. Define a convolution kernel, which is a small matrix of weights that will be used to modify the intensity values of the pixels.
  2. Slide the kernel over the image, one pixel at a time, and multiply the intensity values of the overlapping pixels by the corresponding weights in the kernel.
  3. Sum the results of the multiplications to produce a new intensity value for the center pixel of the kernel.
  4. Repeat the process for all pixels in the image.
  5. The resulting image will have the same size as the original image, but the intensity values of the pixels will have been modified according to the weights in the        convolution kernel.
