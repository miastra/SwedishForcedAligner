README_mix2lab.c

In the Waxholm data base, every recorded utterance has a correlating smp.mix-file. It is a text file with some meta data, orthographic text of the utterance, phonetic text of the utterance, and a manually corrected alignment. You can find an example of an smp.mix-file in this directory. In the project, we used the Waxholm data base to evaluate the alignments of our trained model and the pretrained model.
The mix2lab program takes an smp.mix-file and creates a lab-file and a text-file. The lab file is just the orthographic text, which is needed for alignment. The text file is used for creating a dictionary, and contains three lines: the orthograhic text, the phonetic text, as it is written in the smp.mix file, and the phonetic text as it is automatically generated from the alignment below in the smp.mix file. When creating the dictionary, we only used the two first lines in the text file, but we did not know which lines to use when writing mix2lab. Actually, we found that it would probably have worked better if we had created the dictionary (i.e. the phonetic text for the dictionary) from the alignments, but at that point it was too late.
To execute mix2lab, type: mix2lab <input_file> <destination>
If you have many smp.mix files in one directory, type:
for %a in (*smp.mix) do mix2lab "%~a" <destination_directory>

Note: The program only works with ANSI files, so you have to convert the resulting lab files to UTF8 before using them with MFA.
Some mistakes we did during the project are corrected in this version.
There are som debug outputs which you can comment out for faster execution.