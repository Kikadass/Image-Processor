#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "SDL_Wrapper.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Removes all Autosaves from the folder
int RemoveAutosaves();

// Takes an image from a location. If the image cannot be founf there is an error message
SDL_Surface* TakeImage(string Location);

// Creates the Menu
int DetectMenu(SDL_Surface* Image, string name, int choice);

// Creates a Black and white image
void Blacknwhite(SDL_Surface* Surface);

// Creates a Sepia image
void Sepia(SDL_Surface* Image);

// Changes the Brightness of the image with a percentatge inputed
void Brightness(SDL_Surface* Image);

// Autosaves images. Max 9 autosaves + the original image
int Autosave(SDL_Surface* Image, int saves);

// Creates the Negative image
void Negative(SDL_Surface* Image);

// Creates an edge detecting image
void EdgeDetecting(SDL_Surface* Image);

// Creates an image from an area of the original image cropping it
SDL_Surface* Cropping(SDL_Surface* ImageSurface, int *width, int *height);

// This rotates the image in a certain angle
SDL_Surface* Rotate(SDL_Surface* Image, int *width, int *height);

// This Scales the image
SDL_Surface* Scale(SDL_Surface* Image, int *width, int *height);

// This Shrinks the image
SDL_Surface* ShrinkImage(SDL_Surface* Image, SDL_Surface* newImage, float Size);

//This creates a water mark with an image chosen by the user
void Watermark(SDL_Surface* Image, int width, int height);

//This Changes the color of the image
SDL_Surface* ChangeColor(int saves, int choice, int RED, int GREEN, int BLUE);

// It creates the image and waits until the user closes the image or presses ESC to close it
void CreateImage(SDL_Surface* ImageSurface, string name, int width, int height);

// Detects if the user pressed ESC or closed the window to continue with the program
void DisplaySurfaceUntilClose(SDL_Surface* Surface);

#endif
