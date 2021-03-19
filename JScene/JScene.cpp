//
// Made by Jordan Hoosman
//

#ifndef __JSCENE__
	#include "JScene.h"
#endif

#ifndef __ANDROID__
bool operator==(const JScene::Video& Video1, const JScene::Video& Video2) {
	return Video1.name == Video2.name;
}
#endif

void JScene::NULLLOADFUNCTION(Window* w, std::string s, int n){
	return;
}
void JScene::sleep(int Delay) {
	std::this_thread::sleep_for(std::chrono::milliseconds(Delay));
}
bool JScene::fileExists(std::string File) {
	struct stat buffer;
	int32_t res = stat(File.c_str(), &buffer);

	return res == 0 && buffer.st_mode & S_IFREG;
}
bool JScene::directoryExists(std::string Directory) {
	struct stat buffer;
	int32_t res = stat(Directory.c_str(), &buffer);

	return res == 0 && buffer.st_mode & S_IFDIR;
}
bool JScene::createFile(std::string File) {
	if (fileExists(File)) return true;

	std::ofstream nfile;
	nfile.open(File, std::ofstream::out | std::ofstream::trunc);
	nfile.close();

	return true;
}
bool JScene::createDirectory(std::string Directory) {
	if (directoryExists(Directory)) return true;

	int32_t res = mkdir(Directory.c_str(), 0770);

	return res == 0;
}
void JScene::appendToFile(std::string File, std::string Text) {
	std::ofstream sfile;
	sfile.open(File, std::ios_base::app);
	sfile << Text << std::endl;
	sfile.close();
}

void JScene::scaleRect(SDL_Rect& Rect, int Pixels) {
	if (Pixels < 2) return;

	Rect.w += Pixels;
	Rect.h += Pixels;

	Rect.x -= (Pixels / 2);
	Rect.y -= (Pixels / 2);
}
int JScene::xwidth(const SDL_Rect& Rect) {
	return Rect.x + Rect.w;
}
int JScene::xwidth(SDL_Rect& Rect) {
	return Rect.x + Rect.w;
}
int JScene::yheight(const SDL_Rect& Rect) {
	return Rect.y + Rect.h;
}
int JScene::yheight(SDL_Rect& Rect) {
	return Rect.y + Rect.h;
}
int JScene::ymidheight(SDL_Rect& Rect) {
	return Rect.y + (Rect.h / 2);
}
int JScene::ymidheight(const SDL_Rect& Rect) {
	return Rect.y + (Rect.h / 2);
}
double JScene::xwidth(JScene::Dynamic_Rect& Rect) {
	return Rect.x + Rect.w;
}

SDL_Color JScene::numberToRGB(uint32_t Number) {
	SDL_Color ret;

	ret.r = Number & 0xFF;
	Number = Number >> 8;

	ret.g = Number & 0xFF;
	Number = Number >> 8;

	ret.b = Number & 0xFF;
	Number = Number >> 8;

	ret.a = Number & 0xFF;

	return ret;
}
SDL_Color JScene::invertRGB(SDL_Color Color) {
	Color.r = 255 - Color.r;
	Color.g = 255 - Color.g;
	Color.b = 255 - Color.b;

	return Color;
}
SDL_Color JScene::HSVtoRGB(double H, double S, double V, uint8_t A) {
	double c = V * S;
	double x = c * (1.0 - fabs(fmod(H / 60.0, 2) - 1.0));
	double m = V - c;

	     if (H >= 000.0 && H < 060.0) return { (uint8_t)(c + m), (uint8_t)(x + m), (uint8_t)m };
	else if (H >= 060.0 && H < 120.0) return { (uint8_t)(x + m), (uint8_t)(c + m), (uint8_t)m };
	else if (H >= 120.0 && H < 180.0) return { (uint8_t)m, (uint8_t)(c + m), (uint8_t)(x + m) };
	else if (H >= 180.0 && H < 240.0) return { (uint8_t)m, (uint8_t)(x + m), (uint8_t)(c + m) };
	else if (H >= 240.0 && H < 300.0) return { (uint8_t)(x + m), (uint8_t)m, (uint8_t)(c + m) };
	else if (H >= 300.0 && H < 360.0) return { (uint8_t)(c + m), (uint8_t)m, (uint8_t)(x + m) };
	
	return { (uint8_t)m, (uint8_t)m, (uint8_t)m };
}
SDL_Color JScene::HSLtoRGB(int H, double S, double L, uint8_t A) {
	if (S == 0.0) return { (uint8_t)(L * 255.0), (uint8_t)(L * 255.0), (uint8_t)(L * 255.0), A };

	// C = (1 - | 2L - 1 |) * S
	double c = (1.0 - fabs((2.0 * L) - 1.0)) * S;
	// X = C * (1 - | (H / 60) % 2 - 1 |)
	double x = c * (1.0 - fabs(fmod((H / 60.0), 2.0) - 1.0));
	// M = L - (C / 2)
	double m = L - (c / 2.0);

	SDL_Color ret;
	double r, g, b;

	     if (H >= 0   && H < 60)  { r = c; g = x; b = 0; }
	else if (H >= 60  && H < 120) { r = x; g = c; b = 0; }
	else if (H >= 120 && H < 180) { r = 0; g = c; b = x; }
	else if (H >= 180 && H < 240) { r = 0; g = x; b = c; }
	else if (H >= 240 && H < 300) { r = x; g = 0; b = c; }
	else if (H >= 300 && H < 360) { r = c; g = 0; b = x; }
	
	ret.r = (double)((r + m) * 255.0);
	ret.g = (double)((g + m) * 255.0);
	ret.b = (double)((b + m) * 255.0);
	ret.a = A;

	return ret;
}

JScene::JColors_HSL JScene::RGBtoHSL(SDL_Color Color) {
	JColors_HSL ret;
	
	double rr = Color.r / 255.0;
	double gg = Color.g / 255.0;
	double bb = Color.b / 255.0;

	double cmax = fmax(rr, fmax(gg, bb));
	double cmin = fmin(rr, fmin(gg, bb));
	double d = cmax - cmin;

	// Hue
		 if (d == 0) 	 ret.h = 0;
	else if (cmax == rr) ret.h = 60.0 * ( fmod((gg - bb) / d, 6.0) );
	else if (cmax == gg) ret.h = 60.0 * ( ((bb - rr) / d) + 2.0 );
	else if (cmax == bb) ret.h = 60.0 * ( ((rr - gg) / d) + 4.0 );

	// Saturation
	if (d == 0) ret.s = 0.0;
	else 		ret.s = d / ( 1.0 - fabs(2.0 * ((cmax + cmin) / 2.0) - 1.0) );

	// Lightness
	ret.l = (cmax / cmin) / 2.0;

	return ret;
}

template<typename T> bool JScene::findInVector(std::vector<T>* Vector, T Value) {
	return find(Vector->begin(), Vector->end(), Value) != Vector->end();
}
template<typename T> void JScene::removeFromVector(std::vector<T>* Vector, T Value) {
	Vector->erase(remove(Vector->begin(), Vector->end(), Value));
}

/***********************************************************************************\
|*   Joystick
\***********************************************************************************/
JScene::KeyState JScene::Joystick::getButtonState(int Button) {
	switch (Button) {
		case SDL_CONTROLLER_BUTTON_A: return abutton; 
		case SDL_CONTROLLER_BUTTON_B: return bbutton;
		case SDL_CONTROLLER_BUTTON_X: return xbutton;
		case SDL_CONTROLLER_BUTTON_Y: return ybutton;
		case SDL_CONTROLLER_BUTTON_DPAD_UP: return dbuttonup; 
		case SDL_CONTROLLER_BUTTON_DPAD_DOWN: return dbuttondown;
		case SDL_CONTROLLER_BUTTON_DPAD_LEFT: return dbuttonleft;
		case SDL_CONTROLLER_BUTTON_DPAD_RIGHT: return dbuttonright;
		case SDL_CONTROLLER_BUTTON_LEFTSHOULDER: return leftshoulder;
		case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER: return rightshoulder;
		case SDL_CONTROLLER_BUTTON_LEFTSTICK: return leftstick;
		case SDL_CONTROLLER_BUTTON_RIGHTSTICK: return rightstick;
		case SDL_CONTROLLER_BUTTON_START: return start;
		case SDL_CONTROLLER_BUTTON_BACK: return back;
		default: break;
	}

	return KeyState::Null;
}
bool JScene::Joystick::checkInput(std::string Key, KeyState Expected) {
	bool ReturnVal = false;

	if (jmap.find(Key) == jmap.end()) {
		//window->PRINT_WINDOW_UPDATE("GameError: Can't check input on controller for invalid key - " + Key);
	} else {
		switch (jmap[Key]) {
			case LUp: ReturnVal = !inputdelayedl && !centeredl && leftaxisangle > 45 && leftaxisangle < 135; break;
			case LDown: ReturnVal = !inputdelayedl && !centeredl && leftaxisangle > 225 && leftaxisangle < 315; break;
			case LLeft: ReturnVal = !inputdelayedl && !centeredl && leftaxisangle > 315 && leftaxisangle < 360; break;
			case LRight: ReturnVal = !inputdelayedl && !centeredl && leftaxisangle > 135 && leftaxisangle < 225; break;
			case LTrigger: ReturnVal = !inputdelayedl && !(lefttrigger == KeyState::Down); break;

			case RUp: ReturnVal = !inputdelayedr && !centeredr && rightaxisangle > 45 && rightaxisangle < 135; break;
			case RDown: ReturnVal = !inputdelayedr && !centeredr && rightaxisangle > 225 && rightaxisangle < 315; break;
			case RLeft: ReturnVal = !inputdelayedr && !centeredr && rightaxisangle > 315 && rightaxisangle < 360; break;
			case Right: ReturnVal = !inputdelayedr && !centeredr && rightaxisangle > 135 && rightaxisangle < 225; break;
			case RTrigger: ReturnVal = !inputdelayedr && !(righttrigger == KeyState::Down); break;

			case LShoulder: ReturnVal = getButtonState(SDL_CONTROLLER_BUTTON_LEFTSHOULDER) == Expected; break;
			case LStick: ReturnVal = getButtonState(SDL_CONTROLLER_BUTTON_LEFTSTICK) == Expected; break;
			case RShoulder: ReturnVal = getButtonState(SDL_CONTROLLER_BUTTON_LEFTSHOULDER) == Expected; break;
			case RStick: ReturnVal = getButtonState(SDL_CONTROLLER_BUTTON_LEFTSHOULDER) == Expected;

			case DUp: ReturnVal = getButtonState(SDL_CONTROLLER_BUTTON_DPAD_UP) == Expected; break;
			case DDown: ReturnVal = getButtonState(SDL_CONTROLLER_BUTTON_DPAD_DOWN) == Expected; break;
			case DLeft: ReturnVal = getButtonState(SDL_CONTROLLER_BUTTON_DPAD_LEFT) == Expected; break;
			case DRight: ReturnVal = getButtonState(SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == Expected; break;

			case A: ReturnVal = getButtonState(SDL_CONTROLLER_BUTTON_A) == Expected; break;
			case B: ReturnVal = getButtonState(SDL_CONTROLLER_BUTTON_B) == Expected; break;
			case X: ReturnVal = getButtonState(SDL_CONTROLLER_BUTTON_X) == Expected; break;
			case Y: ReturnVal = getButtonState(SDL_CONTROLLER_BUTTON_Y) == Expected; break;
			case Start: ReturnVal = getButtonState(SDL_CONTROLLER_BUTTON_START) == Expected; break;
			case Back: ReturnVal = getButtonState(SDL_CONTROLLER_BUTTON_BACK) == Expected; break;
			case Guide: ReturnVal = getButtonState(SDL_CONTROLLER_BUTTON_GUIDE) == Expected; break;

			default: ReturnVal = false; break;
		}

		if ((!inputdelayedl || !inputdelayedr) && ReturnVal) {
			delayStickInput(inputdelayedr ? 1 : 0);
		}
	}

	return ReturnVal;
}
void JScene::Joystick::setJoystickMap(std::vector<std::string> Keys) {
    for (int jkey = 0; jkey < (int)Keys.size(); jkey++) {
        if ((int)Keys.size() >= jkey && Keys[jkey] != "") {
            jmap[Keys[jkey]] = (JoystickButton)jkey;
        }
    }
}
void JScene::Joystick::setJoystickMapDefaults(std::vector<std::string> Keys) {
	if (Keys.size() < 10) {
		//window->PRINT_WINDOW_UPDATE("WindowError: Can't set joystickmap - Not enough keys");
		return;
	} else if (Keys.size() > 10) {
		//window->PRINT_WINDOW_UPDATE("WindowError: Can't set joystickmap - Too many keys");
		return;
	}

	jmap[Keys[0]] = LUp;
	jmap[Keys[1]] = LDown;
	jmap[Keys[2]] = LLeft;
	jmap[Keys[3]] = LRight;
	jmap[Keys[4]] = A;
	jmap[Keys[5]] = B;
	jmap[Keys[6]] = X;
	jmap[Keys[7]] = Y;
	jmap[Keys[8]] = Start;
	jmap[Keys[9]] = Guide;
}
void JScene::Joystick::delayStickInput(int Stick) {
	std::thread(&Joystick::delaystickthread, this, Stick).detach();
}
void JScene::Joystick::setInputDelay(int Duration) {
	delayduration = Duration;
}
void JScene::Joystick::setSensitivity(int Sensitivity) {
	sensitivity = Sensitivity;
}
void JScene::Joystick::delaystickthread(int Stick) {
	if (Stick == 0) inputdelayedl = true;
	else inputdelayedr = true;
	
	if (delayduration > 0)
		sleep(delayduration);
	else
		if (Stick == 0) while (!centeredl) std::this_thread::yield();
		else while (!centeredr) std::this_thread::yield();

	if (Stick == 0) inputdelayedl = false;
	else inputdelayedr = false;
}

/***********************************************************************************\
|*   Audio
\***********************************************************************************/
bool JScene::Audio::isPlaying() {
	return playing;
}
void JScene::Audio::play() {
	play(0);
}
void JScene::Audio::play(int Loops) {
	playing = true;
	Mix_PlayMusic(audio, Loops);
}
void JScene::Audio::seek(double Time) {
	Mix_SetMusicPosition(Time);
}
void JScene::Audio::volume(int Volume) {
	if (Volume < 0 || Volume > 100) return;
	Mix_VolumeMusic(Volume);
}

/***********************************************************************************\
|*   Video
\***********************************************************************************/
#ifndef __ANDROID__
int JScene::Video::play() {
	AVCodec* Codec = NULL;
	
	if (address.size() < 0) {
		//window->PRINT_WINDOW_UPDATE("WindowError: Error playing video - Invalid video");
		return VideoInitFailed;
	}
	// open input file
	if (ret = avformat_open_input(&fmt_ctx, address.c_str(), NULL, NULL) < 0) {
		//window->PRINT_WINDOW_UPDATE("WindowError: Error playing video - Cannot open input file");
		return VideoInitFailed;
	}
	//get stream info
	if (ret = avformat_find_stream_info(fmt_ctx, NULL) < 0) {
		//window->PRINT_WINDOW_UPDATE("WindowError: Error playing video - Cannot get stream info");
		return VideoInitFailed;
	}
	// get video stream index
	for (int i = 0; i < (int)fmt_ctx->nb_streams; i++) {
		if (fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
			videoindex = i;
			break;
		}
	}
	if (videoindex < 0) {
		//window->PRINT_WINDOW_UPDATE("WindorError: Error playing video - No video stream");
		return VideoInitFailed;
	}
	// dump video stream info
	//av_dump_format(fmt_ctx, videoindex, address.c_str(), false);

	//alloc memory for codec context
	codec_ctx = avcodec_alloc_context3(NULL);

	// retrieve codec params from format context
	if (ret = avcodec_parameters_to_context(codec_ctx, fmt_ctx->streams[videoindex]->codecpar) < 0) {
		//window->PRINT_WINDOW_UPDATE("WindowError: Error playing video - Cannot get codec parameters");
		return VideoInitFailed;
	}

	// find decoding codec
	Codec = avcodec_find_decoder(codec_ctx->codec_id);
	if (Codec == NULL) {
		//window->PRINT_WINDOW_UPDATE("WindowError: Error playing video - No decoder found");
		return VideoInitFailed;
	}

	// try to open codec
	if (ret = avcodec_open2(codec_ctx, Codec, NULL) < 0) {
		//window->PRINT_WINDOW_UPDATE("WindowError: Error playing video - Cannot open video decoder");
		return VideoInitFailed;
	}

	//init packet
	pkt = av_packet_alloc();
	av_init_packet(pkt);
	if (!pkt) {
		//window->PRINT_WINDOW_UPDATE("WindowError: Error playing video - Cannot init packet");
		return VideoInitFailed;
	}

	// init frame
	frame = av_frame_alloc();
	if (!frame) {
		//window->PRINT_WINDOW_UPDATE("WindowError: Error playing video - Cannot init frame");
		return VideoInitFailed;
	}

	//open input file
	fin = fopen(address.c_str(), "rb");
	if (!fin) {
		//window->PRINT_WINDOW_UPDATE("WindowError: Error playing video - Cannot open input file");
		return VideoInitFailed;
	}

	framesize.x = 0;
	framesize.y = 0;
	framesize.w = codec_ctx->width;
	framesize.h = codec_ctx->height;
	texture = SDL_CreateTexture(vrender, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING, framesize.w, framesize.h);

	playing = true;
	if (autoupdate) {
		std::thread(&JScene::Video::videoThread, this).detach();
	}

	return WindowSuccess;
}
void JScene::Video::close(){
	playing = false;
}
void JScene::Video::remove() {
	decode(true);

	if (fin) fclose(fin);
	if (fout) fclose(fout);
	if (codec_ctx) avcodec_close(codec_ctx);
	if (fmt_ctx) avformat_close_input(&fmt_ctx);
	if (frame) av_frame_free(&frame);
	if (pkt) av_packet_free(&pkt);

	SDL_DestroyTexture(texture);
	
	vrender = NULL;

	std::unordered_map<std::string, Video>* vmap = window->getVideos();

	window = NULL;

	if (name != "") vmap->erase(name);
}
void JScene::Video::pause() {
	paused = true;
}
void JScene::Video::unpause() {
	paused = false;
}
void JScene::Video::drawFrame() {
	drawFrame(NULL);
}
void JScene::Video::drawFrame(SDL_Rect* Zoom) {
	if (texture == NULL) return;

	zoom = Zoom == NULL ? drawsize : *Zoom;

	SDL_RenderCopyEx(vrender, texture, &zoom, &drawsize, 0.0, NULL, SDL_FLIP_NONE);
}
void JScene::Video::changeZoom(SDL_Rect Zoom) {
	zoom = Zoom;
}
int JScene::Video::videoThread() {
	for (int loop = 0; playing && (loops == -1 || loop < loops); loop++) {
		while (true) {
			if (!playing) {
				break;
			} else if (paused) {
				while (paused) std::this_thread::yield();
			}

			// read an encoded packet from file
			if (ret = av_read_frame(fmt_ctx, pkt) < 0) break;
			// if packet data is video data then send it to decoder
			if (pkt->stream_index == videoindex) {
				decode(false);
			}

			// release packet buffers to be allocated again
			av_packet_unref(pkt);

			vTimeNow = std::chrono::system_clock::now();
			vDelta = vTimeNow - vTimeLast;
			while (vDelta.count() * 1000.0 < 1000.0 / fps) {
				std::this_thread::yield();

				vTimeNow = std::chrono::system_clock::now();
				vDelta = vTimeNow - vTimeLast;
			}
			vTimeLast = vTimeNow;
		}

		if (!playing) break;

		avio_seek(fmt_ctx->pb, 0, SEEK_SET);
		avformat_seek_file(fmt_ctx, videoindex, 0, 0, fmt_ctx->streams[videoindex]->duration, 0);
	}

	remove();

	return WindowSuccess;
}
void JScene::Video::decode(bool close) {
	if (!playing) return;

	if (close) pkt = NULL;

	//send packet to decoder
	ret = avcodec_send_packet(codec_ctx, pkt);
	if (ret < 0) {
		//window->PRINT_WINDOW_UPDATE("WindowError: Video decode error - Error sending a packet for decoding");
		return;
	}

	while (ret >= 0) {
		// receive frame from decoder
		ret = avcodec_receive_frame(codec_ctx, frame);
		if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) return;
		else if (ret < 0) {
			//window->PRINT_WINDOW_UPDATE("WindowError: Video decode error - Error during decoding");
			return;
		}

		SDL_UpdateYUVTexture(texture, &framesize, frame->data[0], frame->linesize[0], frame->data[1], frame->linesize[1], frame->data[2], frame->linesize[2]);
	}
}
#endif

/***********************************************************************************\
|*   Box
\***********************************************************************************/
int JScene::Box::width() {
	return size.w;
}
int JScene::Box::height() {
	return size.h;
}
void JScene::Box::moveup(int Pixels) {
	size.y -= Pixels;
}
void JScene::Box::movedown(int Pixels) {
	size.y += Pixels;
}
void JScene::Box::moveleft(int Pixels) {
	size.x -= Pixels;
}
void JScene::Box::moveright(int Pixels) {
	size.x += Pixels;
}
void JScene::Box::setposition(int X, int Y) {
	size.x = X;
	size.y = Y;
}
void JScene::Box::click() {
	clicked = true;
}
void JScene::Box::unclick() {
	clicked = false;
}
bool JScene::Box::wasclicked() {
	return clicked;
}
bool JScene::Box::leftclicked() {
	return window->leftClickedAt(this->size);
}
bool JScene::Box::rightclicked() {
	return window->rightClickedAt(this->size);
}
bool JScene::Box::leftpressed() {
	return window->leftPressedAt(this->size);
}
bool JScene::Box::middlepressed() {
	return window->middlePressedAt(this->size);
}
bool JScene::Box::rightpressed() {
	return window->leftPressedAt(this->size);
}
bool JScene::Box::fingertapped() {
	return window->fingerTappedAt(size);
}
bool JScene::Box::fingerpressed() {
	return window->fingerPressedAt(size);
}
bool JScene::Box::fingerinteracted() {
	return window->fingerInteractedAt(size);
}

/***********************************************************************************\
|*   Image
\***********************************************************************************/
void JScene::Image::rotateleft(int Degrees) {
	angle -= Degrees;
}
void JScene::Image::rotateright(int Degrees) {
	angle += Degrees;
}

/***********************************************************************************\
|*   TextBox
\***********************************************************************************/
void JScene::TextBox::draw() {
	window->drawTextBox(name);
}
void JScene::TextBox::clear() {
	text.text = "";
}
void JScene::TextBox::changetext(std::string NewText) {
	text.text = NewText;
}
void JScene::TextBox::appendtext(std::string Text) {
	text.text += Text;
}
void JScene::TextBox::select() {
	window->selectTextBox(name);
}
bool JScene::TextBox::isselected() {
	return name == window->getSelectedTextBox()->name;
}

/***********************************************************************************\
|*   Button
\***********************************************************************************/
void JScene::Button::changeFontSize(int Size) {
	text.fontsize = Size;
}
void JScene::Button::changeText(std::string NewText) {
	text.text = NewText;
}
void JScene::Button::changeButtonColor(SDL_Color NewColor) {
	color = NewColor;
}
void JScene::Button::changeSize(SDL_Rect NewSize) {
	size = NewSize;
}
void JScene::Button::changeTextColor(SDL_Color NewColor) {
	text.fontcolor = NewColor;
}
void JScene::Button::changeTexture(Window* w, SDL_Texture* NewTexture) {
	texture = NewTexture;
}
void JScene::Button::increaseTransparency(int Amount) {
	color.a += Amount;
}
void JScene::Button::decreaseTransparency(int Amount) {
	color.a -= Amount;
}
void JScene::Button::draw() {
	window->drawButton(id);
}
void JScene::Button::clean() {
	if (texture != NULL) {
		//Images.erase(Buttons[button.first].address);

		//SDL_DestroyTexture(Buttons[button.first].texture);
	}
	window->cleanButton(id);
}
/***********************************************************************************\
|*   Window - Private
\***********************************************************************************/
SDL_Texture* JScene::Window::loadTexture(std::string Path) {
	SDL_Surface *LoadedSurface = IMG_Load(Path.c_str());
	SDL_Texture *NewTexture = NULL;

	if (LoadedSurface == NULL) {
		PRINT_WINDOW_UPDATE("SDLError: Failed to load texture surface - "  + Path + " - " + std::string(IMG_GetError()));
	} else {
		NewTexture = SDL_CreateTextureFromSurface(SDLRenderer, LoadedSurface);
		if (NewTexture == NULL) {
			PRINT_WINDOW_UPDATE("SDLError: Failed to create texture - " + Path + " - " +  std::string(SDL_GetError()));
		}

		SDL_SetTextureBlendMode(NewTexture, SDL_BLENDMODE_BLEND);
		SDL_FreeSurface(LoadedSurface);
	}

	return NewTexture;
}
SDL_Surface* JScene::Window::loadSurface(std::string Path) {
	SDL_Surface *LoadedSurface = IMG_Load(Path.c_str());
	SDL_Surface *NewSurface = NULL;

	if (LoadedSurface == NULL) {
		PRINT_WINDOW_UPDATE("SDLError: Failed to load surface - " + Path + " - " + std::string(SDL_GetError()));
	} else {
		NewSurface = SDL_ConvertSurface(LoadedSurface, SDLSurface->format, NULL);
		if (NewSurface == NULL) {
			PRINT_WINDOW_UPDATE("SDLError: Failed to create surface - " + Path + " - " + std::string(SDL_GetError()));
		}
		
		SDL_FreeSurface(LoadedSurface);
	}

	return NewSurface;
}
std::pair<SDL_Surface*, SDL_Texture*> JScene::Window::loadSurfaceTexture(std::string Path) {
	SDL_Surface *NewSurface = IMG_Load(Path.c_str());
	SDL_Texture *NewTexture = NULL;

	if (NewSurface == NULL) {
		PRINT_WINDOW_UPDATE("SDLError: Failed to load texture surface - "  + Path + " - " + std::string(IMG_GetError()));
	} else {
		NewTexture = SDL_CreateTextureFromSurface(SDLRenderer, NewSurface);
		if (NewTexture == NULL) {
			PRINT_WINDOW_UPDATE("SDLError: Failed to create texture - " + Path + " - " +  std::string(SDL_GetError()));
		}

		SDL_SetTextureBlendMode(NewTexture, SDL_BLENDMODE_BLEND);
	}

	return std::make_pair(NewSurface, NewTexture);
}

bool JScene::Window::init(std::string Title, int Width, int Height, int Flags, SDL_Color BackgroundColor) {
	// Init SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		PRINT_WINDOW_UPDATE("SDLError: SDL could not initialize - " + std::string(SDL_GetError())  );
		LastError = IEINIT;
		return false;
	}

	// Create window
	SDLWindow = SDL_CreateWindow(Title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Width, Height, Flags);
	if (SDLWindow == NULL) {
		PRINT_WINDOW_UPDATE("SDLError: Window could not be created - " + std::string(SDL_GetError()));
		LastError = IEWINDOW;
		return false;
	}
	
	// Init SDL_image
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		PRINT_WINDOW_UPDATE("SDLError: SDL_image could not initialize - " + std::string(IMG_GetError()));
		LastError = IEIMG;
		return false;
	}

	// Init SDL_mixer
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		PRINT_WINDOW_UPDATE("SDLError: SDL audio could not initialize");
		LastError = IEAUDIO;
		return false;
	}

	// Init Renderer
	SDLRenderer = SDL_CreateRenderer(SDLWindow, -1, SDL_RENDERER_ACCELERATED);
	if (SDLRenderer == NULL) {
		PRINT_WINDOW_UPDATE("SDLError: Renderer could not be created - " + std::string(SDL_GetError()));
		LastError = IERENDER;
		return false;
	}
	
	// Init SDL_ttf
	if (TTF_Init() == -1) {
		PRINT_WINDOW_UPDATE("SDLError: SDL_ttf could not initialize - " + std::string(TTF_GetError()));
		LastError = IETTF;
	}
	
	// Finish setup
	SDL_SetRenderDrawColor(SDLRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDLSurface = SDL_GetWindowSurface(SDLWindow);

	SDL_Rect devicedisplay;
	SDL_GetDisplayBounds(0, &devicedisplay);
	Properties.devicewidth = devicedisplay.w;
	Properties.deviceheight = devicedisplay.h;

	LastError = IESUCCESS;
	return true;
}
void JScene::Window::stop(int Level) {
	SDLWindow = NULL;
	SDLRenderer = NULL;
	SDLSurface = NULL;

	SDL_DestroyWindow(SDLWindow);
	SDL_DestroyRenderer(SDLRenderer);
	SDL_FreeSurface(SDLSurface);

	if (Level == 1) {
		Mix_Quit();
		IMG_Quit();
		SDL_Quit();
	}
}
void JScene::Window::render(SDL_Texture* Texture, SDL_Rect* Zoom, SDL_Rect* Size, double Angle, SDL_Point* Center, int Flip) {
	if (SDLRenderer == NULL) {
		PRINT_WINDOW_UPDATE("WindowFatalError: Cannot load window - Renderer is NULL.");
		exit(1);
	} else if (Texture == NULL) {
		PRINT_WINDOW_UPDATE("WindowError: Cannot render window - Null texture");
		return;
	}

	ImageDrawRect = *Size;
	ImageZoomRect = Zoom == NULL ? *Size : *Zoom;
	
	SDL_RenderCopyEx(SDLRenderer, Texture, Zoom, &ImageDrawRect, Angle, Center, (SDL_RendererFlip)Flip);
}

void JScene::Window::updateMouse() {
	SDL_GetMouseState(&Mouse.x, &Mouse.y);

	bool leftmouseclick = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT);
	bool rightmouseclick = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT);
	bool middlemouseclick = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE);

	// Update left mouse button	
	if (leftmouseclick) {
		Mouse.down[0] = true;
	} else if (!leftmouseclick && Mouse.down[0] && !Mouse.click[0]) {
		Mouse.click[0] = true;
		Mouse.down[0] = false;
	} else if (!leftmouseclick && Mouse.click[0]) {
		Mouse.click[0] = false;
	}

	// Update right mouse button
	if (rightmouseclick) {
		Mouse.down[1] = true;
	} else if (!rightmouseclick && Mouse.down[1] && !Mouse.click[1]) {
		Mouse.click[1] = true;
		Mouse.down[1] = false;
	} else if (!rightmouseclick && Mouse.click[1]) {
		Mouse.click[1] = false;
	}

	// Update middle mouse button
	if (middlemouseclick){
		Mouse.down[2] = true;
	} else if (!middlemouseclick && Mouse.down[2] && !Mouse.click[2]){
		Mouse.click[2] = true;
		Mouse.down[2] = false;
	} else if (!middlemouseclick && Mouse.down[2] && Mouse.click[2]){
		Mouse.click[2] = false;
	}
	
	// Reset touch click
	Mouse.click[3] = false;
	Mouse.down[3] = false;
}
void JScene::Window::updateKeyboard() {
	SDL_PumpEvents();
	
	KeyboardState = const_cast<Uint8*>(SDL_GetKeyboardState(NULL));

	// Update keyboard states
	Keyboard.clickedkeys.clear();
	
	// A - Z
	for (int key = SDL_SCANCODE_A; key <= SDL_SCANCODE_Z; key++) if (KeyboardState[key] && !findInVector(&Keyboard.pressedkeys, key)) Keyboard.pressedkeys.push_back(key);
	// 0 - 9
	for (int key = SDL_SCANCODE_1; key <= SDL_SCANCODE_0; key++) if (KeyboardState[key] && !findInVector(&Keyboard.pressedkeys, key)) Keyboard.pressedkeys.push_back(key);
	// Spec
	for (int key = SDL_SCANCODE_RETURN; key <= SDL_SCANCODE_UP; key++) if (KeyboardState[key] && !findInVector(&Keyboard.pressedkeys, key)) Keyboard.pressedkeys.push_back(key);

	if (KeyboardState[SDL_SCANCODE_LSHIFT] && !findInVector(&Keyboard.pressedkeys, (int)SDL_SCANCODE_LSHIFT)) Keyboard.pressedkeys.push_back(SDL_SCANCODE_LSHIFT);

	for (int key : Keyboard.pressedkeys) {
		// Set keystate to clicked
		if (!KeyboardState[key]) {
			if (findInVector(&Keyboard.pressedkeys, key)) {
				Keyboard.clickedkeys.push_back(key);
				removeFromVector(&Keyboard.pressedkeys, key);
				
				//Update current textbox
				if (SelectedTextBox != "") {
					TextBox* WTextBox = getTextBox(SelectedTextBox);
					std::string NewChar = scancodeToString((SDL_Scancode)key);
					if (NewChar == "\b") {
						if (WTextBox->text.text.length() > 0) WTextBox->text.text = WTextBox->text.text.substr(0, WTextBox->text.text.length() - 1);
					} else {
						WTextBox->text.text += NewChar;
					}
				}
			}
		}
	}
}
void JScene::Window::updateButtons() {
	for (auto const& button : Buttons) {
		Buttons[button.first].clicked = false;
	}
}
void JScene::Window::updateJoysticks() {
	for (auto const& joystick: Joysticks) {
		Joystick* wj = &Joysticks[joystick.first];

		if (wj->abutton == KeyState::Clicked) wj->abutton = KeyState::Up;
		if (wj->bbutton == KeyState::Clicked) wj->bbutton = KeyState::Up;
		if (wj->xbutton == KeyState::Clicked) wj->xbutton = KeyState::Up;
		if (wj->ybutton == KeyState::Clicked) wj->ybutton = KeyState::Up;
		if (wj->dbuttonup == KeyState::Clicked) wj->dbuttonup = KeyState::Up;
		if (wj->dbuttondown == KeyState::Clicked) wj->dbuttondown = KeyState::Up;
		if (wj->dbuttonleft == KeyState::Clicked) wj->dbuttonleft = KeyState::Up;
		if (wj->dbuttonright == KeyState::Clicked) wj->dbuttonright = KeyState::Up;
		if (wj->leftshoulder == KeyState::Clicked) wj->leftshoulder = KeyState::Up;
		if (wj->rightshoulder == KeyState::Clicked) wj->rightshoulder = KeyState::Up;
		if (wj->leftstick == KeyState::Clicked) wj->leftstick = KeyState::Up;
		if (wj->rightstick == KeyState::Clicked) wj->rightstick = KeyState::Up;
		if (wj->start == KeyState::Clicked) wj->start = KeyState::Up;
		if (wj->back == KeyState::Clicked) wj->back = KeyState::Up;
	}
}
void JScene::Window::updateJoystickAxis(SDL_JoystickID ID, Uint8 Axis, Sint16 Value) {
	Joystick *WJoystick = getJoystick(ID);
	if (WJoystick->controller == NULL) return;

	int lr = -1;

	switch (Axis) {
		case SDL_CONTROLLER_AXIS_LEFTX: {
			lr = 0;

			if (Value > WJoystick->sensitivity) WJoystick->leftaxis[0] = 1;
			else if (Value < -WJoystick->sensitivity) WJoystick->leftaxis[0] = -1;
			else WJoystick->leftaxis[0] = 0;
			WJoystick->lastleftx = Value;

			break;
		}
		case SDL_CONTROLLER_AXIS_LEFTY: {
			lr = 0;

			if (Value > WJoystick->sensitivity) WJoystick->leftaxis[1] = 1;
			else if (Value < -WJoystick->sensitivity) WJoystick->leftaxis[1] = -1;
			else WJoystick->leftaxis[1] = 0;
			WJoystick->lastlefty = Value;

			break;
		}
		case SDL_CONTROLLER_AXIS_RIGHTX: {
			lr = 1;

			if (Value > WJoystick->sensitivity) WJoystick->rightaxis[0] = 1;
			else if (Value < -WJoystick->sensitivity) WJoystick->rightaxis[0] = -1;
			else WJoystick->rightaxis[0] = 0;
			WJoystick->lastrightx = Value;

			break;
		}
		case SDL_CONTROLLER_AXIS_RIGHTY: {
			lr = 1;

			if (Value > WJoystick->sensitivity) WJoystick->rightaxis[1] = 1;
			else if (Value < -WJoystick->sensitivity) WJoystick->rightaxis[1] = -1;
			else WJoystick->rightaxis[1] = 0;
			WJoystick->lastrighty = Value;

			break;
		}
		case SDL_CONTROLLER_AXIS_TRIGGERRIGHT: {
			lr = 1;

			if (Value > WJoystick->sensitivity) WJoystick->righttrigger = KeyState::Down;
			else WJoystick->righttrigger = KeyState::Up;

			break;
		}
		case SDL_CONTROLLER_AXIS_TRIGGERLEFT: {
			lr = 0;

			if (Value > WJoystick->sensitivity) WJoystick->lefttrigger = KeyState::Down;
			else WJoystick->lefttrigger = KeyState::Up;

			break;
		}
		default: break;
	}
}
void JScene::Window::updateJoystickButton(SDL_JoystickID ID, Uint8 Button, Uint8 State) {
	Joystick *WJoystick = getJoystick(ID);
	if (WJoystick->controller == NULL) {
		PRINT_WINDOW_UPDATE("WindowError: Invalid controller - " + ID);
		return;
	}
	
	KeyState *CButton = NULL;
	switch (Button) {
		case SDL_CONTROLLER_BUTTON_A: CButton = &WJoystick->abutton; break;
		case SDL_CONTROLLER_BUTTON_B: CButton = &WJoystick->bbutton; break;
		case SDL_CONTROLLER_BUTTON_X: CButton = &WJoystick->xbutton; break;
		case SDL_CONTROLLER_BUTTON_Y: CButton = &WJoystick->ybutton; break;
		case SDL_CONTROLLER_BUTTON_DPAD_UP: CButton = &WJoystick->dbuttonup; break;
		case SDL_CONTROLLER_BUTTON_DPAD_DOWN: CButton = &WJoystick->dbuttondown; break;
		case SDL_CONTROLLER_BUTTON_DPAD_LEFT: CButton = &WJoystick->dbuttonleft; break;
		case SDL_CONTROLLER_BUTTON_DPAD_RIGHT: CButton = &WJoystick->dbuttonright; break;
		case SDL_CONTROLLER_BUTTON_LEFTSHOULDER: CButton = &WJoystick->leftshoulder; break;
		case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER: CButton = &WJoystick->rightshoulder; break;
		case SDL_CONTROLLER_BUTTON_LEFTSTICK: CButton = &WJoystick->leftstick; break;
		case SDL_CONTROLLER_BUTTON_RIGHTSTICK: CButton = &WJoystick->rightstick; break;
		case SDL_CONTROLLER_BUTTON_START: CButton = &WJoystick->start; break;
		case SDL_CONTROLLER_BUTTON_BACK: CButton = &WJoystick->back; break;
	}

		 if (CButton != NULL && State == SDL_PRESSED)								 *CButton = KeyState::Down;
	else if (CButton != NULL && State == SDL_RELEASED && *CButton == KeyState::Down) *CButton = KeyState::Clicked;
}
void JScene::Window::handleTouchPosition() {
	Mouse.x = Properties.width * SDLEvent.tfinger.x;
	Mouse.y = Properties.height * SDLEvent.tfinger.y;	
}
// TODO
// - Touch event handler
//   - Swipes, Multiple touches
bool JScene::Window::handlePolledEvents() {
	while (SDL_PollEvent(&SDLEvent) != 0) {
		switch (SDLEvent.type) {
			case SDL_QUIT: stop(1); return false;

		#ifdef __ANDROID__
			case SDL_WINDOWEVENT:
				if (SDLEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
					Properties.devicewidth = SDLEvent.window.data1;
					Properties.deviceheight = SDLEvent.window.data2;
				}
				break;
			case SDL_FINGERMOTION: case SDL_FINGERDOWN: 
				Mouse.down[3] = true;
				handleTouchPosition();
				break;
			case SDL_FINGERUP:
				Mouse.click[3] = true;
				Mouse.down[3] = false;
				handleTouchPosition();
				break;
			// TODO
			// - Option to adjust angle
			// 
			case SDL_MULTIGESTURE: 
				// Rotation
				if (fabs(SDLEvent.mgesture.dTheta) > 3.14 / 180.0) {

				} 
				// Pinch
				else if (fabs(SDLEvent.mgesture.dTheta) > .002) {
					// Open
					if (SDLEvent.mgesture.dDist > 0) {

					}
					// Close
					else {

					}
				}

				break;
		#endif
			case SDL_MOUSEWHEEL:
				if (SDLEvent.wheel.y > 0) Mouse.scroll--;
				else if (SDLEvent.wheel.y < 0) Mouse.scroll++;
				SDLEvent.wheel.y = 0;
				break;
			case SDL_CONTROLLERDEVICEADDED:
				if ((int)Joysticks.size() <= SDLEvent.cdevice.which)
					addJoystick(SDLEvent.cdevice.which);
				break;
			case SDL_CONTROLLERDEVICEREMOVED:
				removeJoystick(SDLEvent.cdevice.which);
				break;
			case SDL_CONTROLLERAXISMOTION:
				updateJoystickAxis(SDLEvent.caxis.which, SDLEvent.caxis.axis, SDLEvent.caxis.value);
				break;
			case SDL_CONTROLLERBUTTONDOWN: case SDL_CONTROLLERBUTTONUP:
				updateJoystickButton(SDLEvent.cbutton.which, SDLEvent.cbutton.button, SDLEvent.cbutton.state);
				break;
			
			default: break;
		}
	}

	return true;
}

void JScene::Window::calculateFPS() {
	while (RunFPSThread) {
		Properties.fps = Properties.fpsframe;
		Properties.fpsframe = 0;
		
		Properties.windowfps = Properties.windowfpsframe;
		Properties.windowfpsframe = 0;

		sleep(1000);
	}
}
bool JScene::Window::capsLockOn() {
	#ifdef __ANDROID__
		return false;
	#elif _WIN32
		return GetKeyState(VK_CAPITAL) == 1;
	#elif __linux__
		XKeyboardState x;
		return (x.led_mask & 1);
	#else
		return false;
	#endif
}
std::string JScene::Window::scancodeToString(int Scancode) {
	if (getKeyState(SDL_SCANCODE_LSHIFT) == KeyState::Down || getKeyState(SDL_SCANCODE_LSHIFT) == KeyState::Down || (capsLockOn() && Scancode >= SDL_SCANCODE_A && Scancode <= SDL_SCANCODE_Z)) {
		if (Scancode >= SDL_SCANCODE_A && Scancode <= SDL_SCANCODE_Z) {
			return SDL_GetScancodeName((SDL_Scancode) Scancode);
		} else {
			switch (Scancode) {
				case SDL_SCANCODE_1: return "!";
				case SDL_SCANCODE_2: return "@";
				case SDL_SCANCODE_3: return "#";
				case SDL_SCANCODE_4: return "$";
				case SDL_SCANCODE_5: return "%";
				case SDL_SCANCODE_6: return "^";
				case SDL_SCANCODE_7: return "&";
				case SDL_SCANCODE_8: return "*";
				case SDL_SCANCODE_9: return "(";
				case SDL_SCANCODE_0: return ")";

				case SDL_SCANCODE_MINUS: return "_";
				case SDL_SCANCODE_EQUALS: return "+";
				case SDL_SCANCODE_LEFTBRACKET: return "{";
				case SDL_SCANCODE_RIGHTBRACKET: return "}";
				case SDL_SCANCODE_BACKSLASH: return "|";
				case SDL_SCANCODE_SEMICOLON: return ":";
				case SDL_SCANCODE_APOSTROPHE: return "\"";
				case SDL_SCANCODE_GRAVE: return "~";
				case SDL_SCANCODE_COMMA: return "<";
				case SDL_SCANCODE_PERIOD: return ">";
				case SDL_SCANCODE_SLASH: return "?";
				default: return "";
			}
		}
	} else {
		std::string SName = SDL_GetScancodeName((SDL_Scancode) Scancode);
		if (Scancode >= SDL_SCANCODE_A && Scancode <= SDL_SCANCODE_Z) {
			std::transform(SName.begin(), SName.end(), SName.begin(), ::tolower);
			return SName;
		} else if (Scancode == SDL_SCANCODE_BACKSPACE) {
			return "\b";
		} else if (Scancode == SDL_SCANCODE_SPACE) {
			return " ";
		} else if (find(InputKeysToIgnore.begin(), InputKeysToIgnore.end(), Scancode) != InputKeysToIgnore.end()) {
			return "";
		} else {
			return SName;
		}
	}
}

void JScene::Window::drawGeometry(SDL_Rect Size, SDL_Color Color, Geometry Type) {
	drawGeometry(Size, Color, Type, Color.a);
}
void JScene::Window::drawGeometry(SDL_Rect Size, SDL_Color Color, Geometry Type, int Alpha) {
	SDL_SetRenderDrawBlendMode(SDLRenderer, SDL_BLENDMODE_BLEND);
	setColor(Color, Alpha);

	switch (Type) {
		case Geometry::Fill: SDL_RenderFillRect(SDLRenderer, &Size); break;
		case Geometry::Outline: SDL_RenderDrawRect(SDLRenderer, &Size); break;
		case Geometry::Line: SDL_RenderDrawLine(SDLRenderer, Size.x, Size.y, Size.w, Size.h); break;
		case Geometry::Point: SDL_RenderDrawPoint(SDLRenderer, Size.x, Size.y); break;
		default: PRINT_WINDOW_UPDATE("WindowError: Invalid Geometry - " + int(Type)); break;
	}
}

bool JScene::Window::addJoystick(Sint32 ID) {
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		PRINT_WINDOW_UPDATE("SDLError: Linear texture filtering not enabled!");
	} else {
		if (SDL_NumJoysticks() < 1) {
			PRINT_WINDOW_UPDATE("WindowError: No joysticks connected.");
		} else {
			Joystick joystick;
			joystick.window = this;
			joystick.controller = SDL_GameControllerOpen(ID);

			if (joystick.controller == NULL || !SDL_IsGameController(ID)) {
				PRINT_WINDOW_UPDATE("SDLError: Unable to open game controller -" + std::string(SDL_GetError()));
				SDL_GameControllerClose(joystick.controller);
			} else {
				joystick.joystick = SDL_GameControllerGetJoystick(joystick.controller);
				joystick.joystickid = SDL_JoystickInstanceID(joystick.joystick);
				Joysticks[std::to_string(ID)] = joystick;

				ControllerAdded(&Joysticks[std::to_string(ID)]);

				return true;
			}
		}
	}

	return false;
}
int JScene::Window::removeJoystick(Sint32 ID) {
	Joystick WJoystick = *getJoystick(ID);
	if (WJoystick.controller == NULL) {
		PRINT_WINDOW_UPDATE("WindowError: Invalid joystick ID - " + ID);
		return InvalidJoystick;
	}

	WJoystick.controller = NULL;
	SDL_GameControllerClose(WJoystick.controller);

	Joysticks.erase(std::to_string(ID));

	return WindowSuccess;
}

std::string JScene::Window::generateTextTextureID(std::string Text, int X, int Y, int W, int H, double A) {
	return Text + std::to_string(X) + std::to_string(Y) + std::to_string(W) + std::to_string(H) + std::to_string(A);
}
std::string JScene::Window::generateTextTextureID(Text* Text) {
	return generateTextTextureID(Text->text, Text->size.x, Text->size.y, Text->size.w, Text->size.h, Text->angle);
}

/***********************************************************************************\
|*   Window - Public
\***********************************************************************************/
JScene::Window::Window(std::string Title, int Flags, int FPS) {
	Window(Title, NULL, NULL, Flags, PreloadedColors.white, FPS);
}
JScene::Window::Window(std::string Title, int Width, int Height, int Flags, SDL_Color BackgroundColor, int FPS) {
	Properties.title = Title;
	Properties.width = Width;
	Properties.height = Height;
	Properties.flags = Flags;
	Properties.backgroundcolor = BackgroundColor;
	Properties.fpslimit = FPS;
	Properties.drawmode = DrawMode::TopLeft;
	Properties.id = (rand() % 999999);
	
	Font.fontsize = 10;
	Font.fontcolor = WColor.black;

	TimeLast = std::chrono::system_clock::now();
	TimeNow = std::chrono::system_clock::now();

	InputKeysToIgnore = { SDL_SCANCODE_RETURN, SDL_SCANCODE_ESCAPE, SDL_SCANCODE_TAB, SDL_SCANCODE_CAPSLOCK, SDL_SCANCODE_LSHIFT, SDL_SCANCODE_RSHIFT };
	for (int key = SDL_SCANCODE_F1; key <= SDL_SCANCODE_UP; key++) InputKeysToIgnore.push_back(key);
	
	WINDOW_RECT = { 0, 0, Width, Height };
}
void JScene::Window::PRINT_WINDOW_UPDATE(std::string Error) {
	if (PrintErrors) {
		SDL_Log("%s\n", Error.c_str());
	}
}
std::string JScene::Window::GET_LAST_ERROR() {
	return std::to_string(LastError);
}

bool JScene::Window::start() {
	return init(Properties.title, Properties.width, Properties.height, Properties.flags, Properties.backgroundcolor);
}
void JScene::Window::close() {
	WindowDone = true;
}
void JScene::Window::fullscreen(int Type) {
	SDL_RenderSetLogicalSize(SDLRenderer, Properties.width, Properties.height);
	
	if (Type == SDL_WINDOW_FULLSCREEN_DESKTOP) {
		int display_index = SDL_GetWindowDisplayIndex(SDLWindow);
		if (display_index < 0) {
			PRINT_WINDOW_UPDATE("SDLError: Error getting window display");
			return;
		}

		SDL_Rect usable_bounds;
		if (0 != SDL_GetDisplayUsableBounds(display_index, &usable_bounds)) {
			PRINT_WINDOW_UPDATE("SDLError: Error getting usable bounds");
			return;
		}
		
		SDL_SetWindowFullscreen(SDLWindow, NULL);
		//SDL_SetWindowPosition(SDLWindow, usable_bounds.x, usable_bounds.y);
		//SDL_SetWindowSize(SDLWindow, usable_bounds.w, usable_bounds.h);
	} else if (Type == SDL_WINDOW_FULLSCREEN) {
		SDL_SetWindowFullscreen(SDLWindow, Type);
	} else if (Type == NULL) {
		SDL_SetWindowFullscreen(SDLWindow, Type);
	}
}
void JScene::Window::clear() {
	if (SDLWindow == NULL) {
		PRINT_WINDOW_UPDATE("WindowFatalError: Cannot clear window - Window is NULL.");
		exit(1);
	} else if (SDLRenderer == NULL) {
		PRINT_WINDOW_UPDATE("WindowFatalError: Cannot clear window - Renderer is NULL.");
		exit(1);
	}

	//SDL_FillRect(SDLSurface, NULL, 0x000000);
	SDL_SetRenderDrawColor(SDLRenderer, Properties.backgroundcolor.r, Properties.backgroundcolor.g, Properties.backgroundcolor.b, 255);
	SDL_RenderClear(SDLRenderer);
}
void JScene::Window::load() {
	if (SDLWindow == NULL) {
		PRINT_WINDOW_UPDATE("WindowFatalError: Cannot load window - Window is NULL.");
		exit(1);
	} else if (SDLRenderer == NULL) {
		PRINT_WINDOW_UPDATE("WindowFatalError: Cannot load window - Renderer is NULL.");
		exit(1);
	}

	SDL_RenderPresent(SDLRenderer);
	Properties.fpsframe++;
}
bool JScene::Window::updateEvents() {
	if (WindowDone) {
		stop(1);
		return false;
	}

	updateMouse();
	updateButtons();
	updateKeyboard();
	updateJoysticks();
	
	TimeNow = std::chrono::system_clock::now();
	TimeDelta = TimeNow - TimeLast;
	
	Properties.windowfpsframe++;

	return handlePolledEvents();
}
void JScene::Window::yield() {
	while (TimeDelta.count() * 1000.0 < 1000.0 / Properties.fpslimit) {
		std::this_thread::yield();

		TimeNow = std::chrono::system_clock::now();
		TimeDelta = TimeNow - TimeLast;
	}

	TimeLast = TimeNow;
}

void JScene::Window::aclear() {
	// Frame ready
	if (TimeDelta.count() * 1000.0 >= 1000.0 / Properties.fpslimit) {
		ASyncReady = true;
		
		clear();
	}
}
void JScene::Window::aload() {
	if (ASyncReady) {
		ASyncReady = false;
		TimeLast = TimeNow;

		load();
	}
}

void JScene::Window::showErrors() {
	PrintErrors = false;
}
void JScene::Window::hideErrors() {
	PrintErrors = true;
}
void JScene::Window::hideConsole() {
	#ifdef _WIN32
		HWND hwnd = GetConsoleWindow();
		ShowWindow(hwnd, SW_HIDE);
	#endif
}
void JScene::Window::showConsole() {
	#ifdef _WIN32
		HWND hwnd = GetConsoleWindow();
		ShowWindow(hwnd, SW_SHOW);
	#endif
}
bool JScene::Window::canPrintUpdates() {
	return PrintErrors;
}

void JScene::Window::trackFPS() {
	if (RunFPSThread) {
		PRINT_WINDOW_UPDATE("WindowError: FPS thread Already running");
		return;
	}

	RunFPSThread = true;

	std::thread(&JScene::Window::calculateFPS, this).detach();
}
void JScene::Window::killFPSThread() {
	if (!RunFPSThread) {
		PRINT_WINDOW_UPDATE("WindowWarning: FPS Thread already disabled");
	}

	RunFPSThread = false;
}
int JScene::Window::getFPS() {
	return Properties.fps;
}
int JScene::Window::getWindowFPS() {
	return Properties.windowfps;
}

int JScene::Window::alignMiddleX(int Width) {
	return (Properties.width / 2) - (Width / 2);
}
int JScene::Window::alighMiddleY(int Height) {
	return (Properties.height / 2) - (Height / 2);
}

int JScene::Window::addFont(std::string Name) {
	return addFont(Name, 100);
}
int JScene::Window::addFont(std::string Name, int SizeLimit) {
	if (FontLocationMap.find(Name) != FontLocationMap.end()) {
		PRINT_WINDOW_UPDATE("WindowError: Can't add font " + Name + " - Already added");
		return FontExists;
	}
	#ifndef __ANDROID__ 
	else if (!fileExists(Name)) {
		PRINT_WINDOW_UPDATE("WindowError: Can't add invalid font - " + Name);
		return InvalidFont;
	}
	#endif

	std::vector<TTF_Font*> TempFont;
	for (int size = 1; size <= SizeLimit; size++) {
		TempFont.push_back(TTF_OpenFont(Name.c_str(), size));
	}

	FontLocationMap[Name] = (int)Fonts.size();
	Fonts.push_back(TempFont);

	return WindowSuccess;
}
int JScene::Window::setFont(std::string NewFont) {
	return setFont(NewFont, Font.fontsize, Font.fontcolor);
}
int JScene::Window::setFont(std::string NewFont, int Size, SDL_Color FontColor) {
	if (FontLocationMap.find(NewFont) == FontLocationMap.end()) {
		PRINT_WINDOW_UPDATE("WindowError: Invalid font - " + NewFont);
		return InvalidFont;
	} else if (Size > Fonts[FontLocationMap[NewFont]].size()) {
		PRINT_WINDOW_UPDATE("WindowError: Can't set font " + NewFont + " - Intended size too large: " + std::to_string(Size));
		return InvalidFontSize;
	}

	Font.font = NewFont;
	Font.fontcolor = FontColor;
	Font.fontsize = Size;

	return WindowSuccess;
}
int JScene::Window::setFontSize(int Size) {
	return setFont(Font.font, Size, Font.fontcolor);
}
void JScene::Window::setFontColor(SDL_Color FontColor) {
	Font.fontcolor = FontColor;
}
JScene::Text* JScene::Window::addTextTexture(std::string TextID, std::string Str, int X, int Y, int Width, int Height, double Angle, TextType Fill, TextType Fit, int Buffer) {
	if (Texts.find(TextID) != Texts.end()) {
		PRINT_WINDOW_UPDATE("WindowError: Can't add text texture - " + TextID + " - Already added");
		return &NULLTEXTTEXTURE;
	}
	
	int TextWidth, TextHeight;
	SDL_Rect Rect;

	SDL_Surface* TextSurface = TTF_RenderText_Solid(Fonts.at(FontLocationMap[Font.font]).at(Font.fontsize - 1), Str.c_str(), Font.fontcolor);
	SDL_Texture* TextTexture = SDL_CreateTextureFromSurface(SDLRenderer, TextSurface);
	SDL_QueryTexture(TextTexture, NULL, NULL, &TextWidth, &TextHeight);
	
	if (Fill == TextType::FillX || Fill == TextType::FillY) {
		if (Buffer == NULL && TextWidth > Width) Buffer = 1;
		else if (Buffer == NULL && TextWidth < Width) Buffer = -1;
	}
	
	switch (Fill) {
		case TextType::FillX: {
			if (TextWidth > Width && Buffer == 1) {
				int TempFontSize = Font.fontsize;
				setFontSize(Font.fontsize - 1);

				Text* chk = addTextTexture(TextID, Str, X, Y, Width, Height, Angle, Fill, Fit, 1);
				setFontSize(TempFontSize);

				SDL_FreeSurface(TextSurface);
				SDL_DestroyTexture(TextTexture);

				return chk;
			} else if (TextWidth < Width && Buffer == -1) {
				int TempFontSize = Font.fontsize;
				setFontSize(Font.fontsize + 1);

				Text* chk = addTextTexture(TextID, Str, X, Y, Width, Height, Angle, Fill, Fit, -1);
				setFontSize(TempFontSize);

				SDL_FreeSurface(TextSurface);
				SDL_DestroyTexture(TextTexture);

				return chk;
			}
			break;
		}
		case TextType::FillY: {
			if (TextHeight > Height && Buffer == 1) {
				int TempFontSize = Font.fontsize;
				setFontSize(Font.fontsize - 1);
				
				Text* chk = addTextTexture(TextID, Str, X, Y, Width, Height, Angle, Fill, Fit, 1);
				setFontSize(TempFontSize);

				SDL_FreeSurface(TextSurface);
				SDL_DestroyTexture(TextTexture);

				return chk;
			} else if (TextHeight < Height && Buffer == -1) {
				int TempFontSize = Font.fontsize;
				setFontSize(Font.fontsize + 1);

				Text* chk = addTextTexture(TextID, Str, X, Y, Width, Height, Angle, Fill, Fit, -1);
				setFontSize(TempFontSize);

				SDL_FreeSurface(TextSurface);
				SDL_DestroyTexture(TextTexture);

				return chk;
			}
			break;
		}
		case TextType::ScrollX: {
			if (TextWidth > Width) {
				Text* chk = addTextTexture(TextID, Str.substr(1), X, Y, Width, Height, Angle, Fill, Fit, 1);

				SDL_FreeSurface(TextSurface);
				SDL_DestroyTexture(TextTexture);

				return chk;
			}

			break;
		}
		case TextType::Wrap: {
			if (TextWidth > Width) {
				printf("Please tell the developer this need to be ");
			}
			break;
		}	
		case TextType::Default: break;
	}
	Rect = { X, Y, TextWidth, TextHeight }; 

	if (Fit == TextType::Fit || Fit == TextType::FitX) Rect.x += (abs(Width - TextWidth) / 2);
	if (Fit == TextType::Fit || Fit == TextType::FitY) Rect.y += (abs(Height - TextHeight) / 2);
	
	SDL_FreeSurface(TextSurface);

	Text NewText;
	NewText.texture = TextTexture;
	NewText.id = TextID;
	NewText.text = Str;
	NewText.size = Rect;
	NewText.angle = Angle;

	Texts[TextID] = NewText;
	
	return &Texts[TextID];
}
JScene::Text* JScene::Window::getTextTexture(std::string TextID) {
	if (Texts.find(TextID) == Texts.end()) {
		PRINT_WINDOW_UPDATE("WindowError: Can't get invalid text texture - " + TextID);
		return &NULLTEXT;
	}

	return &Texts[TextID];
}
int JScene::Window::writeText(std::string Text, int X, int Y) {
	return writeText(NULL, Text, X, Y, NULL, NULL, 0.0, TextType::Default, TextType::FitNone, NULL);
}
int JScene::Window::writeText(std::string Text, int X, int Y, double Angle) {
	return writeText(NULL, Text, X, Y, NULL, NULL, Angle, TextType::Default, TextType::FitNone, NULL);
}
int JScene::Window::writeText(std::string Text, int X, int Y, int Width, int Height) {
	return writeText(NULL, Text, X, Y, Width, Height, 0.0, TextType::Default, TextType::FitNone, NULL);
}
int JScene::Window::writeText(std::string Text, int X, int Y, int Width, int Height, double Angle) {
	return writeText(NULL, Text, X, Y, Width, Height, Angle, TextType::Default, TextType::FitNone, NULL);
}
int JScene::Window::writeText(std::string Text, int X, int Y, int Width, int Height, TextType Fill, TextType Fit) {
	return writeText(NULL, Text, X, Y, Width, Height, 0.0, Fill, Fit, NULL);
}
int JScene::Window::writeText(std::string Text, int X, int Y, int Width, int Height, double Angle, TextType Fill, TextType Fit) {
	return writeText(NULL, Text, X, Y, Width, Height, Angle, Fill, Fit, NULL);
}
int JScene::Window::writeText(std::string Text, SDL_Rect Size) {
	return writeText(NULL, Text, Size.x, Size.y, Size.w, Size.h, 0.0, TextType::Default, TextType::FitNone, NULL);
}
int JScene::Window::writeText(std::string Text, SDL_Rect Size, double Angle) {
	return writeText(NULL, Text, Size.x, Size.y, Size.w, Size.h, Angle, TextType::Default, TextType::FitNone, NULL);
}
int JScene::Window::writeText(std::string Text, SDL_Rect Size, TextType Fill, TextType Fit) {
	return writeText(NULL, Text, Size.x, Size.y, Size.w, Size.h, 0.0, Fill, Fit, NULL);
}
int JScene::Window::writeText(std::string Text, SDL_Rect Size, double Angle, TextType Fill, TextType Fit) {
	return writeText(NULL, Text, Size.x, Size.y, Size.w, Size.h, Angle, Fill, Fit, NULL);
}
int JScene::Window::writeText(std::string Text, int X, int Y, int Width, int Height, double Angle, TextType Fill, TextType Fit, int Buffer) {
	return writeText(NULL, Text, X, Y, Width, Height, Angle, Fill, Fit, Buffer);
}
// TODO
// - Dump unused textures
// - Update text generation
//   * Find better memory management for texture with same text but different positions
//   * Textures can move or be written more than once
//   * Maybe some sort of texture garbage collector for un-reused textures
int JScene::Window::writeText(Text* Text, std::string StrText, int X, int Y, int Width, int Height, double Angle, TextType Fill, TextType Fit, int Buffer) {
	std::string textid = generateTextTextureID(StrText, X, Y, Width, Height, Angle);

	if (Text == NULL) {
		bool prerr = PrintErrors; PrintErrors = false;
		Text = getTextTexture(textid);
		PrintErrors = true;

		if (Text->texture == NULL) {
			if (FontLocationMap.find(Font.font) == FontLocationMap.end()) {
				PRINT_WINDOW_UPDATE("WindowError: Can't write text - Invalid font - " + Font.font);
				return InvalidFont;
			} else if (Font.fontsize < 1) {
				PRINT_WINDOW_UPDATE("WindowError: Can't write text - Font size too small.");
				return FontTooSmall;
			} else if (Fonts.size() < 0){
				PRINT_WINDOW_UPDATE("WindowError: Can't write text - No fonts loaded.");
				return NoFontsLoaded;
			} else if (Fonts.at(FontLocationMap[Font.font]).at(Font.fontsize - 1) == NULL) {
				PRINT_WINDOW_UPDATE("WindowError: Can't write text - " + Font.font + " doesn't contain size " + std::to_string(Font.fontsize));
				return InvalidFontSize;
			}

			Text = addTextTexture(textid, StrText, X, Y, Width, Height, Angle, Fill, Fit, Buffer);
		}
	}

	SDL_RenderCopyEx(SDLRenderer, Text->texture, NULL, &Text->size, Angle, NULL, SDL_FLIP_NONE);

	return WindowSuccess;
}
int JScene::Window::writeTextDynamic(std::string Text, SDL_Rect Size, double Angle) {
	if (FontLocationMap.find(Font.font) == FontLocationMap.end()) {
		PRINT_WINDOW_UPDATE("WindowError: Can't write text - Invalid font - " + Font.font);
		return InvalidFont;
	} else if (Font.fontsize < 1) {
		PRINT_WINDOW_UPDATE("WindowError: Can't write text - Font size too small.");
		return FontTooSmall;
	} else if (Fonts.size() < 0){
		PRINT_WINDOW_UPDATE("WindowError: Can't write text - No fonts loaded.");
		return NoFontsLoaded;
	} else if (Fonts.at(FontLocationMap[Font.font]).at(Font.fontsize - 1) == NULL) {
		PRINT_WINDOW_UPDATE("WindowError: Can't write text - " + Font.font + " doesn't contain size " + std::to_string(Font.fontsize));
		return InvalidFontSize;
	}

	int TextWidth;
	int TextHeight;
	SDL_Surface* TextSurface = TTF_RenderText_Solid(Fonts.at(FontLocationMap[Font.font]).at(Font.fontsize - 1), Text.c_str(), Font.fontcolor); 
	SDL_Texture* TextTexture = SDL_CreateTextureFromSurface(SDLRenderer, TextSurface);;
	SDL_QueryTexture(TextTexture, NULL, NULL, &TextWidth, &TextHeight);

	Size = { Size.x, Size.y, Size.w != NULL ? Size.w : TextWidth, Size.h != NULL ? Size.h : TextHeight };

	SDL_RenderCopyEx(SDLRenderer, TextTexture, NULL, &Size, Angle, NULL, SDL_FLIP_NONE);
	SDL_FreeSurface(TextSurface);
	SDL_DestroyTexture(TextTexture);

	return WindowSuccess;
}

int JScene::Window::getTextTextureWidth(std::string Text) {
	int TextSize[2];
	
	queryTextTextureSize(TextSize, Text, Font.fontsize);
	
	return TextSize[0];
}
int JScene::Window::getTextTextureWidth(std::string Text, int FontSize) {
	int TextSize[2];
	
	queryTextTextureSize(TextSize, Text, FontSize);
	
	return TextSize[0];
}
int JScene::Window::getTextTextureHeight(std::string Text) {
	int TextSize[2];
	
	queryTextTextureSize(TextSize, Text, Font.fontsize);
	
	return TextSize[1];
}
int JScene::Window::getTextTextureHeight(std::string Text, int FontSize) {
	int TextSize[2];
	
	queryTextTextureSize(TextSize, Text, FontSize);
	
	return TextSize[1];
}
void JScene::Window::queryTextTextureSize(int* TextSize, std::string Text) {
	queryTextTextureSize(TextSize, Text, Font.fontsize);
}
void JScene::Window::queryTextTextureSize(int* TextSize, std::string Text, int FontSize) {
	// Generate surface and texture
	SDL_Surface* TextSurface = TTF_RenderText_Solid(Fonts.at(FontLocationMap[Font.font]).at(FontSize), Text.c_str(), Font.fontcolor);
	SDL_Texture* TextTexture = SDL_CreateTextureFromSurface(SDLRenderer, TextSurface);

	// Query for text size
	SDL_QueryTexture(TextTexture, NULL, NULL, &TextSize[0], &TextSize[1]);

	// Cleanup
	SDL_FreeSurface(TextSurface);
	SDL_DestroyTexture(TextTexture);
}

JScene::KeyState JScene::Window::getKeyState(int Key) {
	if (KeyboardState[Key]) {
		return KeyState::Down;
	} else if (findInVector(&Keyboard.clickedkeys, Key)) {
		return KeyState::Clicked;
	} else {
		return KeyState::Up;
	}
}
bool JScene::Window::checkInput(std::string Key, KeyState Expected, bool IncludeJoysticks) {
	if (getKeyState(SDL_GetScancodeFromName(Key.c_str())) == Expected) return true;

	if (IncludeJoysticks) {
		for (auto const& joystick : Joysticks) {
			Joystick* wj = &Joysticks[joystick.first];
			if (wj->jmap.find(Key) != wj->jmap.end())
				if (wj->checkInput(Key, Expected)) 
					return true;
		}
	}

	return false;
}
bool JScene::Window::leftClickedAt(int X, int Y) {
	return Mouse.click[0] && Mouse.x == X && Mouse.y == Y;
}
bool JScene::Window::leftClickedAt(int X[], int Y[]) {
	return Mouse.click[0] && Mouse.x >= X[0] && Mouse.x <= X[1] && Mouse.y >= Y[0] && Mouse.y <= Y[1];
}
bool JScene::Window::leftClickedAt(SDL_Rect Size) {
	return Mouse.click[0] && Mouse.x >= Size.x && Mouse.x <= Size.x + Size.w && Mouse.y >= Size.y && Mouse.y <= Size.y + Size.h;
}
bool JScene::Window::rightClickedAt(int X, int Y) {
	return Mouse.click[1] && Mouse.x == X && Mouse.y == Y;
}  
bool JScene::Window::rightClickedAt(int X[], int Y[]) {
	return Mouse.click[1] && Mouse.x >= X[0] && Mouse.x <= X[1] && Mouse.y >= Y[0] && Mouse.y <= Y[1];
}
bool JScene::Window::rightClickedAt(SDL_Rect Size) {
	return Mouse.click[1] && Mouse.x >= Size.x && Mouse.x <= Size.x + Size.w && Mouse.y >= Size.y && Mouse.y <= Size.y + Size.y;
}
bool JScene::Window::middleClickedAt(int X, int Y) {
	return Mouse.click[2] && Mouse.x == X && Mouse.y == Y;
}
bool JScene::Window::middleClickedAt(int X[], int Y[]) {
	return Mouse.click[2] && Mouse.x >= X[0] && Mouse.x <= X[1] && Mouse.y >= Y[0] && Mouse.y <= Y[1];
}
bool JScene::Window::middleClickedAt(SDL_Rect Size) {
	return Mouse.click[2] && Mouse.x >= Size.x && Mouse.x <= Size.x + Size.w && Mouse.y >= Size.y && Mouse.y <= Size.y + Size.y;
}
bool JScene::Window::leftPressedAt(SDL_Rect Size) {
	return Mouse.down[0] && Mouse.x >= Size.x && Mouse.x <= Size.x + Size.w && Mouse.y >= Size.y && Mouse.y <= Size.y + Size.h;
}
bool JScene::Window::rightPressedAt(SDL_Rect Size) {
	return Mouse.down[1] && Mouse.x >= Size.x && Mouse.x <= Size.x + Size.w && Mouse.y >= Size.y && Mouse.y <= Size.y + Size.h;
}
bool JScene::Window::middlePressedAt(SDL_Rect Size){
	return Mouse.click[2] && Mouse.x >= Size.x && Mouse.x <= Size.x + Size.w && Mouse.y >= Size.y && Mouse.y <= Size.y + Size.h;
}
bool JScene::Window::fingerTappedAt(SDL_Rect Size) {
	return Mouse.click[3] && Mouse.x >= Size.x && Mouse.x <= Size.x + Size.w && Mouse.y >= Size.y && Mouse.y <= Size.y + Size.h;
}
bool JScene::Window::fingerPressedAt(SDL_Rect Size) {
	return Mouse.down[3] && Mouse.x >= Size.x && Mouse.x <= Size.x + Size.w && Mouse.y >= Size.y && Mouse.y <= Size.y + Size.h;
}
bool JScene::Window::fingerInteractedAt(SDL_Rect Size) {
	return fingerPressedAt(Size) || fingerTappedAt(Size);
}

template<typename T> int JScene::Window::adjustObjectSizeFromDrawMode(T *Object, JScene::DrawMode DrawMode) {
	if (Object == NULL) {
		#ifdef _WIN32
			PRINT_WINDOW_UPDATE("WindowError: Invalid Object - " + std::string(typeid(Object).raw_name()));
		#elif __linux__
			PRINT_WINDOW_UPDATE("WindowError: Invalid Object - [Placeholder for object name] - Please notify developer or just figure it out");
		#endif
		return InvalidObject;
	}

	switch (DrawMode) {
		case DrawMode::TopLeft: 
		case DrawMode::Default: 
			break;
		case DrawMode::Center: 
			Object->size = { Object->size.x - Object->size.w / 2, Object->size.y - Object->size.h / 2, Object->size.w, Object->size.h }; 
			break;
		case DrawMode::BottomLeft: 
			Object->size = { Object->size.x, Object->size.y - Object->size.h, Object->size.w, Object->size.h }; 
			break;
		case DrawMode::BottomRight: 
			Object->size = { Object->size.x - Object->size.w, Object->size.y - Object->size.h, Object->size.w, Object->size.h }; 
			break;
		case DrawMode::TopRight: 
			Object->size = { Object->size.x - Object->size.w, Object->size.y, Object->size.w, Object->size.h }; 
			break;
		default: break;
	}

	return WindowSuccess;
}
void JScene::Window::setDrawMode(DrawMode NewMode) {
	Properties.drawmode = NewMode;
}
void JScene::Window::setColor(SDL_Color Color) {
	SDL_SetRenderDrawColor(SDLRenderer, Color.r, Color.g, Color.b, Color.a);
}
void JScene::Window::setColor(SDL_Color Color, int Alpha) {
	SDL_SetRenderDrawColor(SDLRenderer, Color.r, Color.g, Color.b, Alpha);
}
void JScene::Window::drawSolidRectangle(SDL_Rect Size, SDL_Color Color) {
	drawGeometry(Size, Color, Geometry::Fill);
}
void JScene::Window::drawSolidRectangle(SDL_Rect Size, SDL_Color Color, int Alpha) {
	drawGeometry(Size, Color, Geometry::Fill, Alpha);
}
void JScene::Window::drawHollowRectangle(SDL_Rect Size, SDL_Color Color) {
	drawGeometry(Size, Color, Geometry::Outline);
}
void JScene::Window::drawHollowRectangle(SDL_Rect Size, SDL_Color Color, int Alpha) {
	drawGeometry(Size, Color, Geometry::Outline, Alpha);
}
void JScene::Window::drawLine(int X1, int Y1, int X2, int Y2, SDL_Color Color) {
	drawGeometry({ X1, Y1, X2, Y2 }, Color, Geometry::Line);
}
void JScene::Window::drawLine(int X1, int Y1, int X2, int Y2, SDL_Color Color, int Alpha) {
	drawGeometry({ X1, Y1, X2, Y2 }, Color, Geometry::Line, Alpha);
}
void JScene::Window::drawLine(SDL_Rect Size, SDL_Color Color) {
	drawGeometry(Size, Color, Geometry::Line);
}
void JScene::Window::drawLine(SDL_Rect Size, SDL_Color Color, int Alpha) {
	drawGeometry(Size, Color, Geometry::Line, Alpha);
}
void JScene::Window::drawPoint(int X, int Y, SDL_Color Color) {
	drawGeometry({ X, Y, NULL, NULL }, Color, Geometry::Point);
}
void JScene::Window::drawPoint(int X, int Y, SDL_Color Color, int Alpha) {
	drawGeometry({ X, Y, NULL, NULL }, Color, Geometry::Point, Alpha);
}

JScene::Image* JScene::Window::addImage(std::string Address) {
	if (Images.find(Address) != Images.end()) return getImage(Address);

	Image NewImage;
	NewImage.address = Address;
	std::pair<SDL_Surface*, SDL_Texture*> surfacetexture = loadSurfaceTexture(Address);
	NewImage.surface = surfacetexture.first;
	NewImage.texture = surfacetexture.second;
	SDL_QueryTexture(NewImage.texture, NULL, NULL, &NewImage.size.w, &NewImage.size.h);

	Images[Address] = NewImage;

	return &Images[Address];
}
JScene::Image* JScene::Window::getImage(std::string Address) {
	if (Images.find(Address) == Images.end()) return &NULLIMAGE;

	return &Images[Address];
}
int JScene::Window::drawImage(std::string Address, int X, int Y) {
	return drawImage(Address, X, Y, NULL, NULL, 0.0, SDL_FLIP_NONE, 255);
}
int JScene::Window::drawImage(std::string Address, int X, int Y, Uint8 Transparency) {
	return drawImage(Address, X, Y, NULL, NULL, 0.0, SDL_FLIP_NONE, Transparency);
}
int JScene::Window::drawImage(std::string Address, int X, int Y, double Angle) {
	return drawImage(Address, X, Y, NULL, NULL, Angle, SDL_FLIP_NONE, 255);
}
int JScene::Window::drawImage(std::string Address, int X, int Y, double Angle, Uint8 Transparency) {
	return drawImage(Address, X, Y, NULL, NULL, Angle, SDL_FLIP_NONE, Transparency);
}
int JScene::Window::drawImage(std::string Address, int X, int Y, int Width, int Height) {
	return drawImage(Address, X, Y, Width, Height, 0.0, SDL_FLIP_NONE, 255);
}
int JScene::Window::drawImage(std::string Address, int X, int Y, int Width, int Height, Uint8 Transparency) {
	return drawImage(Address, X, Y, Width, Height, 0.0, SDL_FLIP_NONE, Transparency);
}
int JScene::Window::drawImage(std::string Address, int X, int Y, int Width, int Height, double Angle) {
	return drawImage(Address, X, Y, Width, Height, Angle, SDL_FLIP_NONE, 255);
}
int JScene::Window::drawImage(std::string Address, int X, int Y, int Width, int Height, double Angle, Uint8 Transparency) {
	return drawImage(Address, X, Y, Width, Height, Angle, SDL_FLIP_NONE, Transparency);
}
int JScene::Window::drawImage(std::string Address, SDL_Rect Size) {
	return drawImage(Address, Size.x, Size.y, Size.w, Size.h, 0.0, SDL_FLIP_NONE, 255, NULLRECT);
}
int JScene::Window::drawImage(std::string Address, SDL_Rect Size, Uint8 Transparency) {
	return drawImage(Address, Size.x, Size.y, Size.w, Size.h, 0.0, SDL_FLIP_NONE, Transparency);
}
int JScene::Window::drawImage(std::string Address, SDL_Rect Size, double Angle) {
	return drawImage(Address, Size.x, Size.y, Size.w, Size.h, Angle, SDL_FLIP_NONE, 255);
}
int JScene::Window::drawImage(std::string Address, SDL_Rect Size, double Angle, Uint8 Transparency) {
	return drawImage(Address, Size.x, Size.y, Size.w, Size.h, Angle, SDL_FLIP_NONE, Transparency, NULLRECT);
}
int JScene::Window::drawImageFlipped(std::string Address, int X, int Y) {
	return drawImage(Address, X, Y, NULL, NULL, 0.0, SDL_FLIP_HORIZONTAL, 255);
}
int JScene::Window::drawImageFlipped(std::string Address, int X, int Y, Uint8 Transparency) {
	return drawImage(Address, X, Y, NULL, NULL, 0.0, SDL_FLIP_HORIZONTAL, Transparency);
}
int JScene::Window::drawImageFlipped(std::string Address, int X, int Y, double Angle) {
	return drawImage(Address, X, Y, NULL, NULL, Angle, SDL_FLIP_HORIZONTAL, 255);
}
int JScene::Window::drawImageFlipped(std::string Address, int X, int Y, double Angle, Uint8 Transparency) {
	return drawImage(Address, X, Y, NULL, NULL, Angle, SDL_FLIP_HORIZONTAL, Transparency);
}
int JScene::Window::drawImageFlipped(std::string Address, int X, int Y, int Width, int Height) {
	return drawImage(Address, X, Y, Width, Height, 0.0, SDL_FLIP_HORIZONTAL, 255);
}
int JScene::Window::drawImageFlipped(std::string Address, int X, int Y, int Width, int Height, Uint8 Transparency) {
	return drawImage(Address, X, Y, Width, Height, 0.0, SDL_FLIP_HORIZONTAL, Transparency);
}
int JScene::Window::drawImageFlipped(std::string Address, int X, int Y, int Width, int Height, double Angle) {
	return drawImage(Address, X, Y, Width, Height, Angle, SDL_FLIP_HORIZONTAL, 255);
}
int JScene::Window::drawImageFlipped(std::string Address, int X, int Y, int Width, int Height, double Angle, Uint8 Transparency) {
	return drawImage(Address, X, Y, Width, Height, Angle, SDL_FLIP_HORIZONTAL, Transparency);
}
int JScene::Window::drawImageFlipped(std::string Address, SDL_Rect Size) {
	return drawImage(Address, Size.x, Size.y, Size.w, Size.h, 0.0, SDL_FLIP_HORIZONTAL, 255);
}
int JScene::Window::drawImageFlipped(std::string Address, SDL_Rect Size, Uint8 Transparency) {
	return drawImage(Address, Size.x, Size.y, Size.w, Size.h, 0.0, SDL_FLIP_HORIZONTAL, Transparency);
}
int JScene::Window::drawImageFlipped(std::string Address, SDL_Rect Size, double Angle) {
	return drawImage(Address, Size.x, Size.y, Size.w, Size.h, Angle, SDL_FLIP_HORIZONTAL);
}
int JScene::Window::drawImageFlipped(std::string Address, SDL_Rect Size, double Angle, Uint8 Transparency) {
	return drawImage(Address, Size.x, Size.y, Size.w, Size.h, Angle, SDL_FLIP_HORIZONTAL, Transparency);
}
int JScene::Window::drawImage(std::string Address, int X, int Y, int Width, int Height, double Angle, int Flip, Uint8 Transparency) {
	return drawImage(Address, X, Y, Width, Height, Angle, (SDL_RendererFlip)Flip, Transparency, NULLRECT);
}
int JScene::Window::drawImage(std::string Address, int X, int Y, int Width, int Height, double Angle, int Flip, Uint8 Transparency, SDL_Rect Zoom) {
	if (Images.find(Address) == Images.end()) {
		PRINT_WINDOW_UPDATE("WindowError: Image hasn't been loaded yet - " + Address);
		return InvalidImage;
	}

	Image* WImage = &Images[Address];
	Width = Width == NULL ? WImage->size.w : Width;
	Height = Height == NULL ? WImage->size.h : Height;

	SDL_Rect Rect = { X, Y, Width, Height };

	adjustObjectSizeFromDrawMode(WImage, Properties.drawmode);

	SDL_SetTextureAlphaMod(WImage->texture, Transparency);
	render(WImage->texture, (Zoom.x == -1 && Zoom.y == -1 && Zoom.w == -1 && Zoom.h == -1) ? NULL : &Zoom, &Rect, Angle, NULL, Flip);

	return WindowSuccess;
}
int JScene::Window::getTotalImages() {
	return TotalImages;
}
// TODO
// - Fix for android/ios
int JScene::Window::countImagesInDirectory(std::string Directory) {
	int count = 0;
#ifdef __ANDROID__
	DIR* dir;
	struct dirent* ent;

	if ((dir = opendir(Directory.c_str())) != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			count++;
		}

		closedir(dir);
	} else {
		PRINT_WINDOW_UPDATE("Error: Failed to open directory to load images");
	}
#else
	for (auto& Item : fs::directory_iterator(Directory)) {
		std::string address = fs::path(Item).u8string();
		if (fs::is_directory(Item)) {
			count += countImagesInDirectory(address);
		} else {
			count++;
		}
	}

	TotalImages = count;

	return count;
#endif
}
int JScene::Window::countImagesInDirectory(std::vector<std::string> Directories) {
	int count = 0;
	
	for (std::string directory : Directories) count += countImagesInDirectory(directory);

	// Store result
	TotalImages = count;

	return count;
}
void JScene::Window::loadImagesFromDirectory(std::string Directory, ImageLoadFunction CounterFunction, int* Counter) {
#ifdef __ANDROID__	
	PRINT_WINDOW_UPDATE("This doesn't work. Load images with addImage()");
#else	
	int n = 0;
	if (Counter == NULL) Counter = &n;

	for (auto& Item : fs::directory_iterator(Directory)) {
		std::string Address = fs::path(Item).u8string();
		if (fs::is_directory(Item)) {
			loadImagesFromDirectory(Address, CounterFunction, Counter);
		} else {
			addImage(fs::path(Item).u8string());
			if (CounterFunction != NULLLOADFUNCTION) {
				clear();
				CounterFunction(this, Address, *Counter);
				load();
			}

			*Counter = *Counter + 1;
		}
	}
#endif
}
void JScene::Window::loadImagesFromDirectory(std::vector<std::string> Directories, ImageLoadFunction Function) {
	int total = NULL;
	
	for (std::string directory : Directories) {
		loadImagesFromDirectory(directory, Function, &total);
	}
}

std::unordered_map<std::string, JScene::Button>* JScene::Window::getAllButtons() {
	return &Buttons;
}
JScene::Button* JScene::Window::addButton(std::string ButtonID, std::string Text, SDL_Rect Size, SDL_Color Color, SDL_Color TextColor, TextType Texttype, TextType TextFit) {
	return addButton(ButtonID, Text, -1, Size, Color, TextColor, Texttype, TextFit, "", Properties.drawmode);
}
JScene::Button* JScene::Window::addButton(std::string ButtonID, std::string Text, SDL_Rect Size, SDL_Color Color, SDL_Color TextColor, TextType Texttype, TextType TextFit, std::string Texture) {
	return addButton(ButtonID, Text, -1, Size, Color, TextColor, Texttype, TextFit, Texture, Properties.drawmode);
}
JScene::Button* JScene::Window::addButton(std::string ButtonID, std::string Text, int FontSize, SDL_Rect Size, SDL_Color Color, SDL_Color TextColor, TextType Texttype, TextType TextFit, std::string Texture) {
	return addButton(ButtonID, Text, FontSize, Size, Color, TextColor, Texttype, TextFit, Texture, Properties.drawmode);
}
JScene::Button* JScene::Window::addButton(std::string ButtonID, std::string Text, int FontSize, SDL_Rect Size, SDL_Color Color, SDL_Color TextColor, TextType Texttype, TextType TextFit) {
	return addButton(ButtonID, Text, FontSize, Size, Color, TextColor, Texttype, TextFit, "", Properties.drawmode);
}
JScene::Button* JScene::Window::addButton(std::string ButtonID, std::string Text, int FontSize, SDL_Rect Size, SDL_Color Color, SDL_Color TextColor, TextType Texttype, TextType TextFit, std::string Texture, DrawMode DrawMode) {
	if (Buttons.find(ButtonID) != Buttons.end()) return getButton(ButtonID);

	Button NewButton;
	NewButton.id = ButtonID;
	NewButton.size = Size;
	NewButton.color = Color;
	NewButton.texttype = Texttype;
	NewButton.textfit = TextFit;
	NewButton.address = Texture;
	NewButton.window = this;

	NewButton.text.text = Text;
	#ifdef _WIN32
		NewButton.text.fontsize = max(FontSize, 0);
	#else
		NewButton.text.fontsize = std::max(FontSize, 0);
	#endif
	NewButton.text.fontcolor = TextColor;
	
	Image* WImg = getImage(Texture);
	if (Texture != "" && WImg == &NULLIMAGE) {
		NewButton.texture = addImage(Texture)->texture;
	} else if (Texture != "") {
		NewButton.texture = Images[Texture].texture;
	}
	
	adjustObjectSizeFromDrawMode(&NewButton, DrawMode);
	
	SelectedButtonMap[(int)Buttons.size()] = ButtonID;
	Buttons[ButtonID] = NewButton;
	
	return &Buttons[ButtonID];
}
JScene::Button* JScene::Window::getButton(std::string Name) {
	if (Buttons.find(Name) == Buttons.end()) {
		PRINT_WINDOW_UPDATE("WindowError: Can't return invalid Button - " + Name);
		return &NULLBUTTON;
	}

	return &Buttons[Name];
}
JScene::Button* JScene::Window::getSelectedButton() {
	if (SelectedButton < 0) return &NULLBUTTON;

	return &Buttons[SelectedButtonMap[SelectedButton]];
}
int JScene::Window::drawButton(std::string Name) {
	Button* WButton = getButton(Name);
	
	if (WButton == &NULLBUTTON) {
		PRINT_WINDOW_UPDATE("WindowError: Attempt to draw invalid button - " + Name);
		return InvalidButton;
	}
	
	if (WButton->address == "" || WButton->texture == NULL) {
		drawGeometry(WButton->size, WButton->color, Geometry::Fill, WButton->color.a);
	} else {
		if (WButton->texture == NULL) {
			WButton->texture = addImage(WButton->address)->texture;
		}

		render(WButton->texture, NULL, &WButton->size, WButton->angle, NULL, NULL);
	}

	if (WButton->text.text.length() > 0) {
		if (WButton->text.fontsize > 0) {
			setFontSize(WButton->text.fontsize);
		}
		
		setFontColor(WButton->text.fontcolor);
		writeText(WButton->text.text, WButton->size, WButton->text.angle, WButton->texttype, WButton->textfit);
	}

	return WindowSuccess;
}
void JScene::Window::drawAllButtons() {
	for (auto const& button: Buttons) drawButton(button.second.id);
}
void JScene::Window::unselectButton() {
	SelectedButton = -1;
}
void JScene::Window::nextButton() {
	if (SelectedButton < 0 && Buttons.size() > 0) {
		SelectedButton = 0;
		return;
	}

	if (SelectedButton < (int)Buttons.size() - 1) {
		SelectedButton++;
	} else {
		SelectedButton = 0;
	}
}
void JScene::Window::previousButton() {
	if (SelectedButton < 0 && Buttons.size() > 0) {
		SelectedButton = 0;
		return;
	}

	if (SelectedButton > 0) {
		SelectedButton--;
	} else {
		SelectedButton = (int)Buttons.size() - 1;
	}
}

std::unordered_map<std::string, JScene::TextBox>* JScene::Window::getAllTextBoxes() {
	return &TextBoxes;
}
JScene::TextBox* JScene::Window::addTextBox(std::string Name, SDL_Rect Size) {
	return addTextBox(Name, Size, WColor.black, WColor.black, Properties.drawmode);
}
JScene::TextBox* JScene::Window::addTextBox(std::string Name, SDL_Rect Size, SDL_Color Outline, SDL_Color TextColor) {
	return addTextBox(Name, Size, Outline, TextColor, Properties.drawmode);
}
JScene::TextBox* JScene::Window::addTextBox(std::string Name, SDL_Rect Size, SDL_Color Outline, SDL_Color TextColor, DrawMode DrawMode) {
	if (TextBoxes.find(Name) != TextBoxes.end()) return getTextBox(Name);
	
	TextBox NewTextBox;
	NewTextBox.name = Name;
	NewTextBox.size = Size;
	NewTextBox.outline = Outline;
	NewTextBox.textcolor = TextColor;
	NewTextBox.window = this;

	adjustObjectSizeFromDrawMode(&NewTextBox, DrawMode);
	
	TextBoxes[Name] = NewTextBox;

	return &TextBoxes[Name];
}
JScene::TextBox* JScene::Window::getTextBox(std::string Name) {
	if (TextBoxes.find(Name) == TextBoxes.end()) return &NULLTEXTBOX;

	return &TextBoxes[Name];
}
JScene::TextBox* JScene::Window::getSelectedTextBox() {
	if (SelectedTextBox == "") return &NULLTEXTBOX;

	return getTextBox(SelectedTextBox);
}
int JScene::Window::drawTextBox(std::string Name) {
	TextBox WTextBox = *getTextBox(Name);
	if (&WTextBox == NULL) {
		PRINT_WINDOW_UPDATE("WindowError: Can't draw invalid textbox - " + Name);
		return InvalidTextBox;
	}

	drawGeometry(WTextBox.size, WTextBox.outline, Geometry::Outline, WTextBox.outline.a);

	if (WTextBox.text.text.length() > 0) {
		setFontColor(WTextBox.textcolor); 
		setFontSize(WTextBox.size.h-2);
		writeText(WTextBox.text.text, { WTextBox.size.x, WTextBox.size.y - 1, WTextBox.size.w, WTextBox.size.h + 2 }, TextType::ScrollX, TextType::FitY);
	}
	return WindowSuccess;
}
void JScene::Window::drawAllTextBoxes() {
	for (auto const& textbox: TextBoxes) drawTextBox(textbox.second.name);
}
void JScene::Window::selectTextBox(std::string Name) {
	SelectedTextBox = Name;
	
}
void JScene::Window::unselectTextBox() {
	SelectedTextBox = "";
}
void JScene::Window::nextTextBox() {
	
}
void JScene::Window::previousTextBox() {

}

std::unordered_map<std::string, JScene::Joystick>* JScene::Window::getAllJoysticks() {
	return &Joysticks;
}
JScene::Joystick* JScene::Window::getJoystick(int ID) {
	if ((int)Joysticks.size() == 0 || (int)Joysticks.size() < ID + 1) {
		PRINT_WINDOW_UPDATE("WindowError: Invalid joystick - " + std::to_string(ID));
		return &NULLJOYSTICK;
	}

	return &Joysticks[std::to_string(ID)];
}
JScene::Joystick* JScene::Window::getJoystickA(int ID) {
	if ((int)Joysticks.size() == 0 || (int)Joysticks.size() < ID + 1) {
		return &NULLJOYSTICK;
	}

	return &Joysticks[std::to_string(ID)];
}
JScene::Joystick* JScene::Window::waitForJoystick(int ID) {
	Joystick* j;
	
	while ((j = getJoystick(ID)) == &NULLJOYSTICK) std::this_thread::yield();

	return j;
}
void JScene::Window::setJoystickAddedHandle(JoystickAddedFunction Function) {
	ControllerAdded = Function;
}

std::unordered_map<std::string, JScene::Audio>* JScene::Window::getAudios() {
	return &Audios;
}
JScene::Audio* JScene::Window::addAudio(std::string AudioID, std::string Address) {
	if (Audios.find(AudioID) != Audios.end()) {
		PRINT_WINDOW_UPDATE("WindowError: Audio ID already being used - " + AudioID);
		return NULL;
	}

	Audio NewAudio;
	NewAudio.id = AudioID;
	NewAudio.audio = Mix_LoadMUS(Address.c_str());
	NewAudio.playing = false;

	Audios[AudioID] = NewAudio;

	return &Audios[AudioID];
}
JScene::Audio* JScene::Window::getAudio(std::string AudioID) {
	if (Audios.find(AudioID) == Audios.end()) return &NULLAUDIO;

	return &Audios[AudioID];
}

#ifndef __ANDROID__
std::unordered_map<std::string, JScene::Video>* JScene::Window::getVideos() {
	return &Videos;
}
JScene::Video* JScene::Window::addVideo(std::string VideoID, std::string VideoAddress, int Loops, bool AutoUpdate, int FPS, SDL_Rect Size) {
	//if (!fileExists(Video)) {
		//PRINT_WINDOW_UPDATE("WindowError: Can't add invalid video - " + Video);
		//return &NULLVIDEO;
	if (Videos.find(VideoID) != Videos.end()) {
		PRINT_WINDOW_UPDATE("WindowError: Can't video already added - " + VideoAddress);
		return &NULLVIDEO;
	}

	Video NewVideo;
	NewVideo.name = VideoID;
	NewVideo.address = VideoAddress;
	NewVideo.autoupdate = AutoUpdate;
	NewVideo.loops = Loops;
	NewVideo.fps = FPS;
	NewVideo.drawsize = Size;
	NewVideo.window = this;
	NewVideo.vrender = SDLRenderer;
	
	Videos[VideoID] = NewVideo;

	return &Videos[VideoID];
}
JScene::Video* JScene::Window::getVideo(std::string VideoID) {
	if (Videos.find(VideoID) == Videos.end()) return &NULLVIDEO;

	return &Videos[VideoID];
}
JScene::Video* JScene::Window::getVideoByAddress(std::string Address) {
	for (auto const& video : Videos) {
		if (Videos[video.first].address == Address) {
			return &Videos[video.first];
		}
	}

	return &NULLVIDEO;
}
#endif

// TODO - Implement
void JScene::Window::cleanButton(std::string ButtonID) {
	Buttons.erase(ButtonID);
}

void JScene::Window::CLEAR_ALL_WINDOW_DATA() {
	clearTextTextures();
	clearImages();
	clearButtons();
	clearTextBoxes();
	clearAudios();
	clearVideos();
}
void JScene::Window::clearTextTextures() {
	for (auto const& text : Texts) {
		SDL_DestroyTexture(Texts[text.first].texture);
	}

	Texts.clear();
}
void JScene::Window::clearImages() {
	for (auto const& image: Images) SDL_DestroyTexture(image.second.texture);
	
	Images.clear();
}
void JScene::Window::clearButtons() {
	for (auto const& button : Buttons) {
		if (button.second.texture != NULL) {
			Images.erase(Buttons[button.first].address);

			SDL_DestroyTexture(Buttons[button.first].texture);
		}
	}

	Buttons.clear();
	SelectedButtonMap.clear();
	SelectedButton = -1;
}
void JScene::Window::clearTextBoxes() {
	TextBoxes.clear();
	SelectedTextBox = "";
}
void JScene::Window::clearAudios() {
	Audios.clear();
}
void JScene::Window::clearVideos() {
#ifndef __ANDROID__	
	for (auto const& video : Videos) {
		SDL_DestroyTexture(Videos[video.first].texture);
	}

	Videos.clear();
#endif
}
int JScene::Window::getWidth() {
	return Properties.width;
}
int JScene::Window::getHeight() {
	return Properties.height;
}

#ifdef _WIN32
HWND JScene::Window::getHWND() {
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(SDLWindow, &wmInfo);
	HWND hwnd = wmInfo.info.win.window;

	return hwnd;
}
#endif
