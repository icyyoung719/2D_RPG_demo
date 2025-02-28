- 将Animation继承于sf::Sprite，方便setTextureRect，并且update也会更加方便
- 参考其他库，添加地图编辑器Map类，从json中读取地图信息，产生实际的地图对象
- 添加基础元素block，作为地图的元素，block产生enactableBlock，用于与地图交互
- 添加基础元素item，用于处理金币等掉落物、projectile等
- 添加基础元素enemy，用于处理敌人，完毕后添加aibrain，用于处理敌人的行为
- 将纹理的加载设置一个register，避免手动编写大量的载入代码


---------------------
将最关键的class设计放入kernel中，产生一个比较完备的框架，以此为基础设计demo