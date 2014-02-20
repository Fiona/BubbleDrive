# Setup
import os
env = Environment()

# Tools
def walk_subdirs(path_dir) :  
    list = [name for name in os.listdir(path_dir) if os.path.isdir(os.path.join(path_dir, name)) and name[0] != '.']
    list.sort()
    return list

# Libraries
env.Append(CPPPATH = ['/usr/include/freetype2/'])
env.Append(LIBS = ['GL', 'GLU', 'GLEW', 'sfml-system', 'sfml-window', 'sfml-graphics', 'sfml-audio', 'freetype'])

# List of sources
dir_list = walk_subdirs('code')

sources = ['code/main.cpp']
for dir in dir_list :
  sources += Glob(os.path.join('code', dir, '*.cpp'))

# Switch to debug
debug = ARGUMENTS.get('debug', 0)
if int(debug):
   env.Append(CCFLAGS = '-g')
else:
   env.Append(CCFLAGS = '-O')

# Build executable
object_list = env.Object(source = sources)
main_executable = env.Program(target = 'bubbledrive', source = object_list)

Default(main_executable)

