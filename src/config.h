#ifndef __CONFIG_H
#define __CONFIG_H

#define LEVEL_INPUT A1

#ifndef LINEAR_STRIP
const int GAUGE_LENGTH = 9;
const int GAUGE_OFFSET = 2;
const int STRIP_LENGTH = 24;
#else
const int GAUGE_LENGTH = 8;
const int GAUGE_OFFSET = 0;
const int STRIP_LENGTH = 8;
#endif

const int BUFFER_SECONDS = 3;
const int SAMPLE_PERIOD_ms = 100UL;

#endif
