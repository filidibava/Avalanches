#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <random>
#include <cstdlib>
#include <cmath>
#include <ctime>

using namespace std;

struct sequenza {
    int random_idx;
    int size;
    int max_value;
};

int main() {
    int num_simulations;
    
    cout << "Quante simulazioni vuoi eseguire? ";
    cin >> num_simulations;
    
    for (int sim = 0; sim < num_simulations; sim++) {
        int nu, rho, steps, n_0, num_runs;
        
        cout << "\n=== SIMULAZIONE " << (sim + 1) << " di " << num_simulations << " ===\n";
        cout << "Inserisci il valore di nu: ";
        cin >> nu;
        cout << "Inserisci il valore di rho: ";
        cin >> rho;
        cout << "Inserisci il valore di n_0: ";
        cin >> n_0;
        cout << "Inserisci la lunghezza della sequenza (steps): ";
        cin >> steps;
        cout << "Quante run vuoi per questa simulazione? ";
        cin >> num_runs;
        
        for (int run = 0; run < num_runs; run++) {
            int seed = time(0) + sim * 1000 + run;
            mt19937 generator(seed);
            uniform_int_distribution<int> uni(0, nu);
            
            vector<int> urn;
            vector<int> flag;
            urn.reserve(steps * (nu + rho));
            flag.reserve(steps * (nu + rho));
            
            ofstream myfile("sim" + to_string(sim+1) + "_nu" + to_string(nu) + "_rho" + to_string(rho) + "_seed" + to_string(seed) + "_N0_" + to_string(n_0) + "_run" + to_string(run) + ".txt");
            sequenza var;
            
            for (int i = 0; i < n_0; i++) {
                urn.emplace_back(i);
                flag.emplace_back(0);
            }
            var.max_value = n_0;
            uniform_int_distribution<int> index_dist(0, n_0 - 1);
            var.random_idx = index_dist(generator);
            flag[var.random_idx] = 1;
            myfile << urn[var.random_idx] << "\n";
            
            for (int i = 0; i < rho; i++) {
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
                uniform_int_distribution<int> index_dist(0, var.size - 1);
                var.random_idx = index_dist(generator);
                myfile << urn[var.random_idx] << "\n";
                
                if (flag[var.random_idx] == 0) {
                    flag[var.random_idx] = 1;
                    for (int i = 0; i < nu + 1; i++) {
                        urn.emplace_back(var.max_value);
                        flag.emplace_back(0);
                        var.max_value += 1;
                    }
                    for (int i = 0; i < rho; i++) {
                        urn.emplace_back(urn[var.random_idx]);
                        flag.emplace_back(1);
                    }
                } else {
                    for (int i = 0; i < rho; i++) {
                        urn.emplace_back(urn[var.random_idx]);
                        flag.emplace_back(1);
                    }
                }
                cout << "sim=" << (sim+1) << " t=" << t << " (run=" << run << ", n_0=" << n_0 << ")\n";
            }
            
            vector<int>().swap(urn);
            vector<int>().swap(flag);
            myfile.close();
        }
        
        cout << "Simulazione " << (sim + 1) << " completata!\n";
    }
    
    cout << "\nTutte le simulazioni sono state completate!\n";
    return 0;
}
