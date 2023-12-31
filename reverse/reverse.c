#include <cs50.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "wav.h"

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);
int num_samples(WAVHEADER header);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 3)
    {
        printf("Usage: ./filter [flag] infile outfile\n");
        return 1;
    }

    // Open input file for reading
    char *infile = argv[1];
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", argv[1]);
        return 1;
    }

    // Read header
    WAVHEADER wf;
    fread(&wf, sizeof(WAVHEADER), 1, inptr);

    // Use check_format to ensure WAV format
    if (!check_format(wf))
    {
        printf("Wrong format!");
        return 1;
    }

    // Open output file for writing
    char *outfile = argv[2];
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        printf("Could not open %s.\n", argv[2]);
        return 1;
    }

    // Write header to file
    fwrite(&wf, sizeof(WAVHEADER), 1, outptr);

    // Use get_block_size to calculate size of block
    int blockSize = get_block_size(wf);

    // Write reversed audio to file
    if (fseek(inptr, blockSize, SEEK_END))
    {
        return 1;
    }

    BYTE block[blockSize];

    while (ftell(inptr) - blockSize > sizeof(wf))
    {
        if (fseek(inptr, -2 * blockSize, SEEK_CUR))
        {
            return 1;
        }
        fread(block, blockSize, 1, inptr);
        fwrite(block, blockSize, 1, outptr);
    }

    fclose(inptr);
    fclose(outptr);

    return 0;
}

int check_format(WAVHEADER header)
{
    char *WAVE = "WAVE";
    for (int i = 0; i < 4; i++)
    {
        if (header.format[i] != WAVE[i])
        {
            return 0;
        }
    }
    return 1;
}

int get_block_size(WAVHEADER header)
{
    return header.numChannels * header.bitsPerSample / 8;
}

int num_samples(WAVHEADER header)
{
    return (header.subchunk2Size / header.numChannels) / (header.bitsPerSample / 8);
}