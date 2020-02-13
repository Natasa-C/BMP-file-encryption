#ifndef ENCRYPTION_FUNCTIONS
#define ENCRYPTION_FUNCTIONS

typedef struct
{
    unsigned char B, G, R;
} Pixel;

void allocateMemoryForUnsignedCharArrays(unsigned char **array, unsigned int length);
void allocateMemoryForPixelArrays(Pixel **array, unsigned int length);
void allocateMemoryForUnsignedIntArrays(unsigned int **array, unsigned int length);
void readWidthAndHeight(unsigned char *source_file_name, int *width, int *height);
void readR0andSV(unsigned char *key_file_name, unsigned int *R0, unsigned int *SV);
void loadLinearizedHeaderInInternalMemory(unsigned char *nume_fisier_sursa, unsigned char **vector);
void loadLinearizedBMPImageInInternalMemory(unsigned char *souce_file_name, Pixel **image_array, unsigned int W, unsigned int H);
void saveLinearizedBMPImageInExternalMemory(unsigned char *destination_file_name, unsigned char *header_array, Pixel *BMP_image_array, unsigned int W, unsigned int H);
void XORSHIFT32(unsigned int length, unsigned int seed, unsigned int **array);
void generateRandomPermutationDurstenfeld(unsigned int permutation_length, unsigned int *random_numbers_array, unsigned int **permutation);
void permuteThePixelsOfLinearizedImage(Pixel **BMP_array, unsigned int *permutation, unsigned int BMP_array_length);
void XORintegerXpixel(unsigned int integer, Pixel **pixel, unsigned int k);
void XORpixelXpixel(Pixel **pixel1, Pixel **pixel2, unsigned int k1, unsigned int k2);
void encryptPixelsOfLinearizedImage(Pixel **BMP_array, unsigned int *random_numbers_array, unsigned int SV, unsigned int W, unsigned int H);
void encryptBMPimage(unsigned char *source_file_name, unsigned char *destination_file_name, unsigned char *key_file_name);
void generateReversePermutation(unsigned int **permutation, unsigned int permutation_length);
void decryptPixelsOfLinearizedImage(Pixel **BMP_array, unsigned int *random_numbers_array, unsigned int SV, unsigned int W, unsigned int H);
void dencryptBMPimage(unsigned char *source_file_name, unsigned char *destination_file_name, unsigned char *key_file_name);
void frequencyOfAValueOnAColorChannel(Pixel *BMP_array, unsigned int BMP_array_length, unsigned int value, float *frequency, unsigned char channel);
void chiSquaredTestOnAColorChannel(Pixel *BMP_array, unsigned int H, unsigned int W, unsigned char channel);
void chiSquaredTest(unsigned char *sorce_file_name);
void removeTrailingTerminator(unsigned char *string);

#endif