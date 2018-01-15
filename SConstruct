env = Environment()
env.Append(CXXFLAGS = ['-std=c++11', '-pthread', '-Wall', '-g'])

libraries = ['pthread']

env.Object('build/shared/tuple.o', 'src/shared/linda/tuple.cpp')
env.Object('build/shared/confFile.o', 'src/shared/utils/confFile.cpp')
env.Object('build/shared/linda.o', 'src/shared/linda/linda.cpp')

env.Object('build/server/FileWorker.o', 'src/server/FileWorker.cpp')
env.Object('build/server/main.o', 'src/server/main.cpp')
env.Object('build/server/server.o', 'src/server/server.cpp')
serverObjects = ['build/server/main.o', 'build/server/server.o', 'build/server/FileWorker.o', 'build/shared/confFile.o', 'build/shared/tuple.o']
env.Program('build/server/server', serverObjects, LIBS = libraries)

env.Object('build/client/client.o', 'src/client/main.cpp')
env.Program('build/client/client', 'build/client/client.o', LIBS = libraries)
