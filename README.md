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
  
  * We can represent the above steps mathematically with the following equation:
  
    **Output[i, j] = sum(Input[x, y] * Kernel[x - i, y - j])**
    
    Where:
      - Output[i, j] is the intensity value of the pixel at position (i, j) in the output image.
      - Input[x, y] is the intensity value of the pixel at position (x, y) in the input image.
      - Kernel[x - i, y - j] is the weight of the kernel at position (x - i, y - j) relative to the center of the kernel.
      - The sum is taken over all x and y within the bounds of the kernel.
      - This equation can be used to calculate the intensity value of each pixel in the output image by sliding the kernel over the input image, multiplying the                 overlapping pixels by the corresponding weights in the kernel, and summing the results.
    
    
    
