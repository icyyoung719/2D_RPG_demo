# 概览

本项目是一个小型的 2D RPG 演示工程，目标是提供：

- 基于 SFML 的渲染循环与窗口管理
- 清晰的代码组织结构，按功能模块划分（核心、实体、图形、世界、物理、工具）
- 资源管理系统（见 `src/core/ResourceManager.cpp`）
- 游戏循环管理（见 `src/core/Game.cpp`）
- 简单的实体与动画系统（见 `src/graphics/Animation.cpp`, `src/entities/Player.cpp`）
- 地图加载器与瓦片集支持（使用 JSON 格式的关卡/tileset，位于 `assests/world/` 与 `assests/maps/`）
- 基础的碰撞检测系统（见 `src/physics/Collider.cpp`）

该工程适合用于学习游戏循环、碰撞与关卡加载，也可作为个人项目的起点。

## 代码组织

代码按照功能模块组织在不同的目录中：
- `core/` - 核心游戏管理类（Game, ResourceManager）
- `entities/` - 游戏实体（Player, Platform 等）
- `graphics/` - 图形与动画系统
- `world/` - 地图与世界管理
- `physics/` - 物理与碰撞检测
- `utils/` - 通用工具类

主函数 (`main.cpp`) 现在非常简洁，只负责初始化资源路径并启动游戏循环。
