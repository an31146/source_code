@echo off
"C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\MSBuild\15.0\Bin\Roslyn\csc.exe" /target:exe /platform:AnyCPU /out:%~n1.exe %1