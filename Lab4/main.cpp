#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cmath>
#include <thread>
#include <mutex>
#include <filesystem>
#include <chrono>

namespace fs = std::filesystem;

const std::vector<std::string> keywords = {"MANKIND", "PURPOSE", "DIVINE"};
std::vector<std::unordered_map<std::string, int>> document_vectors;
std::vector<std::string> document_names;
std::mutex mtx;

// Convierte una palabra a mayúsculas
std::string to_upper(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

// Procesa un documento y cuenta las palabras clave
void process_document(const std::string& filename, int doc_index) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << filename << std::endl;
        return;
    }

    std::unordered_map<std::string, int> word_count;
    std::string word;

    while (file >> word) {
        word = to_upper(word);
        if (std::find(keywords.begin(), keywords.end(), word) != keywords.end()) {
            word_count[word]++;
        }
    }

    std::lock_guard<std::mutex> lock(mtx);
    document_vectors[doc_index] = word_count;
}

// Calcula y muestra los 5 documentos más cercanos por palabra clave
void rank_documents() {
    std::unordered_map<std::string, std::vector<std::pair<std::string, double>>> top_documents;

    for (const auto& keyword : keywords) {
        std::vector<std::pair<std::string, double>> similarities;
        for (size_t i = 0; i < document_vectors.size(); i++) {
            if (document_vectors[i].count(keyword)) {
                similarities.emplace_back(document_names[i], document_vectors[i][keyword]);
            }
        }

        std::sort(similarities.begin(), similarities.end(), [](const auto& a, const auto& b) {
            return a.second > b.second;  // Orden descendente
        });

        std::vector<std::string> unique_docs;
        for (const auto& [doc_name, score] : similarities) {
            std::string base_name = doc_name.substr(0, doc_name.find("_copy"));  
            if (std::find(unique_docs.begin(), unique_docs.end(), base_name) == unique_docs.end()) {
                unique_docs.push_back(base_name);
                top_documents[keyword].emplace_back(doc_name, score);
                if (top_documents[keyword].size() == 5) break;
            }
        }
    }

    for (const auto& [keyword, docs] : top_documents) {
        std::cout << keyword << "\n";
        if (docs.empty()) {
            std::cout << "- No se encontraron documentos relevantes\n";
        }
        for (const auto& [doc, _] : docs) {
            std::cout << "- " << doc << "\n";
        }
        std::cout << std::endl;
    }
}

int main() {
    auto start_time = std::chrono::high_resolution_clock::now(); // Iniciar medición de tiempo

    std::string folder_path = "Documentos/";  
    std::vector<std::string> files;

    for (const auto& entry : fs::directory_iterator(folder_path)) {
        if (entry.path().extension() == ".txt") {
            files.push_back(entry.path().string());
        }
    }

    if (files.empty()) {
        std::cerr << "No se encontraron archivos .txt en " << folder_path << std::endl;
        return 1;
    }

    document_vectors.resize(files.size());
    document_names = files;

    std::vector<std::thread> threads;
    for (size_t i = 0; i < files.size(); i++) {
        threads.emplace_back(process_document, files[i], i);
    }

    for (auto& th : threads) {
        th.join();
    }

    auto end_time = std::chrono::high_resolution_clock::now(); // Fin de medición de tiempo
    double elapsed_time = std::chrono::duration<double>(end_time - start_time).count();

    std::cout << "Procesamiento completado, mostrando resultados...\n";
    rank_documents();

    // Mostrar información de rendimiento
    unsigned int max_threads = std::thread::hardware_concurrency();
    std::cout << "Número máximo de hilos permitidos: " << max_threads << std::endl;
    std::cout << "Threads usados: " << threads.size() << std::endl;
    std::cout << "Tiempo de ejecución: " << elapsed_time << " segundos\n";

    return 0;
}
