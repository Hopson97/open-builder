project('open-builder', 'c', 'cpp', version: '1.0', default_options: ['cpp_std=c++17'])

includes = include_directories('deps/', 'src/common/')

threads_dep = dependency('threads')
sfml_deps = [
	dependency('sfml-audio'),
	dependency('sfml-network'),
	dependency('sfml-graphics'),
	dependency('sfml-window'),
	dependency('sfml-system')]
glm_dep = dependency('glm')
dl_dep = meson.get_compiler('cpp').find_library('dl', required: false)

cpp_args = []

if meson.get_compiler('cpp').get_argument_syntax() == 'msvc'
	cpp_args = ['/W4', '/WX']
elif meson.get_compiler('cpp').get_argument_syntax() == 'gcc'
	cpp_args = ['-Wall', '-Wextra', '-pedantic', '-Wshadow', '-Wpointer-arith']
endif

subdir('deps')
subdir('src/common/common')
subdir('src/client/')
subdir('src/server/')

executable('open-builder',
	'src/main.cpp',
	dependencies: [
		ob_common, ob_client, ob_server, glad, enet, dl_dep],
	include_directories: includes)
