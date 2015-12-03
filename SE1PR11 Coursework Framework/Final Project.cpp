#include "Functions.h"

#define AutosavesLocation "../Autosaves/"

using namespace std;

int main(int argc, char * argv[]){

	RemoveAutosaves();			// Remove the Images in the Autosaves folder
	system("cls");		// Clears the console

	string Location;			// Variable for the location of saving or loading images
	string response;			// Variable for a response from the user to a question

	SDL_Surface* ImageSurface;		// Variable for the Image
	int width = 0, height = 0;		// weight and height of the image

	do{
		cout << "Please enter the location of the image." << endl << "Example:  D://Users//Kikadass//Desktop//Lighthouse.jpg" << endl;
		cin >> Location;
		//Location = "Lighthouse.jpg";		// This line is just for testing
		ImageSurface = TakeImage(Location);		// Loads the image from a folder to the program
			
		// Get The Array of the Pixels (Access to the Raw Data Pointer)
		// This is a 1D array, however represents the entire image. 
		Uint8* ImageArray = GetPixelArray(ImageSurface, &width, &height);

		CreateImage(ImageSurface, "Image", width, height);			// Shows the image to the user

		cout << "Is that the image you wanted?" << endl;
		cin >> response;
		// response = "yes";		// This line is just for testing
		 cout << "Loading... Please wait." << endl;


	} while (response[0] != 'Y' && response[0] != 'y');


	// Variables for the switch
	int saves = 1;				// Variable for the autosaves
	int choice = 0;					// Variable for the choice inside the switch statement
	string SavingLocation = "";
	string name = "Image";

	// Following two lines is to save the original image as the first autosave image
	Location = AutosavesLocation;
	Location += "0.bmp";
	SaveImageBMP(ImageSurface, Location.c_str());


	do{
		choice = DetectMenu(ImageSurface, name, choice);

		switch (choice){
		case 1:					// Case to Create the image Black and White and show it on the screen
			cout << "Loading...  Please wait." << endl;
			Blacknwhite(ImageSurface);
			name = "Black and White";
			saves = Autosave(ImageSurface, saves);
			break;
		case 2:					// Saves the image in a location selected by the user
			if (SavingLocation == ""){
				cout << "Write a Location to save your image." << endl << "Example:  D://Users//Kikadass//Desktop//Lighthouse.bmp" << endl << "WARNING: Images with the same name will be reaplaced!!!" << endl;
				cin >> SavingLocation;
			}
			else {
				cout << "Would you like to save the Image in a new location?" << endl;
				cin >> response;
				if (response[0] == 'Y' || response[0] == 'y'){
					cout << "Write a Location to save your image." << endl << "Example:  D://Users//Kikadass//Desktop//Lighthouse.bmp" << endl << "WARNING: Images with the same name will be reaplaced!!!" << endl;
					cin >> SavingLocation;
				}
			}
			SaveImageBMP(ImageSurface, SavingLocation.c_str());
			break;
		case 3:					// Creates a Sepia image and shows it on the screeen
			cout << "Loading... Please wait." << endl;
			Sepia(ImageSurface);
			name = "Sepia";
			saves = Autosave(ImageSurface, saves);
			break;
		case 4:					// Changes the brightness of an image on a percentatge chosen by the user and shows the result
			Brightness(ImageSurface);
			name = "Brightness";
			saves = Autosave(ImageSurface, saves);
			break;
		case 5:					// Undo the changes that have been done by taking an image from the autosaves
			if (saves == 1) {
				cout << endl << "ERROR. No more saves available." << endl << endl;
				break;
			}
			cout << "Loading... Please wait." << endl;
			saves --;
			Location = AutosavesLocation;
			Location += saves + 47;
			Location += ".bmp";
			ImageSurface = TakeImage(Location);
			GetPixelArray(ImageSurface, &width, &height);
			name = "Last save";
			break;
		case 6:					// Creates a Negative image and shows the result
			cout << "Loading... Please wait." << endl;
			Negative(ImageSurface);
			name = "Negative";
			saves = Autosave(ImageSurface, saves);
			break;
		case 7:					// Creates an image just detecting the edges and shows the result
			cout << "Loading... Please wait." << endl;
			EdgeDetecting(ImageSurface);
			name = "Edge Detection";
			saves = Autosave(ImageSurface, saves);
			break;
		case 8:					// Opens the image and lets the user select an area for cropping the image. Then it shows the result
			CreateWindow("Cropping", width, height);
			ImageSurface = Cropping(ImageSurface, &width, &height);
			name = "Cropped";
			saves = Autosave(ImageSurface, saves);
			break;
		case 9:					// Rotates the image in the degrees chosen by the user
			cout << "Loading... Please wait." << endl;
			ImageSurface = Rotate(ImageSurface, &width, &height);
			name = "Rotating";
			saves = Autosave(ImageSurface, saves);
			break;
		case 10:				// Scales the image (makes it smaller or bigger) depending on the percentatge input by the user
			ImageSurface = Scale(ImageSurface, &width, &height);
			name = "Scaling";
			saves = Autosave(ImageSurface, saves);
			break;
		case 11:				// Creates a Watermark in the image
			Watermark(ImageSurface, width, height);
			name = "Watermark";
			saves = Autosave(ImageSurface, saves);
			break;
		case 12:				// Creates a new menu where the user can modify the red, green and blue values.
			name = "Change Color";
			ImageSurface = ChangeColor(saves, choice, 5, 5, 5);
			saves = Autosave(ImageSurface, saves);
			break;
		default:
			break;
		}
	} while (choice != 0);
	SDL_FreeSurface(ImageSurface);
	return 0;
}