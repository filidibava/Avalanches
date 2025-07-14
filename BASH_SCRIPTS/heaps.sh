for file in *.txt; do

    if [[ -f $file ]]; then
        base=$(basename "$file")
        awk '{if(v[$1]==0)k++; n++; v[$1]++; print n,k}' "$file" > "heaps_$base"
    fi
done
