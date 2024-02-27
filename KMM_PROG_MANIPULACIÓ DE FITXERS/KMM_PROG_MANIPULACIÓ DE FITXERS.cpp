#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// Estructura per al capçalera GIF
struct GIFHeader {
    char signature[3]; // Signatura del GIF
    char version[3];   // Versió del GIF
};

// Estructura per al descritor de pantalla lògica
struct LogicalScreenDescriptor {
    uint16_t width;                // Amplada de la imatge
    uint16_t height;               // Alçada de la imatge
    uint8_t packedFields;          // Camps empaquetats
    uint8_t backgroundColorIndex;  // Índex del color de fons
    uint8_t pixelAspectRatio;      // Relació d'aspecte del píxel
};

// Funció per mostrar la informació del GIF
void displayGIFInfo(const GIFHeader& header, const LogicalScreenDescriptor& lsd, int colorTableSize) {
    cout << "Versio: " << header.version[0] << header.version[1] << header.version[2] << endl;
    cout << "Amplada de la imatge: " << lsd.width << " pixels" << endl;
    cout << "Alçada de la imatge: " << lsd.height << " pixels" << endl;
    cout << "Quantitat de colors: " << (1 << ((lsd.packedFields & 0x07) + 1)) << endl;
}

// Funció per duplicar un fitxer GIF
void duplicateGIF(const char* inputFileName, const char* outputFileName) {
    ifstream inputFile(inputFileName, ios::binary);      // Fitxer d'entrada en mode binari
    ofstream outputFile(outputFileName, ios::binary);    // Fitxer de sortida en mode binari

    if (!inputFile || !outputFile) {  // Comprovar si els fitxers es poden obrir
        cerr << "Error: No es pot obrir el fitxer." << endl;
        return;
    }

    vector<char> buffer(4096); // 4 KB de memòria intermèdia
    while (!inputFile.eof()) { // Llegir fins al final del fitxer
        inputFile.read(buffer.data(), buffer.size()); // Llegir del fitxer d'entrada al buffer
        outputFile.write(buffer.data(), inputFile.gcount()); // Escriure del buffer al fitxer de sortida
    }

    cout << "Fitxer duplicat amb exit." << endl;
}

int main() {
    string fileName;
    cout << "Introdueix el nom del fitxer GIF: ";
    cin >> fileName;

    ifstream file(fileName, ios::binary); // Obrir el fitxer en mode binari
    if (!file) {
        cerr << "No s'ha pogut obrir el fitxer." << endl;
        return 1;
    }

    GIFHeader header;
    file.read(reinterpret_cast<char*>(&header), sizeof(GIFHeader)); // Llegir el capçalera del GIF

    if (strncmp(header.signature, "GIF", 3) != 0) { // Comprovar si el fitxer té una signatura GIF vàlida
        cerr << "El fitxer no te un format GIF valid." << endl;
        return 1;
    }

    LogicalScreenDescriptor lsd;
    file.read(reinterpret_cast<char*>(&lsd), sizeof(LogicalScreenDescriptor)); // Llegir el descritor de pantalla lògica

    int colorTableSize = (1 << ((lsd.packedFields & 0x07) + 1)); // Calcular la grandària de la taula de colors

    displayGIFInfo(header, lsd, colorTableSize); // Mostrar la informació del GIF

    char choice;
    cout << "Vols duplicar el fitxer GIF? (S/N): ";
    cin >> choice;

    if (choice == 's' || choice == 'S') { // Demanar si es vol duplicar el fitxer
        string outputFileName;
        cout << "Introdueix el nom del fitxer de sortida: ";
        cin >> outputFileName;
        duplicateGIF(fileName.c_str(), outputFileName.c_str()); // Duplicar el fitxer GIF
    }

    return 0;
}
