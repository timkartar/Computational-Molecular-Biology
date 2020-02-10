#!/bin/bash
mkdir rabbit
cd rabbit
wget ftp://ftp.ensembl.org/pub/release-99/fasta/oryctolagus_cuniculus/dna/Oryctolagus_cuniculus.OryCun2.0.dna.*
gunzip Oryctolagus_cuniculus.OryCun2.0.dna.*
cat Oryctolagus_cuniculus.OryCun2.0.dna.* > rabbit.fasta
cd ../

mkdir human
cd human
wget ftp://ftp.ensembl.org/pub/release-99/fasta/homo_sapiens/dna/Homo_sapiens.GRCh38.dna.*
gunzip Homo_sapiens.GRCh38.dna.*
cat Homo_sapiens.GRCh38.dna.* > human.fasta
cd ../


