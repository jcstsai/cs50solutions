// Problem Set 5 - jpg/recover.c

#include <stdio.h>

// chunks of 512
#define BUFSIZE 512

// define a "chunk"
typedef unsigned char CHUNK[BUFSIZE];

int main(int argc, char *argv[]) {
    FILE *fp = fopen("card.raw", "r");
    FILE *outfile;
    CHUNK c;
    char filename[8];
    int currentFileNumber = -1;
    
    while (!feof(fp)) {
        fread(&c, BUFSIZE, 1, fp);
        
        if (c[0] == 0xff && c[1] == 0xd8 && c[2] == 0xff && (c[3] == 0xe0 || c[3] == 0xe1)) {
            // If we found the start of a new file:
            // increment current file number and write it to filename
            currentFileNumber++;
            sprintf(filename, "%03d.jpg", currentFileNumber);
            
            // open a new outfile and write this chunk
            outfile = fopen(filename, "w");
            fwrite(&c, BUFSIZE, 1, outfile);
        } else if (currentFileNumber > -1) {
            // Else if we're not at the beginning, simply write this chunk to current file
            fwrite(&c, BUFSIZE, 1, outfile);
        }
    }
    
    fclose(fp);
}
