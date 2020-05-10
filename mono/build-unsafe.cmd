@echo off
".\Microsoft.Net.Compilers.Toolset.3.5.0\tasks\net472\csc.exe" /target:exe /platform:AnyCPU /unsafe /optimize+ /out:%~n1.exe %1