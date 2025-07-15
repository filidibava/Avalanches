#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <random>
#include <ctime>
#include <string>
#include <filesystem>

using namespace std;

struct sequenza {
    int random_idx;
    int size;
    int max_value;
};

struct HeapsData {
    string filename;
    int nu;
    int rho;
    int N0;
    int length;
};

void run_simulation(const string& filename, int nu, int rho, int n_0, int length, int seed) {
    std::mt19937 generator(seed);
    std::uniform_int_distribution<int> uni(0, n_0 - 1);

    std::vector<int> urn;
    std::vector<int> flag;

    // Crea la cartella UMT_SEQUENCES se non esiste
    if (!std::filesystem::exists("UMT_SEQUENCES_LARGER_THAN_500")) {
        std::filesystem::create_directory("UMT_SEQUENCES_LARGER_THAN_500");
    }

    // Nome del file di output
    string output_filename = "UMT_SEQUENCES_LARGER_THAN_500/" + filename + "_UMT.txt";
    ofstream myfile(output_filename);

    sequenza var;

    for (int i = 0; i < n_0; i++) {
        urn.emplace_back(i);
        flag.emplace_back(0);
    }

    var.max_value = n_0;
    var.random_idx = uni(generator);
    flag[var.random_idx] = 1;

    myfile << urn[var.random_idx] << "\n";

    for (int i = 0; i < (rho - (nu + 1)); i++) {
        urn.emplace_back(urn[var.random_idx]);
        flag.emplace_back(1);
    }

    for (int i = 0; i < nu + 1; i++) {
        urn.emplace_back(var.max_value);
        flag.emplace_back(0);
        var.max_value += 1;
    }

    for (int t = 1; t < length; t++) {  // Usa length invece di steps
        var.size = urn.size();
        std::uniform_int_distribution<int> dist(0, var.size - 1);
        var.random_idx = dist(generator);

        myfile << urn[var.random_idx] << "\n";

        if (flag[var.random_idx] == 0) {
            flag[var.random_idx] = 1;
            for (int i = 0; i < nu + 1; i++) {
                urn.emplace_back(var.max_value);
                flag.emplace_back(0);
                var.max_value += 1;
            }
            for (int i = 0; i < (rho - (nu + 1)); i++) {
                urn.emplace_back(urn[var.random_idx]);
                flag.emplace_back(1);
            }
        } else {
            for (int i = 0; i < rho; i++) {
                urn.emplace_back(urn[var.random_idx]);
                flag.emplace_back(1);
            }
        }

        if (t % 100000 == 0) {
            cout << "File: " << filename << ", nu=" << nu << ", rho=" << rho << ", n_0=" << n_0 << ", t=" << t << "/" << length << "\n";
        }
    }

    myfile.close();
    cout << "Completata simulazione per " << filename << " -> " << output_filename << "\n";
}

vector<HeapsData> readHeapsResults(const string& filename) {
    vector<HeapsData> data;
    ifstream file(filename);
    string line;
    
    if (!file.is_open()) {
        cerr << "Errore: impossibile aprire il file " << filename << endl;
        return data;
    }

    // Salta la riga di intestazione
    getline(file, line);

    while (getline(file, line)) {
        istringstream iss(line);
        HeapsData item;
        
        if (iss >> item.filename >> item.nu >> item.rho >> item.N0 >> item.length) {
            data.push_back(item);
        }
    }

    file.close();
    return data;
}

int main() {
    cout << "Lettura del file heaps_analysis_results.txt...\n";
    
    vector<HeapsData> heapsData = readHeapsResults("heaps_analysis_results_larger_than_500.txt");
    
    if (heapsData.empty()) {
        cerr << "Errore: nessun dato trovato nel file o file non trovato.\n";
        return 1;
    }

    cout << "Trovate " << heapsData.size() << " sequenze da simulare.\n\n";

    for (size_t i = 0; i < heapsData.size(); i++) {
        const auto& item = heapsData[i];
        
        cout << "Simulazione " << (i+1) << "/" << heapsData.size() << ":\n";
        cout << "File: " << item.filename << ", nu=" << item.nu << ", rho=" << item.rho 
             << ", N0=" << item.N0 << ", length=" << item.length << "\n";

        // Usa il tempo corrente + indice come seed per varietà
        int seed = time(0) + i;
        
        run_simulation(item.filename, item.nu, item.rho, item.N0, item.length, seed);
        cout << "\n";
    }

    cout << "Tutte le simulazioni sono state completate!\n";
    cout << "I file sono stati salvati nella cartella UMT_SEQUENCES/\n";
    
    return 0;
}