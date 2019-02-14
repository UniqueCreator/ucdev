rem msbuild ucdev_render.vcxproj /t:Build /p:Configuration=Debug /p:Platform=x64 /verbosity:detailed
rem rem msbuild uc_engine.vcxproj /t:Build /p:Configuration=Debug /p:Platform=x64 /verbosity:detailed
msbuild uc_engine.vcxproj /t:Build /p:Configuration=Debug /p:Platform=x64 /verbosity:detailed
rem msbuild uc_engine_textured_quad.vcxproj /t:Build /p:Configuration=debug /p:Platform=x64 /verbosity:detailed

