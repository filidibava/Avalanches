#!/bin/bash

# To parallelize choose the number of jobs
MAX_JOBS=10

# Declare eta,nu,rho
declare -a PARAMS=(
    "0.2 4 3"
)

# Funzione per eseguire il lavoro per una singola combinazione
execute_param() {
    local param="$1"
    read L P Q <<< "$param"  # Estrae i valori della tripla
    
    echo "Iniziando elaborazione per L=$L, P=$P, Q=$Q (PID: $$)"
    
    # Crea il nome della cartella principale (es. E091N11R20)
    MAIN_FOLDER="E$(printf "%03d" $(echo "$L * 100" | bc | cut -d. -f1))N${P}R${Q}"
    mkdir -p "$MAIN_FOLDER"
    
    # Array per i PID dei processi figli
    declare -a child_pids=()
    
    # Loop over the seeds and N_0
    for seed in 123456; do
        for n in 60 70 80 90 ; do
            {
                echo "  Elaborando seed=$seed, n=$n per L=$L, P=$P, Q=$Q (Sub-PID: $$)"
                
                # Creazione della sottocartella per ogni n
                mkdir -p "$MAIN_FOLDER/$n"
                
                # Esecuzione del comando
                ./exe -o $seed -n $n -p $P -q $Q -s 500000 -l $L
                
                # Spostamento dei file nella sottocartella corretta
                mv wr_PUST_Nin_${n}_p_${P}_q_${Q}.*_l_${L}*_seed_${seed}.txt "$MAIN_FOLDER/$n/" 2>/dev/null
                
                echo "  Completato seed=$seed, n=$n per L=$L, P=$P, Q=$Q"
            } &
            
            # Salva il PID del processo figlio
            child_pids+=($!)
            
            # Controllo del numero di processi paralleli
            while [ ${#child_pids[@]} -ge $MAX_JOBS ]; do
                # Aspetta che almeno un processo termini
                wait -n
                # Rimuovi i PID terminati dall'array
                local new_pids=()
                for pid in "${child_pids[@]}"; do
                    if kill -0 "$pid" 2>/dev/null; then
                        new_pids+=("$pid")
                    fi
                done
                child_pids=("${new_pids[@]}")
            done
        done
    done
    
    # Aspetta che tutti i processi figli di questo parametro terminino
    for pid in "${child_pids[@]}"; do
        wait "$pid"
    done
    
    echo "Completata elaborazione per L=$L, P=$P, Q=$Q"
    
    # Creazione dello script novs.sh dentro la cartella principale
    cat << 'EOF' > "$MAIN_FOLDER/novs.sh"
#!/bin/bash
    for file in wr*.txt; do
        if [[ -f $file ]]; then
            base=$(basename "$file")
            awk '{ n++; if(v[$1]==0)print n ; v[$1]++; }' "$file" > "nov_$base"
        fi
    done
EOF
    
    # Dare permessi di esecuzione allo script novs.sh
    chmod +x "$MAIN_FOLDER/novs.sh"
}

# Esportare la funzione per renderla disponibile ai subshell
export -f execute_param

echo "Avviando elaborazione parallela con massimo $MAX_JOBS processi..."

# Array per i PID dei processi principali
declare -a main_pids=()

# Loop principale parallelizzato sui parametri
for param in "${PARAMS[@]}"; do
    execute_param "$param" &
    main_pids+=($!)
    
    echo "Avviato processo per parametri: $param (PID: $!)"
done

# Aspetta che tutti i processi principali terminino
echo "Attendendo completamento di tutti i processi..."
for pid in "${main_pids[@]}"; do
    wait "$pid"
    echo "Processo $pid completato"
done

echo "Script completato! Tutti i file sono stati generati e organizzati."

# Opzionale: esegui tutti gli script novs.sh in parallelo
echo "Vuoi eseguire tutti gli script novs.sh ora? (y/n)"
read -r response
if [[ "$response" =~ ^[Yy]$ ]]; then
    echo "Eseguendo script novs.sh in parallelo..."
    for param in "${PARAMS[@]}"; do
        read L P Q <<< "$param"
        MAIN_FOLDER="E$(printf "%03d" $(echo "$L * 100" | bc | cut -d. -f1))N${P}R${Q}"
        if [[ -f "$MAIN_FOLDER/novs.sh" ]]; then
            {
                cd "$MAIN_FOLDER" && ./novs.sh
                echo "Completato novs.sh per $MAIN_FOLDER"
            } &
        fi
    done
    wait
    echo "Tutti gli script novs.sh completati!"
fi
