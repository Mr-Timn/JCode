/*
	"Name": Name
	"Returns": [ "returnvalue", ... ]
	"Parameters": [ "[TYPE] NAME - DESCRIPTION", ... ],
	"Description": [ "DESCRIPTION", ... ],
	"Examples": [ "EAMPLE1", ... ],
	"Todo": [ "TODO1", ... ]
*/

var window_api = {
	"Constructor": {
		"Parameters": [
			"string Title - Title of the Window",
			"int Width - Width of the Window",
			"int Height - Height of the Window",
			"int Flags - Flags to init window with. Ex. SDL_WINDOW_FULLSCREEN, SDL_WINDOW_SHOWN",
			"SDL_Color BackgroundColor - Default background color of the Window each time it's rendered",
			"int FPS - Maximum number of frames the Window can run at if yieldUntilNextFrame() is used"
		],
		"Description": [
			"Generates a new Window with the given default values",
			"Does not actually launch the window.start() must be called after a window instance is created"
		],
		"Examples": [
			"Window window1 = new Window(\"My Window 1\", 1920, 1080, SDL_WINDOW_FULLSCREEN, { 143, 210, 180 }, 60);",
			"Window window2 = new Window(\"My Window 2\", 1080, 720, SDL_WINDOW_SHOWN, WindowColor.white, 30);"
		]
	},
	"Functions": {
		"break1": { "Type": "Window" },

		"start": {
			"Type": "bool",
			"Returns": [ "True if Window launched correctly", "False if otherwise" ],
			"Parameters": [ "None" ],
			"Description": [ "Starts the Window" ],
			"Examples": [ "window1.start()" ],
			"Todo": [ "None" ]
		},
		"close": {
			"Type": "void",
			"Returns": [ "Nothing" ],
			"Parameters": [ "None" ],
			"Description": [ "Closes the Window" ],
			"Examples": [ "window1.close()" ],
			"Todo": [ "None" ]
		},
		"fullscreen": {
			"Type": "void",
			"Returns": [ "Nothing" ],
			"Parameters": [ "int Type - Level of fullscreen" ],
			"Description": [
				"Sets the desired window Fullscreen mode",
				"Use NULL to return to windowed mode",
				"To launch window in fullscreen, set the flag to SDL_WINDOW_FULLSCREEN or SDL_WINDOW_FULLSCREEN_DESKTOP"
			],
			"Examples": [
				"window1.fullscreen(SDL_WINDOW_FULLSCREEN_DESKTOP))",
				"window1.fullscreen(SDL_WINDOW_FULLSCREEN)",
				"window1.fullscreen(NULL)"
			],
			"Todo": [ "None" ]
		},
		"clear": {
			"Type": "void",
			"Returns": [ "Nothing" ],
			"Parameters": [ "None" ],
			"Description": [ "Clears the window's screen" ],
			"Examples": [ "window1.clear()" ],
			"Todo": [ "None" ]
		},
		"load": {
			"Type": "void",
			"Returns": [ "Nothing" ],
			"Parameters": [ "None" ],
			"Description": [ "Presents everything draw to the window, moving the frame forward" ],
			"Examples": [ "window1.load()" ],
			"Todo": [ "None" ]
		},
		"updateEvents": {
			"Type": "bool",
			"Returns": [ "False if window was closed", "True otherwise" ],
			"Parameters": [ "None" ],
			"Description": [
				"Updates all events that happened in the window",
				"This includes input from the mouse, keyboard, and any connected controllers"
			],
			"Examples": [ "window1.updateEvents()" ],
			"Todo": [ "None" ]
		},

		"break2": { "Type": "FPS Functions" },

		"trackFPS": {
			"Type": "void",
			"Returns": [ "Nothing" ],
			"Parameters": [ "None" ],
			"Description": [
				"Starts fps tracking thread",
				"Should only be called once"
			],
			"Examples": [ "window1.trackFPS()" ],
			"Todo": [ "None" ]
		},
		"killFPSThread": {
			"Type": "void",
			"Returns": [ "Nothing" ],
			"Parameters": [ "None" ],
			"Description": [ "Stops FPS thread if it is currently running" ],
			"Examples": [ "window1.killFPSThread()" ],
			"Todo": [ "None" ]
		},
		"getFPS": {
			"Type": "int",
			"Returns": [ "Current FPS count for that second" ],
			"Parameters": [ "None" ],
			"Description": [ "Used to get the amount of frames that were displayed within a given second" ],
			"Examples": [ "int fps = window1.getFPS()" ],
			"Todo": [ "None" ]
		},
		"yieldUntilNextFrame": {
			"Type": "void",
			"Returns": [ "Nothing" ],
			"Parameters": [ "None" ],
			"Description": [
				"Pauses current thread until the frame limiter catches up to the threads time",
				"Shold be used at the end of "
			],
			"Examples": [ "window1.yieldUntilNextFrame()" ],
			"Todo": [ "None" ]
		}


	}
}
