/*
    Copyright 2015-2016 Clément Gallet <clement.gallet@ens-lyon.org>

    This file is part of libTAS.

    libTAS is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libTAS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with libTAS.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LINTAS_GAMEINFOWINDOW_H_INCLUDED
#define LINTAS_GAMEINFOWINDOW_H_INCLUDED

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>

#include "../Context.h"

class GameInfoWindow {
    public:
        GameInfoWindow(Context *c);

        /* Update UI elements */
        void update();

        Context *context;

        Fl_Double_Window *window;

        Fl_Box *video_box;
        Fl_Box *audio_box;
        Fl_Box *keyboard_box;
        Fl_Box *mouse_box;
        Fl_Box *joystick_box;

};

#endif
