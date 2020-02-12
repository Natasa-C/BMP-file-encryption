#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "encryption_functions.h"

#define HEADER_LENGTH 54
#define FILE_NAME_LENGTH 100
#define WIDTH_POSITION_IN_HEADER 18
#define HEIGHT_POSITION_IN_HEADER 22

/*
To compile:  	gcc encryption_script.c encryption_functions.c -o script
To run:		    ./script
*/

int main()
{
    unsigned char source_file_name[] = "input_resources/criptare.txt";
    unsigned char BMP_sorce_file_name[FILE_NAME_LENGTH];
    unsigned char BMP_encrypted_file_name[FILE_NAME_LENGTH];
    unsigned char BMP_decrypted_file_name[FILE_NAME_LENGTH];
    unsigned char key_file_name[FILE_NAME_LENGTH];

    FILE *fin;

    fin = fopen(source_file_name, "r");
    if (fin == NULL)
    {
        printf("File could not be opened %s!", source_file_name);
        return 0;
    }

    fgets(BMP_sorce_file_name, FILE_NAME_LENGTH, fin);
    removeTrailingTerminator(BMP_sorce_file_name);
    fgets(BMP_encrypted_file_name, FILE_NAME_LENGTH, fin);
    removeTrailingTerminator(BMP_encrypted_file_name);
    fgets(BMP_decrypted_file_name, FILE_NAME_LENGTH, fin);
    removeTrailingTerminator(BMP_decrypted_file_name);
    fgets(key_file_name, FILE_NAME_LENGTH, fin);
    removeTrailingTerminator(key_file_name);

    encryptBMPimage(BMP_sorce_file_name, BMP_encrypted_file_name, key_file_name);
    dencryptBMPimage(BMP_encrypted_file_name, BMP_decrypted_file_name, key_file_name);

    chiSquaredTest(BMP_sorce_file_name);
    printf("\n");
    chiSquaredTest(BMP_encrypted_file_name);

    if (fclose(fin) != 0)
        printf("\nIn function -main- file could not be close %s\n!", source_file_name);

    return 0;
}
