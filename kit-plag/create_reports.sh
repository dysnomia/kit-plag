#!/bin/sh

./kit-plag --references=test/refs_goethe.txt --queries=test/queries_goethe.txt
./kit-plag --references=test/refs_raven.txt --queries=test/queries_raven.txt
./kit-plag --references=test/refs_samuel.txt --queries=test/queries_samuel.txt
./kit-plag --references=test/refs_news.txt --queries=test/queries_news.txt
./kit-plag --references=test/refs_big.txt --queries=test/queries_big.txt --visualize=0
