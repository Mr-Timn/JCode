/*
	Constructor: {
		Parameters: [
			"",
		],
		Description: [
			"",
		],
		Examples: [
			""
		]
	}

		: {
			Type: "",
			Returns: [ "" ],
			Parameters: [ "" ],
			Description: [ "" ],
			Examples: [ "" ],
			Variations: [ "" ]
		}
*/

var window_api = {
	Name: "window_api",
	Constructor: {
		Parameters: [
			"string Title - Title of the Window",
			"int Width - Width of the Window",
			"int Height - Height of the Window",
			"int Flags - Flags to init window with. Ex. SDL_WINDOW_FULLSCREEN, SDL_WINDOW_SHOWN",
			"SDL_Color BackgroundColor - Default background color of the Window",
			"int FPS - Maximum number of frames the Window can run at if yieldUntilNextFrame() is used"
		],
		Description: [
			"Generates a new Window",
			"Does not actually launch the window. You have to call start() after the window is created"
		],
		Examples: [
			"Window window1 = new Window(\"My Window 1\", 1920, 1080, SDL_WINDOW_FULLSCREEN, { 143, 210, 180 }, 60);",
			"Window window2 = new Window(\"My Window 2\", 1080, 720, SDL_WINDOW_SHOWN, WindowColor.white, 30);"
		]
	},
	Functions: {
		break: { Type: "Window" },
		
		start: {
			Type: "bool",
			Returns: [ "True if Window launched correctly", "False if otherwise" ],
			Parameters: [ "None" ],
			Description: [ "Starts the Window" ],
			Examples: [ "window.start()" ]
		},
		close: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ "None" ],
			Description: [ "Closes the Window" ],
			Examples: [ "window.close()" ]
		},
		fullscreen: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ "int Type - Level of fullscreen" ],
			Description: [
				"Sets the desired window Fullscreen mode",
				"Use NULL to return to windowed mode",
				"To launch window in fullscreen, set the flag to SDL_WINDOW_FULLSCREEN or SDL_WINDOW_FULLSCREEN_DESKTOP"
			],
			Examples: [
				"window.fullscreen(SDL_WINDOW_FULLSCREEN_DESKTOP))",
				"window.fullscreen(SDL_WINDOW_FULLSCREEN)",
				"window.fullscreen(NULL)"
			]
		},
		clear: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ "None" ],
			Description: [ "Clears the window's screen" ],
			Examples: [ "window.clear()" ]
		},
		load: { 
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ "None" ],
			Description: [ "Presents everything draw to the window, moving the frame forward" ],
			Examples: [ "window.load()" ]
		},
		updateEvents: {
			Type: "bool",
			Returns: [ "False if window was closed", "True otherwise" ],
			Parameters: [ "None" ],
			Description: [
				"Updates all events that happened in the window",
				"This includes input from the mouse, keyboard, and any connected controllers"
			],
			Examples: [ "window.updateEvents()" ]
		},
		yieldUntilNextFrame: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ "None" ],
			Description: [
				"Pauses current thread until the frame limiter catches up to the threads time",
				"Shold be used at the end of "
			],
			Examples: [ "window.yieldUntilNextFrame()" ]
		},
		
		break: { Type: "Console" },
		
		disableConsole: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ "Nothing" ],
			Description: [ "Disbles text updates to the console" ],
			Examples: [ "window.disableConsole()" ],
			Variations: [ "None" ]
		},
		enableConsole: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ "Nothing" ],
			Description: [ "Enables text updates to the console" ],
			Examples: [ "window.enableConsole()" ],
			Variations: [ "None" ]
		},
		canPrintErrors: {
			Type: "bool",
			Returns: [ "True if printing updates on the window console is disabled", "False otherwise" ],
			Parameters: [ "Nothing" ],
			Description: [ "Checks if window can print updates" ],
			Examples: [ "bool chk = window.canPrintErrors()" ],
			Variations: [ "None" ]
		},
		hideConsole: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ "Nothing" ],
			Description: [ "Hides the window console" ],
			Examples: [ "window.hideConsole()" ],
			Variations: [ "None" ]
		},
		showConsole: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ "Nothing" ],
			Description: [ "Reveals the window console" ],
			Examples: [ "window.showConsole()" ],
			Variations: [ "None" ]
		},

		break: { Type: "FPS Functions" },

		trackFPS: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ "None" ],
			Description: [
				"Starts fps tracking thread",
				"Should only be called once"
			],
			Examples: [ "window.trackFPS()" ]
		},
		killFPSThread: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ "None" ],
			Description: [ "Stops FPS thread if it is currently running" ],
			Examples: [ "window.killFPSThread()" ]
		},
		getFPS: {
			Type: "int",
			Returns: [ "Current FPS count for that second" ],
			Parameters: [ "None" ],
			Description: [ "Used to get the amount of frames that were displayed within a given second" ],
			Examples: [ "int fps = window.getFPS()" ]
		},
		
		break: { Type: "Text" },

		addTextTexture: {
			Type: "WindowText*",
			Returns: [
				"Pointer to created WindowText object",
				"&NULLTEXTTEXTURE if texture has already been added"
			],
			Parameters: [
				"string Name - Location of the font. Typically .ttf",
				"int SizeLimit - Amount of font to be loaded"
			],
			Description: [
				"Adds font to the window",
				"If SizeLimit isn't given it defaults to 100"
			],
			Examples: [ "window.addFont(\"Fonts/Ariel.ttf\", 20)" ],
			Variations: [ "addFont(string Name)" ]
		},
		getTextTexture: {
			Type: "WindowText*",
			Returns: [ 
				"The texttexture's representative WindowText", 
				"&NULLTEXTTEXTURE if the text texture doesn't exist" 
			],
			Parameters: [ "string TextID - Name of the text texture" ],
			Description: [ "" ],
			Examples: [ "WindowText* mytext = window.getTextTexture(\"MyText1\")" ],
			Variations: []
		},
		addFont: {
			Type: "int",
			Returns: [
				"WindowSuccess if font was added",
				"InvalidFont if font alreday exists"
			],
			Parameters: [
				"string Name - Location of the font. Typically .ttf",
				"int SizeLimit - Amount of font to be loaded"
			],
			Description: [
				"Adds font to the window",
				"If SizeLimit isn't given it defaults to 100"
			],
			Examples: [ "window.addFont(\"Fonts/Ariel.ttf\", 20)" ],
			Variations: [ "addFont(string Name)" ]
		},
		setFont: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [
				"string NewFont - Name of the new font",
				"int Size - Desired size of the font",
				"SDL_Color FontColor - Desired color of the font"
			],
			Description: [ "Sets the window's font" ],
			Examples: [ "window.setFont(\"Fonts/Ariel.ttf\", 20, WindowColor.black)" ],
			Variations: [
				"setFont(string NewFont)",
				"setFontColor(SDL_Color FontColor)",
				"setFontSize(int Size)"
			]
		},
		writeText: {
			Type: "int",
			Returns: [ "WindowSuccess", "InvalidFont", "FontTooSmall", "NoFontsLoaded", "InvalidFontSize" ],
			Parameters: [
				"SDL_Texture *Texture - Texture to load to window",
				"string Text - String version of the text to add",
				"int X - X position of the text",
				"int Y - Y position of the text",
				"int Width - Width of the box around the text",
				"int Height - Height of the box around the text",
				"double Angle - Angle of the text box, rotated counter-clockwise",
				"WindowTextType Fill - Fill method the text box",
				"WindowTextType Fit - Fit method of the text box",
				"int Buffer - Recursion variable, just set as NULL"
			],
			Description: [ 
				"Writes text to the window",
				"If processing speed isn't a concern use writeTextDynamic(...)",
				"Uses more memory than writeTextDymanic(...)"
			],
			Examples: [ "window.writeText(NULL, \"Hello World!\", 100, 150, 200, 40, 30.5, TextFillX, TextFit, 0)" ],
			Variations: [
				"window.writeText(string Text, int X, int Y)",
				"window.writeText(string Text, int X, int Y, double Angle)",
				"window.writeText(string Text, int X, int Y, int Width, int Height)",
				"window.writeText(string Text, int X, int Y, int Width, int Height, double Angle)",
				"window.writeText(string Text, int X, int Y, int Width, int Height, WindowTextType Fill, WindowTextType Fit)",
				"window.writeText(string Text, int X, int Y, int Width, int Height, double Angle, WindowTextType Fill, WindowTextType Fit)",
				"window.writeText(string Text, SDL_Rect Size)",
				"window.writeText(string Text, SDL_Rect Size, double Angle)",
				"window.writeText(string Text, SDL_Rect Size, WindowTextType Fill, WindowTextType Fit)",
				"window.writeText(string Text, SDL_Rect Size, double Angle, WindowTextType Fill, WindowTextType Fit)",
				"window.writeText(string Text, int X, int Y, int Width, int Height, double Angle, WindowTextType Fill, WindowTextType Fit, int Buffer)",
				"window.writeText(SDL_Texture* Texture, string Text, int X, int Y, int Width, int Height, double Angle, WindowTextType Fill, WindowTextType Fit, int Buffer)",
				"window.writeTextDynamic(string Text, SDL_Rect Size, double Angle)"
			]
		},

		break: { Type: "Keyboard and Mouse" },

		getKeyState: {
			Type: "WindowKeyState",
			Returns: [ "WindowKeyDown", "WindowKeyUp", "WindowKeyUp" ],
			Parameters: [ "int key - Keycode to check" ],
			Description: [ "Gets the current KeyState of a given key" ],
			Examples: [ "window.getKeyState(SDL_GetScancodeFromName(\"B\"))" ]
		},
		checkInput: {
			Type: "bool",
			Returns: [ "True if the Key mathes the expected state", "False otherwise" ],
			Parameters: [ 
				"string Key - Key to check",
				"WindowKeyState Expected - Expected key state",
				"bool IncludeJoysticks - Check joystick input as well"
			],
			Description: [ "Returns if the given keystate matches the value of the given key" ],
			Examples: [ "bool chk = window.checkInput(\"A\", WindowKeyState::WindowKeyClicked, false)" ],
			Variations: [ "None" ]
		},
		leftClickedAt: {
			Type: "bool",
			Returns: [ "true", "false" ],
			Parameters: [
				"int X - X coordinate to check",
				"int Y - Y coordinate to check"
			],
			Description: [ "Checks if the user left clicked in the given bound or coordinates" ],
			Examples: [
				"window.leftClickedAt(new int[2]{ 50, 100 }, new int[2]{ 75, 200 })",
				"window.leftClickedAt({ 50, 75, 50, 135 })",
				"window.rightClickedAt(..)",
				"window.middleClickedAt(...)"
			],
			Variations: [
				"window.leftClickedAt(int X[], int Y[])",
				"window.leftClickedAt(SDL_Rect Size)"
			]
		},

		break: { Type: "Geometry" },

		setColor: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [
				"SDL_Color Color - Color to set",
				"int Alpha - Transparency of color"
			],
			Description: [ "Sets the windows drawing color" ],
			Examples: [
				"window.setColor(WindowColor.blue, 180)",
				"window.setColor(WindowColor.purple)",
			 	"window.setColor({ 0, 230, 156, 255 })"
			],
			Variations: [ "window.setColor(SDL_Color Color)" ]
		},
		drawSolidRectangle: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [
				"SDL_Rect Size - Size of the rectangle",
				"SDL_Color Color - Color of the recangle",
				"int Alpha - Transparency of the rectangle"
			],
			Description: [ "Draws a solid rectangle to the window" ],
			Examples: [
				"window.drawSolidRectangle({ 20, 40, 100, 50 }, WindowColor.black, 200)",
				"window.drawSolidRectangle({ 20, 40, 100, 50 }, { 0, 0, 0, 200 })"
			],
			Variations: [ "window.drawSolidRectangle(SDL_Rect Size, SDL_Color Alpha)" ]
		},
		drawHollowRectangle: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [
				"SDL_Rect Size - Size of the rectangle",
				"SDL_Color Color - Color of the recangle",
				"int Alpha - Transparency of the rectangle"
			],
			Description: [ "Draws an outlined rectangle to the window" ],
			Examples: [
				"window.drawHollowRectangle({ 20, 40, 100, 50 }, WindowColor.black, 200)",
				"window.drawHollowRectangle({ 20, 40, 100, 50 }, { 0, 0, 0, 200 })"
			],
			Variations: [ "window.drawHollowRectangle(SDL_Rect Size, SDL_Color Alpha)" ]
		},
		drawLine: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [
				"SDL_Rect Size - Size of the line",
				"SDL_Color Color - Color of the line",
				"int Alpha - Transparency of the line"
			],
			Description: [ "Draws a line to the window" ],
			Examples: [
				"window.drawLine({ 50, 0, 50, 600 }, WindowColor.black, 200)",
				"window.drawLine({ 50, 0, 50, 600 }, { 0, 0, 0, 200 })",
				"window.drawLine(50, 0, 50, 600, WindowColor.black, 200)",
				"window.drawLine(50, 0, 50, 600, { 0, 0, 0, 200 })"
			],
			Variations: [
				"window.drawLine(SDL_Rect Size, SDL_Color Alpha)",
				"window.drawLine(int X1, int Y1, int X2, int Y2, SDL_Color Color)",
				"window.drawLine(int X1, int Y1, int X2, int Y2, SDL_Color Color, int Alpha)"
			]
		},
		drawPoint: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [
				"int X - X coordinate of the point",
				"int Y - Y coordinate of the point",
				"SDL_Color Color - Color of the point",
				"int Alpha - Transparency of the point"
			],
			Description: [ "Draws a point to the window" ],
			Examples: [
				"window.drawPoint(150, 300, WindowColor.black, 200)",
				"window.drawPoint(150, 300, { 0, 0, 0, 200 })"
			],
			Variations: [ "window.drawLine(int X, int Y, SDL_Color Alpha)" ]
		},

		break: { Type: "Images" },

		addImage: {
			Type: "WindowImage*",
			Returns: [ "The representative WindowImage of the added image" ],
			Parameters: [ "string Address - Address of the image" ],
			Description: [ "Adds image to the window so it can be loaded" ],
			Examples: [ "window.addImage(\"Images/Background/mybackground.png\")" ],
			Variations: [ "" ]
		},
		getImage: {
			Type: "WindowImage*",
			Returns: [ "The image's WindowImage", "NULL if the image wasn't found" ],
			Parameters: [ "string Address - Address of the image" ],
			Description: [ "Returns the representative WindowImage of the image" ],
			Examples: [
				"WindowImage *myimg = window.getImage(\"Images/myimg.png\")",
				"WindowImage myimg = window.getImageA(\"Images/myimg\")"
			],
			Variations: [ "window.getImageA(string Address)" ]
		},
		drawImage: {
			Type: "int",
			Returns: [ "InvalidImage", "WindowSuccess" ],
			Parameters: [
				"string Address - Address of the image",
				"int X - X coordinate of the image",
				"int Y - Y coordinate of the image",
				"int Width - Width of the images",
				"int Height - Height of the image",
				"double Angle - Clockwise rotation of the image",
				"int Flip - Flip flag",
				"Uint8 Transparency - Transparency of the image",
				"SDL_Rect Zoom - Zoom bounds on the image"
			],
			Description: [
				"Draws an image to the window",
				"drawImageFlipped(...) exists for all variations of drawImage(...)"
			],
			Examples: [
				"window.drawImage(\"Images/myimg.png\", { 20, 20, 100, 50 })"
			],
			Variations: [
				"window.drawImage(string Address, int X, int Y)",
				"window.drawImage(string Address, int X, int Y, Uint8 Transparency)",
				"window.drawImage(string Address, int X, int Y, double Angle)",
				"window.drawImage(string Address, int X, int Y, double Angle, Uint8 Transparency)",
				"window.drawImage(string Address, int X, int Y, int Width, int Height)",
				"window.drawImage(string Address, int X, int Y, int Width, int Height, Uint8 Transparency)",
				"window.drawImage(string Address, int X, int Y, int Width, int Height, double Angle)",
				"window.drawImage(string Address, int X, int Y, int Width, int Height, double Angle, Uint8 Transparency)",
				"window.drawImage(string Address, SDL_Rect Rect)",
				"window.drawImage(string Address, SDL_Rect Rect, Uint8 Transparency)",
				"window.drawImage(string Address, SDL_Rect Rect, double Angle)",
				"window.drawImage(string Address, SDL_Rect Rect, double Angle, Uint8 Transparency)",
				"window.drawImage(string Address, int X, int Y, int Width, int Height, double Angle, itn Flip, Uint8 Transparency, SDL_Rect Zoom)"
			]
		},
		getTotalImages: {
			Type: "int",
			Returns: [ "The count of images generated from window.countImagesInDirectory(...)" ],
			Parameters: [ "" ],
			Description: [],
			Examples: [ "int maximgs = window.getTotalImages()" ],
			Variations: [ "" ]
		},
		countImagesInDirectory: {
			Type: "int",
			Returns: [ "The number of images within the given directory" ],
			Parameters: [ "string Directory - Directory to search" ],
			Description: [ "Generates and returns the number of images within the given directory" ],
			Examples: [ "window.countImagesInDirectory(\"Images/PreLoad/1\")" ],
			Variations: [ "window.countImagesInDirectory(vector<string> Directories)" ]
		},
		loadImagesFromDirectory: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [
				"string Directory - Directory to load from",
			 	"void CounterFunction - Function called each image tick"
			],
			Description: [
				"Loads all the images in directory to the window",
				"Use &NULLLOADFUNCTION if you don't want to use a count function"
			],
			Examples: [ "window.loadImagesFromDirectory(\"Images\")" ],
			Variations: [ "window.loadImagesFromDirectory(vector<string> Directory)" ]
		},
		
		break: { Type: "Buttons" },
		
		addButton: {
			Type: "int",
			Returns: [ "ButtonExists", "WindowSuccess" ],
			Parameters: [
				"string Name - Name/ID of the button",
				"string Text - Text on the button",
				"SDL_Rect Size - Size of the button",
				"SDL_Color Color - Color of the button",
				"SDL_Color TextColor - Color of the button's text",
				"WindowTextType TextType - Fill style of the buttons text",
				"WindowTextType TextFit - Fit style of the buttons text",
				"string Texture - Image on the button",
				"WindowDrawMode DrawMode - Drawing orientation of the button"
			],
			Description: [ "Adds a button to the screen" ],
			Examples: [ "window.addButton(\"button1\", \"Hello World!\", { 50, 200, 100, 30 }, WindowColor.green, WindowColor.black, TextFit, TextFill, \"Images/bimg1.png\", DrawDefault)" ],
			Variations: [
				"window.addButton(string Name, string Text, SDL_Rect Size, SDL_Color Color, SDL_Color TextColor, WindowTextType TextType, WindowTextType TextFit)",
				"window.addButton(string Name, string Text, SDL_Rect Size, SDL_Color Color, SDL_Color TextColor, WindowTextType TextType, WindowTextType TextFit, string Texture)",
			]
		},
		getButton: {
			Type: "WindowButton*",
			Returns: [ "The button's WindowButton", "NULL if the button isn't found" ],
			Parameters: [ "string Name - Name of the button" ],
			Description: [ "Returns the representative WindowButton of the button" ],
			Examples: [ "WindowButton *mybuton = getButton(\"Button1\")" ],
			Variations: [
				"window.getButtonA(string Name)",
				"window.getSelectedButton()",
				"unordered_map<string, WindowButton>* buttons = window.getAllButtons()"
			]
		},
		drawButton: {
			Type: "int",
			Returns: [ "WindowSuccess", "InvalidButton" ],
			Parameters: [ "string Name - Name of the button" ],
			Description: [ "Draws the button to the window" ],
			Examples: [ "window.drawButton(\"Button1\")" ],
			Variations: [ "" ]
		},
		unselectButton: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ "None" ],
			Description: [ "Unselects the selected button if there is one" ],
			Examples: [ "window.unselectButton()" ],
			Variations: [ "" ]
		},
		nextButton:  {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ "None" ],
			Description: [
				"Selects the next available button",
			 	"Button is selected in the order that it is added"
			],
			Examples: [ "window.nextButton()" ],
			Variations: [ "" ]
		},
		previousButton:  {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ "None" ],
			Description: [
				"Selects the previous available button",
			 	"Button is selected in the order that it is added"
			],
			Examples: [ "window.previousButton()" ],
			Variations: [ "" ]
		},

		break: { Type: "TextBoxes" },
		
		addTextBox: {
			Type: "int",
			Returns: [ "TextBoxExists", "WindowSuccess" ],
			Parameters: [
				"string Name - Name/ID of the textbox",
				"SDL_Rect Size - Size of the textbox",
				"SDL_Color Outline - Outine color of the textbox",
				"SDL_Color TextColor - Color of the text",
				"WindowDrawMode DrawMode - Drawmode of the textbox"
			],
			Description: [ "Adds a TextBox to the screen" ],
			Examples: [ "window.addTextBox(\"TextBox1\", { 50, 200, 100, 30 }, WindowColor.black, WindowColor.white, DrawDefault)" ],
			Variations: [
				"window.addTextBox(string Name, SDL_Rect Size)",
				"window.addTextBox(string Name, SDL_Rect Size, SDL_Color Outline, SDL_Color TextColor)"
			]
		},
		getTextBox: {
			Type: "WindowTextBox*",
			Returns: [ "The textbox's WindowTextBox", "NULL if the textbox isn't found" ],
			Parameters: [ "string Name - Name of the textbox" ],
			Description: [ "Returns the representative WindowTextBox of the textbox" ],
			Examples: [ "WindowTextBox *mytextbox = getTextBox(\"textbox1\")" ],
			Variations: [
				"window.getSelectedTextBox()",
				"unordered_map<string, WindowTextBox>* textboxes = window.getAllTextBoxes()"
			]
		},
		drawTextBox: {
			Type: "int",
			Returns: [ "WindowSuccess", "InvalidTextBox" ],
			Parameters: [ "string Name - Name of the textbox" ],
			Description: [ "Draws the textbox to the window" ],
			Examples: [ "window.drawTextBox(\"TextBox1\")" ],
			Variations: [ "window.drawAllTextBoxes()" ]
		},
		unselectTextBox: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ "None" ],
			Description: [ "Unselects the selected textbox if there is one" ],
			Examples: [ "window.unselectTextBox()" ],
			Variations: [ "" ]
		},
		nextTextBox:  {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ "None" ],
			Description: [
				"Selects the next available textbox",
			 	"Textbox is selected in the order that it is added"
			],
			Examples: [ "window.nextTextBox()" ],
			Variations: [ "" ]
		},
		previousTextBox:  {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ "None" ],
			Description: [
				"Selects the previous available textbox",
			 	"Textbox is selected in the order that it is added"
			],
			Examples: [ "window.previousTextBox()" ],
			Variations: [ "" ]
		},

		break: { Type: "Joysticks" },
		
		getJoystick: {
			Type: "WindowJoystick*",
			Returns: [ "The joysticks representative WinowJoyStick", "NULL if the joystick was not found" ],
			Parameters: [ "int ID - ID of the controller" ],
			Description: [ "Returns the representative WindowJoystick of the joystick" ],
			Examples: [
				"WindowJoyStick *js = getJoyStick(0)",
				"WindowJoyStick js = getJoystickA(0)",
				"unordered_map<string, WindowJoystick>* joysticks = window.getAllJoysticks()"
			],
			Variations: [ "getJoyStickA(int ID)" ]
		},
		adjustJoystickSensitivity: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ "int Sensitivity - New sensitivity for the joysticks in the window" ],
			Description: [
				"Changes the sensitivity of all window joysticks",
				"Default sensitivity is 2700"
			],
			Examples: [ "window.adjustJoystickSensitivity(2900)" ],
			Variations: [ "" ]
		},

		break: { Type: "Audio" },

		addAudio: {
			Type: "WindowAudio",
			Returns: [ "The created WindowAudio" ],
			Parameters: [
				"string AudioID - ID of audio",
				"string Address - Address of the audio"
			],
			Description: [ "Adds audio file to the window" ],
			Examples: [ "window.addAudio(\"audio1\", \"Audio/MyAudio1.mp3\")" ],
			Variations: [ "" ]
		},
		getAudio: {
			Type: "WindowAudio",
			Returns: [ "The representative WindowAudio of the audio" ],
			Parameters: [ "string AudioID - ID of the audio" ],
			Description: [ "Return the WindowAudio of the audio" ],
			Examples: [
				"WindowAudio *audio1 = getAudio(\"Audio/MyAudio1.mp3\")",
				"WindowAudio audio1 = getAudioA(\"Audio/MyAudio1.mp3\")",
				"unordered_map<string, WindowAudio>* audios = window.getAllAudio()"
			],
			Variations: [ "" ]
		},

		break: { Type: "Video" },

		addVideo: {
			Type: "WindowVideo",
			Returns: [ "The created WindowVideo" ],
			Parameters: [
				"string Name - Name/ID of the Video",
				"int Loops - Number of times to loop video",
				"bool AutoUpdate - Auto plays the video in the background",
				"int FPS - FPS to run the video at",
				"SDL_Rect Size - Position and size of the video"
			],
			Description: [ "Adds the video to the window" ],
			Examples: [ "window.addVideo(\"Video1\", \"Videos/myvideo1.mp4\", -1, true, 60, { 0, 0, 1920, 1080 })" ],
			Variations: [ "" ]
		},
		getVideo: {
			Type: "WindowVideo",
			Returns: [ "The representative WindowVideo of the video" ],
			Parameters: [ "string Name - Name/ID of the video" ],
			Description: [ "Return the WindowVideo of the video" ],
			Examples: [
				"WindowVideo *video1 = getVideo(\"Videos/myvideo1.mp4\")",
				"WindowVideo video1 = getVideoA(\"Videos/myvideo1.mp4\")",
				"unordered_map<string, WindowVideo>* videos = window.getAllVideos()"
			],
			Variations: [ "" ]
		},
		
		break: { Type: "Resets" },
		
		CLEAR_ALL_WINDOW_DATA: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ "None" ],
			Description: [ "Clears all data from the window" ],
			Examples: [ "window.CLEAR_ALL_WINDOW_DATA()" ],
			Variations: [ "" ]
		},
		clearTextTextures: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ "None" ],
			Description: [ "Clears all text textures from the window" ],
			Examples: [ "window.clearTextTextures()" ],
			Variations: [ "" ]
		},
		clearImages: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ "None" ],
			Description: [ "Removes all loaded images in the window" ],
			Examples: [ "window.clearImages()" ],
			Variations: [ "" ]
		},
		clearButtons: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ "None" ],
			Description: [ "Clears all buttons from the window" ],
			Examples: [ "window.clearButtons()" ],
			Variations: [ "" ]
		},
		clearTextBoxes: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ "None" ],
			Description: [ "Clears all textboxes from the window" ],
			Examples: [ "window.clearTextBoxes()" ],
			Variations: [ "" ]
		},
		clearAudio: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ "None" ],
			Description: [ "Clears all audios from the window" ],
			Examples: [ "window.clearAudio()" ],
			Variations: [ "" ]
		},
		clearVideo: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ "None" ],
			Description: [ "Clears all videos from the window" ],
			Examples: [ "window.clearVideos()" ],
			Variations: [ "" ]
		}
	},
	Objects: {
		Enums: {
			WindowStatus: [
				"WindowSuccess",
				"SDLError",
				"InvalidFont",
				"NoFontsLoaded",
				"FontTooSmall",
				"InvalidFontSize",
				"InvalidObject",
				"InvalidGeometry",
				"InvalidImage",
				"InvalidButton",
				"InvalidTextBox",
				"InvalidJoystick",
				"InvalidAudio",
				"invalidVideo",
				"TextBoxExists",
				"ImageExists",
				"ButtonExists",
				"AudioExists",
				"VideoExists",
				"VideoInitFailed"
			],
			WindowKeyState: [
				"WindowKeyDown",
				"WindowKeyUp",
				"WindowKeyClicked",
				"WindowKeyNull"
			],
			WindowTextType: [
				"WindowTextDefault",
				"WindowTextWrap",
				"WindowTextFill",
				"WindowTextFillX",
				"WindowTextFillY",
				"WindowTextFillScroll",
				"WindowTextFillNone",
				"WindowTextFit",
				"WindowTextFitX",
				"WindowTextFitY",
				"WindowTextFitNone"
			],
			WindowGeometry: [
				"WindowDrawFill",
				"WindowDrawOutline",
				"WindowDrawLine",
				"WindowDrawPont"
			],
			WindowDrawMode: [
				"WindowDrawDefault",
				"WindowDrawDefault",
				"WindowDrawCenter",
				"WindowDrawTopLeft",
				"WindowDrawTopRight",
				"WindowDrawBottomLeft",
				"WindowDrawBottomRight",
				"WindowDrawMiddleX",
				"WindowDrawMiddleY"
			]
		},
		Structs: {
			WindowPropertes: {
				Variables: [
					"string title",
					"int width",
					"int height",
					"int id",
					"int flags",
					"int fps",
					"int fpsframe",
					"int fpslimit",
					"SDL_Color background",
					"WindDrawMode drawmode"
				],
				Description: [
					"Not needed for development and can be disregarded"
				],
				Functions: [ "None" ]
			},
			WindowMouse: {
				Variables: [
					"int x",
					"int y",
					"int scroll",
					"bool down[3]",
					"bool click[3]"
				],
				Description: [
					"Not needed for development and can be disregarded"
				],
				Functions: [ "None" ]
			},
			WindowKeyboard: {
				Variables: [
					"Uint8* state",
					"vector<int> pressedkeys",
					"vector<int> clickedkeys"
				],
				Description: [
					"Not needed for development and can be disregarded"
				],
				Functions: [ "None" ]
			},
			WindowJoystick: {
				Variables: [
					"Window* window",
					"SDL_GameController* controller",
					"SDL_Joystick* joystick",
					"SDL_Joystick joystickid",
					"bool centered",
					"int lastleftx",
					"int lastlefty",
					"int lastrightx",
					"int lastrighty",
					"double leftaxisangle",
					"double rightaxisangle",
					"int int leftaxis[2]",
					"int rightaxis[2]"
				],
				Description: [
					"Object representing each controller in the window",
					"Buttons include SDL_CONTROLLER_BUTTON_*",
					"* - A, B, X, Y, DPAD_UP, DPAD_DOWN, DPAD_LEFT, DPAD_RIGHT, LEFTSHOULDER, RIGHTSHOUDLER, LEFTSTICK, RIGHTSTICK, START, BACK"
				],
				Functions: [
					"WindowKeyState getButtonState(int Button)",
					"bool checkInput(string Key, WindowKeyState Expected)",
					"void setJoystickMap(vector<string> Keys)",
					"void setJoystickMapDefaults(vector<string> Keys)",
					"void delayStickInput(int Stick)",
					"void setSensitivity(int Sensitivity)"
				]
			},
			WindowAudio: {
				Variables: [
					"string id",
					"Mix_Music* audio",
					"bool playing"
				],
				Description: [
					"Object representing audio in the window",
					"No pause - yet"
				],
				Functions: [
					"bool isPlaying()",
					"void play()",
					"void play(int Loops)",
					"void seek(doublt Time)",
					"void volume(int Volume)"
				]
			},
			WindowVideo: {
				Variables: [
					"string name",
					"string address",
					"bool autoupdate",
					"bool replay",
					"int videoindex",
					"int ret",
					"int fps",
					"int loops",
					"SDL_Rect framesize",
					"SDL_Rect drawsize",
					"SDL_Texture* texture"
				],
				Description: [
					"Object representing videos in the window"
				],
				Functions: [
					"int play()",
					"void close()",
					"void pause()",
					"void unpause()",
					"int drawFrame()",
					"int drawFrame(SDL_Rect* Zoom)",
				]
			},
			WidowText: {
				Variables: [
					"string text",
					"string name",
					"string font",
					"unsigned int textsize",
					"SDL_Color fontcolor",
					"SDL_Rect size",
					"double angle"
				],
				Description: [
					"Object representing text in the window"
				],
				Functions: [
					"None"
				]
			},
			WindowBox: {
				Variables: [
					"SDL_Rect size",
					"WindowDrawMode drawmode"
				],
				Description: [
					"Parent object for other window structures"
				],
				Functions: [
					"int getWidth()",
					"int getHeight()",
					"void moveUp(int Distance)",
					"void moveDown(int Distance)",
					"void moveLeft(int Distance)",
					"void moveRight(int Distance)",
					"void changePosition(int X, int Y)"
				]
			},
			WindowImage: {
				Variables: [
					"string address",
					"SDL_Texture* texture",
					"double angle"
				],
				Description: [
					"Child of WindowBox",
					"Object representing images in the window"
				],
				Functions: [
					"rotateLeft(int Degrees)",
					"rotateRight(int Degrees)"
				]
			},
			WindowTextBox: {
				Variables: [
					"string name",
					"WindowText text",
					"SDL_Color outline",
					"SDL_Color textcolor"
				],
				Description: [
					"Child of WindowBox",
					"Object representing textboxes in the window"
				],
				Functions: [
					"void clear()",
					"void changeText(string NewText)",
					"void appendText(string Text)"
				]
			},
			WindowButton: {
				Variables: [
					"Window* window",
					"string id",
					"WindowTextType textype",
					"WindowTextType textfit",
					"WindowText text",
					"SDL_Color color",
					"bool clicked"
				],
				Description: [
					"Child of WindowImage, WindowBox",
					"Object representing buttons in the window"
				],
				Functions: [
					"void changeText(string NewText)",
					"void changeButtonColor(SDL_Color NewColor)",
					"void changeSize(SDL_Rect NewSize)",
					"void changeTextColor(SDL_Color NewColor)",
					"void changeTexture(Window* w, SDL_Texture* NewTexture)",
					"void increaseTransparency(int Amount)",
					"void decreaseTransparency(int Amount)",
					"void clickButton()",
					"bool wasClicked()",
					"bool leftClicked(Window* w)",
					"bool rightClicked(Window* w)"
				]
			}
		},
		Functions: {
			NULLLOADFUNCTION: {
				Type: "void",
				Returns: [ "Nothing" ],
				Parameters: [
					"Window* w - Active window",
					"string s - Set to \"\"",
					"int n - Set to NULL"
				],
				Description: [
					"NULL function to use if no load function in desired"
				],
				Examples: [ "None" ]
			},
			sleep: {
				Type: "void",
				Returns: [ "Nothing" ],
				Parameters: [
					"int Delay - Time in milliseconds to delay thread"
				],
				Description: [
					"Delays the thread to X amount of milliseconds"
				],
				Examples: [ "None" ]
			},
			fileExists : {
				Type: "bool",
				Returns: [ "true", "false" ],
				Parameters: [
					"string Name - Name/Directory of the file"
				],
				Description: [
					"Determine if the given directory exists"
				],
				Examples: [ "None" ]
			},
			findInVector: {
				Type: "template&lt;typename T&gt; bool",
				Returns: [ "T Value" ],
				Parameters: [
					"vector&lt;T&gt; *Vector - Vector to search",
					"T Value - Value to find"
				],
				Description: [
					"Searches the given vector for a given value"
				],
				Examples: [ "None" ]
			},
			removeFromVector: {
				Type: "template&lt;typename T&gt; void",
				Returns: [ "Nothing" ],
				Parameters: [
					"vector&lt;T&gt; *Vector - Vector to search",
					"T Value - Value to remove"
				],
				Description: [
					"Searches the given vector and removes given value"
				],
				Examples: [ "None" ]
			}
		},
		Variables: {
			NATIVE_IMAGE: {
				Type: "SDL_Rect",
				Description: [ "Tell the window to draw the image at its native dimensions" ]
			},
			WINDOW_RECT: {
				Type: "SDL_Rect",
				Description: [ "Tells the window to draw the image across the whole window's dimensions" ]
			},
			NO_IMAGE: {
				Type: "string",
				Description: [ "Identifier for no image for an object" ]
			},
			NO_VIDEO: {
				Type: "string",
				Description: [ "Identifier for no video for an object" ]
			},
			NULL_RECT: {
				Type: "SDL_Rect",
				Description: [ "Representation of a NULL rect" ]
			},
			NULLTEXT: {
				Type: "WindowText",
				Description: [ "Representation of a NULL WindowText Object" ]
			},
			NULLTEXT: {
				Type: "WindowText",
				Description: [ "Representation of a NULL WindowTextObject" ]
			},
			NULLTEXTBOX: {
				Type: "WindowTextBox",
				Description: [ "Representation of a NULL WindowTextBox Object" ]
			},
			NULLBUTTON: {
				Type: "WindowButton",
				Description: [ "Representation of a NULL WindowButton Object" ]
			},
			NULLJOYSTICK: {
				Type: "WindowJoystick",
				Description: [ "Representation of a NULL WindowJoystick Object" ]
			},
			NULLAUDIO: {
				Type: "WindowAudio",
				Description: [ "Representation of a NULL WindowAudio Object" ]
			},
			NULLVIDEO: {
				Type: "WindowVideo",
				Description: [ "Representation of a NULL WindowWindow Object" ]
			},
			NULLIMAGE: {
				Type: "WindowImage",
				Description: [ "Representation of a NULL WindowImage Object" ]
			},
		}
	}
};

var game_api = {
	Name: "game_api",
	Constructor: {
		Parameters: [
			"string Title - Title of the game window",
			"int Width - Width of the game window",
			"int Height - Height of the game window",
			"SDL_Color BackgroundColor - Background color of the game window",
			"string DefaultFont - Default font of the game window",
			"int FPS - Maximum number of frames the game window will run at"
		],
		Description: [ "Generates a new game" ],
		Examples: [ "Game game = new Game(\"My game\", 1920, 1080, WindowColor.black, \"Fonts/Ariel.ttf\", 60)" ]
	},
	Functions: {
		break: { Type: "Game" },
		
		start: {
			Type: "bool",
			Returns: [ "True if the game window was launched", "False otherwise" ],
			Parameters: [ "int Buffer" ],
			Description: [ 
				"Starts the game window",
				"Loads the settings file if one exists",
				"Buffer is used if a settings file isn't provided or errors",
				"Buffer can be set to LaunchFullscreen, LaunchFullscreenWindowed, or NULL"
			],
			Examples: [ "game.start()" ],
			Variations: [ "None" ]
		},
		clearScreen: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ "None" ],
			Description: [ 
				"Clears the game window",
				"Extension of Window.h's window.clear()"
			],
			Examples: [ "game.clearScreen()" ],
			Variations: [ "None" ]
		},
		updateScreen: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ "None" ],
			Description: [ 
				"Updates the game window with everything that was drawn to it",
				"Extension of Window.h's window.update()"
			],
			Examples: [ "game.updateScreen()" ],
			Variations: [ "None" ]
		},
		holdFrame: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ "None" ],
			Description: [ 
				"Holds the game window until the frame catches up with the frame rate",
				"Extension of Window.h's window.load()"
			],
			Examples: [ "game.holdFrame()" ],
			Variations: [ "None" ]
		},
		updateGame: {
			Type: "bool",
			Returns: [ 
				"False if the window was closed",
				"True otherwise"
			],
			Parameters: [ "None" ],
			Description: [ 
				"Updates all the events in the game window",
				"Extension of Window.h's window.updateEvents()"
			],
			Examples: [ "game.updateGame()" ],
			Variations: [ "None" ]
		},
		setGameState: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ "int State - New state of the game" ],
			Description: [ 
				"Built in function to assist with game flow and organization",
			],
			Examples: [ "game.setGameState(2)" ],
			Variations: [ "None" ]
		},
		getGameState: {
			Type: "int",
			Returns: [ "The current gamestate value" ],
			Parameters: [ "None" ],
			Description: [ "Returns the current gamestate of the game" ],
			Examples: [ "game.getGameState()" ],
			Variations: [ "None" ]
		},
		addFont: {
			Type: "int",
			Returns: [ "WindowSuccess", "InvalidFont" ],
			Parameters: [ "string Font - Location of the font" ],
			Description: [ 
				"Add the font to the game window",
				"Extension of Window.h's window.addFont(...)"
			],
			Examples: [ "game.addFont(\"Font/Ariel.ttf\")" ],
			Variations: [ "None" ]
		},
		changeFont: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ "string Font - Location of the font" ],
			Description: [ 
				"Changes the font on the game window",
				"Extension of Window.h's window.setFont(...)"
			],
			Examples: [ "game.changeFont(\"Fonts/Ariel.ttf\")" ],
			Variations: [ "None" ]
		},
		adjustFontSize: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ "int Size - New size of the font" ],
			Description: [ 
				"Changes the font of the game window",
				"Extension of the Window.h's window.setFontSize(...)" 
			],
			Examples: [ "game.adjustFontSize(25)" ],
			Variations: [ "None" ]
		},
	
		break: { Type: "Display" },
		
		fullscreen: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ "None" ],
			Description: [ "Fullscreens the game window" ],
			Examples: [ "game.fullscreen()" ],
			Variations: [ "None" ]
		},
		windowedFullscreen: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ "None" ],
			Description: [ "Sets the game window to windowed fullscreen" ],
			Examples: [ "game.windowedFullscreen()" ],
			Variations: [ "None" ]
		},
		toggleFullscreen: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ "None" ],
			Description: [ "Toggles the fullscreen state of the window" ],
			Examples: [ "game.toggleFullscreen()" ],
			Variations: [ "None" ]
		},
		
		break: { Type: "FPS" },
		
		startFPSThread: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ "None" ],
			Description: [ "Starts the game window FPS tracking thread" ],
			Examples: [ "game.startFPSThread()" ],
			Variations: [ "None" ]
		},
		closeFPSThread: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ "None" ],
			Description: [ "Closes the game window FPS tracking thread" ],
			Examples: [ "game.closeFPSThread()" ],
			Variations: [ "None" ]
		},
		toggleFPSThread: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ "None" ],
			Description: [ "Toggles the game window FPS tracking thread" ],
			Examples: [ "game.toggleFPSThread()" ],
			Variations: [ "None" ]
		},
		drawFPS: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ 
				"int X - X Coordinate to draw the FPS",
				"int Y - Y Coordinate to draw the FPS"
			],
			Description: [ "Draws the amount of frames draw in the last second of the game window" ],
			Examples: [ "game.drawFPS(0, 0)" ],
			Variations: [ "None" ]
		},
		getFPSCap: {
			Type: "int",
			Returns: [ "The maximum frames the window can run at" ],
			Parameters: [ "" ],
			Description: [ "" ],
			Examples: [ "window.getFPSCap()" ],
			Variations: [ "" ]
		},

		break: { Type: "Images" },
		
		loadTextures: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ 
				"string ImageDirectory - Directory to load images from",
				"ImageLoadFunction Function - Optional function to run each time an image is loaded",
				"int* Counter - Placeholder for recursive searching, set to NULL or 0"
			],
			Description: [ "Loads all images within the given directory" ],
			Examples: [ "window.loadTextures(\"Images\", &MyLoadFunction, NULL)" ],
			Variations: [ "window.loadTextures(vector<string> Directories, ImageLoadFunction Function)" ]
		},
		countImagesInDirectory: {
			Type: "int",
			Returns: [ "The number of images in the Directory" ],
			Parameters: [ "string Directory - Directory to count the images from" ],
			Description: [ 
				"Returns the number of images in the given directory",
				"Extension of Window.h's window.countImagesInDirectory(...)"
			],
			Examples: [ "int numimgs = game.countImagesInDirectory(\"Images\")" ],
			Variations: [ "window.countImagesInDirectory(vector<string> Directories)" ]
		},
		getImageCount: {
			Type: "int",
			Returns: [ "The number of images counted from countImagesInDirectory()" ],
			Parameters: [ "None" ],
			Description: [ "Returns the number of images counted" ],
			Examples: [ "int numimg = window.getImageCount()" ],
			Variations: [ "None" ]
		},
		
		break: { Type: "Input" },
		
		addKeyboard: {
			Type: "GameKeyboard*",
			Returns: [ "Nothing" ],
			Parameters: [ 
				"int Player - Player to connect keyboard to",
				"string Up - Up key",
				"string Down - Down key",
				"string Left - Left key",
				"string Right - Right key",
				"string A - A key",
				"string B - B key",
				"string X - X key",
				"string Y - Y key"
			],
			Description: [ 
				"Adds a keyboard to be used in the game",
				"Each time a keyboard is added it coresponds with its respectively added player unless specified",
				"I.e The first keyboard added is controlled by the first player, the second with second player, etc",
				"At least one keyboard is needed first to run any menus or scenes"
			],
			Examples: [ "game.addKeyboard(\"W\", \"S\", \"A\", \"D\", \"H\", \"J\", \"K\", \"L\")" ],
			Variations: [ "game.addKeyboard(string Up, string Down, string Left, string Right, string A, string B, string X, string Y)" ]
		},
		getKeyboard: {
			Type: "GameKeyboard*",
			Returns: [ "The player's represetative GameKeyboard" ],
			Parameters: [ "int Player - Desired player's keyboard to get" ],
			Description: [ "Returns the players keyboard" ],
			Examples: [ "Gamekeyboard* keyboard = game.getKeyboard(0)" ],
			Variations: [ "" ]
		},
		
		break: { Type: "Events" },
		
		addEvent: {
			Type: "GameEvent*",
			Returns: [ 
				"The created CameEvent object",
				"NULLEVENT if the event already exists"
			],
			Parameters: [ 
				"string EventID - ID of the event",
				"string Parent - Menu parent of the event",
				"string Background - Default background of the event",
				"bool IsVideo - Set to true if desired background is a video [Will be depreciated later]",
				"SceneFunction Function - Custom function for the event"
			],
			Description: [ "" ],
			Examples: [ "GameEvent* e = game.addEvent(\"StoryMode1\", \"MainMenu\", \"Image/GameBackground.png\", false, &STORYFUNCION1)" ],
			Variations: [ "None" ]
		},
		getEvent: {
			Type: "WindowEvent*",
			Returns: [ "The representative WindowEvent from its ID" ],
			Parameters: [ "string EventID - ID of the event" ],
			Description: [ "" ],
			Examples: [
				"WindowEvent* event = game.getEvent(\"StoryMode1\")",
				"unordered_map<string, WindowEvent>* events = window.getAllEvents()"
			],
			Variations: [ "" ]
		},
		
		break: { Type: "Scenes" },
		
		addScene: {
			Type: "GameScene*",
			Returns: [ 
				"The created GameScene object",
				"NULLSCENE if the scene already exists"
			],
			Parameters: [ 
				"string SceneID - ID of the scene",
				"string Background - Default background of the scene",
				"int Duration - Duration of the scene. Positive integers or INDEFINITE_DURATION (-1)",
				"SceneFunction Function - Custom function for the scene"
			],
			Description: [ "" ],
			Examples: [ "GameScene* scene = game.addScene(\"Scene1\", \"Image/GameBackground.png\", INDEFINITE_DURATION, &SCENEFUNCION1)" ],
			Variations: [ "None" ]
		},
		getScene: {
			Type: "WindowScene*",
			Returns: [ "The representative WindowScene from its ID" ],
			Parameters: [ "string SceneID - ID of the scene" ],
			Description: [ "" ],
			Examples: [
				"WindowScene* scene = game.getScene(\"Scene1\")",
				"unordered_map<string, WindowScene>* scenes = game.getAllScenes()"
			],
			Variations: [ "game.getCurrentScene()" ]
		},
		
		break: { Type: "Menus" },
		
		addMenu: {
			Type: "GameMenu*",
			Returns: [ 
				"The created GameMenu object",
				"NULLMENU if the menu already exists"
			],
			Parameters: [ 
				"string MenuID - ID of the menu",
				"string Parent - Parent of the menu. Use NO_MENU_PARENT for the head menu",
				"vector<string> OptionText - Respective text for each menu button",
				"vector<string> OptionID - ID pointing to each respective menu",
				"string Background - Background of the menu",
				"bool IsVideo - Set to IS_VIDEO or NOT_VIDEO. Ensures the engine knows the background is a video. Will be depreciated.",
				"MenuFunction Function - Custom function for the menu"
			],
			Description: [ "" ],
			Examples: [ "GameMenu* menu = game.addMenu(\"MainMenu\", NO_MENU_PARENT, { \"Single Player\", \"Multiplayer\" }, { \"\\\\%s\" }, \"Image/GameBackground.png\", NO_VIDEO, &MAINMENU1)" ],
			Variations: [ "" ]
		},
		getMenu: {
			Type: "WindowMenu*",
			Returns: [ "The representative WindowMenu from its ID" ],
			Parameters: [ "string MenuID - ID of the menu" ],
			Description: [ "" ],
			Examples: [
				"WindowMenu* menu = game.getMenu(\"MainMenu\")",
				"unordered_map<string, WindowMenu>* menus = game.getAllMenus()"
			],
			Variations: [ "game.addExitGameMenu(string Parent)" ]
		},
		
		break: { Type: "Resets" },
		
		CLEAR_ALL_GAME_DATA: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ "None" ],
			Description: [ "Clears all game data" ],
			Examples: [ "game.CLEAR_ALL_GAME_DATA()" ],
			Variations: [ "None" ]
		},
		clearScene: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ "None" ],
			Description: [ "Clears all data for the game's scenes" ],
			Examples: [ "game.clearScenes()" ],
			Variations: [ "None" ]
		},
		clearMenus: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ "None" ],
			Description: [ "Clears all data for the game's menus" ],
			Examples: [ "game.clearMenus()" ],
			Variations: [ "None" ]
		},
		clearEvents: {
			Type: "void",
			Returns: [ "Nothing" ],
			Parameters: [ "None" ],
			Description: [ "Clears all data for the game's events" ],
			Examples: [ "game.clearEvents()" ],
			Variations: [ "None" ]
		}
	},
	Objects: {
		Enums: {
			GameStatus: [
				"GameError",
				"GameSuccess",
				"InvalidScene",
				"InvalidSceneVariable",
				"NullCurrentScene",
				"InvalidPlayer",
				"MenuExists",
				"VariableExists",
				"ObjectExists",
				"SceneExists",
				"PlayerExists",
				"NoMatchingKeyboard",
				"InvalidMenu"
			],
			GameWindow: [
				"LaunchNormal",
				"LaunchFullscreen",
				"LaunchWindowedFullscreen"
			],
			GameObjectType: [
				"GametypeNone",
				"GametypeSprite",
				"GametypeSpriteA", 
				"GametypeImage"
			],
			GameStates: [
				"GameStateScene",
				"GameStateMenu",
				"GameStateEvent"
			],
			GameBackgroundScrollType: [
				"FollowPlayerMiddle",
				"FollowPlayerMiddleExceptRightEnd",
				"FollowPlayerMiddleExceptLeftEnd",
				"FollowPlayerMiddleExceptEnds",	
				"FollowPlayerBound",
				"FollowPlayerBoundExceptEnds",
				"FollowPlayersLeftHalfAlways",
				"FollowPlayersLeftHalfExceptEnds",
				"FollowPlayersWhole",
				"FollowPlayersStrech",
				"FollowPlayersBound"
			],
			GameFunctionState: [
				"GameFunctionInit",
				"GameFunctionTick",
				"GameFunctionDone"
			]
		},
		Structs: {
			GameProperties: {
				Variables: [ 
					"double aspectratio",
					"int width",
					"int height",
					"int fps",
					"string currentfont",
					"bool fullscreened",
					"fpsthreadrunning"
				],
				Description: [ "Not needed for development and can be disregarded" ],
				Functions: [
					"None"
				]
			},
			GameRect: {
				Variables: [
					"double x",
					"double y",
					"double w",
					"double h"
				],
				Description: [
					"Not needed for development and can be disregarded"
				],
				Functions: [
					"None"
				]
			},
			GameObject: {
				Variables: [
					"GameObjectType type",
					"string id",
					"string image",
					"string animation",
					"string direction",
					"SDL_Rect size",
					"SDL_Rect lastsize",
					"GameRect size2",
					"WindowDrawMode drawmode",
					"bool coolidable",
					"bool onplatform",
					"bool collidableside[4]",
					"bool framemoving",
					"bool animationdone",
					"bool updateframesize",
					"bool canholdanimation",
					"bool holdanimationframe",
					"bool enableimgflip",
					"int frame",
					"int framespeed",
					"int frametimer",
					"int lastframewidth",
					"Uint8 alpha",
					"double angle",
					"double limitdrawx",
					"double scale"
				],
				Description: [
					"Container for an object in a scene or event"
				],
				Functions: [
					"void disablefliph()",
					"void changeImage(string Image)",
					"void chagneSize(SDL_Rect Size)",
					"void changeFrame(int Frame)",
					"void changeTransparency(int Transparency)",
					"void changeAngle(double Angle)",
					"void moveUp(double Distance)",
					"void moveDown(double Distance)",
					"void moveLeft(double Distance)",
					"void moveRight(double Distance)"
				]
			},
			GameVariable: {
				Variables: [
					"string id",
					"any value"
				],
				Description: [ "Not needed for development and can be disregarded" ],
				Functions: [ "None" ]
			},
			GameBackground: {
				Variables: [
					"Game* game",
					"WindowVideo* video",
					"GameBackgroundScrollType scrolltype",
					"string image",
					"bool scroll",
					"bool repeat",
					"bool stretch",
					"double offset",
					"double scalevalue",
					"double alpha",
					"int boundx[2]",
					"int boundy[2]"
				],
				Description: [
					"Background object for menu, sceens, and events"
				],
				Functions: [
					"void draw()",
					"void setImage(string Image)",
					"void enableScroll()",
					"void disableScroll()",
					"void enableScale(double Scale)",
					"void enableScaleFull()",
					"void disableScale()",
					"void enableStretch()",
					"void disableStretch()",
					"void enableRepeat()",
					"void disableRepeat()",
					"void setScrollType(GameBackgroundScrollType type)",
					"void setBoundLimits(int Min, int Max)",
					"void setBoundLimits(int Width)",
					"void setTransparency(int Transparency)",
					"double getOffset()"
				]
			},
			GameKeyboard: {
				Variables: [ 
					"string up",					
					"string down",					
					"string left",					
					"string right",					
					"string a",					
					"string b",					
					"string x",					
					"string y",					
					"string start",					
					"string select"				
				],
				Description: [ "General input for game control" ],
				Functions: [ "None" ]
			},
			GameBoundary: {
				Variables: [ 
					"int xmin", 
					"int xmax", 
					"int ymin", 
					"int ymax" 
				],
				Description: [ "Not needed for development and can be disregarded" ],
				Functions: [ "None" ]
			},
			GamePlayer: {
				Variables: [ 
					"Game game",
					"GameKeyboard keyboard",
					"GameBoundary boundary",
					"string jumpkey",
					"int pid",
					"double x",
					"double y",
					"double speed",
					"double velx",
					"double vely",
					"double friction",
					"double gravity",
					"double jumpstrength",
					"double ground",
					"bool hasgravity",
					"bool canmovex",
					"bool jumping",
					"bool falling",
					"bool fullanimationplaying",
					"bool independentmovement",
					"vector<string> holdanimationkeys",
					"vector<string> fullanimationkeys",
					"vector<string> inputhistory",
					"map<string, string> playeranimation"
				],
				Description: [ "Object representing the user in an event" ],
				Functions: [ 
					"void addinputkey(string Key)",
					"string getKey(string Key)",
					"checkFullAnimation(string Key)",
					"void changeAnimation(string Animation)",
					"void setFullAnimationKeys(vector<string> Keys)",
					"void setHoldAnimationKeys(vector<string> Keys)",
					"void setJumpKey(string Key, double JumpStrength)",
					"int mapInputToAnimation(vector<string> Animations)"
				]
			},
			GameContainer: {
				Variables: [ 
					"Game* game",
					"Window* window",
					"GameBackground* background",
					"string id",
					"bool playing",
					"unordered_map<string, GameVariable> variables",
					"unordered_map<string, GameObject> objects",
					"unordered_map<string, int> framespeeds"
				],
				Description: [ "" ],
				Functions: [ 
					"template<typename T> addVariable(string Variable, T Value)",
					"template<typename T>* getVariableRaw(string Variable)",
					"template<typename T> getVariable(string Variable)",
					"template<typename T> editVariable(string Variable, T Value)",
					"void removeVariable(string Variable)",
					"GameObject* addObject(string ObjectID, GameObjectType ObjectType, string Image, string Animation, SDL_Rect Size, int FrameSpeed, WindowDrawMode Drawmode, bool Collidable)",
					"GameObject* getObject(string ObjectID)",
					"void removeObjects(string ObjectID)",
					"void drawObjects()",
					"void start()",
					"void stop()",
					"void remove()",
					"template<typename T> void drawAnimatedObject(T* o, bool NoInput)",
					"void drawObject(WindowImage* Image, GameObject* o)",
					"void mapFrameSpeed(string Folder, int Speed)",
					"void mapAnimationToFrameSpeed(string Folder, vector<string> Images, vector<int> Speeds)"
				]
			},
			GameScene: {
				Variables: [
					"SceneFunction func",
					"int duration",
					"bool playing"
				],
				Description: [ "Child of GameContainer" ],
				Functions: [ 
					"int duration",
					"bool playing" 
				]
			},
			GameMenu: {
				Variables: [
					"MenuFunction func",
					"string parent",
					"bool isevent",
					"bool ignorebuttons",
					"bool buttonswitched",
					"vector<string> options",
					"vecotr<string> ",
				],
				Description: [ "Child of GameContainer" ],
				Functions: [ 
					"void ignoreButtons()",
					"void enableButtons()",
					"void drawButtons()",
					"void nextButtons()",
					"void buttonSwitched()",
					"void autoAddButtons(int StartX, int StartY, int ButtonWidth, int ButtonHeight, int Buffer, int FontSize, string Texture, WindowTextType Fill, WindowTextType Fit)"
				]
			},
			GameEvent: {
				Variables: [
					"EventFunction func",
					"unordered_map<string, GamePlayer> players",
					"unordered_map<string, GamePlayer> ai",
					"GamePlayer* minplayer",
					"GamePlayer* maxplayer",
					"unordered_map<string, GamePlayer>* getPlayers()",
					"GamePlayer* addPlayer(string Folder, double Scale, double Speed, double Friction, double Gravity, bool HasGravity, bool CanMoveX, bool IndependentMovement, GameBoundary Boundaries, GameObjectType Type, string Animation, WindowDrawMode Drawmode)",
					"GamePlayer* addPlayer(string Folder, SDL_Rect Size, double Scale, double Speed, double Friction, double Gravity, bool HasGravity, bool CanMoveX, bool IndependentMovement, GameBoundary Boundaries, GameObjectType Type, string Animation, WindowDrawMode Drawmode)",
					"GamePlayer* getPlayer(int PlayerID)",
					"bool playerInputHandle(GamePlayer* Player, string Key)",
					"void autoHandle()"
				],
				Description: [ "Child of GameContainer" ],
				Functions: [ "None" ]
			}
		},
		Functions: {
			collided: {
				Type: "collided",
				Returns: [ "True if the given rects overlap" ],
				Parameters: [
					"SDL_Rect Rect1 - First rect",
					"SDL_Rect Rect2 - Second rect"
				],
				Description: [
					"Checks if the given rects are overlapping"
				],
				Examples: [ "game.collided({ 10, 30, 50, 50 }, { 0, 0, 15, 100 })" ]
			}
		},
		Variables: {
			NO_ANIMATION: {
				Type: "string",
				Description: [ "Can be used for object - image" ]
			},
			NO_INPUT: {
				Type: "string",
				Description: [ "Used for player's default animation - Not really needed" ]
			},
			NO_MENU_PARENT: {
				Type: "string",
				Description: [ "Tells the game the menu has no parent exit" ]
			},
			QUITGAME_MENU_ID: {
				Type: "string",
				Description: [ "Not needed for development" ]
			},
			MENU_IDMATCH: {
				Type: "string",
				Description: [ "Tells the game to match the menu's id to it's text" ]
			},
			VideoTypes: {
				Type: "vector<string>",
				Description: [ "Not needed for development" ]
			},
			MENU_MATCH_OPTIONS: {
				Type: "vector<string>",
				Description: [ "Tells the game to match all menu id's and text" ]
			},
			INDEFINITE_DURATION: {
				Type: "int",
				Description: [ "Tells the game to play the scene indefinitely" ]
			},
			TRANSPARENCY_NONE: {
				Type: "int",
				Description: [ "More readable variable for an invisible object", "0" ]
			},
			TRANSPARENCY_FULL: {
				Type: "int",
				Description: [ "More readable variable for an fully visible object", "255" ]
			},
			CAN_MOVE_X: {
				Type: "bool",
				Description: [ "Tells the game to automate movement on the X axis",  ]
			},
			CANT_MOVE_X: {
				Type: "bool",
				Description: [ "" ]
			},
			HAS_GRAVITY: {
				Type: "bool",
				Description: [ "" ]
			},
			NO_GRAVITY: {
				Type: "bool",
				Description: [ "" ]
			},
			INDEPENDENT_MOVEMENT: {
				Type: "bool",
				Description: [ "" ]
			},
			DEPENDENT_MOVEMENT: {
				Type: "bool",
				Description: [ "" ]
			},
			SCALE_HALF: {
				Type: "bool",
				Description: [ "" ]
			},
		}
	}
}
