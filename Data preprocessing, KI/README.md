# KI Preprocessing Scripts

These scripts help preprocess the KI data to produce files that can be handled by MFA.

## split_data.py
This script takes the data provided to us by KI and splits the full audio files into audio files containing only one utterance per file. The metadata is processed to produce a .LAB file for every corresponding audio file. Two other file types are created as well: .FTARGET and .FOBS. These are used by the next script.

## build_lexicon.py
The files created by the previous script are used in this phase to produce the lexicon. We only use the .FTARGET files as they contain the "correct" pronounciation of an utterance. The pronounciations are written in X-SAMPA but need to be preprocessed slightly to produce the dictionary format that is accepted by MFA.
