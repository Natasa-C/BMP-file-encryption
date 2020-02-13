# File-Encryption

An algorithm for encrypting and decrypting bmp files. The encrypted images have a uniform distribution of pixel values in each color channel, so as to hide the uneven distribution of pixel values in the original image that can provide relevant statistical information.

## Compile and run
### encryption_script.c
> To compile:  	gcc encryption_script.c encryption_functions.c -o script

> To run:		    ./script

### encryption_verifier.c
> To compile:  	gcc encryption_verifier.c -o verifier

> To run:		    ./verifier

> Name of encrypted file to test: output/BMPencrypted.bmp 

### Output for the encryption_script.c

BMPencrypted.bmp           |  BMPdecrypted.bmp        
:-------------------------:|:-------------------------:
![BMPcriptat](https://user-images.githubusercontent.com/57111995/74319362-78278100-4d87-11ea-9b2f-16eb94137a67.jpg)  |  ![BMPdecriptat](https://user-images.githubusercontent.com/57111995/74319402-8b3a5100-4d87-11ea-8461-9c7afb2c4bea.jpg) |  
![chi](https://user-images.githubusercontent.com/57111995/74325582-89758b00-4d91-11ea-8dee-71f2daf85861.png) 

### Encrypting steps:
- generate R: a 2 * W * H - 1 long sequence consisting of 32-bit unsigned random integers, using the XORSHIFT32 generator initialized with a non-zero value R<sub>0</sub>
- generate a random permutation σ of size W × H, using Durstenfeld's algorithm - the modern version of the Fisher-Yates algorithm and the pseudo-random numbers R<sub>1</sub>, ..., R<sub> W * H - 1</sub>
- the pixels of the image P are permuted according to the permutation σ, obtaining an intermediate image P'
- the encrypted image C = (C<sub>0</sub>, C<sub>1</sub>, ..., C<sub> W * H - 1</sub>) is obtained by applying the following substitution relation to each pixel of the image P' = P<sub>0</sub>,  P<sub>1</sub> , ..., P<sub> W * H - 1</sub>:
![hh](https://user-images.githubusercontent.com/57111995/74433186-9a450000-4e68-11ea-8171-a183100e0404.png)


The figure being symmetrical, the decryption algorithm is complementary to the one of
encryption. 

### Decryption algorithm of an encrypted color image C(ciphered image) of dimension W × H in linear form C =
C! , C! , ..., C! ∗ !!! using a secret key S is the following:

## Resouces
- [peppers.bmp](https://homepages.cae.wisc.edu/~ece533/images/){ width: 200px; height: 100px; }









<table>
    <thead>
        <tr>
            <th>BMPencrypted.bmp </th>
            <th>BMPdecrypted.bmp </th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td>![Alt BMPcriptat](https://user-images.githubusercontent.com/57111995/74319362-78278100-4d87-11ea-9b2f-16eb94137a67.jpg)</td>
            <td>![BMPdecriptat](https://user-images.githubusercontent.com/57111995/74319402-8b3a5100-4d87-11ea-8461-9c7afb2c4bea.jpg)</td>
        </tr>
        <tr>
            <td colspan="2">![chi](https://user-images.githubusercontent.com/57111995/74325582-89758b00-4d91-11ea-8dee-71f2daf85861.png)</td>
        </tr>
    </tbody>
</table>
