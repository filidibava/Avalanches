#!/bin/bash

# Definizione delle triple (L, P, Q)
declare -a PARAMS=(
    "0.50 10 10"
    "0.50 20 20"
    "0.50 40 40"
    "0.50 80 80"
    "0.83 6 10"
    "0.83 12 20"
    "0.83 24 40"
    "0.83 48 80"
)

# Loop sulle diverse combinazioni di L, P, Q
for param in "${PARAMS[@]}"; do
    read L P Q <<< "$param"  # Estrae i valori della tripla
    
    # Costruisce il nome della cartella principale
    MAIN_FOLDER="E$(printf "%03d" $(echo "$L * 100" | bc | cut -d. -f1))N${P}R${Q}"

    # Controlla se la cartella esiste
    if [[ -d "$MAIN_FOLDER" ]]; then
        # Creazione dello script novs.sh dentro la cartella principale
        cat << 'EOF' > "$MAIN_FOLDER/novs.sh"
#!/bin/bash
for n in 1000 5000 10000; do
    if [[ -d "$n" ]]; then  # Controlla se la sottocartella esiste
        cd "$n" || continue  # Entra nella sottocartella di Nin
        for file in wr*.txt; do
            if [[ -f $file ]]; then
                base=$(basename "$file")
                awk '{ n++; if(v[$1]==0)print n ; v[$1]++; }' "$file" > "nov_$base"
            fi
        done
        cd ..  # Torna alla cartella principale
    fi
done
EOF

        # Dare permessi di esecuzione allo script novs.sh
        chmod +x "$MAIN_FOLDER/novs.sh"

        # Eseguire lo script novs.sh
        (cd "$MAIN_FOLDER" && ./novs.sh)
    else
        echo "Cartella $MAIN_FOLDER non trovata, saltata."
    fi
done

echo "Script novs.sh creati ed eseguiti nelle cartelle esistenti."
