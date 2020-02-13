#include <stdio.h>
#include <string.h>

#define HEADER_LENGTH 54
#define WIDTH_POSITION_IN_HEADER 18
#define FILE_NAME_LENGTH 100

/*
To compile:  	gcc encryption_verifier.c -o verifier
To run:		    ./verifier
Name of encrypted file to test: output/BMPencrypted.bmp
*/

int fileDimension(FILE *f)
{
    int p, dim;

    p = ftell(f);
    fseek(f, 0, SEEK_END);
    dim = ftell(f);
    fseek(f, p, SEEK_SET);

    return dim;
}

int main()
{
    FILE *ftest, *fok;

    char tested_image_name[FILE_NAME_LENGTH];
    char well_encrypted_image_name[] = "input_resources/enc_peppers_ok.bmp";

    unsigned int img_width, img_heigth, i, j, padding;
    unsigned char pOK[3], pTest[3], byteTest, byteOK, h;

    printf("Name of encrypted file to test: ");
    fgets(tested_image_name, FILE_NAME_LENGTH, stdin);
    tested_image_name[strlen(tested_image_name) - 1] = '\0';

    fok = fopen(well_encrypted_image_name, "rb");
    if (fok == NULL)
    {
        printf("\nThe reference encrypted image could not be found(%s)!\n", well_encrypted_image_name);
        return 0;
    }

    ftest = fopen(tested_image_name, "rb");
    if (ftest == NULL)
    {
        printf("\nThe encrypted image to be tested could not be found (%s)!\n", tested_image_name);
        return 0;
    }

    if (fileDimension(fok) != fileDimension(ftest))
    {
        printf("\nThe tested encrypted image (%s) does not have the correct octet size!\n", tested_image_name);
        fclose(fok);
        fclose(ftest);
        return 0;
    }

    for (h = 0; h < HEADER_LENGTH; h++)
    {
        fread(&byteOK, 1, 1, fok);
        fread(&byteTest, 1, 1, ftest);

        if (byteOK != byteTest)
        {
            printf("\nImaginea criptata testata (%s) are header-ul incorect la octetul %u!\n", tested_image_name, h);
            fclose(fok);
            fclose(ftest);
            return 0;
        }
    }

    fseek(fok, WIDTH_POSITION_IN_HEADER, SEEK_SET);
    fread(&img_width, sizeof(unsigned int), 1, fok);
    fread(&img_heigth, sizeof(unsigned int), 1, fok);

    if (img_width % 4 != 0)
        padding = 4 - (3 * img_width) % 4;
    else
        padding = 0;

    fseek(fok, HEADER_LENGTH, SEEK_SET);
    fseek(ftest, HEADER_LENGTH, SEEK_SET);

    for (i = 0; i < img_heigth; i++)
    {
        for (j = 0; j < img_width; j++)
        {
            fread(pOK, 3, 1, fok);
            fread(pTest, 3, 1, ftest);

            if (memcmp(pOK, pTest, 3) != 0)
            {
                printf("\nThe tested encrypted image (%s) has an incorrect pixel value on line %u and column %u!\n", tested_image_name, img_heigth - 1 - i, j);
                fclose(fok);
                fclose(ftest);
                return 0;
            }
        }

        fseek(fok, padding, SEEK_CUR);
        fseek(ftest, padding, SEEK_CUR);
    }

    printf("\nThe tested encrypted image (%s) is properly encrypted!\n", tested_image_name);

    fclose(fok);
    fclose(ftest);

    return 0;
}
