/*
 * Implement a program that recover the jpg 

*/


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// make struct to save the pixel
typedef struct
{
    uint8_t buffer [512] ;
}jpg ; 

int main (int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr,"Usage: ./recover image. \n"); 
        return 1 ; 
    }
    
    // open the file to read 
    FILE* input = fopen(argv[1], "r"); 
    
    // check it's run correct
    if (input == NULL)
    {
        fprintf(stderr, " can't open the file. \n "); 
        return 2; 
    }
    
    // creat struct
    jpg jpg1 ;
    
    // to count the image 
    int image = 0 ;
    
    // creat varible file name 
    char *filename =malloc(sizeof(int)*3 + sizeof(char)*4) ; 
    sprintf(filename,"%03i.jpg",image);
    
    // open output file for .jpg 
    FILE *output = fopen(filename,"a");
    
    // check it's run correct
    if (output == NULL)
      {
          return 1; 
      }

    
    // read the card each time 512 byte 
    while (fread(&jpg1 ,512 , 1 ,input ) == 1 )
    {
    
        // if it's start if jpg 
        if (jpg1.buffer[0] == 0xff && jpg1.buffer[1] == 0xd8 
            && jpg1.buffer[2] == 0xff && (jpg1.buffer[3] & 0xf0) == 0xe0 )
            {
             
                // if it's the frist time 
                if (image == 0 )
                {
                    
                    // write in the momery 
                    fwrite(&jpg1 , 512 , 1 , output );
                    
                    
                }
                // if it's the second time 
                else if (image > 0 ) 
                {
                    // close the previous jpg 
                    fclose(output); 
                    
                    // start the new jpg 
                    sprintf(filename,"%03i.jpg",image);
                    output = fopen(filename, "a"); 
                    
                    if (output == NULL)
                    {
                        return 1;
                    }
    
                   // write in the momery
                    fwrite(&jpg1 , 512 , 1 , output );
                }
                
                // count the image 
               image++;  
            }
            
            // if it's find the jpg write in momery
            else if (image > 0 )
            {
                fwrite(&jpg1 , 512 , 1 , output );
            }
            
            
            
    }
    
    // free the memory 
    free(filename);
    // close the output and the input 
    fclose(output);       
    fclose(input); 
    
    return 0; 
}

