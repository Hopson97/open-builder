ob_common_lib = static_library('ob-common',
	'file_io.cpp',
	'debug.cpp',
	'network/net_host.cpp',
	'world/chunk.cpp',
	'world/chunk_manager.cpp',
	'world/coordinate.cpp',
	dependencies: [threads_dep, sfml_deps, dl_dep],
	include_directories: includes,
	cpp_args: [cpp_args])

ob_common = declare_dependency(link_with: ob_common_lib)
