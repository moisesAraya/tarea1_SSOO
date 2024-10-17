# Definir el compilador y las banderas
CXX = g++
CXXFLAGS = -std=c++11 -pthread -Wall

# Regla principal para compilar el programa
all: tarea1_SSOO

# Compilar el archivo ejecutable
tarea1_SSOO: tarea1_SSOO.o
	$(CXX) $(CXXFLAGS) -o tarea1_SSOO tarea1_SSOO.o

# Compilar el archivo objeto
tarea1_SSOO.o: tarea1_SSOO.cpp
	$(CXX) $(CXXFLAGS) -c tarea1_SSOO.cpp

# Limpiar los archivos generados
clean:
	del *.o tarea1_SSOO.exe
