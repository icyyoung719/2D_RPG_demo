# 构建与运行（Windows / Visual Studio + CMake）

先决条件
- 安装 CMake（>= 3.20 推荐）
- 安装 Visual Studio（含 C++ 工作负载）或等效的 MSVC 工具链
- 下载并安装 SFML（3.x），记住 SFML 的安装路径（`SFML_DIR`）

建议的步骤（命令行）

1. 在仓库根目录打开命令提示符或 PowerShell：

```powershell
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 -DSFML_DIR="C:/path/to/SFML/lib/cmake/SFML" ..
cmake --build . --config Debug
```

2. 或者在 `build/` 里打开生成的 `SFML.sln`（或 `SFML_test.sln`）并用 Visual Studio 编译与运行。

注意事项
- 如果你希望使用系统已安装的 SFML，请把 `SFML_DIR` 指向包含 `SFMLConfig.cmake` 的目录（例如 `C:/SFML/lib/cmake/SFML`）。
- `SFML_test/CMakeLists.txt` 中将 `SFML_STATIC_LIBRARIES` 设为 `TRUE`，因此如果使用动态库（DLL）版本的 SFML，可考虑修改或移除此设置。
- 若遇到链接错误，确认使用的运行时（MT/MD）与 SFML 构建的运行时一致。

运行
- 可直接从 Visual Studio 运行生成的可执行文件，或在 `build/` 下运行：

```powershell
.\Debug\SFML_test.exe
```
