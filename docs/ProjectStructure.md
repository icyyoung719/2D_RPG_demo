# 项目结构

主要目录与文件说明：

- `SFML_test/`：示例子工程（源码、依赖与资产）
  - `src/`：游戏源码（`main.cpp`, `Player.cpp`, `Map.cpp`, `Animation.cpp` 等）
  - `deps/`：自定义的 JSON 解析器等工具
  - `assests/`：素材（sprites、tilesets、maps）
- `build/`：CMake 生成的构建结果与 Visual Studio 解决方案
- `docs/`：本次新增的文档（概览、构建、控制、项目结构）

关键源码位置：
- 游戏入口：`SFML_test/src/main.cpp`
- 玩家：`SFML_test/src/Player.cpp`, `SFML_test/src/Player.h`
- 动画系统：`SFML_test/src/Animation.cpp`, `SFML_test/src/Animation.h`
- 地图与瓦片：`SFML_test/src/Map*.cpp/.h`, `assests/world/`（tileset JSON）


## 想要的项目文件组织形式
- root--->SFML_test
         |--->src
         |      |--->main.cpp
         |      |--->Player.cpp
         |      |--->Map.cpp
         |      |--->Animation.cpp
         |      |--->ai
          |      |--->xxx.h
          |      |--->xxx.cpp

         |
         |--->deps
         |      |--->json
         |              |--->JsonHandler.h
         |              |--->JsonHandler.cpp
         |              |--->...
         |      |--->log
          |              |--->Logger.h
          |              |--->Logger.cpp
         |
         |--->assets
                |--->sprites
                |--->tilesets
                |--->maps
                |--->...


要求：
使用cmake，使得src下的所有文件可以直接include deps下的文件，而不需要写相对路径。
如，Map.cpp和xxx.h中可以直接写
```cpp
#include "json/JsonHandler.h"
#include "log/Logger.h"
```
而不是
```cpp
#include "../deps/json/JsonHandler.h"
//或者
#include "deps/json/JsonHandler.h"
```