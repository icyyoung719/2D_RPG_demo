# 资源与地图（assets / map / tileset）处理说明

本文档汇总项目中关于资源（sprites、tilesets、maps、动画帧）与 JSON 地图文件的组织、格式要点以及加载/使用建议，便于在 `SFML_test` 子项目中统一处理。

**适用路径（示例）**
- 资源根：`assests/`
- 地图：`assests/maps/`（例如 `test_map.json`）
- tileset：`assests/world/`（例如 `basictiles_tileset.json`、`overworld_tileset.json`）
- 精灵/动画：`assests/Hiker/Separated Animation PNGs/`、`Jumping_boy_sprites/Transparent PNG/`

**概述**
- 将纹理、tileset 定义、地图 JSON、以及分离的动画帧分开管理能提高可维护性。常见工作流：在 Tiled / Aseprite 中导出 tileset 与地图 -> 使用程序端的 MapLoader/Texture 管理器加载 JSON -> 根据 tileset 的 tilesize 与 tiles 索引渲染地图层。

资源组织建议
- 按用途分文件夹：`tilesets/`、`maps/`、`sprites/`、`animations/`。
- 保持 tileset 的图片与 tileset JSON（或 Tiled 导出的 tileset 定义）相对路径一致，方便在不同平台上重定位。
- 对于精灵动画，若使用 Aseprite，可导出整张图或分帧 PNG。项目中既有 `.aseprite` 文件也有“Separated Animation PNGs”，推荐使用分帧目录来简化运行时加载。

tileset（常见字段说明）
- `image`：tileset 使用的图片文件路径。
- `tilewidth`, `tileheight`：单个 tile 的像素大小。
- `columns`：图片中每行 tile 的数量（用于按索引切片）。
- `tilecount`：该 tileset 含有的总 tile 数。
- `tiles`（可选）：每个 tile 的扩展定义（如碰撞区域、动画信息、属性）。

map JSON（常见结构及要点）
- `tilesets`：数组，包含对 tileset 的引用。每项常见字段：`firstgid`（该 tileset 在地图中起始全局索引）、`source` 或内联 tileset 定义。
- `layers`：数组，图层可以是 `tilelayer`（瓦片层）、`objectgroup`（对象层）、`imagelayer` 等。
  - `tilelayer` 的 `data` 通常是按行或按列的全局 tile 索引数组，值为 0 表示空白。
  - `objectgroup` 定义物体（碰撞体、触发点、实体出生点），每个对象包含 `x`, `y`, `width`, `height`, `properties` 等。
- `width`, `height`：地图的瓦片数量（以 tiles 为单位）。

常见约定与加载建议
- 通过 `firstgid` 与 tileset 的 `tilecount` 计算任意 tile 索引属于哪个 tileset：若 `firstgid <= gid < firstgid + tilecount`，则该 gid 来自该 tileset。渲染时用 (gid - firstgid) 作为 tileset 内部索引。
- 在渲染时，将单个 tileset 图拆分为 texture atlas，与 `tilewidth`/`tileheight` 对齐以获取子纹理坐标。
- 如果 tiles 含有自定义碰撞（在 tileset 的 `tiles` 字段定义），在加载时解析这些属性并把碰撞信息附到 tile 的 index 或一个碰撞表中。
- 对于对象层（`objectgroup`），把类型（`type`）或自定义属性映射成游戏内实体类型（例如 `spawn_point`, `enemy_spawn`, `collision_rect`）。

动画与精灵（Aseprite / 分帧）
- 推荐把动画帧导出为单张包含多帧的精灵表或直接导出分帧 PNG：项目中已有 `Separated Animation PNGs/`，加载分帧更简单，直接按帧创建 `sf::Texture` 或 `sf::IntRect` 切片。
- 若使用 Aseprite tag 导出动画元数据，可将帧序列与每帧延迟写入 JSON，然后在运行时按帧延迟播放。

示例片段（tileset 引用）
```json
"tilesets": [
  {
    "firstgid": 1,
    "source": "../world/basictiles_tileset.json"
  }
]
```

示例片段（tile layer 数据，简化）
```json
"layers": [
  {
    "type": "tilelayer",
    "width": 10,
    "height": 8,
    "data": [0,0,1,1,2]
  }
]
```

加载器实现要点（MapLoader）
- 解析 tilesets 数组并按 `firstgid` 建立索引表（firstgid -> tileset）。
- 解析每个 `tilelayer` 的数据，并在渲染时将 GID 转成对应 tileset 的局部索引与纹理坐标。
- 解析 `objectgroup`，根据对象类型实例化游戏实体或碰撞盒。
- 在对动画 tile（含在 tileset 的 `tiles` 中定义动画）进行渲染时，维护帧时间累积并切换 frame index（可复用项目中 `Animation` 类）。

调试与常见问题
- 空白 tile：GID 为 0 表示空白，不应尝试渲染或查询 tileset。
- 图像偏移：确保 tileset 的 `margin` 与 `spacing`（若存在）在切片时被正确处理。
- 坐标系：Tiled 的对象 Y 坐标有时表示对象底部或顶端，注意与游戏坐标系的一致性。

工具与导出建议
- 地图编辑器：Tiled（推荐） — 支持导出为 JSON，容易与运行时代码对接。
- 精灵编辑：Aseprite — 支持导出帧与动画元数据 JSON。

附录：项目中相关文件位置
- 地图样例：assests/maps/test_map.json
- Tileset 示例：assests/world/basictiles_tileset.json, assests/world/overworld_tileset.json
- 精灵/动画：assests/Hiker/, assests/Jumping_boy_sprites/

**瓦片属性（碰撞 / 地形）的生成流程**

碰撞与地形信息不在 Tiled 里手工点选，而是由规则文件生成，避免上千次点击并保证可复查：

- 单一真源：`tools/tile_semantics.txt`，按 tileset 分组，支持 `wang`（王块颜色）、`anim`（动画基块及其所有帧）、`range`（网格矩形）、`tile`（单块或列表）四类规则；类别为 `solid` / `water` / `walkable` / `empty` / `?`（未定）。
- 生成：`python3 tools/gen_tile_props.py` — 默认只为地图实际使用到的 tile 写入属性，结果合并进 tileset json 的 `tiles[].properties`（`{"solid": true}`、`{"terrain": "water"}`、`{"walkable": true}`、`{"empty": true}`）。`animation`、`wangsets` 及手写的其他属性不会被改动。
- 校验：`python3 tools/gen_tile_props.py --check` 比较语义而非字节（Tiled 重新保存导致的排版变化不算差异）；若地图用到了尚未分类的 tile，或 json 里的属性与规则不一致，则失败。CI 的 `tile-properties` 任务运行该检查，因此新增地图不会悄悄引入未分类的瓦片。
- 目视复查：`python3 tools/proof_sheet.py [--only-used] [--zoom N]`，输出到 `build/tile-proof/`，边框颜色即类别（红=solid、青=water、绿=walkable、蓝=empty、灰=未分类），左上角白点表示该 tile 被地图使用；用于替代在 Tiled 里逐个点选。
- 规则中的 ID 是 tileset 局部索引（即地图 gid 减去该 tileset 的 `firstgid`，Tiled 状态栏显示的值）；行列号为从 0 开始的网格坐标。

---
如需我把 `MapLoader` 的加载示例代码片段加入本文件，或把文档合并到 `README.md`，我可以继续处理。
