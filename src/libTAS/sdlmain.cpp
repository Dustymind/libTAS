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

#include "sdlmain.h"
#include "sdltimer.h"
#include "sdlwindows.h"
#include "sdlevents.h"
#include "../shared/sockethelpers.h"
#include "logging.h"
#include "../shared/messages.h"
#include "hook.h"
#include "sdlversion.h"
#include "threadwrappers.h"

namespace libtas {

/* Function pointers to real functions */
namespace orig {
    static int (*SDL_Init)(Uint32 flags) = nullptr;
    static int (*SDL_InitSubSystem)(Uint32 flags) = nullptr;
    static void (*SDL_Quit)(void) = nullptr;
}

/* Override */ int SDL_Init(Uint32 flags){
    DEBUGLOGCALL(LCF_SDL);

    /* Get and remember which sdl version we are using. */
    int SDLver = get_sdlversion();

    LINK_NAMESPACE_SDLX(SDL_Init);

    /* In both SDL1 and SDL2, SDL_Init() calls SDL_InitSubSystem(),
     * but in SDL2, SDL_Init() can actually never be called by the game,
     * so we put the rest of relevent code in the SubSystem function.
     *
     * ...well, this is in theory. If on SDL2 we call SDL_Init(), it
     * does not call our SDL_InitSubSystem() function. Maybe it has to do with
     * some compiler optimization, because the real SDL_Init() function looks
     * like this:
     *      int SDL_Init(Uint32 flags) {
     *          return SDL_InitSubSystem(flags);
     *      }
     * So maybe the compiler is inlining stuff. To fix this, we call
     * ourselves our own SDL_InitSubSystem() function.
     */
    if (SDLver == 1)
        return orig::SDL_Init(flags);
    if (SDLver == 2)
        return SDL_InitSubSystem(flags);
    return 0;
}

/* Override */ int SDL_InitSubSystem(Uint32 flags){
    DEBUGLOGCALL(LCF_SDL);

    /* Get which sdl version we are using. */
    int SDLver = get_sdlversion();
    GameInfo::Flag sdl_flag = (SDLver==2)?GameInfo::SDL2:((SDLver==1)?GameInfo::SDL1:GameInfo::UNKNOWN);

    /* Link function pointers to SDL functions */
    LINK_NAMESPACE_SDLX(SDL_InitSubSystem);
    LINK_NAMESPACE_SDLX(SDL_Quit);

    link_sdlwindows();
    link_sdlevents();
    link_sdlthreads();
    link_sdltimer();

    /* The thread calling this is probably the main thread */
    setMainThread();

    if (flags & SDL_INIT_TIMER)
        debuglog(LCF_SDL, "    SDL_TIMER enabled.");

    if (flags & SDL_INIT_AUDIO) {
        debuglog(LCF_SDL, "    SDL_AUDIO fake enabled.");
        game_info.audio = sdl_flag;
    }

    if (flags & SDL_INIT_VIDEO) {
        debuglog(LCF_SDL, "    SDL_VIDEO enabled.");
        game_info.video |= sdl_flag;
    }

    if (flags & SDL_INIT_JOYSTICK) {
        debuglog(LCF_SDL, "    SDL_JOYSTICK fake enabled.");
        game_info.joystick = sdl_flag;
    }

    if (flags & SDL_INIT_HAPTIC)
        debuglog(LCF_SDL, "    SDL_HAPTIC enabled.");

    if (flags & SDL_INIT_GAMECONTROLLER) {
        debuglog(LCF_SDL, "    SDL_GAMECONTROLLER fake enabled.");
        game_info.joystick = sdl_flag;
    }

    if (flags & SDL_INIT_EVENTS)
        debuglog(LCF_SDL, "    SDL_EVENTS enabled.");

    game_info.keyboard = sdl_flag;
    game_info.mouse = sdl_flag;

    game_info.tosend = true;

    /* Disabling Joystick subsystem, we don't need any initialization from SDL */
    flags &= 0xFFFFFFFF ^ SDL_INIT_JOYSTICK;

    /* Disabling GameController subsystem, we don't need any initialization from SDL */
    flags &= 0xFFFFFFFF ^ SDL_INIT_GAMECONTROLLER;

    /* Disabling Audio subsystem so that it does not create an extra thread */
    flags &= 0xFFFFFFFF ^ SDL_INIT_AUDIO;

    return orig::SDL_InitSubSystem(flags);
}

/* Override */ void SDL_Quit(){
    DEBUGLOGCALL(LCF_SDL);

    is_exiting = true;

    sendMessage(MSGB_QUIT);
    orig::SDL_Quit();
}

}
