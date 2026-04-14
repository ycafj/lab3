This project is dedicated to text encryption using the polyalphabetic Vigenère cipher and its automated cryptanalysis (breaking) without prior knowledge of the key using the Kasiski method.

Function Descriptions
readfile: Reads the content of the input file (open.txt) as a single continuous string for further processing.

cleartext: Cleans the text by removing punctuation and non-alphabetic characters, and converts all text to lowercase for standardization before encryption.

vizhenera: Implements polyalphabetic encryption, where each character is shifted by the value of the corresponding key character modulo 26.

devizhenera: Performs text decryption using the found or entered key, returning the characters to their original form.

find_bigr: Scans the text and extracts individual character sequences (bigrams and trigrams), recording their content and exact position (index) in the text.

find_bigr2: Analyzes the obtained list of bigrams/trigrams. It compares them with each other and selects only those sequences that repeat in the text two or more times.

calc_key: Calculates the mathematical distance (number of characters) between the positions of the identical bigrams found by the previous function.

factorize: Decomposes the obtained distances into prime factors (2, 3, 5, etc.). Common factors for different distances indicate the most probable key length.

search_freq: Calculates the actual frequency of each letter (from 0 to 100%) in a specific fragment of the encrypted text.

freq_analysis: The main analytical function. It breaks the ciphertext into groups (based on the assumed key length) and selects a shift for each group such that the difference between the observed frequency and the reference (ENGLISH_FREQ) is minimized (least squares method).

Console Output Explanation
Original text: The cleaned text from the file.

Shifr text: The result of encryption with the key "crypto".

List of bigrams: Displays the found repeats and their positions.

Distances: Numbers showing how many characters apart the identical bigrams repeat.

Key length input: The program asks you to analyze the factors and enter the probable length (e.g., 6 for "crypto").

[RESULT] Probable key: The program outputs the word it calculated statistically.

Decrypted text: The final result of the text recovery.
