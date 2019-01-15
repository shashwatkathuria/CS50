// Helper functions for music
#include <cs50.h>
#include <math.h>
#include "helpers.h"
#include <string.h>
#include <stdio.h>

// Converts a fraction formatted as X/Y to eighths
int duration(string fraction)
{
    int n = fraction[2] - 48;
    int x = fraction[0] - 48;
    //switch case calculates the multiples of eight notes required
    switch (n)
    {
        case 1:
            x *= 8;
            break;
        case 2:
            x *= 4;
            break;
        case 4:
            x *= 2;
            break;
        case 8:
            break;
    }
    //returns it as an integer
    return x;
}

// Calculates frequency (in Hz) of a note
int frequency(string note)
{
    int oct;
    //standard A4 frequency is 440 Hz
    float freq = 440;
    char let;
    if (strlen(note) == 3)
    {
        oct = note[2] - 48;
        //handles the frequency of differently spaced semitones
        switch (note[0])
        {
            case 'C':
                freq = 440.0 / (pow(2.0, (9.0 / 12.0)));
                break;
            case 'D':
                freq = 440.0 / (pow(2.0, (7.0 / 12.0)));
                break;
            case 'E':
                freq = 440.0 / (pow(2.0, (5.0 / 12.0)));
                break;
            case 'F':
                freq = 440.0 / (pow(2.0, (4.0 / 12.0)));
                break;
            case 'G':
                freq = 440.0 / (pow(2.0, (2.0 / 12.0)));
                break;
            case 'A':
                freq = 440.0;
                break;
            case 'B':
                freq = 440.0 * (pow(2.0, (2.0 / 12.0)));
                break;
            default :
                return 0;
        }
        //Checks if there is any modifer like # or b
        if (note[1] == '#')
        {
            freq *= pow(2, (1.0 / 12.0));
        }
        else
        {
            freq /= pow(2, (1.0 / 12.0));
        }
        //Calculates freq according to octave number
        freq *= pow(2, oct - 4);
    }
    else if (strlen(note) == 2)
    {
        oct = note[1] - 48;
        let = note[0];
        //handles the frequency of differently spaced semitones
        switch (note[0])
        {
            case 'C':
                freq = 440.0 / (pow(2.0, (9.0 / 12.0)));
                break;
            case 'D':
                freq = 440.0 / (pow(2.0, (7.0 / 12.0)));
                break;
            case 'E':
                freq = 440.0 / (pow(2.0, (5.0 / 12.0)));
                break;
            case 'F':
                freq = 440.0 / (pow(2.0, (4.0 / 12.0)));
                break;
            case 'G':
                freq = 440.0 / (pow(2.0, (2.0 / 12.0)));
                break;
            case 'A':
                freq = 440.0;
                break;
            case 'B':
                freq = 440.0 * (pow(2.0, (2.0 / 12.0)));
                break;
            default :
                return 0;
        }
        //Calculates freq according to octave number
        freq *= pow(2, oct - 4);
    }
    //returns the rounded off frequency
    return round(freq);
}

// Determines whether a string represents a rest
bool is_rest(string s)
{
    //checks if there is a blank line representing a rest
    if (strcmp(s, "") == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
