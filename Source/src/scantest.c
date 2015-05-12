////////////////////////////////////////////////////////////////////////////
//                             **** eDog ****                             //
//                                                                        //
//                  Electronic Dog Home Security System                   //
//                                 on the                                 //
//                           STM32F4-Discovery                            //
//                                                                        //
//                    Copyright (c) 2014 David Bryant                     //
//                          All Rights Reserved                           //
//        Distributed under the GNU Software License (see COPYING)        //
////////////////////////////////////////////////////////////////////////////

// scantest.c
//
// David Bryant
// August 17, 2014

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "scan.h"

// This module provides a test harness for the scan.c module that can be compiled as a command-line
// program and process raw audio data using the same algorithm as the embedded version. To aid in
// debugging it can also create output files containing intermediate values inside the audio
// scanning algorithm used for detecting "knocks" and "rings".
//
// Build right here on Cygwin or Linux:  gcc -I../inc scantest.c scan.c -o scantest

#define BUFFER_SAMPLES 16

static const char *usage =
" Usage:   scantest [-options] infile.pcm [outfile.pcm]\n\n"
" Options: -h  = high sensitivity mode (probably more false positives)\n"
"          -v  = verbose (all diagnostic information dsiplayed to stdout)\n"
"          -q  = quiet (don't even display knock/ring event detections)\n"
"          -k  = output data samples for knock detection debug\n"
"          -r  = output data samples for ring detection debug\n"
"          -fn = set specific option and debug flags (in hex)\n\n"
" Flags:   0x1 = high sensitivity\n"
"          0x2 = display peak thresholds every 10 seconds\n"
"          0x4 = display detected events and special cases\n"
"          0x8 = display every processed peak\n"
"          0x10 = output decorrelated audio\n"
"          0x20 = output decorrelated audio level (decaying average)\n"
"          0x40 = output normalized audio\n"
"          0x80 = output windowed level\n"
"          0x100 = output biquad-filtered audio\n"
"          0x200 = output biquad-filtered audio level (decaying average)\n\n";

int main (argc, argv) int argc; char **argv;
{
    int error_count = 0, output_words = 0, knocks = 0, rings = 0, flags = SCAN_DISP_EVENTS;
    int16_t in_sample_buffer [BUFFER_SAMPLES], *out_sample_buffer = NULL;
    FILE *infile = NULL, *outfile = NULL;

    // loop through command-line arguments

    while (--argc) {
        if ((**++argv == '-') && (*argv)[1])
            while (*++*argv)
                switch (**argv) {

                    case 'H': case 'h':
                        flags |= SCAN_HIGH_SENSITIVITY;
                        break;

                    case 'K': case 'k':
                        flags |= SCAN_OUTP_NORMAL_AUDIO | SCAN_OUTP_WINDOW_LEVEL;
                        break;

                    case 'R': case 'r':
                        flags |= SCAN_OUTP_NORMAL_AUDIO | SCAN_OUTP_FILTER_LEVEL;
                        break;

                    case 'F': case 'f':
                        flags |= strtol (++*argv, argv, 16);
                        --*argv;
                        break;

                    case 'V': case 'v':
                        flags |= SCAN_DISP_THRESHOLDS | SCAN_DISP_PEAKS;
                        break;

                    case 'Q': case 'q':
                        flags &= ~SCAN_DISP_EVENTS;
                        break;

                    default:
                        fprintf (stderr, "illegal option: %c !\n", **argv);
                        ++error_count;
                }
        else if (!infile) {
            infile = fopen (*argv, "rb");

            if (!infile) {
                fprintf (stderr, "can't open file for reading: %s !\n", *argv);
                ++error_count;
            }
        }
        else if (!outfile) {
            outfile = fopen (*argv, "wb");

            if (!outfile) {
                fprintf (stderr, "can't open file for writing: %s !\n", *argv);
                ++error_count;
            }
        }
        else {
            fprintf (stderr, "extra argument: %s !\n", *argv);
            ++error_count;
        }
    }

    // check for various command-line argument problems

    if (!infile) {
        fputs (usage, stderr);
        return 1;
    }

    if (error_count)
        return 1;

    if (flags & SCAN_OUTP_DECORR_AUDIO) output_words++;
    if (flags & SCAN_OUTP_DECORR_LEVEL) output_words++;
    if (flags & SCAN_OUTP_NORMAL_AUDIO) output_words++;
    if (flags & SCAN_OUTP_WINDOW_LEVEL) output_words++;
    if (flags & SCAN_OUTP_FILTER_AUDIO) output_words++;
    if (flags & SCAN_OUTP_FILTER_LEVEL) output_words++;

    if (output_words) {
        if (!outfile) {
            fprintf (stderr, "need to specify outfile file for debug sample data !\n");
            return 1;
        }

        out_sample_buffer = malloc (output_words * sizeof (int16_t) * BUFFER_SAMPLES);
    }

    scan_audio_init ();

    while (1) {
        int sample_count = fread (in_sample_buffer, sizeof (int16_t), BUFFER_SAMPLES, infile);
        int res;

        if (!sample_count)
            break;

        res = scan_audio (in_sample_buffer, sample_count, out_sample_buffer, flags);

        if (res & SCAN_KNOCK_DETECTED)
            knocks++;

        if (res & SCAN_BELL_DETECTED)
            rings++;

        if (outfile && output_words && !fwrite (out_sample_buffer, sizeof (int16_t), sample_count * output_words, outfile)) {
            fprintf (stderr, "can't write to output file!\n");
            break;
        }
    }

    printf ("final results: %d knocks and %d rings detected\n", knocks, rings);

    if (out_sample_buffer) free (out_sample_buffer);
    if (outfile) fclose (outfile);
    if (infile) fclose (infile);

    return 0;
}

void Dbg_puts (const char *s)
{
    fputs (s, stdout);
}

void Dbg_printf (const char *format, ...)
{
    va_list args;

    va_start (args, format);
    vprintf (format, args);
    va_end (args);
}
