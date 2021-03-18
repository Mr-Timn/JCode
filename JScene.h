//
// Made by Jordan Hoosman
//

#define __JSCENE__

#include <string>						// ...
#include <vector>						// ...
#include <map>							// ...
#include <unordered_map>				// ...
#include <thread>						// ...
#include <algorithm>					// std::transform std::find
#include <fstream>						// ...
#if defined(__ANDROID__)
	#include <dirent.h>
	#include <sys/stat.h>				// stat mkdir

    #include <SDL.h>
    #include <SDL_image.h>
    #include <SDL_ttf.h>
    #include <SDL_syswm.h>
    #include <SDL_mixer.h>
#elif defined(_WIN32)
	#include <windows.h>
	#include <filesystem>
	
	#include <SDL.h>
	#include <SDL_image.h>
	#include <SDL_ttf.h>
	#include <SDL_syswm.h>
	#include <SDL_mixer.h>

	extern "C" {
		#include <libavcodec/avcodec.h>
		#include <libavformat/avformat.h>
	}
	
	namespace fs = std::filesystem;
#elif defined(__linux__)
	#include <sys/stat.h>				// stat mkdir
	#include <experimental/filesystem>	// ...

	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
	#include <SDL2/SDL_ttf.h>
	#include <SDL2/SDL_syswm.h>
	#include <SDL2/SDL_mixer.h>

	extern "C" {
		#include </usr/include/x86_64-linux-gnu/libavcodec/avcodec.h>
		#include </usr/include/x86_64-linux-gnu/libavformat/avformat.h>
	}

	namespace fs = std::experimental::filesystem;
#endif

#define HSL_HMAX 360
#define HSL_SMAX 1.0
#define HSL_LMAX 1.0

#define DEGTORAD 3.14159 / 180.0

namespace JScene {
	/***********************************************************************************\
	|*   Variables
	\***********************************************************************************/
	class Window;
	class Joystick;
	struct JColors_HSL;

	const std::string NO_IMAGE = "@$%$%~NO_IMAGE&&*!@%&$";
	const std::string NO_VIDEO = "&*#@#@NO_VIDEO)()@#!@#";
	const SDL_Rect NULLRECT = { -1, -1, -1, -1 };
	const SDL_Rect NATIVE_IMAGE = { 0, 0, NULL, NULL };

	inline SDL_Rect WINDOW_RECT;

	typedef void(*ImageLoadFunction)(Window* Win, std::string Str, int N);
	typedef void(*JoystickAddedFunction)(Joystick* Joystick);

	/***********************************************************************************\
	|*   Functions
	\***********************************************************************************/
	void NULLLOADFUNCTION(Window* w, std::string s, int n);
	inline void sleep(int Delay);

	bool fileExists(std::string File);
	bool directoryExists(std::string Directory);

	bool createFile(std::string File);
	bool createDirectory(std::string Directory);
	
	void appendToFile(std::string File, std::string Text);

	template<typename T> bool findInVector(std::vector<T>* Vector, T Value);
	template<typename T> void removeFromVector(std::vector<T>* Vector, T Value);

	void scaleRect(SDL_Rect& Rect, int Pixels);

	int xwidth(SDL_Rect& Rect);
	int yheight(SDL_Rect& Rect);
	int ymidheight(SDL_Rect& Rect);

	int xwidth(const SDL_Rect& Rect);
	int yheight(const SDL_Rect& Rect);
	int ymidheight(const SDL_Rect& Rect);

	SDL_Color numberToRGB(uint32_t Number);
	SDL_Color invertRGB(SDL_Color Color);
	SDL_Color HSVtoRGB(double H, double S, double V, uint8_t A);
	SDL_Color HSLtoRGB(int H, double S, double L, uint8_t A); // { [0-360], [0-1], [0-1] }

	JColors_HSL RGBtoHSL(SDL_Color ColorRGB);

	/***********************************************************************************\
	|*   Enums    
	\***********************************************************************************/
	enum Error {
		WindowSuccess,
		SDLError,

		InvalidFont,
		NoFontsLoaded,
		FontTooSmall,
		InvalidFontSize,

		InvalidObject,
		InvalidGeometry,
		InvalidImage,
		InvalidButton,
		InvalidTextBox,
		InvalidJoystick,
		InvalidAudio,
		InvalidVideo,

		FontExists,
		TextBoxExists,
		ImageExists,
		ButtonExists,
		AudioExists,
		VideoExists,

		VideoInitFailed
	};
	enum InitError {
		IESUCCESS,
		IEINIT,
		IEWINDOW,
		IEIMG,
		IEAUDIO,
		IERENDER,
		IETTF
	};
	enum JoystickButton {
		LUp,
		LDown,
		LLeft,
		LRight,
		LTrigger,
		LShoulder,
		LStick,

		RUp,
		RDown,
		RLeft,
		Right,
		RTrigger,
		RShoulder,
		RStick,

		DUp,
		DDown,
		DLeft,
		DRight,

		A, B, X, Y,
		Start,
		Back,
		Guide
	};
	enum class KeyState {
		Down,
		Up,
		Clicked,
		Null
	};
	enum class TextType {
		Default,
		Wrap,
		Fill,
		FillX,
		FillY,
		FillNone,
		
		ScrollX,

		Fit,
		FitX,
		FitY,
		FitNone
	};
	enum class Geometry {
		Fill,
		Outline,
		Line,
		Point
	};
	enum class DrawMode {
		Default,
		Center,
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight,
		MiddleX,
		MiddleY
	};
	
	/***********************************************************************************\
	|*   Structs  
	\***********************************************************************************/
	struct BasicColors {
		SDL_Color black =	{ 000, 000, 000, 255 };
		SDL_Color white =	{ 255, 255, 255, 255 };
		SDL_Color red =		{ 255, 000, 000, 255 };
		SDL_Color orange =	{ 255, 125, 000, 255 };
		SDL_Color yellow =	{ 255, 255, 000, 255 };
		SDL_Color green =	{ 000, 255, 000, 255 };
		SDL_Color aqua =	{ 000, 255, 255, 255 };
		SDL_Color blue =	{ 000, 000, 255, 255 };
		SDL_Color purple =	{ 190, 000, 255, 255 };
		SDL_Color pink =	{ 255, 000, 255, 255 };
	};
	inline BasicColors PreloadedColors;
	
	// [0-360], [0-1], [0-1]
	struct JColors_HSL {
		int h = 0;
		double s = 0.0;
		double l = 0.0;
	};

	struct Text {
		std::string id = "";
		std::string text = "";
		std::string font = "";

		unsigned int fontsize = 0;
		double angle = 0.0;

		SDL_Color fontcolor = { 0, 0, 0, 255 };
		SDL_Rect size = { 0, 0, 0, 0 };
		
		SDL_Texture* texture = NULL;
	};
	struct Box {
		Window* window = NULL;
		SDL_Rect size = { 0, 0, 0, 0 };
		DrawMode drawmode = DrawMode::Default;
		bool clicked = false;

		int width();
		int height();
		void moveup(int Pixels);
		void movedown(int Pixels);
		void moveleft(int Pixels);
		void moveright(int Pixels);
		void setposition(int X, int Y);

		void click();
		void unclick();
		bool wasclicked();
		bool leftclicked();
		bool rightclicked();

		bool leftpressed();
		bool middlepressed();
		bool rightpressed();
		
		bool fingertapped();
		bool fingerpressed();
		bool fingerinteracted();
	};
	struct Image : public Box {
		std::string address = "";
		SDL_Surface* surface = NULL;
		SDL_Texture* texture = NULL;
		double angle = 0.0;

		void rotateleft(int Degrees);
		void rotateright(int Degrees);
	};
	struct TextBox : public Box {
		public:
			std::string name = "";
			Text text;
			SDL_Color outline = { 0, 0, 0, 255 };
			SDL_Color textcolor = { 0, 0, 0, 255 };

			void draw();
			void clear();
			void changetext(std::string NewText);
			void appendtext(std::string Text);
			void select();
			bool isselected();
		private:
			bool selected;
	};
	struct Button: public Image {
		std::string id = "";

		TextType texttype = TextType::Default;
		TextType textfit = TextType::Default;
		Text text;
		SDL_Color color = { 0, 0, 0, 255 };
		
		void changeFontSize(int Size);
		void changeText(std::string NewText);
		void changeButtonColor(SDL_Color NewColor);
		void changeSize(SDL_Rect NewSize);
		void changeTextColor(SDL_Color NewColor);
		void changeTexture(Window* w, SDL_Texture* NewTexture);
		void increaseTransparency(int Amount);
		void decreaseTransparency(int Amount);

		void draw();
		void clean();
	};

	struct WProperties {
		std::string title = "";

		int width = 0;
		int height = 0;
		int devicewidth = 0;
		int deviceheight = 0;
		int id = 0;
		int flags = 0;
		int mobileflag = 0;
		
		int fps = 0;
		int fpsframe = 0;
		int fpslimit = 0;
		
		int windowfps = 0;
		int windowfpsframe = 0;

		SDL_Color backgroundcolor = { 0, 0, 0, 255 };
		DrawMode drawmode = DrawMode::Default;
	};
	struct WKeyboard {
		Uint8* state = NULL;
		std::vector<int> pressedkeys;
		std::vector<int> clickedkeys;
	};
	struct WMouse {
		int x, y, scroll;
		bool down[4] = { false };
		bool click[4] = { false };
	};

	/***********************************************************************************\
	|*   Classes  
	\***********************************************************************************/
	// TODO
	// 
	// - Replace and compare joystick mapping with uint32_t
	//
	class Joystick {
		public:
			Window* window = NULL;
			SDL_GameController* controller = NULL;
			SDL_Joystick* joystick = NULL;
			SDL_JoystickID joystickid = -1;
			
			std::map<std::string, JoystickButton> jmap;

			bool centeredl = true;
			bool centeredr = true;
			bool inputdelayedl = false;
			bool inputdelayedr = false;

			int delayduration = 0;
			int sensitivity = 2700;
			int lastleftx = 0;
			int lastlefty = 0;
			int lastrightx = 0;
			int lastrighty = 0;
			double leftaxisangle = 0.0;
			double rightaxisangle = 0.0;
			int leftaxis[2] = { 0 };
			int rightaxis[2] = { 0 };

			KeyState lefttrigger = { KeyState::Up };
			KeyState righttrigger = { KeyState::Up };
			KeyState abutton = { KeyState::Up };
			KeyState bbutton = { KeyState::Up };
			KeyState xbutton = { KeyState::Up };
			KeyState ybutton = { KeyState::Up };
			KeyState dbuttonup = { KeyState::Up };
			KeyState dbuttondown = { KeyState::Up };
			KeyState dbuttonleft = { KeyState::Up };
			KeyState dbuttonright = { KeyState::Up };
			KeyState leftshoulder = { KeyState::Up };
			KeyState rightshoulder = { KeyState::Up };
			KeyState leftstick = { KeyState::Up };
			KeyState rightstick = { KeyState::Up };
			KeyState start = { KeyState::Up };
			KeyState back = { KeyState::Up };
			KeyState guide = { KeyState::Up };

			KeyState getButtonState(int Button);
			bool checkInput(std::string Key, KeyState Expected);
			// l-udlr, ltrigger, lshoulder, lstick, **r**,  d-udlr, a, b, x, y, start, back, guide
			void setJoystickMap(std::vector<std::string> Keys);
			void setJoystickMapDefaults(std::vector<std::string> Keys);
			// 0 for left - 1 for right
			void delayStickInput(int Stick);
			void setInputDelay(int Duration);
			void setSensitivity(int Sensitivity);
		private: 
			void delaystickthread(int Stick);
	};
	class Audio {
		public:
			std::string id;
			Mix_Music* audio = NULL;
			bool playing = false;

			bool isPlaying();
			void play();
			void play(int Loops);
			void seek(double Time);
			void volume(int Volume);
	};
	#ifndef __ANDROID__
	class Video {
		public:
			std::string name = "";
			std::string address = "";
			bool playing = false;
			bool paused = false;
			bool autoupdate = false;
			bool replay = false;
			int videoindex = -1;
			int ret = 0;
			int fps = 0;
			int loops = 0;
			
			std::chrono::time_point<std::chrono::system_clock> vTimeLast = std::chrono::system_clock::now();
			std::chrono::time_point<std::chrono::system_clock> vTimeNow = std::chrono::system_clock::now();
			std::chrono::duration<double> vDelta = vTimeNow - vTimeLast;
			
			SDL_Rect framesize = { 0, 0, 0, 0 };
			SDL_Rect drawsize = { 0, 0, 0, 0 };
			SDL_Rect zoom = NULLRECT;
			SDL_Texture* texture = NULL;
			
			Window* window = NULL;
			SDL_Renderer* vrender = NULL;
			AVFormatContext* fmt_ctx = NULL;
			AVCodecContext* codec_ctx = NULL;
			FILE* fin = NULL;
			FILE* fout = NULL;
			AVFrame* frame = NULL;
			AVPacket* pkt = NULL;

			int play();
			void close();
			void pause();
			void unpause();
			void drawFrame();
			void drawFrame(SDL_Rect* Zoom);
			void changeZoom(SDL_Rect Zoom);
		private:
			void decode(bool close);
			int videoThread();
			void remove();
	};
	#endif
	class Window {
		public:
		// Variables
			SDL_Window* SDLWindow = NULL;
			SDL_Renderer* SDLRenderer = NULL;
			SDL_Surface* SDLSurface = NULL;
			SDL_Event SDLEvent;

			BasicColors WColor;
			WProperties Properties;
			WMouse Mouse;
			WKeyboard Keyboard;
			Text Font;
			
			Text NULLTEXT;
			Text NULLTEXTTEXTURE;
			TextBox NULLTEXTBOX;
			Button NULLBUTTON;
			Joystick NULLJOYSTICK;
			Audio NULLAUDIO;
			#ifndef __ANDROID__
			Video NULLVIDEO;
			#endif
			Image NULLIMAGE;
			
			bool ASyncReady = false;

		// Functions
			Window(std::string Title, int Flags, int FPS);
			Window(std::string Title, int Width, int Height, int Flags, SDL_Color BackgroundColor, int FPS);
			
			void PRINT_WINDOW_UPDATE(std::string Error);
			std::string GET_LAST_ERROR();

			// Sync 
			bool start();
			void close();
			void fullscreen(int Type);
			void clear();
			void load();
			bool updateEvents();
			void yield();
			// Async 
			void aclear();
			void aload();

			void showErrors();
			void hideErrors();
			void showConsole();
			void hideConsole();
			bool canPrintUpdates();

			void trackFPS();
			void killFPSThread();
			int getFPS();
			int getWindowFPS();

			int alignMiddleX(int Width);
			int alighMiddleY(int Height);

			int addFont(std::string Name);
			int addFont(std::string Name, int SizeLimit);
			int setFont(std::string NewFont);
			int setFont(std::string NewFont, int Size, SDL_Color FontColor);
			int setFontSize(int Size);
			void setFontColor(SDL_Color FontColor);
			Text* addTextTexture(std::string TextID, std::string Text, int X, int Y, int Width, int Height, double Angle, TextType Fill, TextType Fit, int Buffer);
			Text* getTextTexture(std::string TextID);
			int writeText(std::string Text, int X, int Y);
			int writeText(std::string Text, int X, int Y, double Angle);
			int writeText(std::string Text, int X, int Y, int Width, int Height);
			int writeText(std::string Text, int X, int Y, int Width, int Height, double Angle);
			int writeText(std::string Text, int X, int Y, int Width, int Height, TextType Fill, TextType Fit);
			int writeText(std::string Text, int X, int Y, int Width, int Height, double Angle, TextType Fill, TextType Fit);
			int writeText(std::string Text, SDL_Rect Size);
			int writeText(std::string Text, SDL_Rect Size, double Angle);
			int writeText(std::string Text, SDL_Rect Size, TextType Fill, TextType Fit);
			int writeText(std::string Text, SDL_Rect Size, double Angle, TextType Fill, TextType Fit);
			int writeText(std::string Text, int X, int Y, int Width, int Height, double Angle, TextType Fill, TextType Fit, int Buffer);
			int writeText(Text* Text, std::string StrText, int X, int Y, int Width, int Height, double Angle, TextType Fill, TextType Fit, int Buffer);
			int writeTextDynamic(std::string Text, SDL_Rect Size, double Angle);

			KeyState getKeyState(int Key);
			bool checkInput(std::string Key, KeyState Expected, bool IncludeJoysticks);
			bool leftClickedAt(int X, int Y);
			bool leftClickedAt(int X[], int Y[]);
			bool leftClickedAt(SDL_Rect Size);
			bool rightClickedAt(int X, int Y);
			bool rightClickedAt(int X[], int Y[]);
			bool rightClickedAt(SDL_Rect Size);
			bool middleClickedAt(int X, int Y);
			bool middleClickedAt(int X[], int Y[]);
			bool middleClickedAt(SDL_Rect Size);
			bool leftPressedAt(SDL_Rect Size);
			bool rightPressedAt(SDL_Rect Size);
			bool middlePressedAt(SDL_Rect Size);
			bool fingerTappedAt(SDL_Rect Size);
			bool fingerPressedAt(SDL_Rect Size);
			bool fingerInteractedAt(SDL_Rect Size);

			void setDrawMode(DrawMode NewMode);
			void setColor(SDL_Color Color);
			void setColor(SDL_Color Color, int Alpha);
			void drawSolidRectangle(SDL_Rect Size, SDL_Color Color);
			void drawSolidRectangle(SDL_Rect Size, SDL_Color Color, int Alpha);
			void drawHollowRectangle(SDL_Rect Size, SDL_Color Color);
			void drawHollowRectangle(SDL_Rect Size, SDL_Color Color, int Alpha);
			void drawLine(int X1, int Y1, int X2, int Y2, SDL_Color Color);
			void drawLine(int X1, int Y1, int X2, int Y2, SDL_Color Color, int Alpha);
			void drawLine(SDL_Rect Size, SDL_Color Color);
			void drawLine(SDL_Rect Size, SDL_Color Color, int Alpha);
			void drawPoint(int X, int Y, SDL_Color Color);
			void drawPoint(int X, int Y, SDL_Color Color, int Alpha);

			Image* addImage(std::string Address);
			Image* getImage(std::string Address);
			int drawImage(std::string Address, int X, int Y);
			int drawImage(std::string Address, int X, int Y, Uint8 Transparency);
			int drawImage(std::string Address, int X, int Y, double Angle);
			int drawImage(std::string Address, int X, int Y, double Angle, Uint8 Transparency);
			int drawImage(std::string Address, int X, int Y, int Width, int Height);
			int drawImage(std::string Address, int X, int Y, int Width, int Height, Uint8 Transparency);
			int drawImage(std::string Address, int X, int Y, int Width, int Height, double Angle);
			int drawImage(std::string Address, int X, int Y, int Width, int Height, double Angle, Uint8 Transparency);
			int drawImage(std::string Address, SDL_Rect Size);
			int drawImage(std::string Address, SDL_Rect Size, Uint8 Transparency);
			int drawImage(std::string Address, SDL_Rect Size, double Angle);
			int drawImage(std::string Address, SDL_Rect Size, double Angle, Uint8 Transparency);
			int drawImageFlipped(std::string Address, int X, int Y);
			int drawImageFlipped(std::string Address, int X, int Y, Uint8 Transparency);
			int drawImageFlipped(std::string Address, int X, int Y, double Angle);
			int drawImageFlipped(std::string Address, int X, int Y, double Angle, Uint8 Transparency);
			int drawImageFlipped(std::string Address, int X, int Y, int Width, int Height);
			int drawImageFlipped(std::string Address, int X, int Y, int Width, int Height, Uint8 Transparency);
			int drawImageFlipped(std::string Address, int X, int Y, int Width, int Height, double Angle);
			int drawImageFlipped(std::string Address, int X, int Y, int Width, int Height, double Angle, Uint8 Transparency);
			int drawImageFlipped(std::string Address, SDL_Rect Size);
			int drawImageFlipped(std::string Address, SDL_Rect Size, Uint8 Transparency);
			int drawImageFlipped(std::string Address, SDL_Rect Size, double Angle);
			int drawImageFlipped(std::string Address, SDL_Rect Size, double Angle, Uint8 Transparency);
			int drawImage(std::string Address, int X, int Y, int Width, int Height, double Angle, int Flip, Uint8 Transparency);
			int drawImage(std::string Address, int X, int Y, int Width, int Height, double Angle, int Flip, Uint8 Transparency, SDL_Rect Zoom);
			int getTotalImages();
			int countImagesInDirectory(std::string Directory);
			int countImagesInDirectory(std::vector<std::string> Directories);
			void loadImagesFromDirectory(std::string Directory, ImageLoadFunction CounterFunction, int* Counter);
			void loadImagesFromDirectory(std::vector<std::string> Directories, ImageLoadFunction Function);

			std::unordered_map<std::string, Button>* getAllButtons();
			Button* addButton(std::string ButtonID, std::string Text, SDL_Rect Size, SDL_Color Color, SDL_Color TextColor, TextType Textype, TextType TextFit);
			Button* addButton(std::string ButtonID, std::string Text, SDL_Rect Size, SDL_Color Color, SDL_Color TextColor, TextType Textype, TextType TextFit, std::string Texture);
			Button* addButton(std::string ButtonID, std::string Text, int FontSize, SDL_Rect Size, SDL_Color Color, SDL_Color TextColor, TextType Texttype, TextType TextFit, std::string Texture);
			Button* addButton(std::string ButtonID, std::string Text, int FontSize, SDL_Rect Size, SDL_Color Color, SDL_Color TextColor, TextType Texttype, TextType TextFit);
			Button* addButton(std::string ButtonID, std::string Text, int FontSize, SDL_Rect Size, SDL_Color Color, SDL_Color TextColor, TextType Texttype, TextType TextFit, std::string Texture, DrawMode DrawMode);
			Button* getButton(std::string Name);
			Button* getSelectedButton();
			int drawButton(std::string Name);
			void drawAllButtons();
			void unselectButton();
			void nextButton();
			void previousButton();

			std::unordered_map<std::string, TextBox>* getAllTextBoxes();
			TextBox* addTextBox(std::string Name, SDL_Rect Size);
			TextBox* addTextBox(std::string Name, SDL_Rect Size, SDL_Color Outline, SDL_Color TextColor);
			TextBox* addTextBox(std::string Name, SDL_Rect Size, SDL_Color Outline, SDL_Color TextColor, DrawMode DrawMode);
			TextBox *getTextBox(std::string Name);
			TextBox *getSelectedTextBox();
			// TODO
			// - Add arrow scroll
			int drawTextBox(std::string Name);
			void drawAllTextBoxes();
			void selectTextBox(std::string Name);
			// TODO
			// - Implement
			void unselectTextBox();
			void nextTextBox();
			void previousTextBox();

			std::unordered_map<std::string, Joystick>* getAllJoysticks();
			Joystick* getJoystick(int ID);
			Joystick* getJoystickA(int ID);
			Joystick* waitForJoystick(int ID);
			void setJoystickAddedHandle(JoystickAddedFunction Function);

			std::unordered_map<std::string, Audio>* getAudios();
			Audio* addAudio(std::string AudioID, std::string Address);
			Audio *getAudio(std::string AudioID);
			
			#ifndef __ANDROID__
			std::unordered_map<std::string, Video>* getVideos();
			Video* addVideo(std::string VideoID, std::string Video, int Loops, bool AutoUpdate, int FPS, SDL_Rect Size);
			Video* getVideo(std::string VideoID);
			Video* getVideoByAddress(std::string Address);
			#endif

			void cleanButton(std::string ButtonID);

			void CLEAR_ALL_WINDOW_DATA();
			void clearTextTextures();
			void clearImages();
			void clearButtons();
			void clearTextBoxes();
			void clearAudios();
			void clearVideos();

			int getWidth();
			int getHeight();

			// Windows only functions
			#ifdef _WIN32
			HWND getHWND(); 
			#endif

		private:
		// Variables
			std::unordered_map<std::string, Text> Texts;
			std::unordered_map<std::string, Image> Images;
			std::unordered_map<std::string, Button> Buttons;
			std::unordered_map<std::string, TextBox> TextBoxes;
			std::unordered_map<std::string, Joystick> Joysticks;
			std::unordered_map<std::string, Audio> Audios;
			#ifndef __ANDROID__
			std::unordered_map<std::string, Video> Videos;
			#endif
			
			std::vector<std::vector<TTF_Font*>> Fonts;
			std::unordered_map<int, std::string> SelectedButtonMap;
			std::unordered_map<std::string, int> FontLocationMap;

			std::string SelectedTextBox;

			JoystickAddedFunction ControllerAdded;

			SDL_Rect ImageDrawRect;
			SDL_Rect ImageZoomRect;

			int SelectedButton = -1; 
			int TotalImages = -1;

			bool PrintErrors = true;
			bool RunFPSThread = false;
			bool WindowDone = false;
			int LastError = 0;

			Uint8* KeyboardState = NULL;

			std::chrono::time_point<std::chrono::system_clock> TimeLast, TimeNow;
			std::chrono::duration<double> TimeDelta;
			std::vector<int> InputKeysToIgnore;

		// Functions
			SDL_Texture* loadTexture(std::string Path);
			SDL_Surface* loadSurface(std::string Path);
			std::pair<SDL_Surface*, SDL_Texture*> loadSurfaceTexture(std::string Path);

			bool init(std::string Title, int Width, int Height, int Flags, SDL_Color BackgroundColor);
			void stop(int Level);
			void render(SDL_Texture* Texture, SDL_Rect* Zoom, SDL_Rect* Size, double Angle, SDL_Point* Center, int Flip);

			void updateMouse();
			void updateKeyboard();
			void updateButtons();
			void updateJoysticks();
			void updateJoystickAxis(SDL_JoystickID ID, Uint8 Axis, Sint16 Value);
			void updateJoystickButton(SDL_JoystickID ID, Uint8 Button, Uint8 State);
			void handleTouchPosition();
			bool handlePolledEvents();

			void calculateFPS();
			bool capsLockOn();
			std::string scancodeToString(int Scancode);

			void drawGeometry(SDL_Rect Size, SDL_Color Color, Geometry Type);
			void drawGeometry(SDL_Rect Size, SDL_Color Color, Geometry Type, int Alpha);

			bool addJoystick(Sint32 ID);
			int removeJoystick(Sint32 ID);

			template<typename T> int adjustObjectSizeFromDrawMode(T *Object, DrawMode DrawMode);
			std::string generateTextTextureID(std::string Text, int X, int Y, int W, int H, double A);
			std::string generateTextTextureID(Text* Text);
	};
}