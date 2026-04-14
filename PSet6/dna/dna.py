import csv
import sys


def main():

    # Check for command-line usage
    if len(sys.argv) != 3:
        print("Missing command-line argument")
        sys.exit(1)

    # Read database file into a variable
    read_database = []
    database = open(sys.argv[1], "r")
    # DictReader automatically ignores the heards (the first row) in a dictionary.
    reader = csv.DictReader(database)
    for row in reader:
        read_database.append(row)

    # Read DNA sequence file into a variable
    dna = open(sys.argv[2], "r")
    sequence = dna.read()

    # Find longest match of each STR in DNA sequence
    # We got three subsequences: AGATC, AATG and TATC. am gonna create three variables
    # to keep track of their count.
    strs = reader.fieldnames
    strs.remove("name")
    str_counts = {str: longest_match(sequence, str) for str in strs}

    # TODO: Check database for matching profile
    # basically we got to check the counts for each STR, and if we find a matching
    # person that has got the exact same counts for the subsequences, we just print their name and
    # if we don't find any matches, we type no match.
    for person in read_database:
        match = True
        for str in str_counts:
            if int(person[str]) != str_counts[str]:
                match = False
                break
        if match:
            print(person["name"])
            return
    print("No match")
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
