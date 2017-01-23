#!/bin/bash
route=$1
time=$2
amount=$3

start=$(date "+%s")

for asc in 0,1; do
{
    for busy in 0,1; do
    {
        for percent in 0.0,0.1,0.3,0.5,0.7,0.9,1.0; do
        {
            for ((i=0;i<amount;i++)); do
            {
                ./bin/MCM2017 ${route} ${asc} ${busy} ${percent} ${time} "${route}_${asc}_${busy}_${percent}_${i}.txt"
            }&
            done
        }&
        done
    }&
    done
}&
done
wait

now=$(date "+%s")
interval=$((now-start))

echo "finish in ${interval}"

