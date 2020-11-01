project('space-escape', 'cpp')
compiler = meson.get_compiler('cpp')
libGLU = compiler.find_library('GLU')
executable('space-escape',
           'asteroid.cpp',
           'camera.cpp',
           'drawOFF.cpp',
           'frame.cpp',
           'light.cpp',
           'main.cpp',
           'object-3d.cpp',
           'shield.cpp',
           'ship.cpp',
           dependencies: [
            dependency('glut'),
            libGLU,
            dependency('OpenGL')
           ]
)
