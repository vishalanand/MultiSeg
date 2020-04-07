#!/bin/bash
# Author: Vishal Anand <vishal.anand@columbia.edu>

#echo "\n" > /dev/stderr
#echo "\nCount " $# > /dev/stderr
#echo "\n0" $0 > /dev/stderr
#echo "\n1" $1 > /dev/stderr
#echo "\n2" $2 > /dev/stderr
#echo "\n3" $3 > /dev/stderr
#echo "\n4" $4 > /dev/stderr
#echo "\n5" $5 > /dev/stderr
#echo "\n6" $6 > /dev/stderr
#echo "\n7" $7 > /dev/stderr
if [[ $# -eq 7 ]]; then
    #echo "7" &> /dev/stderr
    emb=$1
    dic=$2
    prefix=$3
    out=$4
    merge_two_embeddings=$5
    embedding_pr=$6
    embedding_en=$7

#    python3 revised_computeWordTranslation.py \
#      --emb $emb \
#      --dic $dic \
#      --prefix $prefix \
#      --out $out \
#      --merge_two_embeddings $merge_two_embeddings \
#      --embedding_pr $embedding_pr \
#      --embedding_en $embedding_en
    python3 computeWordTranslation.py \
      --emb $emb \
      --dic $dic \
      --prefix $prefix \
      --merge_two_embeddings $merge_two_embeddings \
      --embedding_pr $embedding_pr \
      --embedding_en $embedding_en
elif [[ $# -eq 8 ]]; then
    #echo "8" &> /dev/stderr
    emb=$1
    dic=$2
    prefix=$3
    out=$4
    merge_two_embeddings=$5
    embedding_pr=$6
    embedding_en=$7
    case=$8

#    python3 revised_computeWordTranslation.py \
#      --emb $emb \
#      --dic $dic \
#      --prefix $prefix \
#      --out $out \
#      --merge_two_embeddings $merge_two_embeddings \
#      --embedding_pr $embedding_pr \
#      --embedding_en $embedding_en \
#      --case True
    python3 computeWordTranslation.py \
      --emb $emb \
      --dic $dic \
      --prefix $prefix \
      --merge_two_embeddings $merge_two_embeddings \
      --embedding_pr $embedding_pr \
      --embedding_en $embedding_en \
      --case True
else
    echo "`basename $0` modelFile modelFormat lang"
    echo "lang: en or de"
    exit
fi

#if [ -d ./data/crosslingual ]   # for file "if [-f /home/rama/file]"
#then
#    :
#else
#    echo "Dir created for " &> /dev/stderr
#    echo $output
#    mkdir
#fi


rm $emb

