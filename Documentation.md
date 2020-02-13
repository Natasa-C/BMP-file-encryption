# Documentation

## Project theme
Cryptography and digital image processing are two very important areas of mathematics and computer science. The project theme combines two problems from these sub-domains, namely the encryption and decryption of a message (customized in this project to an image) and the recognition of patterns (customized in this project to handwritten numbers) in an image.

The scenario to be implemented in this project is the following: person A sends to person B an image I encrypted using an encryption algorithm. B can decrypt the encrypted image received from A thus obtaining the initial image I on which applies an algorithm to recognize the handwritten figures in the image I. Person B then sends to the person A the resulting image that he encrypts. Person A decrypts the encrypted image received from B and can view the solution, recognizing the numbers written by hand. The theme of the project is the implementation of the encryption / decryption and recognition modules of handwritten numbers which are going to be integrated into a final program.

In this project we will be working with color images, which we can manipulate in C language as binary files. Images are in BMP (bitmap) format. Unlike other formats (JPG, JPEG, PNG, etc.) the BMP format does not compress images, but stores 3 bytes per pixel for color images. This feature makes the BMP format suitable for this project as we can explicitly access the pixel intensity values that make up the color image. To view the images on our personal computer we must have a specific program installed (IrfanView, Paint, Gimp, Preview, ImageJ etc.).

The project will be structured in two modules. The two modules to be implemented are: the encryption module that deals with the problem of encryption / decryption of a message and the pattern recognition module that deals with the recognition of handwritten numbers in an image using the template matching method.

## BMP format
BMP format (bitmap) is a file format used to store two-dimensional digital images of arbitrary width, height and resolution, monochrome or color. In this project you will only work with color images. Basically, in BMP format, the image is viewed as a pixel array, and each pixel is a full value. The first 3 bytes of a pixel representation are the intensity of the 3 color channels R (Red), G (green), B (blue). Consequently, the intensity of each color channel R, G, B is given by a natural value between 0 and 255. For example, a pixel with values (0, 0, 0) represents a black pixel, and a pixel with a black color the values (255, 255, 255) represent a white pixel. At [RGB Color Codes Chart](https://www.rapidtables.com/web/color/RGB_Color.html) you can find the correspondence between RGB triplets and colors.

The BMP format is described here: [BMP file format](https://en.wikipedia.org/wiki/BMP_file_format) in a comprehensive way. The BMP format includes a fixed-size data area, called a header, and a variable-size data area that contains the pixels of the image itself. The header, which occupies the first 54 bytes of the file, contains information about the BMP format, as well as information about the image size, the number of bytes used to represent a pixel, etc.

The size of the image in bytes is specified in the header by an unsigned integer value, so it is stored on 4 bytes, starting with the byte with the order number 2. The image size in pixels is expressed as W × H, where W represents the number of pixels per width, and H represents the number of pixels per height. The image width expressed in pixels is stored on four unsigned bytes starting with the 18th byte in the header, and the height is stored on the next 4 unsigned bytes in the header, respectively starting with the 22nd octet in the header.

For fast processing of images in reading and writing, BMP images have the property that each line has a number of bytes that is a multiple of 4. This is achieved by adding padding bytes so that the total number of bytes on a line becomes multiple of 4. The number of bytes of a line is 3 × width (3 bytes per pixel on a line). Thus, if an image has 11 pixels in width (as with the templates we will work with) the number of bytes of padding is 3 (3 × 11 = 33 bytes per line, so at the end of each line, 3 padding bytes will be added so that we have 33 + 3 = 36 multiple of 4 bytes). Usually the padding bytes added are 0. 

Because the encoding of a BMP image in a binary file follows the little-endian standard, the bytes corresponding to the 3 color channels R, G, B are stored from right to left, that is, in order B, G, R. A color image can be transformed (Figure 1) into a grayscale image by replacing the (R, G, B) values of each pixel with the values (R', G', B') defined as follows:

<p align="center">R' = G' = B' = [ 0.299 ∗ R + 0.587 ∗ G + 0.114 ∗ B]</p>

where by [x] we note the whole part of the real number x. The grayscale image obtained has the property that it also uses 3 bytes to represent the color of each pixel, but all the values on the 3 channels are equal.

![fig1](https://user-images.githubusercontent.com/57111995/74440714-31b14f80-4e77-11ea-9a73-ce1ddb057b31.png)


## The encryption / decryption module
Cryptography is a branch of mathematics that deals with the secure transmission of information. One of the desires of cryptography is to ensure the confidentiality of data, so that certain information can only be accessed by authorized persons. We achieve this goal by using an encryption process by which intelligible information (a text, an image, an audio file, etc.) is transformed into an unintelligible one. The inverse transformation, by which the encrypted information is returned to the original shape (intelligible) is performed by means of a decryption process. 

A symmetrical figure consists of a pair of algorithms that ensure the encryption and decryption processes, as well as a common secret key (known only to people who want to communicate with each other in a secure way) that controls the two processes.

Basically, the common secret key represents an entry date for encryption and decryption algorithms, so not knowing its exact value will lead to the impossibility of decrypting that message by an unauthorized person.

In the following, we will describe a simple symmetric cipher that can be used to ensure the confidentiality of an image. First, we will present some notations:

1) Be I = ( I<sub>i, j</sub>), 0&le; i &lt; H, 0&le; j&lt; W,  a color image with the width of W pixels and the height of H pixels in matrix form. The linearization of the image I implies the creation of a one-dimensional array L by aligning the lines of the two-dimensional picture I, from top to bottom.

matrix form                |  linearized form        
:-------------------------:|:-------------------------:
![ff (copy)](https://user-images.githubusercontent.com/57111995/74440755-45f54c80-4e77-11ea-8fd9-7835214fb874.png)   | ![ff (another copy)](https://user-images.githubusercontent.com/57111995/74440782-53123b80-4e77-11ea-8da2-a18916923c13.png)  



2) considering a color image I in linear form, the value of each pixel I<sub>k</sub> will be a triplet of unsigned bytes I<sub>k</sub> = (I<sub>k</sub><sup>R</sup>, I<sub>k</sub><sup>G</sup>, I<sub>k</sub><sup>B</sup>).

3) we will note with ⨁ the operation or-exclusively (XOR) between 2 unsigned bytes.

4) for 2 pixels P<sub>1</sub> = (P<sub>1</sub><sup>R</sup>, P<sub>1</sub><sup>G</sup>, P<sub>1</sub><sup>B</sup>) and P<sub>2</sub> = (P<sub>2</sub><sup>R</sup>, P<sub>2</sub><sup>G</sup>, P<sub>2</sub><sup>B</sup>) we will consider P<sub>1</sub>⨁P<sub>2</sub> to be the
pixel (P<sub>1</sub><sup>R</sup>⨁P<sub>2</sub><sup>R</sup>, P<sub>1</sub><sup>G</sup>⨁P<sub>2</sub><sup>G</sup>, P<sub>1</sub><sup>B</sup>⨁P<sub>2</sub><sup>B</sup>)

5) for a pixel P = (P<sup>R</sup>, P<sup>G</sup>, P<sup>B</sup>) and a 32-bit unsigned integer X consisting of unsigned bytes (X<sub>3</sub>, X<sub>2</sub>, X<sub>1</sub>, X<sub>0</sub>) we will note with P⨁X the pixel (P<sup>R</sup> ⨁X<sub>2</sub>, P<sup>G</sup>⨁X<sub>1</sub>, P<sup>B</sup> ⨁X<sub>0</sub>), so the most significant byte X<sub>3</sub> of X will not be
used.

6) a pseudo-random number generator is a deterministic algorithm that generates a sequence of numbers having statistical properties similar to those of a perfectly random sequence of numbers (that is, a sequence of numbers for which the probability of occurrence of a given value is independent of all values previously generated) starting from a seed value.  For example, the XORSHIFT32 generator proposed by George Marsaglia in 2003 generates unsigned 32-bit integers with very good pseudo-random character, using shift operations on bits and XOR [Xorshift](https://en.wikipedia.org/wiki/Xorshift).

### Encrypting steps:
Using the above notations, the encryption algorithm of a color image P (plain image) of size W × H in linear form P = (P<sub>0</sub>,  P<sub>1</sub> , ..., P<sub> W * H - 1</sub>) using a secret key S is the following:

- generate R: a 2 * W * H - 1 long sequence consisting of 32-bit unsigned random integers, using the XORSHIFT32 generator initialized with a non-zero value R<sub>0</sub>
- generate a random permutation σ of size W × H, using Durstenfeld's algorithm - the modern version of the [Fisher-Yates algorithm](https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle) and the pseudo-random numbers R<sub>1</sub>, ..., R<sub> W * H - 1</sub>
- the pixels of the image P are permuted according to the permutation σ, obtaining an intermediate image P'.  For example, considering the image P = (P<sub>0</sub>,  P<sub>1</sub> , P<sub>2</sub>, P<sub>3</sub>)  and the permutation σ = (<sup>0</sup><sub>3</sub><sup>1</sup><sub>0</sub> <sup>2</sup><sub>2</sub> <sup>3</sup><sub>1</sub>) we will obtain the image P' = (P<sub>1</sub>,  P<sub>3</sub> , P<sub>2</sub>, P<sub>0</sub>)   because the pair (k, σ (k)) indicates that the pixel at position k in the image P will be moved to the position σ(k) in the image P', that is P'<sub>σ(k)</sub> = P<sub>k</sub> for any k ∈ {0,1, ..., W ∗ H - 1}
- the encrypted image C = (C<sub>0</sub>, C<sub>1</sub>, ..., C<sub> W * H - 1</sub>) is obtained by applying the following relation of substitution to each pixel of the image P' = P<sub>0</sub>,  P<sub>1</sub> , ..., P<sub> W * H - 1</sub>:
![hh](https://user-images.githubusercontent.com/57111995/74433186-9a450000-4e68-11ea-8171-a183100e0404.png)

where SV (starting value) is a 32-bit non-zero integer.

Shared secret key of this cipher is composed of the R and SV, both non-zero unsigned 32-bit integers. To ensure a high level of security of this figure, the common secret key must be changed before each use of the figure with one that has not been used before!

### Decrypting steps:
The cipher being symmetrical, the decryption algorithm is complementary to the encryption. Thus, the decryption algorithm of a ciphered color image C of size W × H in linear form C = (C<sub>0</sub>, C<sub>1</sub>, ..., C<sub> W * H - 1</sub>)using a secret key S is the following: 

-  generate R: a 2 * W * H - 1 long sequence consisting of 32-bit unsigned random integers, using the XORSHIFT32 generator initialized with a non-zero value R<sub>0</sub>
- generate a random permutation σ of size W × H, using Durstenfeld's algorithm - the modern version of the [Fisher-Yates algorithm](https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle) and the pseudo-random numbers R<sub>1</sub>, ..., R<sub> W * H - 1</sub>. Then, calculate its inverse σ<sup>-1</sup>
- apply to every pixel in the encrypted image C = (C<sub>0</sub>, C<sub>1</sub>, ..., C<sub> W * H - 1</sub>) the inverse of the substitution relation used in the encryption process, obtaining an intermediate image C' = (C'<sub>0</sub>, C'<sub>1</sub>, ..., C'<sub> W * H - 1</sub>):

![jjjj](https://user-images.githubusercontent.com/57111995/74443856-94f1b080-4e7c-11ea-8cb0-7840ba08b118.png)

- decrypted image  D = (D<sub>0</sub>, D<sub>1</sub>, ..., D<sub>W * H - 1</sub>) is obtained permuting the pixels of image C' according to the permutation σ<sup>-1</sup> , respectively D<sub>σ(k)<sup>-1</sup></sub> = C'<sub>k</sub> for any k ∈ {0,1, ..., W ∗ H - 1}

The correctness of the decryption algorithm is very easy to prove, using the properties of XOR!
