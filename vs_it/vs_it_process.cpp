/*
VS_IT Copyright(C) 2002 thejam79, 2003 minamina, 2014 msg7086

This program is free software; you can redistribute it and / or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110 - 1301, USA.
*/

#include "vs_it.h"
#include "vs_it_interface.h"

void __stdcall IT::Decide(IScriptEnvironment*env, int n)
{
	if (env->m_blockInfo[n / 5].level != 'U')
		return;

	int base = (n / 5) * 5;
	int i;
	int min0 = FII(base).diffP0;
	for (i = 1; i < 5; ++i) {
		min0 = min(min0, FII(base + i).diffP0);
	}
	int mmin = AdjPara(50);

	for (i = 0; i < 5; ++i) {
		int m = FII(base + i).diffP0;
		if (m >= max(mmin, min0) * 5) {
			FII(base + i).mflag = '.';
		}
		else {
			FII(base + i).mflag = '+';
		}
	}
	//  const int motion1 = 100;

	int ncf = 0;
	int cfi = -1;
	for (i = 0; i < 5; ++i) {
		if (FII(base + i).mflag == '.')
			++ncf;
		else
			cfi = i;
	}

	int mmin2 = AdjPara(50);
	if (ncf == 0) {
		min0 = FII(base).diffS0;
		for (i = 1; i < 5; ++i) {
			min0 = min(min0, FII(base + i).diffS0);
		}
		for (i = 0; i < 5; ++i) {
			int m = FII(base + i).diffS0;
			if (m >= max(mmin2, min0) * 3) {
				FII(base + i).mflag = '.';
			}
			else {
				FII(base + i).mflag = '+';
			}
		}
		ncf = 0;
		cfi = -1;
		for (i = 0; i < 5; ++i) {
			if (FII(base + i).mflag == '.')
				++ncf;
			else
				cfi = i;
		}
	}

	if (ncf == 4 && cfi >= 0) {
		SetFT(env, base, cfi, 'D');
		return;
	}
	if (ncf != 0 || 1) {
		bool flag = false;
		for (i = 0; i < 5; ++i) {
			int rr = (i + 2 + 5) % 5;
			int r = (i + 1 + 5) % 5;
			int l = (i - 1 + 5) % 5;
			if (FII(base + i).mflag != '.' && FII(base + i).match == 'P') {
				if (FII(base + i).mflag == '+') {
					FII(base + i).mflag = '*';
					flag = true;
				}
				if (FII(base + r).mflag == '+') {
					FII(base + r).mflag = '*';
					flag = true;
				}
				if (FII(base + l).mflag == '+') {
					FII(base + l).mflag = '*';
					flag = true;
				}
			}
			if (FII(base + i).match == 'N') {
				if (FII(base + r).mflag == '+') {
					FII(base + r).mflag = '*';
					flag = true;
				}
				if (FII(base + rr).mflag == '+') {
					FII(base + rr).mflag = '*';
					flag = true;
				}
			}

		}

		//31228 39045

		if (flag) {
			for (i = 0; i < 5; ++i) {
				char c = FII(base + i).mflag;
				if (c == '+')
					FII(base + i).mflag = '*';
				if (c == '*')
					FII(base + i).mflag = '+';
			}
		}
		for (i = 0; i < 5; ++i) {
			if (FII(base + i).pos == '2') {
				SetFT(env, base, i, 'd');
				return;
			}
		}
		if (base - 5 >= 0 && env->m_blockInfo[base / 5 - 1].level != 'U') {
			int tcfi = env->m_blockInfo[base / 5 - 1].cfi;
			if (env->m_frameInfo[base + tcfi].mflag == '+') {
				SetFT(env, base, tcfi, 'y');
				return;
			}
		}
		int pnpos[5], pncnt = 0;
		for (i = 0; i < 5; ++i) {
			if (toupper(FII(base + i).match) == 'P') {
				pnpos[pncnt++] = i;
			}
		}
		if (pncnt == 2) {
			int k = pnpos[0];
			if (pnpos[0] == 0 && pnpos[1] == 4) {
				k = 4;
			}
			if (FII(base + k).mflag != '.') {
				SetFT(env, base, k, 'x');
				return;
			}
		}

		pncnt = 0;
		for (i = 0; i < 5; ++i) {
			if (toupper(FII(base + i).match) != 'N') {
				pnpos[pncnt++] = i;
			}
		}
		if (pncnt == 2) {
			int k = pnpos[0];
			if (pnpos[0] == 3 && pnpos[1] == 4) {
				k = 4;
			}
			k = (k + 2) % 5;
			if (FII(base + k).mflag != '.') {
				SetFT(env, base, k, 'x');
				return;
			}
		}

		for (i = 0; i < 5; ++i) {
			if (FII(base + i).mflag == '+') {
				SetFT(env, base, i, 'd');
				return;
			}
		}
	}

	cfi = 0;
	int minx = FII(base).diffS0;
	for (i = 1; i < 5; ++i) {
		int m = FII(base + i).diffS0;
		if (m < minx) {
			cfi = i;
			minx = m;
		}
	}
	SetFT(env, base, cfi, 'z');
	return;
}

void IT::DeintOneField_YV12(IScriptEnvironment*env, VSFrameRef* dst, int n)
{
	const VSFrameRef * srcC = GetChildFrame(n);
	const VSFrameRef *srcR;
	switch (toupper(env->m_iUseFrame)) {
	default:
	case 'C':
		srcR = srcC;
		break;
	case 'P':
		srcR = GetChildFrame(n - 1);
		break;
	case 'N':
		srcR = GetChildFrame(n + 1);
		break;
	}

	const unsigned char *pT;
	const unsigned char *pC;
	const unsigned char *pB;
	const unsigned char *pBB;
	const unsigned char *pC_U;
	const unsigned char *pB_U;
	const unsigned char *pBB_U;
	const unsigned char *pC_V;
	const unsigned char *pB_V;
	const unsigned char *pBB_V;
	unsigned char *pDC;
	unsigned char *pDB;
	unsigned char *pDC_U;
	unsigned char *pDC_V;
	unsigned char *pDB_U;
	unsigned char *pDB_V;

	MakeSimpleBlurMap_YV12(env, env->m_iCurrentFrame);
	MakeMotionMap2Max_YV12(env, env->m_iCurrentFrame);

	unsigned char *pFieldMap;
	pFieldMap = new unsigned char[width * height];
	ZeroMemory(pFieldMap, width * height);
	int x, y;
	for (y = 0; y < height; y += 1)
	{
		unsigned char *pFM = pFieldMap + width * clipY(y);
		for (x = 1; x < width - 1; x++)
		{
			const unsigned char *pmSC = env->m_motionMap4DI + width * clipY(y);
			const unsigned char *pmSB = env->m_motionMap4DI + width * clipY(y + 1);
			const unsigned char *pmMC = env->m_motionMap4DIMax + width * clipY(y);
			const unsigned char *pmMB = env->m_motionMap4DIMax + width * clipY(y + 1);
			const int nTh = 12;
			const int nThLine = 1;
			if (((pmSC[x - 1] > nThLine && pmSC[x] > nThLine && pmSC[x + 1] > nThLine) ||
				(pmSB[x - 1] > nThLine && pmSB[x] > nThLine && pmSB[x + 1] > nThLine)) &&
				((pmMC[x - 1] > nTh && pmMC[x] > nTh && pmMC[x + 1] > nTh) ||
				(pmMB[x - 1] > nTh && pmMB[x] > nTh && pmMB[x + 1] > nTh)))
			{
				pFM[x - 1] = 1;
				pFM[x] = 1;
				pFM[x + 1] = 1;
			}
		}
	}

	const int nPitchSrc = env->vsapi->getStride(srcC, 0); // srcC->GetPitch();
	const int nPitchSrcU = env->vsapi->getStride(srcC, 1); // srcC->GetPitch(PLANAR_U);
	const int nPitchDst = env->vsapi->getStride(dst, 0); // dst->GetPitch();
	const int nRowSizeDst = width; // dst->GetRowSize(); DIVIDE BY SIZEOF(T)
	const int nPitchDstU = env->vsapi->getStride(dst, 1); // dst->GetPitch(PLANAR_U);
	const int nRowSizeDstU = width >> vi->format->subSamplingW; // dst->GetRowSize(PLANAR_U); DIVIDE BY SIZEOF(T)

	for (y = 0; y < height; y += 2) {
		pT = SYP(env, srcR, y - 1);
		pC = SYP(env, srcC, y);
		pB = SYP(env, srcR, y + 1);
		pBB = SYP(env, srcC, y + 2);
		pC_U = SYP(env, srcC, y, PLANAR_U);
		pB_U = SYP(env, srcR, y + 1, PLANAR_U);
		pBB_U = SYP(env, srcC, y + 4, PLANAR_U);
		pC_V = SYP(env, srcC, y, PLANAR_V);
		pB_V = SYP(env, srcR, y + 1, PLANAR_V);
		pBB_V = SYP(env, srcC, y + 4, PLANAR_V);

		pDC = DYP(env, dst, y);
		pDB = DYP(env, dst, y + 1);
		pDC_U = DYP(env, dst, y, PLANAR_U);
		pDB_U = DYP(env, dst, y + 1, PLANAR_U);
		pDC_V = DYP(env, dst, y, PLANAR_V);
		pDB_V = DYP(env, dst, y + 1, PLANAR_V);

		env->BitBlt(pDC, nPitchDst, pC, nPitchSrc, nRowSizeDst, 1);
		if ((y >> 1) % 2)
		{
			env->BitBlt(pDC_U, nPitchDstU, pC_U, nPitchSrcU, nRowSizeDstU, 1);
			env->BitBlt(pDC_V, nPitchDstU, pC_V, nPitchSrcU, nRowSizeDstU, 1);
		}

		const unsigned char *pFM = pFieldMap + width * clipY(y);
		const unsigned char *pFMB = pFieldMap + width * clipY(y + 1);
		for (x = 0; x < width; ++x)
		{
			int x_half = x >> 1;
			if ((pFM[x - 1] == 1 || pFM[x] == 1 || pFM[x + 1] == 1) ||
				(pFMB[x - 1] == 1 || pFMB[x] == 1 || pFMB[x + 1] == 1))
			{
				pDB[x] = BYTE((pC[x] + pBB[x] + 1) >> 1);
			}
			else
			{
				pDB[x] = pB[x];
			}

			if ((y >> 1) % 2)
			{
				pDB_U[x_half] = BYTE((pC_U[x_half] + pBB_U[x_half] + 1) >> 1);
				pDB_V[x_half] = BYTE((pC_V[x_half] + pBB_V[x_half] + 1) >> 1);
			}
		}
	}
	delete[] pFieldMap;
	if (srcC != srcR)
		FreeFrame(srcR);
	FreeFrame(srcC);

	return;
}

#define MAKE_BLUR_MAP_ASM(mmA, mmB) \
	__asm movq mm7, mmA \
	__asm psubusb mmA, mmB \
	__asm psubusb mmB, mm7 \
	__asm por mmA, mmB

void IT::MakeSimpleBlurMap_YV12(IScriptEnvironment*env, int n)
{
	int twidth = width;
	const VSFrameRef * srcC = GetChildFrame(n);
	const VSFrameRef *srcR;
	switch (toupper(env->m_iUseFrame)) {
	default:
	case 'C':
		srcR = srcC;
		break;
	case 'P':
		srcR = GetChildFrame(n - 1);
		break;
	case 'N':
		srcR = GetChildFrame(n + 1);
		break;
	}
	const unsigned char *pT;
	const unsigned char *pC;
	const unsigned char *pB;
	for (int y = 0; y < height; y++)
	{
		unsigned char *pD = env->m_motionMap4DI + y * width;
		{
			if (y % 2)
			{
				pT = SYP(env, srcC, y - 1);
				pC = SYP(env, srcR, y);
				pB = SYP(env, srcC, y + 1);
			}
			else
			{
				pT = SYP(env, srcR, y - 1);
				pC = SYP(env, srcC, y);
				pB = SYP(env, srcR, y + 1);
			}
			_asm {
				mov eax, pC
					mov ebx, pT
					mov ecx, pB
					mov edi, pD
					xor esi, esi
					align 16
				loopA:
				movq mm0, [eax + esi]
					movq mm1, [ebx + esi]
					movq mm2, mm0
					movq mm3, mm1
					MAKE_BLUR_MAP_ASM(mm0, mm1)

					movq mm4, [ecx + esi]
					movq mm1, mm4
					MAKE_BLUR_MAP_ASM(mm2, mm4)

					MAKE_BLUR_MAP_ASM(mm3, mm1)

					paddusb mm0, mm2
					psubusb mm0, mm3
					psubusb mm0, mm3

					lea esi, [esi + 8]
					cmp esi, twidth
					movntq[edi + esi - 8], mm0
					jl loopA
			}
		}
	}
	USE_MMX2;
	if (srcC != srcR)
		FreeFrame(srcR);
	FreeFrame(srcC);
}

#define MAKE_MOTION_MAP2_ASM_INIT(C, P) \
	__asm mov eax, C \
	__asm mov ebx, P

#define MAKE_MOTION_MAP2_ASM(mmm, step) \
	__asm movq mmm, [eax + esi*step] \
	__asm movq mm2, mmm \
	__asm movq mm1, [ebx + esi*step] \
	__asm psubusb mmm, mm1 \
	__asm psubusb mm1, mm2 \
	__asm por mmm, mm1

void IT::MakeMotionMap2Max_YV12(IScriptEnvironment*env, int n)
{
	const int twidth = width >> 1;

	PVideoFrame srcP = GetChildFrame(n - 1);
	PVideoFrame srcC = GetChildFrame(n);
	PVideoFrame srcN = GetChildFrame(n + 1);

	//	for(int y = 0; y < height; y += 2) {
	for (int y = 0; y < height; y++) {
		unsigned char *pD = env->m_motionMap4DIMax + y * width;
		//		unsigned char *pDB = m_motionMap4DIMax + (y + 1) * width;
		{
			const unsigned char *pC = SYP(env, srcC, y);
			const unsigned char *pP = SYP(env, srcP, y);
			const unsigned char *pN = SYP(env, srcN, y);
			const unsigned char *pC_U = SYP(env, srcC, y, PLANAR_U);
			const unsigned char *pP_U = SYP(env, srcP, y, PLANAR_U);
			const unsigned char *pN_U = SYP(env, srcN, y, PLANAR_U);
			const unsigned char *pC_V = SYP(env, srcC, y, PLANAR_V);
			const unsigned char *pP_V = SYP(env, srcP, y, PLANAR_V);
			const unsigned char *pN_V = SYP(env, srcN, y, PLANAR_V);

			_asm {
				mov edi, pD
					xor esi, esi
					align 16
				loopA:
				///P
				MAKE_MOTION_MAP2_ASM_INIT(pC, pP)
					MAKE_MOTION_MAP2_ASM(mm0, 2)

					MAKE_MOTION_MAP2_ASM_INIT(pC_U, pP_U)
					MAKE_MOTION_MAP2_ASM(mm3, 1)

					MAKE_MOTION_MAP2_ASM_INIT(pC_V, pP_V)
					MAKE_MOTION_MAP2_ASM(mm4, 1)

					pmaxub mm3, mm4
					punpcklbw mm3, mm3
					pmaxub mm0, mm3

					///N
					MAKE_MOTION_MAP2_ASM_INIT(pC, pN)
					MAKE_MOTION_MAP2_ASM(mm5, 2)

					MAKE_MOTION_MAP2_ASM_INIT(pC_U, pN_U)
					MAKE_MOTION_MAP2_ASM(mm3, 1)

					MAKE_MOTION_MAP2_ASM_INIT(pC_V, pN_V)
					MAKE_MOTION_MAP2_ASM(mm4, 1)

					pmaxub mm3, mm4
					punpcklbw mm3, mm3
					pmaxub mm5, mm3

					//				pminub mm0,mm5
					pmaxub mm0, mm5

					lea esi, [esi + 4]
					cmp esi, twidth
					movntq[edi + esi * 2 - 8], mm0
					jl loopA
			}
		}
	}
	USE_MMX2;
	FreeFrame(srcC);
	FreeFrame(srcP);
	FreeFrame(srcN);
}