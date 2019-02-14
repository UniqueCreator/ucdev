@echo off


echo ^<?xml version="1.0" encoding="utf-8"?^> > ucdev_app_h.msbuild
echo ^<Project DefaultTargets="Build" ToolsVersion="15.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003"^> >> ucdev_app_h.msbuild

ucdev_build_file_generator_r.exe --input ..\src\app\ --mode h >> ucdev_app_h.msbuild
ucdev_build_file_generator_r.exe --input ..\src\uc_dev\ --mode h >> ucdev_app_h.msbuild
ucdev_build_file_generator_r.exe --input ..\include\uc_dev\ --mode h >> ucdev_app_h.msbuild

echo ^</Project^> >> ucdev_app_h.msbuild

echo ^<?xml version="1.0" encoding="utf-8"?^> > ucdev_app_cpp.msbuild
echo ^<Project DefaultTargets="Build" ToolsVersion="15.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003"^> >> ucdev_app_cpp.msbuild

ucdev_build_file_generator_r.exe --input ..\src\app\ --mode cpp >> ucdev_app_cpp.msbuild
ucdev_build_file_generator_r.exe --input ..\src\uc_dev\ --mode cpp    >> ucdev_app_cpp.msbuild

echo ^</Project^> >> ucdev_app_cpp.msbuild


echo ^<?xml version="1.0" encoding="utf-8"?^> > ucdev_app_hlsl.msbuild
echo ^<Project DefaultTargets="Build" ToolsVersion="15.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003"^> >> ucdev_app_hlsl.msbuild

ucdev_build_file_generator_r.exe --input ..\src\uc_dev\private\gx\dx12\shaders\ --mode hlsl >> ucdev_app_hlsl.msbuild

echo ^</Project^> >> ucdev_app_hlsl.msbuild

echo ^<?xml version="1.0" encoding="utf-8"?^> > ucdev_app_pso.msbuild
echo ^<Project DefaultTargets="Build" ToolsVersion="15.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003"^> >> ucdev_app_pso.msbuild

ucdev_build_file_generator_r.exe --input ..\src\uc_dev\private\gx\dx12\shaders\ --mode pso >> ucdev_app_pso.msbuild

echo ^</Project^> >> ucdev_app_pso.msbuild

echo ^<?xml version="1.0" encoding="utf-8"?^> > ucdev_app.vcxproj.filters
echo ^<Project DefaultTargets="Build" ToolsVersion="15.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003"^> >> ucdev_app.vcxproj.filters

ucdev_build_file_generator_r.exe --type filters --input ..\src\app\ --mode h >> ucdev_app.vcxproj.filters
ucdev_build_file_generator_r.exe --type filters --input ..\src\uc_dev\ --mode h >> ucdev_app.vcxproj.filters
ucdev_build_file_generator_r.exe --type filters --input ..\include\uc_dev\ --mode h >> ucdev_app.vcxproj.filters


ucdev_build_file_generator_r.exe --type filters --input ..\src\app\ --mode cpp >> ucdev_app.vcxproj.filters
ucdev_build_file_generator_r.exe --type filters --input ..\src\uc_dev\ --mode cpp    >> ucdev_app.vcxproj.filters


ucdev_build_file_generator_r.exe --type filters --input ..\src\uc_dev\private\gx\dx12\shaders\ --mode hlsl >> ucdev_app.vcxproj.filters

ucdev_build_file_generator_r.exe --type filters --input ..\src\uc_dev\private\gx\dx12\shaders\ --mode pso >> ucdev_app.vcxproj.filters

echo ^</Project^> >> ucdev_app.vcxproj.filters





