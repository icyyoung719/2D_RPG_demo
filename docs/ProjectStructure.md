# 项目结构

主要目录与文件说明：

- `SFML_test/`：示例子工程（源码、依赖与资产）
  - `src/`：游戏源码（`main.cpp`, `Player.cpp`, `Map.cpp`, `Animation.cpp` 等）
  - `dependency/`：自定义的 JSON 解析器等工具
  - `assests/`：素材（sprites、tilesets、maps）
- `build/`：CMake 生成的构建结果与 Visual Studio 解决方案
- `docs/`：本次新增的文档（概览、构建、控制、项目结构）

关键源码位置：
- 游戏入口：`SFML_test/src/main.cpp`
- 玩家：`SFML_test/src/Player.cpp`, `SFML_test/src/Player.h`
- 动画系统：`SFML_test/src/Animation.cpp`, `SFML_test/src/Animation.h`
- 地图与瓦片：`SFML_test/src/Map*.cpp/.h`, `assests/world/`（tileset JSON）
