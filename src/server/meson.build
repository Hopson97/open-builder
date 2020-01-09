ob_server_lib = static_library('ob-server',
	'server_engine.cpp',
	'network/server.cpp',
	'world/terrain_generation.cpp',
	dependencies: [glad, threads_dep, ob_common, sfml_deps, dl_dep],
	include_directories: includes,
	cpp_args: [cpp_args])

ob_server = declare_dependency(link_with: ob_server_lib)
