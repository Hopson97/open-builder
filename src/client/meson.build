ob_client_lib = static_library('ob-client',
	'input/keyboard.cpp',
	'gl/vertex_array.cpp',
	'gl/shader.cpp',
	'gl/textures.cpp',
	'gl/gl_errors.cpp',
	'gl/primitive.cpp',
	'network/client_commands.cpp',
	'client.cpp',
	'maths.cpp',
	'client_engine.cpp',
	'window.cpp',
	'world/chunk_mesh_generation.cpp',
	'world/chunk_mesh.cpp',
	dependencies: [threads_dep, sfml_deps, dl_dep, ob_common],
	include_directories: includes,
	cpp_args: [cpp_args])

ob_client = declare_dependency(link_with: ob_client_lib)
