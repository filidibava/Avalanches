for file in *.txt; do

    if [[ -f $file ]]; then
        base=$(basename "$file")
        awk '{ n++; if(v[$1]==0)print n ; v[$1]++; }' "$file" > "nov_$base"
    fi
done
