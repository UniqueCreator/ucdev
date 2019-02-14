rem msbuild uc_engine.vcxproj /t:Build /p:Configuration=release /p:Platform=x64 /verbosity:detailed
rem msbuild ucdev_app.vcxproj /t:Clean /p:Configuration=release /p:Platform=x64 /verbosity:detailed
rem msbuild ucdev_app.vcxproj /t:Clean /p:Configuration=debug /p:Platform=x64 /verbosity:detailed /p:DeployOnBuild=true
rem msbuild /t:Build /p:Configuration=debug /p:Platform=x64 /verbosity:detailed /p:DeployOnBuild=true ucdev_app.vcxproj
msbuild uc_engine.sln /t:Build /p:Configuration=release /p:Platform=x64 /verbosity:minimal


