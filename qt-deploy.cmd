set qtdeploy_dir=C:\Qt\Qt5.13.0\5.13.0\mingw73_32\bin
%qtdeploy_dir%\\windeployqt InnoScriptGen.exe
copy %qtdeploy_dir%\libgcc_s_dw2-1.dll .\
copy %qtdeploy_dir%\libstdc*.dll .\