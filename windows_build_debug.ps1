$env:SFML_ROOT = "D:\Projects\libraries\SFML-2.6.2"

mkdir -Force ./build
cd ./build
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM="C:\\Program Files\\Ninja\\ninja.exe" -G "Ninja" ..
cmake --build .
cd ..
.\build\zombie-arena.exe