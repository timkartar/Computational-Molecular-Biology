Assignment 1 (Math578A)
Implementation of Boyer-Moore Algorithm by Raktim Mitra
--------------------------------------------------------

compilation and run instruction:

    gcc boyer_moore.c  -o boyer_moore; ./boyer_moore <PATTERN> <PATH TO FASTA>

    gcc boyer_moore_count.c  -o boyer_moore_count; ./boyer_moore_count <PATTERN> <PATH TO FASTA>




Notes: 

    1. Command used to make genomes uppercase: tr '[:lower:]' '[:upper:]' < human_lower.fasta > human_upper.fasta

    2. Runtime reported by boyer_mooore_count reports runtime of BOyer-Moore algorithm without preprocessing and data loading. 

    3. link to rabbit genome : https://www.ncbi.nlm.nih.gov/genome/?term=Oryctolagus+cuniculus

    4. link to HG38 genome used : https://www.ncbi.nlm.nih.gov/genome/?term=Homo+sapiens

    5. Random 50 base pair ALU sequence used : GGCGGGCAGATCATGAGGTCAGGAGATCGAGACCATCCTGGCTAACACGG

    6. All result reported as an answer to Q1 in the report.
    
    7. boyer_moore_count.c contains detailed comments of each part of the code. 
