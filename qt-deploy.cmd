rmdir /s /q target
set exe_file=InnoScriptGen.exe
set qt_cmd_file=make_dep.cmd
mkdir target
copy %exe_file% .\target\
copy %qt_cmd_file% .\target\
cd target
call %qt_cmd_file%

