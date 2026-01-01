- 将Animation继承于sf::Sprite，方便setTextureRect，并且update也会更加方便
- 参考其他库，添加地图编辑器Map类，从json中读取地图信息，产生实际的地图对象
- 添加基础元素block，作为地图的元素，block产生enactableBlock，用于与地图交互
- 添加基础元素item，用于处理金币等掉落物、projectile等
- 添加基础元素enemy，用于处理敌人，完毕后添加aibrain，用于处理敌人的行为
- 将纹理的加载设置一个register，避免手动编写大量的载入代码


---------------------
将最关键的class设计放入kernel中，产生一个比较完备的框架，以此为基础设计demo


## 12.28
1. 添加日志模块，使用传统的单例+宏定义
	- 分级，LOG_ERROR, LOG_WARNING, LOG_INFO, LOG_TRACE
	- 日志输出到控制台或文件
2. ~~调整代码组织，不要都放在src下~~ ✅ **完成** (2026-01-01)
	- 已创建模块化目录结构：core/, entities/, graphics/, world/, physics/, utils/
3. ~~调整main函数，不能放这么多东西，要有个清晰的逻辑去统一管理~~ ✅ **完成** (2026-01-01)
	- 已创建 Game 类管理游戏循环
	- 已创建 ResourceManager 单例管理资源加载
	- main.cpp 现在只负责初始化和启动游戏
```cpp
int main() {
	// 初始化日志
	Logger::getInstance().init("log.txt");

	// 加载资源
	ResourceManager::getInstance().loadResources("resources/");

	// 或者更详细，也可以分模块加载
	// 加载图片资源、音频资源、地图资源

	// 创建游戏对象
	// 创建人物、地图等
	Game game;
	game.run();

	return 0;
}
```
4. 帧率控制，不能无限制的跑，太吃CPU了
5. 文件路径参数使用`std::filesystem::path`，不要一个string拼接一大堆，也方便跨平台
6. format代码，统一使用clang-format
7. 
8. 文件组织，不要写`#include "../deps/JsonHandler.h"`，我记得cmake可以设置include路径，直接include`deps/JsonHandler.h`，或者之后调整了代码组织，写成`#include "json/JsonHandler.h"`和`include "log/Logger.h"`之类的
9. 暂时没想好其他特别需要的

不那么迫切的：
1. 地图切换
2. 敌人、人物、AI
3. 实体、交互
4. 对话框、UI、菜单（一定要确保UI的流畅性，要做到极其丝滑）
5. 音乐、音效
6. 文本、剧情、交流（注意localization，先全用英语）
7. 存档、读档
8. 粒子系统
9. 注意防止资源重复加载、实现资源延迟加载