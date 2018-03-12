/**
 *  Copyright (c) 2017 Leonardo A. Bautista-Gomez
 *  All rights reserved
 *
 *  FTI - A multi-level checkpointing library for C/C++/Fortran applications
 *
 *  Revision 1.0 : Fault Tolerance Interface (FTI)
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice, this
 *  list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *  this list of conditions and the following disclaimer in the documentation
 *  and/or other materials provided with the distribution.
 *
 *  3. Neither the name of the copyright holder nor the names of its contributors
 *  may be used to endorse or promote products derived from this software without
 *  specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 *  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  @author Kai Keller (kellekai@gmx.de)
 *  @file   ftiff.h
 *  @date   October, 2017
 *  @brief  Header file for the FTI File Format (FTI-FF).
 */

#ifndef _FTIFF_H
#define _FTIFF_H

#define MBR_CNT(TYPE) int TYPE ## _mbrCnt
#define MBR_BLK_LEN(TYPE) int TYPE ## _mbrBlkLen[]
#define MBR_TYPES(TYPE) MPI_Datatype TYPE ## _mbrTypes[]
#define MBR_DISP(TYPE) MPI_Aint TYPE ## _mbrDisp[]


#include "fti.h"
#include <assert.h>

/**

  +-------------------------------------------------------------------------+
  |   FTI-FF TYPES                                                          |
  +-------------------------------------------------------------------------+

 **/

/** @typedef    FTIFF_headInfo
 *  @brief      Runtime meta info for the heads.
 *
 *  keeps all necessary meta data information for the heads, in order to
 *  perform the checkpoint.
 *
 */
typedef struct FTIFF_headInfo {
    int exists;
    int nbVar;
    char ckptFile[FTI_BUFS];
    long maxFs;
    long fs;
    long pfs;
} FTIFF_headInfo;

/** @typedef    FTIFF_L2Info
 *  @brief      Meta data for L2 recovery.
 *
 *  keeps meta data information that needs to be exchanged between the ranks.
 *
 */
typedef struct FTIFF_L2Info {
    int FileExists;
    int CopyExists;
    int ckptID;
    int rightIdx;
    long fs;
    long pfs;
} FTIFF_L2Info;

/** @typedef    FTIFF_L3Info
 *  @brief      Meta data for L3 recovery.
 *
 *  keeps meta data information that needs to be exchanged between the ranks.
 *
 */
typedef struct FTIFF_L3Info {
    int FileExists;
    int RSFileExists;
    int ckptID;
    long fs;
    long RSfs;  // maxFs
} FTIFF_L3Info;

/**

  +-------------------------------------------------------------------------+
  |   MPI DERIVED DATA TYPES                                                |
  +-------------------------------------------------------------------------+

 **/

// ID MPI types
enum {
    FTIFF_HEAD_INFO,
    FTIFF_L2_INFO,
    FTIFF_L3_INFO,
    FTIFF_NUM_MPI_TYPES
};

// declare MPI datatypes
extern MPI_Datatype FTIFF_MpiTypes[FTIFF_NUM_MPI_TYPES];

typedef struct FTIFF_MPITypeInfo {
    MPI_Datatype        raw;
    int                 mbrCnt;
    int*                mbrBlkLen;
    MPI_Datatype*       mbrTypes;
    MPI_Aint*           mbrDisp;
} FTIFF_MPITypeInfo;

/**

  +-------------------------------------------------------------------------+
  |   FUNCTION DECLARATIONS                                                 |
  +-------------------------------------------------------------------------+

 **/

void FTIFF_InitMpiTypes();
void FTIFF_FreeDbFTIFF(FTIFF_db* last);
int FTIFF_Checksum(FTIT_execution* FTI_Exec, FTIT_dataset* FTI_Data, char* checksum);
int FTIFF_Recover( FTIT_execution *FTI_Exec, FTIT_dataset *FTI_Data, FTIT_checkpoint *FTI_Ckpt );
int FTIFF_RecoverVar( int id, FTIT_execution *FTI_Exec, FTIT_dataset *FTI_Data, FTIT_checkpoint *FTI_Ckpt );
int FTIFF_UpdateDatastructFTIFF( FTIT_execution* FTI_Exec, FTIT_dataset* FTI_Data );
long FTIFF_InsertChunk( FTIT_execution* FTI_Exec, FTIT_dataset* FTI_Data, int idx, long chunksize );
int FTIFF_ReadDbFTIFF( FTIT_execution *FTI_Exec, FTIT_checkpoint* FTI_Ckpt );
int FTIFF_WriteFTIFF(FTIT_configuration* FTI_Conf, FTIT_execution* FTI_Exec,
        FTIT_topology* FTI_Topo, FTIT_checkpoint* FTI_Ckpt,
        FTIT_dataset* FTI_Data);
int FTIFF_CreateMetadata( FTIT_execution* FTI_Exec, FTIT_topology* FTI_Topo,
        FTIT_dataset* FTI_Data, FTIT_configuration* FTI_Conf );
int FTIFF_CheckL1RecoverInit( FTIT_execution* FTI_Exec, FTIT_topology* FTI_Topo,
        FTIT_checkpoint* FTI_Ckpt );
int FTIFF_CheckL2RecoverInit( FTIT_execution* FTI_Exec, FTIT_topology* FTI_Topo,
        FTIT_checkpoint* FTI_Ckpt, int *exists);
int FTIFF_CheckL3RecoverInit( FTIT_execution* FTI_Exec, FTIT_topology* FTI_Topo,
        FTIT_checkpoint* FTI_Ckpt, int* erased);
int FTIFF_CheckL4RecoverInit( FTIT_execution* FTI_Exec, FTIT_topology* FTI_Topo,
        FTIT_checkpoint* FTI_Ckpt, char *checksum);
void FTIFF_GetHashMetaInfo( unsigned char *hash, FTIFF_metaInfo *FTIFFMeta );
void FTIFF_PrintDataStructure( int rank, FTIT_execution* FTI_Exec );
#endif
