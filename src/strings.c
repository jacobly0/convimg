/*
 * Copyright 2017-2022 Matt "MateoConLechuga" Waltz
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "strings.h"

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

char *strdupcat(const char *s, const char *c)
{
    char *d;

    if (s == NULL)
    {
        return strdup(c);
    }
    else if (c == NULL)
    {
        return strdup(s);
    }

    d = malloc(strlen(s) + strlen(c) + 1);
    if (d != NULL)
    {
        strcpy(d, s);
        strcat(d, c);
    }
    return d;
}

char *strings_trim(char *str)
{
    char *end;

    while (isspace((int)*str))
    {
        str++;
    }

    if (*str == '\0')
    {
        return str;
    }

    end = str + strlen(str) - 1;
    while (end > str && isspace((int)*end))
    {
        end--;
    }

    end[1] = '\0';

    return str;
}

char *strings_basename(const char *path)
{
    char *result = strdup(path);
    char *tmp;

    tmp = strrchr(result, '/');
    if (tmp != NULL && *tmp && *(tmp + 1))
    {
        size_t len = strlen(tmp + 1);
        memmove(result, tmp + 1, len);
        result[len] = '\0';
    }

    tmp = strchr(result, '.');
    if (tmp != NULL)
    {
        *tmp = '\0';
    }

    return result;
}

glob_t *strings_find_images(const char *fullPath)
{
    char *path;

    if (!strstr(fullPath, ".png") &&
        !strstr(fullPath, ".bmp"))
    {
        path = strdupcat(fullPath, ".png");
    }
    else
    {
        path = strdup(fullPath);
    }

    glob_t *globbuf = calloc(sizeof(glob_t), 1);
    glob(path, 0, NULL, globbuf);
    free(path);

    return globbuf;
}

int strings_utf8_to_iso8859_1(const char *in, int inlen, char *out, int maxlen)
{
    unsigned int codepoint = 0;
    int len;

    if (in == NULL || out == NULL || inlen == 0)
    {
        return 0;
    }

    len = 0;
    while (maxlen && inlen)
    {
        unsigned char ch = *in;
        inlen--;

        if (ch <= 0x7f)
        {
            codepoint = ch;
        }
        else if (ch <= 0xbf)
        {
            codepoint = (codepoint << 6) | (ch & 0x3f);
        }
        else if (ch <= 0xdf)
        {
            codepoint = ch & 0x1f;
        }
        else if (ch <= 0xef)
        {
            codepoint = ch & 0x0f;
        }
        else
        {
            codepoint = ch & 0x07;
        }

        in++;
        if ((*in & 0xc0) != 0x80 && codepoint <= 0x10ffff)
        {
            *out = codepoint <= 255 ? codepoint & 0xff : '?';
            maxlen--;
            out++;
            len++;
        }
    }

    return len;
}
