#include "Functions.h"

#define PI 3.14159265

#define AutosavesLocation "../Autosaves/"

using namespace std;

// Removes all Autosaves from the folder
int RemoveAutosaves(){
	for (int i = 0; i <= 9; i++){
		string Location = AutosavesLocation;
		Location += i + 48;
		Location += ".bmp";
		if (remove(Location.c_str()) != 0){				// Removes the image. If not, sends a message saying that it has finished
			perror("Cache deleted. ");
			return 0;
		}
	}
}


// Takes an image from a location. If the image cannot be founf there is an error message
SDL_Surface* TakeImage(string Location){

	cout << "Loading Image : " << Location << endl << endl;
	SDL_Surface* ImageSurface = LoadImage(Location.c_str());			// Load in the Image

	if (ImageSurface == NULL){							// Check if the image has been loaded correctly
		cout << "!!! Unable to Load Image !!!" << endl;
		cout << "Press Enter to Exit the Program." << endl;
		getchar();
		return NULL;
	}
	return ImageSurface;
}

// Creates the menu
int DetectMenu(SDL_Surface* Image, string name, int choice){
	system("cls");		// Clears the console
	int open = 1;		// Can also be a boolean for thw while loop

	cout << "What would you like to do with the image?" << endl << "Press the arrows on your keyboard (UP & DOWN) and then ENTER to choose." << endl;

	// Next two lines are for not getting out of bounds
	if (choice > 12) choice = 12;
	if (choice < 0) choice = 0;

	// The menu is shown depending on the choice that the user can use by pressing enter
	for (int i = 0; i < 13; i++){
		if (i == choice) cout << "[ ";
		else cout << "  ";
		if (i == 0) cout << "0. Close the program";
		if (i == 1) cout << "1. Black and White";
		if (i == 2) cout << "2. Save the image";
		if (i == 3) cout << "3. Sepia";
		if (i == 4) cout << "4. Brightness";
		if (i == 5) cout << "5. Undo";
		if (i == 6) cout << "6. Negative";
		if (i == 7) cout << "7. Edge Detection";
		if (i == 8) cout << "8. Cropp";
		if (i == 9) cout << "9. Rotate";
		if (i == 10) cout << "10. Scale";
		if (i == 11) cout << "11. Watermark";
		if (i == 12) cout << "12. Change Color";
		if (i == choice) cout << " ]";
		cout << endl;
	}

	SDL_Event event;										// Creates an event for looking for the keys pressed
	CreateWindow(name.c_str(), Image->w, Image->h);			// Creates the window
	DisplaySurface(Image);									// Displays the image

	// while there is not choice
	while (open == 1){
		if (SDL_PollEvent(&event)){

			switch (event.type){
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym){
				case SDLK_UP:								// If Up is pressed (arrow) go to the previous choice
					open = 0;
					choice--;
					return DetectMenu(Image, name, choice);
					break;
				case SDLK_DOWN:								// If Down is pressed (arrow) go to next choice
					open = 0;
					choice++;
					return DetectMenu(Image, name, choice);
					break;
				case SDLK_KP_ENTER:							// If Enter in the number pad is pressed return the choice (User has selected a choice)
					open = 0;
					CloseWindow();
					return choice;
					break;
				case SDLK_RETURN:							// If Enter in the main keyboard is pressed return the coice (User has selected a choice)
					CloseWindow();
					open = 0;
					return choice;
					break;
				default:
					break;
				}

				break;
			default:
				break;
			}
		}
	}
}

// Creates a Black and white image
void Blacknwhite(SDL_Surface* Image){
	// Goes through all the pixels and depending on the brightness changes the color into a color in the Grey Scale
	for (int y = Image->h - 1; y >= 0; y--){
		for (int x = 0; x < Image->w; x++){
			Uint8 R = 0, G = 0, B = 0;
			GetRGBValues(Image, x, y, &R, &G, &B);

			for (int i = 0; i <= 255; i++) {
				if ((int)R > i || (int)G > i || (int)B > i){
					SetRGBValues(Image, x, y, i, i, i);
				}
			}
		}
	}
}


// Creates a Sepia image
void Sepia(SDL_Surface* Image){
	// Goes through all the pixels and depending on the brightness changes the color into a color in the Brown Scale.
	for (int y = Image->h - 1; y >= 0; y--){
		for (int x = 0; x < Image->w; x++){
			Uint8 R = 0, G = 0, B = 0;
			GetRGBValues(Image, x, y, &R, &G, &B);

			for (int i = 0; i <= 255; i++) {
				if ((int)R > i || (int)G > i || (int)B > i){
					SetRGBValues(Image, x, y, i, i*0.7, i*0.4);
				}
			}
		}
	}
}


// Changes the Brightness of the image with a percentatge inputed
// Goes through all the pixels and multiplies the brightness with the value already there
// But if one of the colors of the pixels (RGB) is higher then 255 put that color on 255 instead then a higher value. 
void Brightness(SDL_Surface* Image){
	float bright;
	cout << "Write the percentage of the brightness you want. (60%)" << endl;
	
	// Next lines are to make sure that the user input a valid number (from 0 onwards) by looking at each character and look if is a digit or not
	string str;
	cin >> str;

	for (int i = 0; i < str.size(); i++){
		if (!isdigit(str[i])){
			cout << "WARNING: You are trying to enter an invalid input." << endl;
			bright = 100;
			_sleep(5000);
			break;
		}
		else {
			bright = atoi(str.c_str());
		}
	}

	bright /= 100;					// converts the value from a percentatge to the decimal value
	cout << "Loading... Please wait." << endl;

	for (int y = Image->h - 1; y >= 0; y--){
		for (int x = 0; x < Image->w; x++){
			Uint8 R = 0, G = 0, B = 0;
			GetRGBValues(Image, x, y, &R, &G, &B);
			if ((int)R*bright <= 255 && (int)G*bright <= 255 && (int)B*bright <= 255){
				SetRGBValues(Image, x, y, (int)R*bright, (int)G*bright, (int)B*bright);
			}
			else {
				int a, b, c;
				if ((int)R*bright >= 255) a = 255;
				else a = (int)R*bright;
				if ((int)G*bright >= 255) b = 255;
				else b = (int)G*bright;
				if ((int)B*bright >= 255) c = 255;
				else c = (int)B*bright;
				SetRGBValues(Image, x, y, a, b, c);
			}
		}
	}
}


// Autosaves images. Max 9 autosaves + the original image
// Also Returns the new value of the saves variable
int Autosave(SDL_Surface* Image, int saves){
	// If there are already 9 Autosaves it deletes the first Autosave and puts all the rest in the previous one (the second into the first, the third into the second...)
	// And puts the new Autosave in the 9th position
	if (saves == 10) {
		SDL_Surface* newImage;
		for (int i = 1; i < 9; i++){
			string Location = AutosavesLocation;
			Location += i + 49;
			Location += ".bmp";
			newImage = TakeImage(Location);
			Location = AutosavesLocation;
			Location += i + 48;
			Location += ".bmp";
			SaveImageBMP(newImage, Location.c_str());
		}
		string Location = AutosavesLocation;
		Location += saves + 47;
		Location += ".bmp";
		SaveImageBMP(Image, Location.c_str());
		return 10;
	}
	// If there are not 9 Autosaves yet save the new autosave in a new position.
	else {
		string Location = AutosavesLocation;
		Location += saves + 48;
		Location += ".bmp";
		SaveImageBMP(Image, Location.c_str());
		return saves + 1;
	}
}


// Creates the Negative image
void Negative(SDL_Surface* Image){
	// Goes through all the pixels and changes their value into the oposite one.
	for (int y = Image->h - 1; y >= 0; y--){
		for (int x = 0; x < Image->w; x++){
			Uint8 R = 0, G = 0, B = 0;
			GetRGBValues(Image, x, y, &R, &G, &B);
			SetRGBValues(Image, x, y, 255 - (int)R, 255 - (int)G, 255 - (int)B);
		}
	}
}


// Creates an edge detecting image
void EdgeDetecting(SDL_Surface* Image){
	// First it creates a Black and White image so is easier to see the edges
	Blacknwhite(Image); 
	double threshold = 20.0;			// That is the value that can be changed to have more or less noise in the image

	// It checks the pixels on the botom, on the left and in diagonal and the difference on brightness to see if is an edge or not. It changes the pixel to blue if it is and to black if it isn't
	for (int y = 0; y < Image->h; y++){
		for (int x = 0; x < Image->w; x++){
			Uint8 R1 = 0, G = 0, B = 0, R2 = 0, R3 = 0, R4 = 0;
			GetRGBValues(Image, x, y, &R1, &G, &B);
			GetRGBValues(Image, x+1, y+1, &R2, &G, &B);
			GetRGBValues(Image, x+1, y, &R3, &G, &B);
			GetRGBValues(Image, x, y+1, &R4, &G, &B);

			double D1 = (int)R1 - (int)R2;
			double D2 = (int)R4 - (int)R3;
			double resultado = sqrt(D1*D1 + D2*D2);

			if ( resultado >= threshold){
				SetRGBValues(Image, x, y, 0, 0, 255);
			}
			else SetRGBValues(Image, x, y, 0, 0, 0);
		}
	}
}

// Creates an image from an area of the original image cropping it
SDL_Surface* Cropping(SDL_Surface* ImageSurface, int *width, int *height){
	cout << "Select with your mouse the section you would like to cropp." << endl;
	int open = 1;
	int x1, y1, x2, y2;			// Variables for the first position of the mouse and the second
	SDL_Event event;

	// Display the image in the window
	DisplaySurface(ImageSurface);

	// While we aren't closing the window...
	while (open == 1){
		if (SDL_PollEvent(&event)){
			// Close the window if it is closed
			if (CheckClosePressed(&event)){
				open = 0;
				continue;
			}

			// This detects the coordinates when the mouse button is pressed and again when is released to crop the image and calculates the width and height of the new image
			switch (event.type){
			case SDL_MOUSEBUTTONDOWN:
				x1 = event.button.x;
				y1 = event.button.y;
				break;
			case SDL_MOUSEBUTTONUP:
				x2 = event.button.x;
				y2 = event.button.y;
				if (x1 < x2)  *width = x2 - x1;
				else *width = x1 - x2;
				if (y1 < y2) *height = y2 - y1;
				else *height = y1 - y2;
				open = 0;
				break;
			default:
				break;
			}
		}
	}
	CloseWindow(); // Close the window (you don't have to do this, this is just an example of what it does).

	// This creates the new Image
	SDL_Surface* newImage = CreateNewImage(*width, *height);

	for (int y = 0; y < newImage->h; y++){
		for (int x = 0; x < newImage->w; x++){
			Uint8 R = 0, G = 0, B = 0;
			GetRGBValues(ImageSurface, x + x1, y + y1, &R, &G, &B);
			SetRGBValues(newImage, x, y, R, G, B);
		}
	}

	return newImage;
}


// This rotates the image in a certain angle
SDL_Surface* Rotate(SDL_Surface* Image, int *width, int *height){
	float angle;
	cout << "Please enter the angle of the rotation in degrees." << endl;
	
	// Next lines are to make sure that the user input a valid number (from 0 onwards) by looking at each character and look if is a digit or not
	string str;
	cin >> str;

	for (int i = 0; i < str.size(); i++){
		if (!isdigit(str[i])){
			cout << "WARNING: You are trying to enter an invalid input." << endl;
			angle = 0;
			_sleep(5000);
			break;
		}
		else {
			angle = atoi(str.c_str());
		}
	}


	// In order to not get out of bounds:
	while (angle >= 360) angle -= 360;
	while (angle < 0) angle += 360;
	angle = angle * 2 * PI / 360;		// Passing the angle into radians

	//	Calculates the new heights and widhts 
	float w = *width;
	float h = *height;
	float h1 = w*sin(angle);
	float h2 = h*cos(angle);
	float w1 = h*sin(angle);
	float w2 = w*cos(angle);

	w = abs(w1) + abs(w2);
	h = abs(h1) + abs(h2);

	*width = w;
	*height = h;
	int x2, y2;

	// Creates tyhe new image with the new width and height 
	// depending if the angle is between 0 and 90, 90 and 180, 180 and 270 or 270 and 360
	SDL_Surface* newImage = CreateNewImage(w, h);
	for (int y = 0; y < Image->h; y++){
		for (int x = 0; x < Image->w; x++){
			Uint8 R = 0, G = 0, B = 0;
			GetRGBValues(Image, x, y, &R, &G, &B);
			x2 = x*cos(angle) + y*sin(angle);
			y2 = -x*sin(angle) + y*cos(angle);
			if (angle <= (float)(PI / 2)) y2 += h1;
			else if (angle <= (float)PI) {
				y2 += h - 1;
				x2 += abs(w2);
			}
			else if (angle <= (float)(PI * 3 / 2)){
				y2 += abs(h2);
				x2 += w;
			}
			else if (angle <= (float)(PI * 2))	x2 += abs(w1);
			SetRGBValues(newImage, x2, y2, R, G, B);
		}
	}

	// Some pixels are in black because the new pixels had to be an in an integer position of x and y. 
	// That is why those black pixels have to be filed with the next loop.
	for (int y = 0; y < newImage->h; y++){
		for (int x = 0; x < newImage->w; x++){
			if (x != 0 && x != newImage->w - 1){
				Uint8 R1 = 0, G1 = 0, B1 = 0, R2 = 0, G2 = 0, B2 = 0, R3 = 0, G3 = 0, B3 = 0;
				GetRGBValues(newImage, x, y, &R2, &G2, &B2);
				GetRGBValues(newImage, x + 1, y, &R3, &G3, &B3);
				if ((int)R2 == 0 && (int)G2 == 0 && (int)B2 == 0 && (int)R3 != 0 && (int)G3 != 0 && (int)B3 != 0){
					GetRGBValues(newImage, x - 1, y, &R1, &G1, &B1);
					SetRGBValues(newImage, x, y, R1, G1, B1);
				}
			}
		}
	}
	return newImage;
}


// It scales The image making it bigger or smaller depending on the percentatge input by the user
SDL_Surface* Scale(SDL_Surface* Image, int *width, int *height){
	float Size;
	cout << "Enter the percentage of the size of the image you want." << endl;

	// Next lines are to make sure that the user input a valid number (from 0 onwards) by looking at each character and look if is a digit or not
	string str;
	cin >> str;

	for (int i = 0; i < str.size(); i++){
		if (!isdigit(str[i])){
			cout << "WARNING: You are trying to enter an invalid input." << endl;
			Size = 100;
			_sleep(5000);
			break;
		}
		else {
			Size = atoi(str.c_str());
		}
	}
	cout << "Loading... Please wait." << endl;

	Size /= 100;

	// It calculates the new size of the image
	int h = *height * Size;
	int w = *width * Size;
	
	*width = w;
	*height = h;

	SDL_Surface* newImage = CreateNewImage(w, h);
	return ShrinkImage(Image, newImage, Size);
}

// Resizes the image. Is in a different function because is used inside the watermark with constant variables instead of an input from the user
SDL_Surface* ShrinkImage(SDL_Surface* Image, SDL_Surface* newImage, float Size){
	for (int y = 0; y < newImage->h; y++){
		for (int x = 0; x < newImage->w; x++){
			Uint8 R = 0, G = 0, B = 0;
			GetRGBValues(Image, x/Size, y/Size, &R, &G, &B);
			SetRGBValues(newImage, x, y, R, G, B);
		}
	}

	return newImage;
}


// Creates a watermark with a new image input by the user
// It loads the new image input by the user and converts it into a black and white image and resizes it in order to fit the original image
// and takes the position of the darkest pixels and in the original image at the same position but centered adds brightness to that pixel creating the watermark
void Watermark(SDL_Surface* Image, int width, int height){
	string Location;
	cout << "Input the location of the image you want to be the water mark." << endl;
	cin >> Location;
	//Location = "D:/Users/Kikadass/Desktop/Logo-Apple-iPhone.jpg";  // Just for testing

	SDL_Surface* newImage1 = TakeImage(Location);		// Loads the image from a folder to the program

	cout << "Loading... Please wait." << endl;

	// Get The Array of the Pixels (Access to the Raw Data Pointer)
	// This is a 1D array, however represents the entire image. 
	int w, h;
	Uint8* ImageArray = GetPixelArray(newImage1, &w, &h);

	Blacknwhite(newImage1);			// Creates the black and white image of the new image

	// Next lines Looks for the percentatge of change that the new image has to change in order to fit the image 
	float Size;
	
	if (w > width || h > height){
		if (w >= h) Size = (float)width / (float)w;
		else Size = (float)height / (float)h;
	}
	else Size = 1;

	// Calculates the height and width of the new image
	h = h * Size;
	w = w * Size;

	SDL_Surface* newImage = CreateNewImage(w, h);
	newImage = ShrinkImage(newImage1, newImage, Size);

	// the next loops are the ones that looks the position of the pixels to change and change the brightness of the pixels in the original image 
	for (int y = 0; y < newImage->h; y++){
		for (int x = 0; x < newImage->w; x++){
			Uint8 R1 = 0, G = 0, B = 0, R2 = 0;
			GetRGBValues(newImage, x, y, &R1, &G, &B);
			if ((int)R1 <= 125){
				GetRGBValues(Image, x + Image->w / 2 - w / 2, y + Image->h / 2 - h / 2, &R2, &G, &B);
				int bright = 3;
				if ((int)R2*bright <= 255 && (int)G*bright <= 255 && (int)B*bright <= 255){
					SetRGBValues(Image, x + Image->w/2 - w/2, y + Image->h/2 - h/2, (int)R2*bright, (int)G*bright, (int)B*bright);
				}
				else {
					int a, b, c;
					if ((int)R2*bright >= 255) a = 255;
					else a = (int)R2*bright;
					if ((int)G*bright >= 255) b = 255;
					else b = (int)G*bright;
					if ((int)B*bright >= 255) c = 255;
					else c = (int)B*bright;
					SetRGBValues(Image, x + Image->w / 2 - w / 2, y + Image->h / 2 - h / 2, a, b, c);
				}
			}
		}
	}
}

// It opens a new menu user friendly where the user can modify the colors by adding or substracting red, green and blue
// It always takes the previous image in order to modify it and it only saves the changes when the user exits that menu in order to go to the original menu
SDL_Surface* ChangeColor(int saves, int choice, int RED, int GREEN, int BLUE){
	
	// Next lines of code are to take the previous image autosaved
	if (saves == 10) saves--;
	string Location = AutosavesLocation;
	Location += saves + 47;
	Location += ".bmp";
	SDL_Surface* newImage = TakeImage(Location);

	system("cls");		// Clears the console
	int open = 1;		// Can also be a boolean for the while loop

	cout << "Change the color of your image." << endl << "Press the arrows on your keyboard (UP, DOWN, LEFT & RIGHT) and then ENTER to choose the quit." << endl;
	
	// This sets the limits of color changes. 
	if (RED > 10) RED = 10;
	if (RED < 0) RED = 0;
	if (GREEN > 10) GREEN = 10;
	if (GREEN < 0) GREEN = 0;
	if (BLUE > 10) BLUE = 10;
	if (BLUE < 0) BLUE = 0;

	// This changes the color of the pixels but checking that it doesn't get over 255.
	for (int y = 0; y < newImage->h; y++){
		for (int x = 0; x < newImage->w; x++){
			Uint8 R = 0, G = 0, B = 0;
			GetRGBValues(newImage, x, y, &R, &G, &B);
			if ((float)R*(float)RED*0.2 <= 255 && (float)G*(float)GREEN*0.2 <= 255 && (float)B*(float)BLUE*0.2 <= 255){
				SetRGBValues(newImage, x, y, (float)R*(float)RED*0.2, (float)G*(float)GREEN*0.2, (float)B*(float)BLUE*0.2);
			}
			else {
				int a, b, c;
				if ((float)R*(float)RED*0.2 >= 255) a = 255;
				else a = (float)R*(float)RED*0.2;
				if ((float)G*(float)GREEN*0.2 >= 255) b = 255;
				else b = (float)G*(float)GREEN*0.2;
				if ((float)B*(float)BLUE*0.2 >= 255) c = 255;
				else c = (float)B*(float)BLUE*0.2;
				SetRGBValues(newImage, x, y, a, b, c);
			}
		}
	}


	// Next two lines are for not getting out of bounds
	if (choice > 3) choice = 3;
	if (choice < 0) choice = 0;

	// The menu is shown depending on the choice that the user can use by pressing enter
	for (int i = 0; i < 4; i++){
		if (i == choice) cout << "[ ";
		else cout << "  ";
		if (i == 0) {
			cout << "RED   [";
			for (int j = 0; j < RED; j++){
				cout << "|";
			}
			for (int j = RED; j < 10; j++){
				cout << " ";
			}
			cout << "]";
		}
		if (i == 1) {
			cout << "GREEN [";
			for (int j = 0; j < GREEN; j++){
				cout << "|";
			}
			for (int j = GREEN; j < 10; j++){
				cout << " ";
			}
			cout << "]";
		}
		if (i == 2) {
			cout << "BLUE  [";
			for (int j = 0; j < BLUE; j++){
				cout << "|";
			}
			for (int j = BLUE; j < 10; j++){
				cout << " ";
			}
			cout << "]";
		}
		if (i == 3) cout << "QUIT";
		if (i == choice) cout << " ]";
		cout << endl;
	}

	SDL_Event event;										// Creates an event for looking for the keys pressed
	CreateWindow("Change Color", newImage->w, newImage->h);			// Creates the window
	DisplaySurface(newImage);									// Displays the image

	// while there is not choice
	while (open == 1){
		if (SDL_PollEvent(&event)){

			switch (event.type){
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym){
				case SDLK_UP:								// If Up is pressed (arrow) go to the previous choice
					open = 0;
					choice--;
					return ChangeColor(saves, choice, RED, GREEN, BLUE);
					break;
				case SDLK_DOWN:								// If Down is pressed (arrow) go to next choice
					open = 0;
					choice++;
					return ChangeColor(saves, choice, RED, GREEN, BLUE);
					break;
				case SDLK_LEFT:								// If Left is pressed (arrow) substract some color depending on the choice
					if (choice < 3){
						open = 0;
						CloseWindow();
						if (choice == 0) {
							RED--;
							return ChangeColor(saves, choice, RED, GREEN, BLUE);
						}
						if (choice == 1) {
							GREEN--;
							return ChangeColor(saves, choice, RED, GREEN, BLUE);
						}
						if (choice == 2) {
								BLUE--;
								return ChangeColor(saves, choice, RED, GREEN, BLUE);
							}
					}
					break;
				case SDLK_RIGHT:							// If Left is pressed (arrow) substract some color depending on the choice
					if (choice < 3){
						open = 0;
						CloseWindow();
						if (choice == 0) {
							RED++;
							return ChangeColor(saves, choice, RED, GREEN, BLUE);
						}
						if (choice == 1) {
							GREEN++;
							return ChangeColor(saves, choice, RED, GREEN, BLUE);
						}
						if (choice == 2) {
							BLUE++;
							return ChangeColor(saves, choice, RED, GREEN, BLUE);
						}
					}
					break;
				case SDLK_KP_ENTER:							// If Enter in the number pad is pressed return the choice (User has selected a choice)
					if (choice == 3){
						cout << "Loading... Please wait." << endl;
						open = 0;
						CloseWindow();
						return newImage;
					}
					break;
				case SDLK_RETURN:							// If Enter in the main keyboard is pressed return the coice (User has selected a choice)
					if (choice == 3){
						cout << "Loading... Please wait." << endl;
						open = 0;
						CloseWindow();
						return newImage;
					}
				default:
					break;
				}

				break;
			default:
				break;
			}
		}
	}
}


// It creates the image and waits until the user closes the image or presses ESC to close it
void CreateImage(SDL_Surface* ImageSurface, string name, int width, int height){
	// Creates a window called 'Lighthouse' at the same width and height as the original image
	CreateWindow(name.c_str(), width, height);
	cout << "To continue: close the window, or press ESC in window." << endl;
	DisplaySurfaceUntilClose(ImageSurface); // Display the image in the window
	CloseWindow(); // Close the window (you don't have to do this, this is just an example of what it does).
}


// Detects if the user pressed ESC or closed the window to continue with the program
void DisplaySurfaceUntilClose(SDL_Surface* Surface){
	int open = 1; // In C++, one would ordinarily use bool for this. In C, something like this has to be used.
	SDL_Event event;

	// Display the image in the window
	DisplaySurface(Surface);

	// While we aren't closing the window...
	while (open == 1){
		if (SDL_PollEvent(&event)){
			// Close the window if it is closed
			if (CheckClosePressed(&event)){
				open = 0;
				continue;
			}

			/* This switch statement shows an example of how to check keys that the user has pressed. Most keys on a keyboard can be checked using the SDLK_<key> enum values,
			for instance SDLK_q specifies the 'q' key, or SDLK_RETURN specifies the return/enter key (not the one on the number pad).

			If you are interested, many other events can be checked; however you will have to research the SDL 1.2 documentation yourself! */
			switch (event.type){
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym){
				case SDLK_ESCAPE:
					open = 0;
					break;
				default:
					break;
				}

				break;
			default:
				break;
			}
		}
	}
}