// Copyright (c) 2012 by Zuse-Institute Berlin and the Technical University of Denmark.
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "backend-cbf.h"
#include "cbf-format.h"
#include <stddef.h>
#include <stdio.h>

static CBFresponsee
  write(const char *file, const CBFdata data);

static CBFresponsee
  writeVER(FILE *pFile, const CBFdata data);

static CBFresponsee
  writeOBJSENSE(FILE *pFile, const CBFdata data);

static CBFresponsee
  writePSDVAR(FILE *pFile, const CBFdata data);

static CBFresponsee
  writeVAR(FILE *pFile, const CBFdata data);

static CBFresponsee
  writeINT(FILE *pFile, const CBFdata data);

static CBFresponsee
  writeCON(FILE *pFile, const CBFdata data);

static CBFresponsee
  writePSDCON(FILE *pFile, const CBFdata data);

static CBFresponsee
  writeOBJFCOORD(FILE *pFile, const CBFdata data);

static CBFresponsee
  writeOBJACOORD(FILE *pFile, const CBFdata data);

static CBFresponsee
  writeOBJBCOORD(FILE *pFile, const CBFdata data);

static CBFresponsee
  writeFCOORD(FILE *pFile, const CBFdata data);

static CBFresponsee
  writeACOORD(FILE *pFile, const CBFdata data);

static CBFresponsee
  writeBCOORD(FILE *pFile, const CBFdata data);

static CBFresponsee
  writeHCOORD(FILE *pFile, const CBFdata data);

static CBFresponsee
  writeDCOORD(FILE *pFile, const CBFdata data);


// -------------------------------------
// Global variable
// -------------------------------------

CBFbackend const backend_cbf = { "cbf", "cbf", write };


// -------------------------------------
// Function definitions
// -------------------------------------

static CBFresponsee write(const char *file, const CBFdata data) {
  CBFresponsee res = CBF_RES_OK;
  FILE *pFile = NULL;

  pFile = fopen(file, "wt");
  if (!pFile) {
    return CBF_RES_ERR;
  }

  if (res == CBF_RES_OK)
    res = writeVER(pFile, data);

  if (res == CBF_RES_OK)
    res = writeOBJSENSE(pFile, data);

  if (res == CBF_RES_OK)
    res = writePSDVAR(pFile, data);

  if (res == CBF_RES_OK)
    res = writeVAR(pFile, data);

  if (res == CBF_RES_OK)
    res = writeINT(pFile, data);

  if (res == CBF_RES_OK)
    res = writeCON(pFile, data);

  if (res == CBF_RES_OK)
    res = writePSDCON(pFile, data);

  if (res == CBF_RES_OK)
    res = writeOBJFCOORD(pFile, data);

  if (res == CBF_RES_OK)
    res = writeOBJACOORD(pFile, data);

  if (res == CBF_RES_OK)
    res = writeOBJBCOORD(pFile, data);

  if (res == CBF_RES_OK)
    res = writeFCOORD(pFile, data);

  if (res == CBF_RES_OK)
    res = writeACOORD(pFile, data);

  if (res == CBF_RES_OK)
    res = writeBCOORD(pFile, data);

  if (res == CBF_RES_OK)
    res = writeHCOORD(pFile, data);

  if (res == CBF_RES_OK)
    res = writeDCOORD(pFile, data);

  fclose(pFile);
  return res;
}

static CBFresponsee writeVER(FILE *pFile, const CBFdata data)
{
  CBFresponsee res = CBF_RES_OK;

  if (fprintf(pFile, "VER\n%i\n\n", data.ver) <= 0)
    res = CBF_RES_ERR;

  return res;
}

static CBFresponsee writeOBJSENSE(FILE *pFile, const CBFdata data)
{
  CBFresponsee res = CBF_RES_OK;
  const char * objsensenam;

  res = CBF_objsensetostr(data.objsense, &objsensenam);

  if (res == CBF_RES_OK)
    if (fprintf(pFile, "OBJSENSE\n%s\n\n", objsensenam) <= 0)
      res = CBF_RES_ERR;

  return res;
}

static CBFresponsee writeCON(FILE *pFile, const CBFdata data)
{
  CBFresponsee res = CBF_RES_OK;
  const char *conenam;
  long long int i;

  if (data.mapnum >= 1 || data.mapstacknum >= 1)
  {
    if (res == CBF_RES_OK)
      if (fprintf(pFile, "CON\n%lli %lli\n", data.mapnum, data.mapstacknum) <= 0)
        res = CBF_RES_ERR;

    for (i=0; i<data.mapstacknum && res==CBF_RES_OK; ++i) {
      CBF_conetostr(data.mapstackdomain[i], &conenam);

      if (fprintf(pFile, "%s %lli\n", conenam, data.mapstackdim[i]) <= 0)
        res = CBF_RES_ERR;
    }

    if (res == CBF_RES_OK)
      if (fprintf(pFile, "\n") <= 0)
        res = CBF_RES_ERR;
  }

  return res;
}

static CBFresponsee writeVAR(FILE *pFile, const CBFdata data)
{
  CBFresponsee res = CBF_RES_OK;
  const char *conenam;
  long long int i;

  if (data.varnum >= 1 || data.varstacknum >= 1)
  {
    if (res == CBF_RES_OK)
      if (fprintf(pFile, "VAR\n%lli %lli\n", data.varnum, data.varstacknum) <= 0)
        res = CBF_RES_ERR;

    for (i=0; i<data.varstacknum && res==CBF_RES_OK; ++i) {
      CBF_conetostr(data.varstackdomain[i], &conenam);

      if (fprintf(pFile, "%s %lli\n", conenam, data.varstackdim[i]) <= 0)
        res = CBF_RES_ERR;
    }

    if (res == CBF_RES_OK)
      if (fprintf(pFile, "\n") <= 0)
        res = CBF_RES_ERR;
  }

  return res;
}

static CBFresponsee writeINT(FILE *pFile, const CBFdata data)
{
  CBFresponsee res = CBF_RES_OK;
  long long int i;

  if (data.intvarnum >= 1)
  {
    if (res == CBF_RES_OK)
      if (fprintf(pFile, "INT\n%lli\n", data.intvarnum) <= 0)
        res = CBF_RES_ERR;

    for (i=0; i<data.intvarnum && res==CBF_RES_OK; ++i)
      if (fprintf(pFile, "%lli\n", data.intvar[i]) <= 0)
        res = CBF_RES_ERR;

    if (res == CBF_RES_OK)
      if (fprintf(pFile, "\n") <= 0)
        res = CBF_RES_ERR;
  }

  return res;
}

static CBFresponsee writePSDCON(FILE *pFile, const CBFdata data)
{
  CBFresponsee res = CBF_RES_OK;
  int i;

  if (data.psdmapnum >= 1)
  {
    if (res == CBF_RES_OK)
      if (fprintf(pFile, "PSDCON\n%i\n", data.psdmapnum) <= 0)
        res = CBF_RES_ERR;

    for (i=0; i<data.psdmapnum && res==CBF_RES_OK; ++i)
      if (fprintf(pFile, "%i\n", data.psdmapdim[i]) <= 0)
        res = CBF_RES_ERR;

    if (res == CBF_RES_OK)
      if (fprintf(pFile, "\n") <= 0)
        res = CBF_RES_ERR;
  }

  return res;
}

static CBFresponsee writePSDVAR(FILE *pFile, const CBFdata data)
{
  CBFresponsee res = CBF_RES_OK;
  int i;

  if (data.psdvarnum >= 1)
  {
    if (res == CBF_RES_OK)
      if (fprintf(pFile, "PSDVAR\n%i\n", data.psdvarnum) <= 0)
        res = CBF_RES_ERR;

    for (i=0; i<data.psdvarnum && res==CBF_RES_OK; ++i)
      if (fprintf(pFile, "%i\n", data.psdvardim[i]) <= 0)
        res = CBF_RES_ERR;

    if (res == CBF_RES_OK)
      if (fprintf(pFile, "\n") <= 0)
        res = CBF_RES_ERR;
  }

  return res;
}

static CBFresponsee writeOBJFCOORD(FILE *pFile, const CBFdata data)
{
  CBFresponsee res = CBF_RES_OK;
  long long int i;

  if (data.objfnnz >= 1)
  {
    if (res == CBF_RES_OK)
      if (fprintf(pFile, "OBJFCOORD\n%lli\n", data.objfnnz) <= 0)
        res = CBF_RES_ERR;

    for (i=0; i<data.objfnnz && res==CBF_RES_OK; ++i)
      if (fprintf(pFile, "%i %i %i %.16lg\n", data.objfsubj[i], data.objfsubk[i], data.objfsubl[i], data.objfval[i]) <= 0)
        res = CBF_RES_ERR;

    if (res == CBF_RES_OK)
      if (fprintf(pFile, "\n") <= 0)
        res = CBF_RES_ERR;
  }

  return res;
}

static CBFresponsee writeOBJACOORD(FILE *pFile, const CBFdata data)
{
  CBFresponsee res = CBF_RES_OK;
  long long int i;

  if (data.objannz >= 1)
  {
    if (res == CBF_RES_OK)
      if (fprintf(pFile, "OBJACOORD\n%lli\n", data.objannz) <= 0)
        res = CBF_RES_ERR;

    for (i=0; i<data.objannz && res==CBF_RES_OK; ++i)
      if (fprintf(pFile, "%lli %.16lg\n", data.objasubj[i], data.objaval[i]) <= 0)
        res = CBF_RES_ERR;

    if (res == CBF_RES_OK)
      if (fprintf(pFile, "\n") <= 0)
        res = CBF_RES_ERR;
  }

  return res;
}

static CBFresponsee writeOBJBCOORD(FILE *pFile, const CBFdata data)
{
  CBFresponsee res = CBF_RES_OK;

  if (data.objbval != 0.0)
  {
    if (res == CBF_RES_OK)
      if (fprintf(pFile, "OBJBCOORD\n%lg\n", data.objbval) <= 0)
        res = CBF_RES_ERR;

    if (res == CBF_RES_OK)
      if (fprintf(pFile, "\n") <= 0)
        res = CBF_RES_ERR;
  }

  return res;
}

static CBFresponsee writeFCOORD(FILE *pFile, const CBFdata data)
{
  CBFresponsee res = CBF_RES_OK;
  long long int i;

  if (data.fnnz >= 1)
  {
    if (res == CBF_RES_OK)
      if (fprintf(pFile, "FCOORD\n%lli\n", data.fnnz) <= 0)
        res = CBF_RES_ERR;

    for (i=0; i<data.fnnz && res==CBF_RES_OK; ++i)
      if (fprintf(pFile, "%lli %i %i %i %.16lg\n", data.fsubi[i], data.fsubj[i], data.fsubk[i], data.fsubl[i], data.fval[i]) <= 0)
        res = CBF_RES_ERR;

    if (res == CBF_RES_OK)
      if (fprintf(pFile, "\n") <= 0)
        res = CBF_RES_ERR;
  }

  return res;
}

static CBFresponsee writeACOORD(FILE *pFile, const CBFdata data)
{
  CBFresponsee res = CBF_RES_OK;
  long long int i;

  if (data.annz >= 1)
  {
    if (res == CBF_RES_OK)
      if (fprintf(pFile, "ACOORD\n%lli\n", data.annz) <= 0)
        res = CBF_RES_ERR;

    for (i=0; i<data.annz && res==CBF_RES_OK; ++i)
      if (fprintf(pFile, "%lli %lli %.16lg\n", data.asubi[i], data.asubj[i], data.aval[i]) <= 0)
        res = CBF_RES_ERR;

    if (res == CBF_RES_OK)
      if (fprintf(pFile, "\n") <= 0)
        res = CBF_RES_ERR;
  }

  return res;
}

static CBFresponsee writeBCOORD(FILE *pFile, const CBFdata data)
{
  CBFresponsee res = CBF_RES_OK;
  long long int i;

  if (data.bnnz >= 1)
  {
    if (res == CBF_RES_OK)
      if (fprintf(pFile, "BCOORD\n%lli\n", data.bnnz) <= 0)
        res = CBF_RES_ERR;

    for (i=0; i<data.bnnz && res==CBF_RES_OK; ++i)
      if (fprintf(pFile, "%lli %.16lg\n", data.bsubi[i], data.bval[i]) <= 0)
        res = CBF_RES_ERR;

    if (res == CBF_RES_OK)
      if (fprintf(pFile, "\n") <= 0)
        res = CBF_RES_ERR;
  }

  return res;
}

static CBFresponsee writeHCOORD(FILE *pFile, const CBFdata data)
{
  CBFresponsee res = CBF_RES_OK;
  long long int i;

  if (data.hnnz >= 1)
  {
    if (res == CBF_RES_OK)
      if (fprintf(pFile, "HCOORD\n%lli\n", data.hnnz) <= 0)
        res = CBF_RES_ERR;

    for (i=0; i<data.hnnz && res==CBF_RES_OK; ++i)
      if (fprintf(pFile, "%i %lli %i %i %.16lg\n", data.hsubi[i], data.hsubj[i], data.hsubk[i], data.hsubl[i], data.hval[i]) <= 0)
        res = CBF_RES_ERR;

    if (res == CBF_RES_OK)
      if (fprintf(pFile, "\n") <= 0)
        res = CBF_RES_ERR;
  }

  return res;
}

static CBFresponsee writeDCOORD(FILE *pFile, const CBFdata data)
{
  CBFresponsee res = CBF_RES_OK;
  long long int i;

  if (data.dnnz >= 1)
  {
    if (res == CBF_RES_OK)
      if (fprintf(pFile, "DCOORD\n%lli\n", data.dnnz) <= 0)
        res = CBF_RES_ERR;

    for (i=0; i<data.dnnz && res==CBF_RES_OK; ++i)
      if (fprintf(pFile, "%i %i %i %.16lg\n", data.dsubi[i], data.dsubk[i], data.dsubl[i], data.dval[i]) <= 0)
        res = CBF_RES_ERR;

    if (res == CBF_RES_OK)
      if (fprintf(pFile, "\n") <= 0)
        res = CBF_RES_ERR;
  }

  return res;
}
