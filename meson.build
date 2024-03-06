project('delta-crdt-decomposition', 'cpp',
        default_options : ['cpp_std=c++20', 'warning_level=3', 'werror=true'],
        license : 'BSL-1.0',
        version : '0.0.1')

# no support with the microsoft compiler (for now)
if meson.get_compiler('cpp').get_id() == 'msvc'
    error('MSVC is currently not supported, consider using either gcc or clang')
endif

# compiler flags
add_project_arguments('-Wshadow', '-Wunused', '-Wconversion', '-Wdouble-promotion',
                      '-Wnon-virtual-dtor', '-Woverloaded-virtual', '-Wformat=2',
                      '-Wold-style-cast', '-Wcast-align', '-Wimplicit-fallthrough',
                      language : 'cpp')

# include directory with the headers
incdir = include_directories('include')

# unit tests
test_gset = executable('gset.out', 'tests/test_gset.cpp',
                       include_directories : [incdir])
test('Grow-Only Set Test', test_gset, suite : 'anonymous')

test_2pset = executable('2pset.out', 'tests/test_2pset.cpp',
                        include_directories : [incdir])
test('Two-Phase Set Test', test_2pset, suite : 'anonymous')

test_lwwset = executable('lwwset.out', 'tests/test_lwwset.cpp',
                         include_directories : [incdir])
test('Last-Writer-Wins Add-Wins Set Test', test_lwwset, suite : 'anonymous')

