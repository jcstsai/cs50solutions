/****************************************************************************
 * copy.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Copies a BMP piece by piece, just because.
 ***************************************************************************/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"


int
main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: resize n infile outfile\n");
        return 1;
    }
    
    // remember filenames
    float scaleFactor = atof(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];
    
    // check scale factor
    if (scaleFactor < 0.0 || scaleFactor > 100.0) {
        printf("Please enter a number between 0 and 100\n");
        return 1;
    }

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
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
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    // track original width
    int originalBiWidth = bi.biWidth;
    int originalBiHeight = bi.biHeight;
    
    // alter size, width, height
    bf.bfSize = ((int) (scaleFactor * bi.biWidth)) * ((int) (/*scaleFactor **/ bi.biHeight))
        + sizeof(BITMAPFILEHEADER);
    bi.biWidth = (int) (scaleFactor * bi.biWidth);
    bi.biHeight = (int) (scaleFactor * bi.biHeight);
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines based on scaleFactor
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int originalPadding = (4 - (originalBiWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // Track all rows
    RGBTRIPLE **rows = malloc(abs(originalBiHeight) * sizeof(RGBTRIPLE *));
    
    // Read original pixels into memory
    for (int i = 0; i < abs(originalBiHeight); i++)
    {
        // create a row
        rows[i] = malloc(originalBiWidth * sizeof(RGBTRIPLE));
        
        // read RGB triple from infile
        for (int j = 0; j < originalBiWidth; j++) {
            fread(&rows[i][j], sizeof(RGBTRIPLE), 1, inptr);
        }
        
        // skip over padding, if any
        fseek(inptr, originalPadding, SEEK_CUR);
    }
    
    // Write to new pixels
    for (int i = 0; i < abs(bi.biHeight); i++) {
        // Establish bounds for pixels of the original image to look at
        int iFloor = floor(((float) i) / scaleFactor);
        int iCeil = ceil(((float) (i + 1)) / scaleFactor);
        
        for (int j = 0; j < bi.biWidth; j++) {
            // Establish bounds for pixels of the original image to look at
            int jFloor = floor(((float) j) / scaleFactor);
            int jCeil = ceil(((float) (j + 1)) / scaleFactor);
            
            // Initialize the RGB triple at 0
            float red = 0.0;
            float green = 0.0;
            float blue = 0.0;
            
            // For each original pixel under consideration, add its color weighted by
            // its proportion
            for (int h = iFloor; h < iCeil; h++) {
                for (int k = jFloor; k < jCeil; k++) {
                    float iProportion = (
                        fminf((h+1) * scaleFactor, (float) (i+1)) -
                        fmaxf(h * scaleFactor, (float) i)
                    );
                    float jProportion = (
                        fminf((k+1) * scaleFactor, (float) (j+1)) -
                        fmaxf(k * scaleFactor, (float) j)
                    );
                    
                    red += iProportion * jProportion * rows[h][k].rgbtRed;
                    green += iProportion * jProportion * rows[h][k].rgbtGreen;
                    blue += iProportion * jProportion * rows[h][k].rgbtBlue;
                }
            }
            
            // Create an RGBTRIPLE struct that's rounded
            RGBTRIPLE triple;
            triple.rgbtRed = round(red);
            triple.rgbtGreen = round(green);
            triple.rgbtBlue = round(blue);
            
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        }
        
        // write padding to outfile
        for (int k = 0; k < padding; k++)
            fputc(0x00, outptr);
    }
    
    // free stored rows
    for (int i = 0; i < abs(originalBiHeight); i++) {
        free(rows[i]);
    }
    free(rows);

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
