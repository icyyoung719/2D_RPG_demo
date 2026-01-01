# 项目结构

主要目录与文件说明：

- `SFML_test/`：示例子工程（源码、依赖与资产）
  - `src/`：游戏源码，按功能模块组织
    - `core/`：核心游戏类（Game, ResourceManager）
    - `entities/`：实体类（Entity, Player, Platform）
    - `graphics/`：图形相关类（Animation, TextureInfo）
    - `world/`：世界与地图类（Map, Tileset, Tile）
    - `physics/`：物理相关类（Collider）
    - `utils/`：工具类（Constants）
    - `main.cpp`：游戏入口
  - `deps/`：自定义的 JSON 解析器等工具
    - `json/`：JSON 解析器
  - `assests/`：素材（sprites、tilesets、maps）
- `build/`：CMake 生成的构建结果与 Visual Studio 解决方案
- `docs/`：文档（概览、构建、控制、项目结构）

关键源码位置：
- 游戏入口：`SFML_test/src/main.cpp`
- 游戏核心：`SFML_test/src/core/Game.cpp`, `SFML_test/src/core/Game.h`
- 资源管理：`SFML_test/src/core/ResourceManager.cpp`, `SFML_test/src/core/ResourceManager.h`
- 玩家：`SFML_test/src/entities/Player.cpp`, `SFML_test/src/entities/Player.h`
- 动画系统：`SFML_test/src/graphics/Animation.cpp`, `SFML_test/src/graphics/Animation.h`
- 地图与瓦片：`SFML_test/src/world/Map*.cpp/.h`, `assests/world/`（tileset JSON）


## 项目文件组织形式
```
root/
└── SFML_test/
    ├── src/
    │   ├── main.cpp
    │   ├── core/
    │   │   ├── Game.h
    │   │   ├── Game.cpp
    │   │   ├── ResourceManager.h
    │   │   └── ResourceManager.cpp
    │   ├── entities/
    │   │   ├── Entity.h
    │   │   ├── Player.h
    │   │   ├── Player.cpp
    │   │   ├── Platform.h
    │   │   └── Platform.cpp
    │   ├── graphics/
    │   │   ├── Animation.h
    │   │   ├── Animation.cpp
    │   │   └── TextureInfo.h
    │   ├── world/
    │   │   ├── Map.h
    │   │   ├── Map.cpp
    │   │   ├── Tileset.h
    │   │   ├── Tileset.cpp
    │   │   └── Tile.h
    │   ├── physics/
    │   │   ├── Collider.h
    │   │   └── Collider.cpp
    │   └── utils/
    │       ├── Constants.h
    │       └── Object.h
    ├── deps/
    │   └── json/
    │       ├── JsonHandler.h
    │       ├── JsonHandler.cpp
    │       └── ...
    └── assets/
        ├── sprites/
        ├── tilesets/
        ├── maps/
        └── ...
```

## 包含路径说明

CMake 配置使得 `src` 和 `deps` 目录都在包含路径中，因此：

1. **跨模块引用**：src 内的文件可以使用相对路径引用其他模块
   ```cpp
   #include "../entities/Player.h"    // 从 core/ 引用 entities/
   #include "../graphics/Animation.h" // 从 entities/ 引用 graphics/
   ```

2. **引用 deps**：src 内的文件可以直接引用 deps 下的文件
   ```cpp
   #include "json/JsonHandler.h"  // 正确 ✓
   ```
   而不需要写成：
   ```cpp
   #include "../deps/json/JsonHandler.h"  // 不推荐
   ```