project('miner', ['cpp'])

sqlite_dep = dependency('sqlite3')

cpp = meson.get_compiler('cpp')

id3_dep = cpp.find_library('id3', required : true)

add_project_arguments(
'-DBDD_DIR="' + meson.project_source_root() + '/bdd"',
language: 'cpp'
)

clasesc = [
'Minero/Filtro.cpp',
'Minero/Lector.cpp',
'Minero/Minero.cpp',
'Minero/BasedeDatos.cpp'
]      				

executable('Minero', sources : clasesc, dependencies : [id3_dep, sqlite_dep])
