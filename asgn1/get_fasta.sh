#!/bin/bash
mkdir rabbit
cd rabbit
wget ftp://ftp.ensembl.org/pub/release-99/fasta/oryctolagus_cuniculus/dna/Oryctolagus_cuniculus.OryCun2.0.dna.*
gunzip Oryctolagus_cuniculus.OryCun2.0.dna.*
cat Oryctolagus_cuniculus.OryCun2.0.dna.* > rabbit.fasta
cd ../
