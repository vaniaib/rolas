project('miner', ['cpp'])
cpp = meson.get_compiler('cpp')
id3_dep = cpp.find_library('id3', required : true)
clasesc = [
	'Minero/Filtro.cpp',
       'Minero/Lector.cpp',
       'Minero/Minero.cpp'
       ]      				
   
executable('Minero', sources : clasesc, dependencies : id3_dep)
