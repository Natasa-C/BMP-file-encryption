# FBMP File Encryption

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
