/**
 * @preserve Copyright (c) 2018-2022 NN Solex, www.sublunar.space
 * License MIT: http://www.opensource.org/licenses/MIT
 */

/**
 * @brief Modified by u-blusky Swep-Wasm
 *
 */

#define ONLINE 1
#define OFFLINE 2

#ifndef USECASE
#endif

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "swephexp.h"

#if USECASE == OFFLINE
#include <emscripten.h>
#endif

#define MY_ODEGREE_STRING "°"
#define BIT_ROUND_SEC 1
#define BIT_ROUND_MIN 2
#define BIT_ZODIAC 4
#define PLSEL_D "0123456789mtABC"
#define PLSEL_P "0123456789mtABCDEFGHI"
#define PLSEL_H "JKLMNOPQRSTUVWX"
#define PLSEL_A "0123456789mtABCDEFGHIJKLMNOPQRSTUVWX"

static char *zod_nam[] = {"ar", "ta", "ge", "cn", "le", "vi",
                          "li", "sc", "sa", "cp", "aq", "pi"};

// static char *dms(double x, long iflag) code from sweph
static char *dms(double x, long iflag)
{
  int izod;
  long k, kdeg, kmin, ksec;
  char *c = MY_ODEGREE_STRING;
  char *sp, s1[50];
  static char s[50];
  int sgn;
  *s = '\0';
  if (iflag & SEFLG_EQUATORIAL)
    strcpy(c, "h");
  if (x < 0)
  {
    x = -x;
    sgn = -1;
  }
  else
    sgn = 1;
  if (iflag & BIT_ROUND_MIN)
    x += 0.5 / 60;
  if (iflag & BIT_ROUND_SEC)
    x += 0.5 / 3600;
  if (iflag & BIT_ZODIAC)
  {
    izod = (int)(x / 30);
    x = fmod(x, 30);
    kdeg = (long)x;
    sprintf(s, "%2ld %s ", kdeg, zod_nam[izod]);
  }
  else
  {
    kdeg = (long)x;
    sprintf(s, " %3ld%s", kdeg, c);
  }
  x -= kdeg;
  x *= 60;
  kmin = (long)x;
  if ((iflag & BIT_ZODIAC) && (iflag & BIT_ROUND_MIN))
    sprintf(s1, "%2ld", kmin);
  else
    sprintf(s1, "%2ld'", kmin);
  strcat(s, s1);
  if (iflag & BIT_ROUND_MIN)
    goto return_dms;
  x -= kmin;
  x *= 60;
  ksec = (long)x;
  if (iflag & BIT_ROUND_SEC)
    sprintf(s1, "%2ld\"", ksec);
  else
    sprintf(s1, "%2ld", ksec);
  strcat(s, s1);
  if (iflag & BIT_ROUND_SEC)
    goto return_dms;
  x -= ksec;
  k = (long)(x * 10000);
  sprintf(s1, ".%04ld", k);
  strcat(s, s1);
return_dms:;
  if (sgn < 0)
  {
    sp = strpbrk(s, "0123456789");
    *(sp - 1) = '-';
  }
  return (s);
}

const char *astro(int year, int month, int day, int hour, int minute, int second, int lonG, int lonM, int lonS, char *lonEW, int latG, int latM, int latS, char *latNS, char *iHouse)
{
  char snam[40], serr[AS_MAXCH];
  double jut = 0.0;
  double tjd_ut, x[6], lon, lat;
  double cusp[12 + 1];
  double ascmc[10];
  long iflag, iflgret;
  int p, i;
  char *buffer = malloc(1000);
  char *bufferResult = malloc(10000);
  int round_flag = 0;

  swe_set_ephe_path("eph");
  iflag = SEFLG_SWIEPH | SEFLG_SPEED;

  jut = (double)hour + (double)minute / 60 + (double)second / 3600;
  tjd_ut = swe_julday(year, month, day, jut, SE_GREG_CAL);

  sprintf(buffer, "%13s\t%14s\t%16s\t%10s\t%10s\n", "", "Longitude", "Latitude", "Distance", "Speed");
  strcat(bufferResult, buffer);

  for (p = SE_SUN; p < SE_NPLANETS; p++)
  {
    if (p == SE_EARTH)
      continue;

    iflgret = swe_calc_ut(tjd_ut, p, iflag, x, serr);

    if (iflgret > 0 && (iflgret & SEFLG_SWIEPH))
    {
      swe_get_planet_name(p, snam);
      sprintf(buffer, "%13s\t%20s\t%10.7f\t%10.7f\t%10.7f\n", snam, dms(x[0], round_flag | BIT_ZODIAC), x[1], x[2], x[3]);
      strcat(bufferResult, buffer);
    }
    else
    {
      swe_get_planet_name(p, snam);
      sprintf(buffer, "%13s\t%20s\t%10s\t%10s\t%10s\n", snam, "Error", "", "", "");
      strcat(bufferResult, buffer);
    }
  }

  lon = lonG + lonM / 60.0 + lonS / 3600.0;
  if (*lonEW == 'W')
    lon = -lon;
  lat = latG + latM / 60.0 + latS / 3600.0;
  if (*latNS == 'S')
    lat = -lat;

  swe_houses_ex(tjd_ut, iflag, lat, lon, (int)*iHouse, cusp, ascmc);
  strcat(bufferResult, "\n");
  sprintf(buffer, "%13s\t%20s\n", "Asc", dms(ascmc[0], round_flag | BIT_ZODIAC));
  strcat(bufferResult, buffer);
  sprintf(buffer, "%13s\t%20s\n\n", "MC", dms(ascmc[1], round_flag | BIT_ZODIAC));
  strcat(bufferResult, buffer);

  for (i = 1; i <= 12; i++)
  {
    sprintf(buffer, "%11s%1d\t%20s\n", "House ", i, dms(cusp[i], round_flag | BIT_ZODIAC));
    strcat(bufferResult, buffer);
  }

  return bufferResult;
}

#if USECASE == OFFLINE
EMSCRIPTEN_KEEPALIVE
const char *get(int year, int month, int day, int hour, int minute, int second, int lonG, int lonM, int lonS, char *lonEW, int latG, int latM, int latS, char *latNS, char *iHouse)
{
  return astro(year, month, day, hour, minute, second, lonG, lonM, lonS, lonEW, latG, latM, latS, latNS, iHouse);
}
#endif
