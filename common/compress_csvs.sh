#!/bin/zsh

total_count=0
current_count=1

for csv in ./**/*.csv; do
    total_count=$((${total_count} + 1))
done

for csv in ./**/*.csv; do
    echo "File ${current_count} of ${total_count}: ${csv}"

    echo "\tgzip ${csv}"

    gzip ${csv}

    current_count=$((${current_count} + 1))
done
