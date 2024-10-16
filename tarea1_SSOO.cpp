#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <mutex>
#include <atomic>

// Estructura para almacenar información de cada auto
struct Auto {
    std::string id;
    int distancia_recorrida;
};

// Mutex para sincronizar el acceso a la consola y al vector de resultados
std::mutex mtx;

// Vector para almacenar el orden de llegada
std::vector<std::string> resultados;

// Función que simula el comportamiento de cada auto
void correr_auto(Auto &auto_actual, int distancia_total) {
    // Inicializar generadores aleatorios
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist_metros(1, 10);      // Avance entre 1 y 10 metros
    std::uniform_int_distribution<> dist_tiempo(100, 500);   // Tiempo de espera entre 100 y 500 ms

    while (auto_actual.distancia_recorrida < distancia_total) {
        // Avanzar una distancia aleatoria
        int avance = dist_metros(gen);
        auto_actual.distancia_recorrida += avance;
        if (auto_actual.distancia_recorrida > distancia_total) {
            auto_actual.distancia_recorrida = distancia_total;
        }

        // Esperar un tiempo aleatorio
        int tiempo_espera = dist_tiempo(gen);
        std::this_thread::sleep_for(std::chrono::milliseconds(tiempo_espera));

        // Mostrar el progreso
        {
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << auto_actual.id << " ha recorrido " 
                      << auto_actual.distancia_recorrida << " metros." << std::endl;
        }
    }

    // Registrar el orden de llegada
    {
        std::lock_guard<std::mutex> lock(mtx);
        resultados.push_back(auto_actual.id);
    }
}

int main() {
    int M, N;

    // Solicitar al usuario los parámetros de la carrera
    std::cout << "Ingrese la distancia total de la carrera (M metros): ";
    std::cin >> M;
    std::cout << "Ingrese el número de autos participantes (N): ";
    std::cin >> N;

    // Crear los autos
    std::vector<Auto> autos(N);
    for(int i = 0; i < N; ++i){
        autos[i].id = "Auto" + std::to_string(i+1);
        autos[i].distancia_recorrida = 0;
    }

    // Vector para almacenar las hebras
    std::vector<std::thread> hebras;

    // Iniciar las hebras para cada auto
    for(int i = 0; i < N; ++i){
        hebras.emplace_back(correr_auto, std::ref(autos[i]), M);
    }

    // Esperar a que todas las hebras terminen
    for(auto &hebra : hebras){
        if(hebra.joinable()){
            hebra.join();
        }
    }

    // Mostrar el orden de llegada
    std::cout << "\nResultados de la carrera:\n";
    for(int i = 0; i < resultados.size(); ++i){
        std::cout << i+1 << "° lugar: " << resultados[i] << std::endl;
    }

    return 0;
}