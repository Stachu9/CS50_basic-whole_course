#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int avarage = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = avarage;
            image[i][j].rgbtGreen = avarage;
            image[i][j].rgbtRed = avarage;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int halfWidth = floor(width / 2);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < halfWidth; j++)
        {
            RGBTRIPLE temp = image[i][width - 1 - j];
            image[i][width - 1 - j] = image[i][j];
            image[i][j] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tempImage[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int zMin = i - 1;
            if (zMin < 0)
            {
                zMin = 0;
            }

            int zMax = i + 1;
            if (zMax >= height)
            {
                zMax = height - 1;
            }

            int xMin = j - 1;
            if (xMin < 0)
            {
                xMin = 0;
            }

            int xMax = j + 1;
            if (xMax >= width)
            {
                xMax = width - 1;
            }

            float counter = 0.0;

            int avRed = 0;
            int avGreen = 0;
            int avBlue = 0;

            for (int z = zMin; z <= zMax; z++)
            {
                for (int x = xMin; x <= xMax; x++)
                {
                    avRed += image[z][x].rgbtRed;
                    avGreen += image[z][x].rgbtGreen;
                    avBlue += image[z][x].rgbtBlue;
                    counter++;
                }
            }
            tempImage[i][j].rgbtRed = round(avRed / counter);
            tempImage[i][j].rgbtGreen = round(avGreen / counter);
            tempImage[i][j].rgbtBlue = round(avBlue / counter);
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = tempImage[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tempImage[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            int zMin = i - 1;
            if (zMin < 0)
            {
                zMin = 0;
            }

            int zMax = i + 1;
            if (zMax >= height)
            {
                zMax = height - 1;
            }

            int xMin = j - 1;
            if (xMin < 0)
            {
                xMin = 0;
            }

            int xMax = j + 1;
            if (xMax >= width)
            {
                xMax = width - 1;
            }

            int gxRed = 0;
            int gxGreen = 0;
            int gxBlue = 0;

            int gyRed = 0;
            int gyGreen = 0;
            int gyBlue = 0;

            for (int z = zMin; z <= zMax; z++)
            {
                for (int x = xMin; x <= xMax; x++)
                {
                    // GxArr
                    if (x < j && z != i)
                    {
                        gxRed += image[z][x].rgbtRed * -1;
                        gxGreen += image[z][x].rgbtGreen * -1;
                        gxBlue += image[z][x].rgbtBlue * -1;
                    }
                    else if (x < j && z == i)
                    {
                        gxRed += image[z][x].rgbtRed * -2;
                        gxGreen += image[z][x].rgbtGreen * -2;
                        gxBlue += image[z][x].rgbtBlue * -2;
                    }
                    else if (x > j && z != i)
                    {
                        gxRed += image[z][x].rgbtRed * 1;
                        gxGreen += image[z][x].rgbtGreen * 1;
                        gxBlue += image[z][x].rgbtBlue * 1;
                    }
                    else if (x > j && z == i)
                    {
                        gxRed += image[z][x].rgbtRed * 2;
                        gxGreen += image[z][x].rgbtGreen * 2;
                        gxBlue += image[z][x].rgbtBlue * 2;
                    }

                    // GyArr
                    if (z < i && x != j)
                    {
                        gyRed += image[z][x].rgbtRed * -1;
                        gyGreen += image[z][x].rgbtGreen * -1;
                        gyBlue += image[z][x].rgbtBlue * -1;
                    }
                    else if (z < i && x == j)
                    {
                        gyRed += image[z][x].rgbtRed * -2;
                        gyGreen += image[z][x].rgbtGreen * -2;
                        gyBlue += image[z][x].rgbtBlue * -2;
                    }
                    else if (z > i && x != j)
                    {
                        gyRed += image[z][x].rgbtRed * 1;
                        gyGreen += image[z][x].rgbtGreen * 1;
                        gyBlue += image[z][x].rgbtBlue * 1;
                    }
                    else if (z > i && x == j)
                    {
                        gyRed += image[z][x].rgbtRed * 2;
                        gyGreen += image[z][x].rgbtGreen * 2;
                        gyBlue += image[z][x].rgbtBlue * 2;
                    }
                }
            }

            int tempR = round(sqrt((gxRed * gxRed * 1.0) + (gyRed * gyRed * 1.0)));
            int tempG = round(sqrt((gxGreen * gxGreen * 1.0) + (gyGreen * gyGreen * 1.0)));
            int tempB = round(sqrt((gxBlue * gxBlue * 1.0) + (gyBlue * gyBlue * 1.0)));

            if (tempR > 255)
            {
                tempR = 255;
            }

            if (tempG > 255)
            {
                tempG = 255;
            }

            if (tempB > 255)
            {
                tempB = 255;
            }

            tempImage[i][j].rgbtRed = tempR;
            tempImage[i][j].rgbtGreen = tempG;
            tempImage[i][j].rgbtBlue = tempB;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = tempImage[i][j];
        }
    }
    return;
}
