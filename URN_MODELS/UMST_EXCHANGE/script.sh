#!/bin/bash

# Maximum number of parallel processes (adjust according to your CPU cores)
MAX_JOBS=10

# Definition of parameter triples (L, P, Q)
declare -a PARAMS=(
    "0.63 8 10"
)

# Function to execute work for a single parameter combination
execute_param() {
    local param="$1"
    read L P Q <<< "$param"  # Extract values from the triple
    
    echo "Starting processing for L=$L, P=$P, Q=$Q (PID: $$)"
    
    # Create main folder name (e.g. E091N11R20)
    MAIN_FOLDER="E$(printf "%03d" $(echo "$L * 100" | bc | cut -d. -f1))N${P}R${Q}"
    mkdir -p "$MAIN_FOLDER"
    
    # Array for child process PIDs
    declare -a child_pids=()
    
    # Loop over seed and n - parallelized for each seed/n combination
    for seed in 123456; do
        for n in 500 1000; do
            {
                echo "  Processing seed=$seed, n=$n for L=$L, P=$P, Q=$Q (Sub-PID: $$)"
                
                # Create subfolder for each n
                mkdir -p "$MAIN_FOLDER/$n"
                
                # Execute command
                ./exe -o $seed -n $n -p $P -q $Q -s 500000 -l $L
                
                # Move files to correct subfolder
                mv wr_PUST_Nin_${n}_p_${P}_q_${Q}.*_l_${L}*_seed_${seed}.txt "$MAIN_FOLDER/$n/" 2>/dev/null
                
                echo "  Completed seed=$seed, n=$n for L=$L, P=$P, Q=$Q"
            } &
            
            # Save child process PID
            child_pids+=($!)
            
            # Control number of parallel processes
            while [ ${#child_pids[@]} -ge $MAX_JOBS ]; do
                # Wait for at least one process to finish
                wait -n
                # Remove finished PIDs from array
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
    
    # Wait for all child processes of this parameter to finish
    for pid in "${child_pids[@]}"; do
        wait "$pid"
    done
    
    echo "Completed processing for L=$L, P=$P, Q=$Q"
}

# Export function to make it available to subshells
export -f execute_param

echo "Starting parallel processing with maximum $MAX_JOBS processes..."

# Array for main process PIDs
declare -a main_pids=()

# Main parallelized loop over parameters
for param in "${PARAMS[@]}"; do
    execute_param "$param" &
    main_pids+=($!)
    
    echo "Started process for parameters: $param (PID: $!)"
done

# Wait for all main processes to finish
echo "Waiting for completion of all processes..."
for pid in "${main_pids[@]}"; do
    wait "$pid"
    echo "Process $pid completed"
done

echo "Script completed! All files have been generated and organized."
