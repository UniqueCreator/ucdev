#pragma once

#define LZHAM_DYNAMIC_LIB 1

#include <windows.h>
#include <lzham.h>

   // FIXME: This stuff should probably be moved to another header.
   #if LZHAM_64BIT
      #define LZHAM_DLL_FILENAME       L"lzham_x64.dll"
      #define LZHAM_DEBUG_DLL_FILENAME L"lzham_x64.dll"
   #else
      #define LZHAM_DLL_FILENAME       L"lzham_x86.dll"
      #define LZHAM_DEBUG_DLL_FILENAME L"lzham_x86.dll"
   #endif

namespace uc
{
	namespace lzham
	{
		// Simple helper class that demonstrates how to dynamically load the LZHAM DLL.
		// The load() method loads the DLL, then initializes the member function pointers in ilzham by calling GetProcAddress() on all exported API's defined in lzham_exports.inc.
		class loader : public ilzham
		{
			loader(const loader &other);
			loader& operator= (const loader &rhs);

		public:
			loader() : ilzham(), m_handle(NULL), m_win32_error(S_OK)
			{
			}

			virtual ~loader()
			{
				unload();
			}

			enum
			{
				cErrorMissingExport = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x201),
				cErrorUnsupportedDLLVersion = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x202),
			};

			// Assumes LZHAM DLL is in the same path as the executable.
#if defined(_MSC_VER)
			static void create_module_path(wchar_t *pModulePath, int size_in_chars, bool debug_dll)
			{
				wchar_t buf[MAX_PATH];
				GetModuleFileNameW(NULL, buf, sizeof(buf));

				wchar_t drive_buf[_MAX_DRIVE], dir_buf[_MAX_DIR], filename_buf[_MAX_FNAME], ext_buf[_MAX_EXT];
				_wsplitpath_s(buf, drive_buf, _MAX_DRIVE, dir_buf, _MAX_DIR, NULL, 0, NULL, 0);
				_wsplitpath_s(debug_dll ? LZHAM_DEBUG_DLL_FILENAME : LZHAM_DLL_FILENAME, NULL, 0, NULL, 0, filename_buf, _MAX_FNAME, ext_buf, _MAX_EXT);
				_wmakepath_s(pModulePath, size_in_chars, drive_buf, dir_buf, filename_buf, ext_buf);
			}
#else
			static void create_module_path(char *pModulePath, int size_in_chars, bool debug_dll)
			{
				strcpy(pModulePath, debug_dll ? LZHAM_DEBUG_DLL_FILENAME : LZHAM_DLL_FILENAME);
			}
#endif

			virtual bool load()
			{
#if defined ( UWP )
#if defined (_X86)
				HRESULT hres = load(L"lzham_uwp_x86.dll");
#endif
#if defined (_X64)
				HRESULT hres = load(L"lzham_uwp_x64.dll");
#endif
#else
				HRESULT hres = load(NULL);
#endif
				return S_OK == hres;
			}

			HRESULT load(const wchar_t* pModulePath)
			{
				unload();

				wchar_t buf[MAX_PATH];
				if (!pModulePath)
				{
					create_module_path(buf, sizeof(buf) / sizeof(buf[0]), false);
					pModulePath = buf;
				}

				m_win32_error = S_OK;

#if defined(UWP)
				m_handle = LoadPackagedLibrary(pModulePath, 0);
#else
				m_handle = LoadLibrary(pModulePath);
#endif
				if (NULL == m_handle)
				{
					m_win32_error = HRESULT_FROM_WIN32(GetLastError());
					return m_win32_error;
				}

				struct
				{
					const char* pName;
					void** pFunc_ptr;
				}
				funcs[] =
				{
		  #define LZHAM_DLL_FUNC_NAME(x) { #x, (void**)&x },
		  #include <lzham_exports.inc>
		  #undef LZHAM_DLL_FUNC_NAME
				};

				const int cNumFuncs = sizeof(funcs) / sizeof(funcs[0]);

				for (int i = 0; i < cNumFuncs; i++)
				{
					if ((*funcs[i].pFunc_ptr = (void*)GetProcAddress(m_handle, funcs[i].pName)) == NULL)
					{
						unload();

						m_win32_error = cErrorMissingExport;
						return m_win32_error;
					}
				}

				int dll_ver = lzham_get_version();

				// Ensure DLL's major version is the expected version.
				if ((dll_ver >> 8U) != (LZHAM_DLL_VERSION >> 8U))
				{
					unload();

					m_win32_error = cErrorUnsupportedDLLVersion;
					return m_win32_error;
				}

				return S_OK;
			}

			virtual void unload()
			{
				if (m_handle)
				{
					FreeLibrary(m_handle);
					m_handle = NULL;
				}

				clear();

				m_win32_error = S_OK;
			}

			virtual bool is_loaded() { return m_handle != NULL; }

			HRESULT get_last_win32_error() { return m_win32_error; }

		private:
			HMODULE m_handle;
			HRESULT m_win32_error;
		};
	}
}
