#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include <ctime>
#include <string>
#include <cstdlib>

using namespace std;

struct sequenza {
    int random_idx;
    int size;
    int max_value;
};

struct SimulationParams {
    int nu;
    int rho;
    int n_0;
    int steps;
};

void run_simulation(int nu, int rho, int n_0, int seed, int steps, vector<string>& generated_files) {
    std::mt19937 generator(seed);
    std::uniform_int_distribution<int> uni(0, n_0 - 1);

    std::vector<int> urn;
    std::vector<int> flag;

    string filename = "nu" + to_string(nu) + "_rho" + to_string(rho) +
                     "_seed" + to_string(seed) + "_N0_" + to_string(n_0) + "_exchange.txt";
    
    ofstream myfile(filename);
    generated_files.push_back(filename);

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

    for (int t = 1; t < steps; t++) {
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

        if (t % 1000000 == 0) {
            cout << "nu=" << nu << ", rho=" << rho << ", n_0=" << n_0 << ", t=" << t << "\n";
        }
    }

    myfile.close();
}

void process_file_with_awk(const string& filename) {
    cout << "Processing file: " << filename << "\n";
    
    // Comando per generare heaps_
    string heaps_cmd = "awk '{if(v[$1]==0)k++; n++; v[$1]++; print n,k}' \"" + filename + "\" > \"heaps_" + filename + "\"";
    
    // Comando per generare nov_
    string nov_cmd = "awk '{ n++; if(v[$1]==0)print n ; v[$1]++; }' \"" + filename + "\" > \"nov_" + filename + "\"";
    
    cout << "Eseguendo: " << heaps_cmd << "\n";
    int result1 = system(heaps_cmd.c_str());
    if (result1 != 0) {
        cout << "Errore nell'esecuzione del comando heaps per " << filename << "\n";
    }
    
    cout << "Eseguendo: " << nov_cmd << "\n";
    int result2 = system(nov_cmd.c_str());
    if (result2 != 0) {
        cout << "Errore nell'esecuzione del comando nov per " << filename << "\n";
    }
}

int main() {
    int num_quadruples, num_runs;
    vector<SimulationParams> param_sets;
    vector<string> all_generated_files;

    cout << "Quante quadruple (nu, rho, n_0, steps) vuoi utilizzare? ";
    cin >> num_quadruples;
    
    cout << "Inserisci " << num_quadruples << " quadruple (nu, rho, n_0, steps) una alla volta:\n";
    
    for (int i = 0; i < num_quadruples; i++) {
        SimulationParams params;
        cout << "Quadrupla " << (i + 1) << ":\n";
        cout << "  nu: ";
        cin >> params.nu;
        cout << "  rho: ";
        cin >> params.rho;
        cout << "  n_0: ";
        cin >> params.n_0;
        cout << "  steps: ";
        cin >> params.steps;
        param_sets.push_back(params);
    }

    cout << "Quante run desideri per ogni quadrupla? ";
    cin >> num_runs;

    // Esegui tutte le simulazioni
    for (const auto& params : param_sets) {
        cout << "\n=== Elaborando quadrupla: nu=" << params.nu 
             << ", rho=" << params.rho 
             << ", n_0=" << params.n_0 
             << ", steps=" << params.steps << " ===\n";
        
        vector<string> current_files;
        
        for (int run = 0; run < num_runs; run++) {
            int seed = time(0) + run + (params.nu * 1000) + (params.rho * 100) + params.n_0;
            cout << "Run " << (run + 1) << "/" << num_runs 
                 << " con seed=" << seed << "...\n";
            
            run_simulation(params.nu, params.rho, params.n_0, seed, params.steps, current_files);
        }
        
        // Processa tutti i file generati per questa quadrupla
        cout << "\nProcessing dei file generati per questa quadrupla...\n";
        for (const string& filename : current_files) {
            process_file_with_awk(filename);
            all_generated_files.push_back(filename);
        }
    }

    cout << "\n=== RIEPILOGO ===\n";
    cout << "Simulazioni completate!\n";
    cout << "File generati in totale: " << all_generated_files.size() << "\n";
    cout << "Per ogni file di sequenza sono stati generati anche i corrispondenti file heaps_ e nov_\n";
    
    return 0;
}
