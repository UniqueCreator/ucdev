#pragma once

#include <vector>
#include <string>
#include <tuple>

#include "uc_build_tasks_dependencies.h"
#include "uc_build_tasks_shaders.h"

namespace uc
{
    namespace build
    {
        namespace tasks
        {
            struct item_meta_data
            {
                std::string               m_file_name;          //file name to compile
                std::string               m_dxbc;               //file name with the dx bytecode
                std::string               m_pdb_file;           //file name for pdb information for debugging the shaders
                std::string               m_dxbc_header;        //header output name to include into cpp
                std::string               m_dxbc_variable_name; //variable name for the output file name

                std::vector< std::string> m_additional_include_directories; //list of include directories for parsing the shader
                std::vector< std::string> m_preprocessor_directives;        //list of preprocessor directives for parsing the shader

                uc::build::tasks::shader_pipeline_stage m_pipeline_stage;   //type of the shader, pixel, vertex, domain, hull, geometry, compute

                std::string              m_cpp_generated_type_name;         //type name for the automatically generated c++ code
                std::string              m_cpp_generated_file_name;         //file name for the automatically generated c++ code
                std::string              m_cpp_generated_file_name_header;  //file name for the automatically generated c++ code

            };


            std::tuple< std::vector< item_meta_data>, file_cache >   build_caches( const std::vector< item_meta_data>& files_to_parse, const std::string& cache_file, const std::vector< dependency_cache_item >& static_dependencies  );
                

            file_cache load_cache(const std::string& cache_file);
            void save_cache(const std::string& cache_file, const file_cache& cache);
        }
    }
}