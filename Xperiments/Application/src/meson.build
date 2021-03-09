debloated_sources = [
  'main.c',
  'debloated-window.c',
]

debloated_deps = [
  dependency('gio-2.0', version: '>= 2.50'),
  dependency('gtk+-3.0', version: '>= 3.22'),
]

gnome = import('gnome')

debloated_sources += gnome.compile_resources('debloated-resources',
  'debloated.gresource.xml',
  c_name: 'debloated'
)

executable('debloated', debloated_sources,
  dependencies: debloated_deps,
  install: true,
)
