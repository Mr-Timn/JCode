//
// Made by Jordan Hoosman
// Contributors
// - 
//

#include "../../../JScene/JScene.h"

namespace JS = JScene;

/*** Global ***/

const JS::BasicColors COLORS = JS::PreloadedColors;

const std::string JCODE_REL_DIR = "../../../";
const std::string DEFAULTFONT = JCODE_REL_DIR + "JScene/Fonts/Vera.ttf";

/*** Chess ***/

void Chess_MainMenu(JS::Window* win) {
	JS::Dynamic_Rect title = { 0, 0, 0, 0 };
	title.w = win->getTextTextureWidth("Chess", 20);

    while (win->updateEvents()) {
        win->clear();

        if (title.h == 0) {
			if (JS::xwidth(title) >= win->getWidth()) title.h = 1;
			else title.x += 5.3;
		} else {
			if (title.x <= 0) title.h = 0;
			else title.x -= 5.3;
		}

		win->writeText("Chess", title.x, title.y);

        win->load();
        win->yield();
    }
}

/*** Main ***/

int main(int argc, char** argv) {
    JS::Window* win = new JS::Window("Chess", 720, 1080, NULL, COLORS.white, 60);

    if (win->start()) {
		win->addFont(DEFAULTFONT);
    	win->setFont(DEFAULTFONT, 20, COLORS.black);
        
		Chess_MainMenu(win);
    } else {
        printf("Failed to start window.\n");
    }

    return 0;
}