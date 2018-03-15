# General information
Created by miastra in March 2018
Repository link: https://github.com/miastra/SwedishForcedAligner

# evaluate_forced_aligner.py
This script evaluates all alignments in a folder (here mostly called "fa" as in forced alignment) with regard to reference alignments in another folder ("target"). The absolute paths to these folders need to be added in main() and the names of corresponding alignments have to be identical. The alignments have to be in the following format:  
_start time_  
_end time_   
"_label_"  
_start time_  
_end time_   
"_label_"  
...  
Textfiles with word alignments on this format can be extracted from standard TextGrid files using the script textgrid_2_textfile.py (more info below).

**Output:**
- A text file called "non_matching_files.txt" containing absolute paths to all
fa-files whose labels did not match the labels in the target file.
- Terminal output for each fa-file including:  
1. Average boundary error (ms)  
2. Alignment accuracy for the chosen tolerance level(s) (%)  
            = what percentage of the automatically labeled boundaries are within
            a given time tolerance (threshold) of the manually labeled boundaries.
3. %-match  
            = the percentage of the automatically labeled boundaries overlapping
            the midpoints of the corresponding manually labeled boundaries.

All metrics are computed based on only the files having a set of labels matching the target file.

**Usage:**  
$ python evaluate_forced_aligner.py  
This runs a small example. Change the paths in main() to evaluate your own alignments.

# textgrid_2_textfile.py
This script creates text files in the format needed by evaluate_alinger.py based on standard TextGrids. It also removes labels signifying silence or noise:  "sil", "sp", and "". The function _textfiles_from_folder_with_folders_ creates textfiles from TextGrids structured the way MFA outputs them, and the function _textfiles_from_tg_folder_ from single TextGrids.

**Usage:**  
$ python textgrid_2_textfile.py
This runs a small example. Change the paths and model_name in main() to convert your own TextGrids.

# differing_file_functions.py
This script contains two functions that can be of help when wanting to perform evaluation on the same files from several different aligners, as MFA does not always succeed to align all files in the input.
- **differing_files(folders)** takes a list of folders and returns a set with the paths to all files that do not exist in all folders.
- **remove_differing_files(differing_files, folders)** removes all files in _differing_files_ from all folders in _folders_

**Usage:**  
$ python differing_file_functions.py
This runs a small example. Import the functions to other scripts or change the paths in main() to compare and possibly remove your own files. Make sure to save relevant copies before calling remove_differing_files().
