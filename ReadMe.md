为了1MGames创建的陪跑游戏。游戏采用主题**“无限”**。玩家扮演一个枪手，尽可能地杀光袭来的外星生物。

# 下载

Github可以直接在Release界面找到Windows的可执行压缩包。
Gitme可以在`版本发布`下找到同样的压缩包。

# 编译

本仓库的本体是一个基于C和Lua的游戏框架（TinyHazelEngine），所以当你使用cmake编译后并不会得到游戏本身，而是得到这个游戏框架。

本仓库目前只能在Windows下编译成功（因为我懒得写音频处理，直接用了Windows的API）：

拉取工程后首先拉取其子工程

```bash
git submodule update --init --recursive
```

然后使用cmake编译(**如果你使用MSVC和Clang编译，需要将最后的hazel.dll重命名为libhazel.dll。如果是MinGW，那么他应当产出libhazel.dll**)

```bash
cmake -S . -B build 
```

然后安装到`install`文件夹：

```bash
cmake --build build --target install
```

游戏本体在`game`文件夹下，所以你需要将`install`文件夹内的所有`.dll`文件和`HazelRunner.exe`拷贝到`game`文件夹下。

然后双击`game/HazelRunner.exe`就可以打开游戏了。

# 体积过大

游戏最后的可执行文件和库是经过压缩的（先`strip`再`upx`），最后结果超出1M很正常。