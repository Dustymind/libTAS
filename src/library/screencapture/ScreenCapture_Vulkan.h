/*
    Copyright 2015-2023 Clément Gallet <clement.gallet@ens-lyon.org>

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

#ifndef LIBTAS_SCREENCAPTURE_VULKAN_H_INCL
#define LIBTAS_SCREENCAPTURE_VULKAN_H_INCL

#include "ScreenCapture_Impl.h"
#include "rendering/vulkanwrappers.h"

#include <stdint.h>

namespace libtas {

class ScreenCapture_Vulkan : public ScreenCapture_Impl {

public:

    /* Initiate the internal variables and buffers, and get the screen dimensions
     * @return 0 if successful or -1 if an error occured
     */
    int init();

    /* Create the screen buffer/surface/texture */
    void initScreenSurface();

    /* Destroy the screen buffer/surface/texture */
    void destroyScreenSurface();

    /* Get the pixel format as an string used by nut muxer. */
    const char* getPixelFormat();

    /* Copy the current screen to the screen buffer/surface/texture.
     * This surface is optimized for rendering, so it may be stored on GPU. */
    int copyScreenToSurface();

    /* Transfer pixels from the screen buffer/surface/texture into an array, pointed by `pixels`
     * Returns the size of the array. */
    int getPixelsFromSurface(uint8_t **pixels, bool draw);

    /* Copy back the stored screen buffer/surface/texture into the screen. */
    int copySurfaceToScreen();

    void clearScreen();

    uint64_t screenTexture();

private:
        
    /* Vulkan screen image */
    VkImage vkScreenImage = VK_NULL_HANDLE;
    VkImageView vkScreenImageView = VK_NULL_HANDLE;
    VkSampler vkScreenSampler = VK_NULL_HANDLE;
    VkDescriptorSet vkScreenDescriptorSet = VK_NULL_HANDLE;
    VkDeviceMemory vkScreenImageMemory = VK_NULL_HANDLE;
}; 
}

#endif
