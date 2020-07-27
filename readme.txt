This program is built for the purpose of computing various types of subsequences from either a single or two sequences.
To this end, a random sequence generation feature is also inlcuded.

The valid argument/option combinations are ("a|b" means "a" and/or "b", and "[a]" means the argument "a" is optional):
-g [-o outputfilename]
-t|-p [-i inputfilename] [-o outputfilename]
-c|-ct|-cp [-t|-p] [-i inputfilename] [-o outputfilename]

-g:	to generate an instance consisting of two sequences over the digit alphabet. Use -o to specify output file name.
-c:	to compute a longest common subsequence (LCS) for the two input sequences. Use -i and -o to specify input and output file names.
-t:	to compute a longest tandem sequence (LTS) for the input sequence. Use -i and -o to specify input and output file names.
-p:	to compute a longest palindrome sequence (LPS) for the input sequence. Use -i and -o to specify input and output file names.
-ct:	to compute a longest common tandem subsequence (LCTS) for the two input sequences. Use -i and -o to specify input and output file names.
-cp:	to compute a longest common palindrome subsequence (LCPS) for the two input sequences. Use -i and -o to specify input and output file names.
-i inputfilename:	to read in sequence(s) from file "inputfilename"
-o outputfilename:	to write all the results into the file "outputfilename"
