msbuild src/tools/tools.sln /t:Build /p:Configuration=release /p:Platform=x64 /verbosity:normal
msbuild build/ucdev.sln /t:Build /p:Configuration=release /p:Platform=x64 /verbosity:normal
msbuild build/uc_engine.sln /t:Build /p:Configuration=release /p:Platform=x64 /verbosity:normal
