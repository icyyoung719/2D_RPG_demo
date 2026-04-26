// Constants.h - Shared game constants
#pragma once

namespace GameConstants {
    // Tile rendering scale - tiles are 16x16 pixels, scaled up 4x for display
    inline constexpr float TILE_SCALE = 4.0f;
    inline constexpr int TILE_SIZE_PIXELS = 16;
    inline constexpr unsigned int TARGET_FPS = 60;
}
