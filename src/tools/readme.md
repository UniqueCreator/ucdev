Tools in this directory:

1. hlsl compiler is an msbuild task extension which generated cpp files from hlsl shaders, ready to be included into a project
2. include_parser parses code with #include and macros and outputs the dependencies. useful for dependency tracking
3. pipeline stage object generates cpp code from pso files, used with the hlsl compiler
4. model imports models from 3rd party tools and converts them to a format for the engine
