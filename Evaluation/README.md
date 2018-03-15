**evaluate_forced_aligner.py**  
This script evaluates all alignments in a folder("fa" as in forced alignment) with regard to reference alignments in another folder ("target"). The absolute paths to these alignment files need to be added in main() and the names of corresponding alignments have to be identical. The alignments have to be in the following format:
_start time_  
_end time_   
"_label_"  
_start time_  
_end time_   
"_label_"  
...  
Textfiles with word alignments on this format can be extracted from standard TextGrid files using the script extractTextGridInfo.py (more info below).

Output:
- A text file called "non_matching_files.txt" containing absolute paths to all
fa-files that did not match the target file in terms of labels.
- Terminal output for each fa-file:
        1) Average boundary error (ms)
        2) Alignment accuracy for the chosen tolerance level(s) (%)
            = what percentage of the automatically labeled boundaries are within
            a given time tolerance (threshold) of the manually labeled boundaries.
        3) %-match
            = the percentage of the automatically labeled boundaries overlapping
            the midpoints of the corresponding manually labeled boundaries.

All metrics are computed based on only the files having a set of labels matching the target file.

Usage:  $ python evaluate_forced_aligner.py
        This runs a small example. Change the paths in main() to run on your own alignments.
