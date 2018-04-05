# Models

These models were trained using the Montreal Forced Aligner. To use them, run the aligner the following way:
~~~
bin/mfa_align [DATA SET] [LEXICON] [MODEL FILE] [OUTDIR]
~~~

The KI Model was trained on the speech of different children with varying degrees of speech disorders.

The second model, the NST Model, was trained on the first third of the NST data set. This data set was much larger than the KI data set (106 000 data points) and training took ~14 hours using 7 threads on an 8-core 4 GHz CPU.
