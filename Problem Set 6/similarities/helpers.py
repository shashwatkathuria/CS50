def lines(a, b):
    """Return lines in both a and b"""
    # splits the strings into lines and store them in respective lists
    linesreceived1 = a.split("\n")
    linesreceived2 = b.split("\n")
    ans = []
    #  checks for same lines
    for line in linesreceived1:
        if line in linesreceived2:
            # avoids repeated lines in ans
            if line not in ans:
                ans.append(line)
    return ans


def sentences(a, b):
    """Return sentences in both a and b"""
    # imports a function which splits given srtings into sentences
    from nltk.tokenize import sent_tokenize
    # returns back lists and are stored as such
    sentencesreceived1 = sent_tokenize(a)
    sentencesreceived2 = sent_tokenize(b)
    ans = []
    # checking for same sentences
    for sentence in sentencesreceived1:
        if sentence in sentencesreceived2:
            if sentence not in ans:
                ans.append(sentence)

    return ans


def substrings(a, b, n):
    """Return substrings of length n in both a and b"""
    # defines lists for subsets of both the strings
    size = n
    beginning = 0
    substrsa = []
    substrsb = []
    ans = []
    # generates subsets of a
    while (size + beginning) <= len(a):
        substrsa.append(a[beginning:beginning + size])
        beginning += 1
    beginning = 0
    # generates subsets of b
    while (size + beginning) <= len(b):
        substrsb.append(b[beginning:beginning + size])
        beginning += 1
    # check for similar substrings
    for substring in substrsa:
        if substring in substrsb:
            # avoids same element in ans
            if substring not in ans:
                ans.append(substring)
    return ans
