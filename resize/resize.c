/**
 * Implement a program that resizes BMPs
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }
    
    int n = atoi(argv[1]);
   
   // check if its less than 100 or equal 1
    if (n > 100 || n <= 0)
    {
        fprintf(stderr, "Usage: n must be from 1 to 100. \n");
        return 1 ;
    }
    // remember filenames
    
    char *infile = argv[2];
    char *outfile = argv[3];
    
    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf1 ,bf2;
    fread(&bf1, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi1 ,bi2;
    fread(&bi1, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf1.bfType != 0x4d42 || bf1.bfOffBits != 54 || bi1.biSize != 40 || 
        bi1.biBitCount != 24 || bi1.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    
    
    bf2 = bf1; 
    bi2 = bi1; 
    
    // change the width and the height by factor
    bi2.biWidth *= n ;
    bi2.biHeight *= n ;
    
    // determine padding for scanlines
    int paddingOld = (4 - (bi1.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int paddingNew = (4 - (bi2.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
   
    // calculate biSizieImage and bfSize
    bi2.biSizeImage = (bi2.biWidth * sizeof(RGBTRIPLE) + paddingNew) * abs(bi2.biHeight);
    bf2.bfSize = bi2.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); 
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf2, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi2, sizeof(BITMAPINFOHEADER), 1, outptr);


    RGBTRIPLE *tripleSave = malloc((sizeof(RGBTRIPLE) * bi2.biWidth));
    
    if (tripleSave == NULL)
    {
                fprintf(stderr, "Memory error .\n");

        return 1; 
    }
    
    
    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi1.biHeight); i < biHeight; i++)
    {
        int counter = 0;
        
            // iterate over pixels in scanline
            for (int j = 0; j < bi1.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;
    
                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                
                for (int h = 0 ; h < n ; h++)
                {
                    // save triple in array
                    tripleSave[counter] = triple;
    
                // write RGB triple to outfile
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                
                counter++; 
                }
                
            }
    
            // skip over padding, if any
            fseek(inptr, paddingOld, SEEK_CUR);
    
            //  add the paddingNew (to demonstrate how)
            for (int k = 0; k < paddingNew; k++)
            {
              
                fputc(0x00, outptr);
            }
            
            //write the the fact 
            for (int v = 0 ; v < n - 1 ; v++)
            {
                
                fwrite(tripleSave, (sizeof(RGBTRIPLE) * bi2.biWidth), 1, outptr );
                
                for (int k = 0; k < paddingNew; k++)
                {
                  
                    fputc(0x00, outptr);
                }                
                
            }
        
    }

    // free tripleSave from the heap 
    free(tripleSave);
    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
