#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "encryption_functions.h"

#define HEADER_LENGTH 54
#define FILE_NAME_LENGTH 100
#define WIDTH_POSITION_IN_HEADER 18
#define HEIGHT_POSITION_IN_HEADER 22

void allocateMemoryForUnsignedCharArrays(unsigned char **array, unsigned int length)
{
    (*array) = (unsigned char *)malloc(length * sizeof(unsigned char));
    if ((*array) == NULL)
        printf("\nIn function -allocateMemoryForUnsignedCharArrays-, memory could not be allocated for the array!\n");
}

void allocateMemoryFoPixelArrays(Pixel **array, unsigned int length)
{
    (*array) = (Pixel *)malloc(length * sizeof(Pixel));
    if ((*array) == NULL)
        printf("\nIn function -allocateMemoryFoPixelArrays-, memory could not be allocated for the array!\n");
}

void allocateMemoryForUnsignedIntArrays(unsigned int **array, unsigned int length)
{
    (*array) = (unsigned int *)malloc(length * sizeof(unsigned int));
    if ((*array) == NULL)
        printf("\nIn function -allocateMemoryForUnsignedIntArrays-, memory could not be allocated for the array!\n");
}

void readWidthAndHeight(unsigned char *source_file_name, int *width, int *height)
{
    FILE *f;

    f = fopen(source_file_name, "rb");
    if (f == NULL)
    {
        printf("\nIn function -readWidthAndHeight- file could not be opened %s!\n", source_file_name);
        return;
    }

    fseek(f, WIDTH_POSITION_IN_HEADER, SEEK_SET);
    fread(width, sizeof(unsigned int), 1, f);
    fseek(f, HEIGHT_POSITION_IN_HEADER, SEEK_SET);
    fread(height, sizeof(unsigned int), 1, f);
    if (fclose(f) != 0)
        printf("\nIn function -readWidthAndHeight- file could not be close %s!\n", source_file_name);
}

void readR0andSV(unsigned char *key_file_name, unsigned int *R0, unsigned int *SV)
{
    FILE *f;

    f = fopen(key_file_name, "r");
    if (f == NULL)
    {
        printf("\nIn function -readR0andSV- file could not be opened %s\n!", key_file_name);
        return;
    }
    fscanf(f, "%u %u", R0, SV);
    if (fclose(f) != 0)
        printf("\nIn function -readR0andSV- file could not be close %s\n!", key_file_name);
}

void loadLinearizedHeaderInInternalMemory(unsigned char *nume_fisier_sursa, unsigned char **vector)
{
    FILE *f;
    unsigned int i;

    f = fopen(nume_fisier_sursa, "rb");
    if (f == NULL)
    {
        printf("\nIn function -loadLinearizedHeaderInInternalMemory- file could not be opened %s\n!", nume_fisier_sursa);
        return;
    }

    fseek(f, 0, SEEK_SET);
    for (i = 0; i < HEADER_LENGTH; i++)
        fread(&((*vector)[i]), sizeof(unsigned char), 1, f);

    if (fclose(f) != 0)
        printf("\nIn function -loadLinearizedHeaderInInternalMemory- file could not be close %s\n!", nume_fisier_sursa);
}

void loadLinearizedBMPImageInInternalMemory(unsigned char *souce_file_name, Pixel **image_array, unsigned int W, unsigned int H)
{
    FILE *f;
    unsigned int i, j;
    unsigned int padding;

    if (W % 4 != 0)
        padding = 4 - (3 * W) % 4;
    else
        padding = 0;

    f = fopen(souce_file_name, "rb");
    if (f == NULL)
    {
        printf("\nIn function -loadLinearizedBMPImageInInternalMemory- file could not be opened %s\n!", souce_file_name);
        return;
    }

    for (i = 0; i < H; i++)
    {
        fseek(f, HEADER_LENGTH + (W * 3 + padding) * (H - 1 - i), SEEK_SET);
        for (j = 0; j < W; j++)
        {
            fread(&((*image_array)[i * W + j].B), sizeof(unsigned char), 1, f);
            fread(&((*image_array)[i * W + j].G), sizeof(unsigned char), 1, f);
            fread(&((*image_array)[i * W + j].R), sizeof(unsigned char), 1, f);
        }
    }
    if (fclose(f) != 0)
        printf("\nIn function -loadLinearizedBMPImageInInternalMemory- file could not be close %s\n!", souce_file_name);
}

void saveLinearizedBMPImageInExternalMemory(unsigned char *destination_file_name, unsigned char *header_array, Pixel *BMP_image_array, unsigned int W, unsigned int H)
{
    unsigned int i, j, padding;
    unsigned char c = '0';
    FILE *f;

    if (W % 4 != 0)
        padding = 4 - (3 * W) % 4;
    else
        padding = 0;

    f = fopen(destination_file_name, "wb");
    if (f == NULL)
    {
        printf("\nIn function -salvareBMPInMeomoriaExterna- file could not be opened %s\n!", destination_file_name);
        return;
    }

    for (i = 0; i < HEADER_LENGTH; i++)
        fwrite(&header_array[i], sizeof(unsigned char), 1, f);

    for (i = 0; i <= H - 1; i++)
    {
        for (j = 0; j < W; j++)
            fwrite(&BMP_image_array[(H - 1 - i) * W + j], 3 * sizeof(unsigned char), 1, f);
        for (j = 1; j <= padding; j++)
            fwrite(&c, sizeof(unsigned char), 1, f);
    }

    if (fclose(f) != 0)
        printf("\nIn function -salvareBMPInMeomoriaExterna- file could not be close %s\n!", destination_file_name);
}

void XORSHIFT32(unsigned int length, unsigned int seed, unsigned int **array)
{
    unsigned int r, k;
    r = seed;
    for (k = 1; k < length; k++)
    {
        r = r ^ r << 13;
        r = r ^ r >> 17;
        r = r ^ r << 5;
        (*array)[k] = r;
    }
}

void generateRandomPermutationDurstenfeld(unsigned int permutation_length, unsigned int *random_numbers_array, unsigned int **permutation)
{
    unsigned int r, k, aux;

    for (k = 0; k < permutation_length; k++)
        (*permutation)[k] = k;

    for (k = permutation_length - 1; k >= 1; k--)
    {
        r = random_numbers_array[permutation_length - k] % (k + 1);
        if (r != k)
        {
            aux = (*permutation)[k];
            (*permutation)[k] = (*permutation)[r];
            (*permutation)[r] = aux;
        }
    }
}

void permuteThePixelsOfLinearizedImage(Pixel **BMP_array, unsigned int *permutation, unsigned int BMP_array_length)
{
    Pixel *aux;
    unsigned int k;

    allocateMemoryFoPixelArrays(&aux, BMP_array_length);

    for (k = 0; k < BMP_array_length; k++)
        aux[permutation[k]] = (*BMP_array)[k];

    for (k = 0; k < BMP_array_length; k++)
        (*BMP_array)[k] = aux[k];

    free(aux);
}

void XORintegerXpixel(unsigned int integer, Pixel **pixel, unsigned int k)
{
    unsigned char *c;

    c = (unsigned char *)&integer;
    (*pixel)[k].B = ((*pixel)[k].B) ^ (*c);
    c++;
    (*pixel)[k].G = ((*pixel)[k].G) ^ (*c);
    c++;
    (*pixel)[k].R = ((*pixel)[k].R) ^ (*c);
}

void XORpixelXpixel(Pixel **pixel1, Pixel **pixel2, unsigned int k1, unsigned int k2)
{
    (*pixel1)[k1].B = ((*pixel1)[k1].B) ^ ((*pixel2)[k2].B);
    (*pixel1)[k1].G = ((*pixel1)[k1].G) ^ ((*pixel2)[k2].G);
    (*pixel1)[k1].R = ((*pixel1)[k1].R) ^ ((*pixel2)[k2].R);
}

void encryptPixelsOfLinearizedImage(Pixel **BMP_array, unsigned int *random_numbers_array, unsigned int SV, unsigned int W, unsigned int H)
{
    unsigned int k;

    k = 0;
    XORintegerXpixel(SV, BMP_array, k);
    XORintegerXpixel(random_numbers_array[W * H], BMP_array, k);

    for (k = 1; k < W * H; k++)
    {
        XORpixelXpixel(BMP_array, BMP_array, k, k - 1);
        XORintegerXpixel(random_numbers_array[W * H + k], BMP_array, k);
    }
}

void encryptBMPimage(unsigned char *source_file_name, unsigned char *destination_file_name, unsigned char *key_file_name)
{
    unsigned int H, W, R0, SV, i;
    unsigned int *R, *P;
    unsigned char *header;
    Pixel *LinearizedBMPimage;

    readWidthAndHeight(source_file_name, &W, &H);
    allocateMemoryForUnsignedCharArrays(&header, HEADER_LENGTH);
    allocateMemoryFoPixelArrays(&LinearizedBMPimage, W * H);
    loadLinearizedHeaderInInternalMemory(source_file_name, &header);
    loadLinearizedBMPImageInInternalMemory(source_file_name, &LinearizedBMPimage, W, H);

    readR0andSV(key_file_name, &R0, &SV);

    allocateMemoryForUnsignedIntArrays(&R, 2 * W * H);
    R[0] = R0;
    XORSHIFT32(2 * W * H, R0, &R);

    allocateMemoryForUnsignedIntArrays(&P, W * H);
    generateRandomPermutationDurstenfeld(W * H, R, &P);

    permuteThePixelsOfLinearizedImage(&LinearizedBMPimage, P, W * H);
    encryptPixelsOfLinearizedImage(&LinearizedBMPimage, R, SV, W, H);
    saveLinearizedBMPImageInExternalMemory(destination_file_name, header, LinearizedBMPimage, W, H);

    free(P);
    free(LinearizedBMPimage);
    free(R);
    free(header);
}

void generateReversePermutation(unsigned int **permutation, unsigned int permutation_length)
{
    unsigned int *aux;
    unsigned int k;

    allocateMemoryForUnsignedIntArrays(&aux, permutation_length);

    for (k = 0; k < permutation_length; k++)
        aux[(*permutation)[k]] = k;

    for (k = 0; k < permutation_length; k++)
        (*permutation)[k] = aux[k];

    free(aux);
}

void decryptPixelsOfLinearizedImage(Pixel **BMP_array, unsigned int *random_numbers_array, unsigned int SV, unsigned int W, unsigned int H)
{
    unsigned int k;
    Pixel *aux;

    allocateMemoryFoPixelArrays(&aux, W * H);

    for (k = 0; k < W * H; k++)
    {
        aux[k].B = (*BMP_array)[k].B;
        aux[k].G = (*BMP_array)[k].G;
        aux[k].R = (*BMP_array)[k].R;
    }

    k = 0;
    XORintegerXpixel(SV, BMP_array, k);
    XORintegerXpixel(random_numbers_array[W * H], BMP_array, k);

    for (k = 1; k < W * H; k++)
    {
        XORpixelXpixel(BMP_array, &aux, k, k - 1);
        XORintegerXpixel(random_numbers_array[W * H + k], BMP_array, k);
    }

    free(aux);
}

void dencryptBMPimage(unsigned char *source_file_name, unsigned char *destination_file_name, unsigned char *key_file_name)
{
    unsigned int H, W, R0, SV, i;
    unsigned int *R, *P;
    unsigned char *header;
    Pixel *LinearizedBMPimage;

    readWidthAndHeight(source_file_name, &W, &H);
    allocateMemoryForUnsignedCharArrays(&header, HEADER_LENGTH);
    allocateMemoryFoPixelArrays(&LinearizedBMPimage, W * H);
    loadLinearizedHeaderInInternalMemory(source_file_name, &header);
    loadLinearizedBMPImageInInternalMemory(source_file_name, &LinearizedBMPimage, W, H);

    readR0andSV(key_file_name, &R0, &SV);

    allocateMemoryForUnsignedIntArrays(&R, 2 * W * H);
    R[0] = R0;
    XORSHIFT32(2 * W * H, R0, &R);

    allocateMemoryForUnsignedIntArrays(&P, W * H);
    generateRandomPermutationDurstenfeld(W * H, R, &P);
    generateReversePermutation(&P, W * H);

    decryptPixelsOfLinearizedImage(&LinearizedBMPimage, R, SV, W, H);
    permuteThePixelsOfLinearizedImage(&LinearizedBMPimage, P, W * H);
    saveLinearizedBMPImageInExternalMemory(destination_file_name, header, LinearizedBMPimage, W, H);

    free(P);
    free(LinearizedBMPimage);
    free(R);
    free(header);
}

void frequencyOfAValueOnAColorChannel(Pixel *BMP_array, unsigned int BMP_array_length, unsigned int value, float *frequency, unsigned char channel)
{
    unsigned int i;

    (*frequency) = 0;
    if (channel == 'R')
        for (i = 0; i < BMP_array_length; i++)
        {
            if (value == BMP_array[i].R)
                (*frequency) = (*frequency) + 1;
        }
    else if (channel == 'G')
        for (i = 0; i < BMP_array_length; i++)
        {
            if (value == BMP_array[i].G)
                (*frequency) = (*frequency) + 1;
        }
    else
        for (i = 0; i < BMP_array_length; i++)
            if (value == BMP_array[i].B)
                (*frequency) = (*frequency) + 1;
}

void chiSquaredTestOnAColorChannel(Pixel *BMP_array, unsigned int H, unsigned int W, unsigned char channel)
{
    unsigned int i;
    float sum = 0;
    float theoretical_frequency, frequency;

    theoretical_frequency = (float)(H * W) / 256;

    for (i = 0; i <= 255; i++)
    {
        frequencyOfAValueOnAColorChannel(BMP_array, W * H, i, &frequency, channel);
        sum = sum + (float)((frequency - theoretical_frequency) * (frequency - theoretical_frequency)) / theoretical_frequency;
    }
    printf("%c: %f\n", channel, sum);
}

void chiSquaredTest(unsigned char *sorce_file_name)
{
    unsigned int H, W, i;
    Pixel *LinearizedBMPimage;

    readWidthAndHeight(sorce_file_name, &W, &H);
    allocateMemoryFoPixelArrays(&LinearizedBMPimage, W * H);
    loadLinearizedBMPImageInInternalMemory(sorce_file_name, &LinearizedBMPimage, W, H);

    printf("Chi-squared test on RGB channels for %s:\n", sorce_file_name);
    chiSquaredTestOnAColorChannel(LinearizedBMPimage, H, W, 'R');
    chiSquaredTestOnAColorChannel(LinearizedBMPimage, H, W, 'G');
    chiSquaredTestOnAColorChannel(LinearizedBMPimage, H, W, 'B');

    free(LinearizedBMPimage);
}

void removeTrailingTerminator(unsigned char *string)
{
    // find the first, if any, \n and end the string before it
    char *p = strchr(string, '\n');
    if (p != NULL)
        *p = '\0';

    p = strchr(string, '\r');
    if (p != NULL)
        *p = '\0';
}