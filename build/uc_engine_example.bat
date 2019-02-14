@echo off

@set example_name=%1

echo ^<?xml version="1.0" encoding="utf-8"?^> > %example_name%_h.msbuild
echo ^<Project DefaultTargets="Build" ToolsVersion="15.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003"^> >> %example_name%_h.msbuild

ucdev_build_file_generator_r.exe --input ..\src\%example_name%\ --mode h >> %example_name%_h.msbuild
ucdev_build_file_generator_r.exe --input ..\include\uc_public\ --mode h >> %example_name%_h.msbuild

echo ^</Project^> >> %example_name%_h.msbuild

echo ^<?xml version="1.0" encoding="utf-8"?^> > %example_name%_cpp.msbuild
echo ^<Project DefaultTargets="Build" ToolsVersion="15.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003"^> >> %example_name%_cpp.msbuild

ucdev_build_file_generator_r.exe --input ..\src\%example_name%\ --mode cpp >> %example_name%_cpp.msbuild

echo ^</Project^> >> %example_name%_cpp.msbuild

echo ^<?xml version="1.0" encoding="utf-8"?^> > %example_name%_hlsl.msbuild
echo ^<Project DefaultTargets="Build" ToolsVersion="15.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003"^> >> %example_name%_hlsl.msbuild

ucdev_build_file_generator_r.exe --input ..\src\%example_name%\shaders\ --mode hlsl --backend unique >> %example_name%_hlsl.msbuild
ucdev_build_file_generator_r.exe --input ..\include\uc_public\gpu\ --mode hlsl  --backend unique >> %example_name%_hlsl.msbuild

echo ^</Project^> >> %example_name%_hlsl.msbuild


echo ^<?xml version="1.0" encoding="utf-8"?^> > %example_name%_pso.msbuild
echo ^<Project DefaultTargets="Build" ToolsVersion="15.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003"^> >> %example_name%_pso.msbuild

ucdev_build_file_generator_r.exe --input ..\src\%example_name%\shaders\ --mode pso  --backend unique >> %example_name%_pso.msbuild

echo ^</Project^> >> %example_name%_pso.msbuild


echo ^<?xml version="1.0" encoding="utf-8"?^> > %example_name%.vcxproj.filters
echo ^<Project DefaultTargets="Build" ToolsVersion="15.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003"^> >> %example_name%.vcxproj.filters

ucdev_build_file_generator_r.exe --type filters --input ..\src\%example_name%\ --mode h >> %example_name%.vcxproj.filters
ucdev_build_file_generator_r.exe --type filters --input ..\include\uc_public\ --mode h >> %example_name%.vcxproj.filters

ucdev_build_file_generator_r.exe --type filters --input ..\src\%example_name%\ --mode cpp >> %example_name%.vcxproj.filters

ucdev_build_file_generator_r.exe --type filters --input ..\src\%example_name%\shaders\ --mode hlsl --backend unique >> %example_name%.vcxproj.filters
ucdev_build_file_generator_r.exe --type filters --input ..\include\uc_public\gpu\ --mode hlsl  --backend unique >> %example_name%.vcxproj.filters

ucdev_build_file_generator_r.exe --type filters --input ..\src\%example_name%\shaders\ --mode pso  --backend unique >> %example_name%.vcxproj.filters

echo ^</Project^> >> %example_name%.vcxproj.filters




