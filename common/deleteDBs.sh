#!/bin/zsh

total_count=0
current_count=1

for db in ./**/test.db; do
    total_count=$((${total_count} + 1))
done

for db in ./**/test.db; do
    echo "File ${current_count} of ${total_count}: ${db}"

    echo "\trm ${db}"

    rm ${db}

    current_count=$((${current_count} + 1))
done
