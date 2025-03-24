#include <windows.h>
#include <iostream>
#include <vector>
#include <string>

// Función para convertir std::string a std::wstring
std::wstring StringToWString(const std::string& str) {
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    std::wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], size_needed);
    return wstr;
}

int main() {
    std::vector<std::string> filenames = {
        "7 Tablets of Creation vol 2",
        "7 Tablets of Creation",
        "Advaita_Vedanta",
        "Aryan Sun Myths - Charles Morris (1889)",
        "augustine city of god and christian doctrine",
        "Autobiography-of-a-Yogi-by-Paramhansa-Yogananda",
        "Bhagavad Gita",
        "Bible_King_James_Version",
        "book of illumination kaballah text",
        "buddhidm in its connexion with brahmanism",
        "Collected Fruits of Occult Teaching by A.P.Sinnett (1920)",
        "epistle to the son of the wolf",
        "Hidden Nature - The Startling Insights of Viktor Schauberger - by Alick Bartholomew",
        "Hildegard Writings",
        "History of Zoroastrianism - M.N. Dhalla",
        "Kaivalya_Upanishad",
        "kitab i ilqan book of certitude",
        "Knowledge of the Higher Worlds - by Rudolf Steiner",
        "Kularnava-Tantra",
        "Kybalion",
        "lob",
        "Machine_Super_Intelligence",
        "nature-gods",
        "Occult_Theocracy",
        "Popol Vuh",
        "sbe10",
        "sbe32 (1)",
        "sbe46",
        "Secret Teachings of the Society of Jesus",
        "The Chaldean Account of Genesis - by George Smith",
        "The Kitab I Aqdas",
        "tpol",
        "Vedanta-sutras_with_the_commentary_by_Ramanuja_(Thibaut)_v1",
        "vendanta sutras patanjali",
        "wov",
        "yoga sutras patanjali"
    };

    std::string directory = "Documentos\\"; // Carpeta donde están los archivos

    for (const std::string& file : filenames) {
        std::string sourcePath = directory + file + ".txt";
        std::wstring wSourcePath = StringToWString(sourcePath);

        for (int i = 1; i <= 697; i++) {
            std::string destinationPath = directory + file + "_copy_" + std::to_string(i) + ".txt";
            std::wstring wDestinationPath = StringToWString(destinationPath);

            if (!CopyFile(wSourcePath.c_str(), wDestinationPath.c_str(), FALSE)) {
                std::cerr << "Error al copiar: " << sourcePath << " a " << destinationPath << std::endl;
            }
            else {
                std::cout << "Copiado: " << destinationPath << std::endl;
            }
        }
    }

    std::cout << "Proceso finalizado." << std::endl;
    return 0;
}
