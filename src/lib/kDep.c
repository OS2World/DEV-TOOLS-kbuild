/* $Id$ */
/** @file
 * kDep - Common Dependency Managemnt Code.
 */

/*
 * Copyright (c) 2004-2010 knut st. osmundsen <bird-kBuild-spamx@anduin.net>
 *
 * This file is part of kBuild.
 *
 * kBuild is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * kBuild is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with kBuild.  If not, see <http://www.gnu.org/licenses/>
 *
 */

/*******************************************************************************
*   Header Files                                                               *
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <limits.h>
#include <sys/stat.h>
#include "k/kDefs.h"
#include "k/kTypes.h"
#if K_OS == K_OS_WINDOWS
# define USE_WIN_MMAP
# include <io.h>
# include <Windows.h>
 extern void nt_fullpath(const char *pszPath, char *pszFull, size_t cchFull); /* nt_fullpath.c */
#else
# include <dirent.h>
# include <unistd.h>
# include <stdint.h>
#endif

#include "kDep.h"


/*******************************************************************************
*   Global Variables                                                           *
*******************************************************************************/
/** List of dependencies. */
static PDEP g_pDeps = NULL;


/**
 * Corrects all slashes to unix slashes.
 *
 * @returns pszFilename.
 * @param   pszFilename     The filename to correct.
 */
static char *fixslash(char *pszFilename)
{
    char *psz = pszFilename;
    while ((psz = strchr(psz, '\\')) != NULL)
        *psz++ = '/';
    return pszFilename;
}


#if K_OS == K_OS_OS2

/**
 * Corrects the case of a path.
 *
 * @param   pszPath     Pointer to the path, both input and output.
 *                      The buffer must be able to hold one more byte than the string length.
 */
static void fixcase(char *pszFilename)
{
    return;
}

#elif K_OS != K_OS_WINDOWS

/**
 * Corrects the case of a path.
 *
 * @param   pszPath     Pointer to the path, both input and output.
 */
static void fixcase(char *pszFilename)
{
    char *psz;

    /*
     * Skip the root.
     */
    psz = pszFilename;
    while (*psz == '/')
        psz++;

    /*
     * Iterate all the components.
     */
    while (*psz)
    {
        char  chSlash;
        struct stat s;
        char   *pszStart = psz;

        /*
         * Find the next slash (or end of string) and terminate the string there.
         */
        while (*psz != '/' && *psz)
            *psz++;
        chSlash = *psz;
        *psz = '\0';

        /*
         * Does this part exist?
         * If not we'll enumerate the directory and search for an case-insensitive match.
         */
        if (stat(pszFilename, &s))
        {
            struct dirent  *pEntry;
            DIR            *pDir;
            if (pszStart == pszFilename)
                pDir = opendir(*pszFilename ? pszFilename : ".");
            else
            {
                pszStart[-1] = '\0';
                pDir = opendir(pszFilename);
                pszStart[-1] = '/';
            }
            if (!pDir)
            {
                *psz = chSlash;
                break; /* giving up, if we fail to open the directory. */
            }

            while ((pEntry = readdir(pDir)) != NULL)
            {
                if (!strcasecmp(pEntry->d_name, pszStart))
                {
                    strcpy(pszStart, pEntry->d_name);
                    break;
                }
            }
            closedir(pDir);
            if (!pEntry)
            {
                *psz = chSlash;
                break;  /* giving up if not found. */
            }
        }

        /* restore the slash and press on. */
        *psz = chSlash;
        while (*psz == '/')
            psz++;
    }

    return;
}

#endif /* !OS/2 && !Windows */


/**
 * 'Optimizes' and corrects the dependencies.
 */
void depOptimize(int fFixCase, int fQuiet)
{
    /*
     * Walk the list correct the names and re-insert them.
     */
    PDEP pDepOrg = g_pDeps;
    PDEP pDep = g_pDeps;
    g_pDeps = NULL;
    for (; pDep; pDep = pDep->pNext)
    {
#ifndef PATH_MAX
        char        szFilename[_MAX_PATH + 1];
#else
        char        szFilename[PATH_MAX + 1];
#endif
        char       *pszFilename;
        struct stat s;

        /*
         * Skip some fictive names like <built-in> and <command line>.
         */
        if (    pDep->szFilename[0] == '<'
            &&  pDep->szFilename[pDep->cchFilename - 1] == '>')
            continue;
        pszFilename = pDep->szFilename;

#if K_OS != K_OS_OS2 && K_OS != K_OS_WINDOWS
        /*
         * Skip any drive letters from compilers running in wine.
         */
        if (pszFilename[1] == ':')
            pszFilename += 2;
#endif

        /*
         * The microsoft compilers are notoriously screwing up the casing.
         * This will screw up kmk (/ GNU Make).
         */
        if (fFixCase)
        {
#if K_OS == K_OS_WINDOWS
            nt_fullpath(pszFilename, szFilename, sizeof(szFilename));
            fixslash(szFilename);
#else
            strcpy(szFilename, pszFilename);
            fixslash(szFilename);
            fixcase(szFilename);
#endif
            pszFilename = szFilename;
        }

        /*
         * Check that the file exists before we start depending on it.
         */
        if (stat(pszFilename, &s))
        {
            if (   !fQuiet
                || errno != ENOENT
                || (   pszFilename[0] != '/'
                    && pszFilename[0] != '\\'
                    && (   !isalpha(pszFilename[0])
                        || pszFilename[1] != ':'
                        || (    pszFilename[2] != '/'
                            &&  pszFilename[2] != '\\')))
               )
                fprintf(stderr, "kDep: Skipping '%s' - %s!\n", pszFilename, strerror(errno));
            continue;
        }

        /*
         * Insert the corrected dependency.
         */
        depAdd(pszFilename, strlen(pszFilename));
    }

    /*
     * Free the old ones.
     */
    while (pDepOrg)
    {
        pDep = pDepOrg;
        pDepOrg = pDepOrg->pNext;
        free(pDep);
    }
}


/**
 * Prints the dependency chain.
 *
 * @returns Pointer to the allocated dependency.
 * @param   pOutput     Output stream.
 */
void depPrint(FILE *pOutput)
{
    PDEP pDep;
    for (pDep = g_pDeps; pDep; pDep = pDep->pNext)
        fprintf(pOutput, " \\\n\t%s", pDep->szFilename);
    fprintf(pOutput, "\n\n");
}


/**
 * Prints empty dependency stubs for all dependencies.
 */
void depPrintStubs(FILE *pOutput)
{
    PDEP pDep;
    for (pDep = g_pDeps; pDep; pDep = pDep->pNext)
        fprintf(pOutput, "%s:\n\n", pDep->szFilename);
}


/* sdbm:
   This algorithm was created for sdbm (a public-domain reimplementation of
   ndbm) database library. it was found to do well in scrambling bits,
   causing better distribution of the keys and fewer splits. it also happens
   to be a good general hashing function with good distribution. the actual
   function is hash(i) = hash(i - 1) * 65599 + str[i]; what is included below
   is the faster version used in gawk. [there is even a faster, duff-device
   version] the magic constant 65599 was picked out of thin air while
   experimenting with different constants, and turns out to be a prime.
   this is one of the algorithms used in berkeley db (see sleepycat) and
   elsewhere. */
static unsigned sdbm(const char *str, size_t size)
{
    unsigned hash = 0;
    int c;

    while (size-- > 0 && (c = *(unsigned const char *)str++))
        hash = c + (hash << 6) + (hash << 16) - hash;

    return hash;
}


/**
 * Adds a dependency.
 *
 * @returns Pointer to the allocated dependency.
 * @param   pszFilename     The filename. Does not need to be terminated.
 * @param   cchFilename     The length of the filename.
 */
PDEP depAdd(const char *pszFilename, size_t cchFilename)
{
    unsigned    uHash = sdbm(pszFilename, cchFilename);
    PDEP        pDep;
    PDEP        pDepPrev;

    /*
     * Check if we've already got this one.
     */
    pDepPrev = NULL;
    for (pDep = g_pDeps; pDep; pDepPrev = pDep, pDep = pDep->pNext)
        if (    pDep->uHash == uHash
            &&  pDep->cchFilename == cchFilename
            &&  !memcmp(pDep->szFilename, pszFilename, cchFilename))
            return pDep;

    /*
     * Add it.
     */
    pDep = (PDEP)malloc(sizeof(*pDep) + cchFilename);
    if (!pDep)
    {
        fprintf(stderr, "\nOut of memory! (requested %lx bytes)\n\n",
                (unsigned long)(sizeof(*pDep) + cchFilename));
        exit(1);
    }

    pDep->cchFilename = cchFilename;
    memcpy(pDep->szFilename, pszFilename, cchFilename);
    pDep->szFilename[cchFilename] = '\0';
    pDep->uHash = uHash;

    if (pDepPrev)
    {
        pDep->pNext = pDepPrev->pNext;
        pDepPrev->pNext = pDep;
    }
    else
    {
        pDep->pNext = g_pDeps;
        g_pDeps = pDep;
    }
    return pDep;
}


/**
 * Frees the current dependency chain.
 */
void depCleanup(void)
{
    PDEP pDep = g_pDeps;
    g_pDeps = NULL;
    while (pDep)
    {
        PDEP pFree = pDep;
        pDep = pDep->pNext;
        free(pFree);
    }
}


/**
 * Performs a hexdump.
 */
void depHexDump(const KU8 *pb, size_t cb, size_t offBase)
{
    const unsigned      cchWidth = 16;
    size_t              off = 0;
    while (off < cb)
    {
        unsigned i;
        printf("%s%0*lx %04lx:", off ? "\n" : "", (int)sizeof(pb) * 2,
               (unsigned long)offBase + (unsigned long)off, (unsigned long)off);
        for (i = 0; i < cchWidth && off + i < cb ; i++)
            printf(off + i < cb ? !(i & 7) && i ? "-%02x" : " %02x" : "   ", pb[i]);

        while (i++ < cchWidth)
                printf("   ");
        printf(" ");

        for (i = 0; i < cchWidth && off + i < cb; i++)
        {
            const KU8 u8 = pb[i];
            printf("%c", u8 < 127 && u8 >= 32 ? u8 : '.');
        }
        off += cchWidth;
        pb  += cchWidth;
    }
    printf("\n");
}


/**
 * Reads the file specified by the pInput file stream into memory.
 *
 * @returns The address of the memory mapping on success. This must be
 *          freed by calling depFreeFileMemory.
 *
 * @param   pInput      The file stream to load or map into memory.
 * @param   pcbFile     Where to return the mapping (file) size.
 * @param   ppvOpaque   Opaque data when mapping, otherwise NULL.
 */
void *depReadFileIntoMemory(FILE *pInput, size_t *pcbFile, void **ppvOpaque)
{
    void       *pvFile;
    long        cbFile;

    /*
     * Figure out file size.
     */
#if defined(_MSC_VER)
    cbFile = _filelength(fileno(pInput));
    if (cbFile < 0)
#else
    if (    fseek(pInput, 0, SEEK_END) < 0
        ||  (cbFile = ftell(pInput)) < 0
        ||  fseek(pInput, 0, SEEK_SET))
#endif
    {
        fprintf(stderr, "kDep: error: Failed to determin file size.\n");
        return NULL;
    }
    if (pcbFile)
        *pcbFile = cbFile;

    /*
     * Try mmap first.
     */
#ifdef USE_WIN_MMAP
    {
        HANDLE hMapObj = CreateFileMapping((HANDLE)_get_osfhandle(fileno(pInput)),
                                           NULL, PAGE_READONLY, 0, cbFile, NULL);
        if (hMapObj != NULL)
        {
            pvFile = MapViewOfFile(hMapObj, FILE_MAP_READ, 0, 0, cbFile);
            if (pvFile)
            {
                *ppvOpaque = hMapObj;
                return pvFile;
            }
            fprintf(stderr, "kDep: warning: MapViewOfFile failed, %d.\n", GetLastError());
            CloseHandle(hMapObj);
        }
        else
            fprintf(stderr, "kDep: warning: CreateFileMapping failed, %d.\n", GetLastError());
    }

#endif

    /*
     * Allocate memory and read the file.
     */
    pvFile = malloc(cbFile + 1);
    if (pvFile)
    {
        if (fread(pvFile, cbFile, 1, pInput))
        {
            ((KU8 *)pvFile)[cbFile] = '\0';
            *ppvOpaque = NULL;
            return pvFile;
        }
        fprintf(stderr, "kDep: error: Failed to read %ld bytes.\n", cbFile);
        free(pvFile);
    }
    else
        fprintf(stderr, "kDep: error: Failed to allocate %ld bytes (file mapping).\n", cbFile);
    return NULL;
}


/**
 * Free resources allocated by depReadFileIntoMemory.
 *
 * @param   pvFile      The address of the memory mapping.
 * @param   pvOpaque    The opaque value returned together with the mapping.
 */
void depFreeFileMemory(void *pvFile, void *pvOpaque)
{
#if defined(USE_WIN_MMAP)
    if (pvOpaque)
    {
        UnmapViewOfFile(pvFile);
        CloseHandle(pvOpaque);
        return;
    }
#endif
    free(pvFile);
}

