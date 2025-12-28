非常棒的问题！这三个 CMake 指令都是**工程组织和依赖管理**的核心，但用途和适用场景有明显区别。下面用你喜欢的风格（对比+解释+示例+原理拆解）系统讲解：

---

# 🌟 一句话总结

| 指令                   | 作用                                                    |
|-----------------------|--------------------------------------------------------|
| `include_directories` | 旧式：全局加入包含头文件的搜索路径                      |
| `target_include_directories` | 推荐：为**指定 target**（可执行文件/库）加头文件搜索路径  |
| `target_link_libraries`      | 为 target 指定要链接哪些库、目标、依赖（静态/动态/对象库） |

---

# 详细解释&使用方式

## 1. `include_directories`

**❌（基本不用，属于老的全局做法）**

- **作用**：给整个项目（所有 target）全局加入头文件查找路径。
- **影响范围**：后续所有 target 都能用这些头文件，不管是不是应该依赖它。
- **格式**：
  ```cmake
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/json)
  ```
- **问题**：容易让项目不小心依赖了本不该依赖的头文件，**污染依赖关系**，大型项目非常难维护。

---

## 2. `target_include_directories`

**✅ 推荐，现代 CMake 正确姿势！**

- **作用**：只为指定的目标（target）添加头文件查找路径，推荐用 `PRIVATE`、`PUBLIC`、`INTERFACE` 来**明确依赖边界**。
- **影响范围**：只影响这个 target 或继承依赖关系（看模式）。
- **格式**：
  ```cmake
  add_executable(MyApp main.cpp)
  target_include_directories(MyApp
      PRIVATE
          ${CMAKE_CURRENT_SOURCE_DIR}/src
          ${CMAKE_CURRENT_SOURCE_DIR}/deps/json
  )
  ```
- **模式说明**：
  - `PRIVATE`: 仅本 target 使用
  - `PUBLIC`: 本 target + 链接它的下游也看到
  - `INTERFACE`: 只导出给下游（本 target不用）

- **优点**：避免依赖污染，明确工程模块边界，是工业界通用最佳实践。

---

## 3. `target_link_libraries`

**必会！链接库/模块/目标的关键指令**

- **作用**：指定 target（可执行/库）链接哪些库（系统库/本地编译的库/第三方库）。
- **影响范围**：只影响本 target
- **格式**：
  ```cmake
  target_link_libraries(MyApp
      PRIVATE sfml-graphics sfml-window
  )
  ```
- **模式**：
  - `PRIVATE`, `PUBLIC`, `INTERFACE`（同上，常用 PRIVATE）
- **灵活性**：
  - 可以链接工程内生成的库，还可以直接链接系统/第三方库。
- **原理**：
  - 自动帮你加链接参数(`-lxxx`)。
  - `target_link_libraries` 也能自动传播 include 路径（如果依赖库正确设置了 PUBLIC/INTERFACE）。

---

# ⚡️ 实例工程演示

```cmake
add_executable(MyApp src/main.cpp)
add_library(MyLib deps/log/Logger.cpp)

# 推荐用法
target_include_directories(MyApp
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/deps/log
)
target_include_directories(MyLib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/deps/log
)

# 接下来只需让MyApp链接MyLib，头文件依赖会自动下传
target_link_libraries(MyApp
    PRIVATE MyLib
    PRIVATE sfml-graphics sfml-window
)
```

**解读：**
- MyApp 可以用 `#include "Logger.h"`，因为 MyLib 已经 PUBLIC 暴露 log 头文件。
- 只有 MyApp/MyLib 能访问 deps/log，不会全局暴露所有头文件目录。
- MyApp 只会链接需要的库，不会污染其他 target。

---

# 🚨 常见新手误区

| 误区                                  | 问题                                           |
|---------------------------------------|------------------------------------------------|
| 全用 `include_directories`            | 导致所有 target 乱 include，工程耦合度高       |
| 随便写 `target_link_libraries`        | 没用到的 target 也被链接，造成体积膨胀/编译慢   |
| 不分 PUBLIC/PRIVATE/INTERFACE         | 模块接口混乱，后期难拆分移植                   |

---

# 💡 大厂最佳实践

- 绝大多数公司内部 CMake 工程都：
  - 用 `target_include_directories` 明确依赖性
  - 用 `target_link_libraries` 明确“我需要啥静态/动态库支持”
  - 很少直接用 `include_directories`，除非是单一小demo

---

# 图示理解

```
MyApp (target_exe)
 ├── target_include_directories --> src/, deps/log/
 └── target_link_libraries --> MyLib, sfml-graphics
MyLib (target_lib)
 └── target_include_directories(PUBLIC) --> deps/log/
```
这样维护依赖关系清晰，哪怕工程100+个模块都好拆分！

---

# 总结&建议

- **实战工程一定优先用 target_include_directories+target_link_libraries。**
- 只有临时单文件demo可以简化为 include_directories。
- build系统清晰，后期可测试、可扩展、可迁移性更强，是现代C++/CMake的基础技能，越早习惯越好！

如还想看更复杂示例，或者想知道 PUBLIC/PRIVATE/INTERFACE 究竟“传递”了什么依赖，随时问我！