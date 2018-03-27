README_idcopy.c

Montreal forced aligner uses speaker adaption in alignment. Therefore, the input must be organized such that every speaker has its own directory with .wav- and .lab-files (alternatively, you can use command --speaker_characters NUMBER).
After preprocessing of input data (at least in the cases of NST and Waxholm datasets), we had one directory with all .wav- and .lab-files. idcopy.c creates directories for each speaker and copies the right files into those directories.
Place idcopy.exe in the directory you want to organize, make sure you are in that directory, and execute it with:
for %x in (*.wav) do idcopy "%x" <start_char> <num_chars>
- this runs through all .wav-files, creates directories from their names and the given start_char and num_chars, and copies the wav-file into the directory
for %x in (*.lab) do idcopy "%x" <start_char> <num_chars>
- this runs through all .lab-files, creates directories from their names and the given start_char and num_chars, and copies the wav-file into the directory
<start_char> and <num_chars> work the following way: the .wav- and .lab-files are assumed to have names containing information about the speakers identity, e.g. st1_01233.wav and st1_01233.lab could mean station 1, speaker 012 and recorded utterance number 33. In this case, you want to have a directory named 012 containing all .wav- and .lab-files for speaker 012.
With <start_char> == 4 and <num_chars> == 3, you would get the wanted directory.
Note that the program contains some debug outputs, which you can comment out for faster execution.